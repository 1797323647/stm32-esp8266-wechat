#include "stm32f10x.h"
#include "oled.h"
#include "delay.h"
#include "led.h"
#include "uart.h"
#include "mpu6050.h"
#include "max30102.h"
#include "algorithm.h"
#include "sys.h"
#include "buzzer.h"

// --- 定义显示模式 ---
#define DISPLAY_MAX30102  1
#define DISPLAY_MPU6050   0

volatile uint8_t button_pressed = 0;
volatile uint32_t last_button_time = 0;
#define DEBOUNCE_TIME 50 // 消抖时间，单位为毫秒

uint8_t display_mode = DISPLAY_MPU6050; // 初始显示模式为 MPU6050

int16_t AX, AY, AZ, GX, GY, GZ;
// MAX30102 相关变量
extern uint8_t dis_hr;
extern uint8_t dis_spo2;
extern int32_t n_heart_rate;
extern int8_t ch_hr_valid;
extern int32_t n_sp02;
extern int8_t ch_spo2_valid;

void EXTI_Button_Config();
uint32_t GetTick(void); // 声明获取系统时钟的函数

// --- 新增变量用于存储上次发送的心率和血氧值 ---
uint8_t last_sent_hr = 0;
uint8_t last_sent_spo2 = 0;

int main(void) {
    LED_Configuration();
    uart_init(115200);//重定向printf
	UART2_Configuration(115200); // 初始化 UART2，波特率 115200
    I2C_Configuration();
    Tick_Delay_Config();
    EXTI_Button_Config(); // 基于中断的按键配置
	BUZZER_Init();//蜂鸣器初始化
    OLED_Init();
    OLED_CLS();
    MPU6050_Init();
    max30102_app_init();
    printf("uart is ok \r\n");
    delay_ms(2000);
    LEDON;
    OLED_ShowStr(32, 0, "Posture:", 1); // 初始显示 MPU6050 提示信息
//	BUZZER_ON;
    while (1) {
        // 使用中断设置的flag来检测按钮按下
        if(button_pressed){
            // 重置按钮标志
            button_pressed = 0;           
            // 切换LED状态
            LED_TOGGLE;
            // 切换显示模式
            display_mode = (display_mode+1)%2;           
            // 清除OLED显示
            OLED_CLS();            
            // 根据新的显示模式更新OLED显示内容
            if(display_mode){
                OLED_ShowStr(32, 0, "HR&SPO2:", 1);
                OLED_ShowStr(0,2,"HR  is  ......",2);
                OLED_ShowStr(0,4,"SP  is  ......",2);
            }
            else{
				BUZZER_OFF;
                OLED_ShowStr(32, 0, "Posture:", 1);
            }
            printf("Mode changed to: %d\r\n", display_mode);
        }
        
        // 根据当前显示模式执行相应的操作
        switch(display_mode){
            case 0: 
                MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
                printf("%5d,%5d,%5d\r\n",AX,AY,AZ);
                printf("%5d,%5d,%5d\r\n",GX,GY,GZ);
                OLED_ShowNumber(0,2,AX,1);
                OLED_ShowNumber(48,2,AY,1);
                OLED_ShowNumber(96,2,AZ,1);
                OLED_ShowNumber(0,4,GX,1);
                OLED_ShowNumber(48,4,GY,1);
                OLED_ShowNumber(96,4,GZ,1);
				
				// --- 添加 MPU6050 数据上传到ESP8266的代码 ---
				char mpu6050_data[100]; // 存储MPU6050数据的字符串缓冲区，增大缓冲区
				sprintf(mpu6050_data, "AX:%d,AY:%d,AZ:%d,GX:%d,GY:%d,GZ:%d\r\n", AX, AY, AZ, GX, GY, GZ); // 格式化数据
				UART2_SendString(mpu6050_data); // 通过UART2发送MPU6050数据到ESP8266
				printf("Sent MPU6050 to ESP8266: %s", mpu6050_data); // 打印发送到ESP8266的MPU6050数据，用于调试
				// --- MPU6050 数据上传代码结束 ---
			
                delay_ms(500);
                break;
            case 1: 
                max30102_task();
                if(dis_hr == 0 && dis_spo2 == 0){
                    printf("HR:--- SpO2:--- \r\n");
                    OLED_ShowNumber(64,2,dis_hr,2);
                    OLED_ShowNumber(64,4,dis_spo2,2);
                }
                else{
                    printf("HR:%3d SpO2:%3d ",dis_hr,dis_spo2);
                    printf("HRvalid=%i, ", ch_hr_valid);
                    printf("SpO2Valid=%i\r\n", ch_spo2_valid);
                    OLED_ShowNumber(64,2,dis_hr,2);
                    OLED_ShowNumber(64,4,dis_spo2,2);
					if(dis_hr > 80 || dis_hr < 40 || dis_spo2 > 100 || dis_spo2 < 60){
						BUZZER_ON;
					}else {
						BUZZER_OFF;
					}
					 // --- 添加数据上传到ESP8266的代码 (仅当数据变化时才发送) ---
                    if (dis_hr != last_sent_hr || dis_spo2 != last_sent_spo2) {
                        char hr_spo2_data[50]; // 存储HR和SpO2数据的字符串缓冲区
                        sprintf(hr_spo2_data, "HR:%d,SpO2:%d\r\n", dis_hr, dis_spo2); // 格式化数据
                        UART2_SendString(hr_spo2_data); // 通过UART2发送数据到ESP8266
                        printf("Sent to ESP8266: %s", hr_spo2_data); // 打印发送到ESP8266的数据，用于调试

                        // 更新上次发送的值
                        last_sent_hr = dis_hr;
                        last_sent_spo2 = dis_spo2;
                    }
                    // --- 数据上传代码结束 ---
					
                }
                delay_ms(50);
                break;
        }
    }
}

// 获取系统时钟函数，使用SysTick来实现
uint32_t GetTick(void) {
    return (uint32_t)SysTick->VAL; // 使用SysTick当前值作为时间戳
    // 注意: 实际应用中应该使用一个持续递增的计数器
    // 如果你有系统tick计数器，请使用它替代此函数
}

// 按键中断配置
void EXTI_Button_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 启用GPIOA时钟和AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    
    // 配置按键GPIO引脚为上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // PA1引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 使用GPIOA
    
    // 将EXTI线连接到GPIO引脚
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1); // PA1
    
    // 配置EXTI线
    EXTI_InitStructure.EXTI_Line = EXTI_Line1; // EXTI1对应PA1
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发（按钮按下）
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    // 启用并设置EXTI中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn; // EXTI1中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; // 高优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 中断处理函数 - 添加消抖逻辑
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET) // 检查EXTI1线路是否有中断
    {
        // 获取当前时间
        uint32_t current_time = GetTick();
        
        // 时间差足够大，认为是有效按键
        if((current_time - last_button_time) > DEBOUNCE_TIME || current_time < last_button_time)
        {
            // 设置按钮标志
            button_pressed = 1;
            // 更新上次按键时间
            last_button_time = current_time;
        }
        
        // 清除中断标志
        EXTI_ClearITPendingBit(EXTI_Line1); // 清除EXTI1中断标志位
    }
}

