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

// --- ������ʾģʽ ---
#define DISPLAY_MAX30102  1
#define DISPLAY_MPU6050   0

volatile uint8_t button_pressed = 0;
volatile uint32_t last_button_time = 0;
#define DEBOUNCE_TIME 50 // ����ʱ�䣬��λΪ����

uint8_t display_mode = DISPLAY_MPU6050; // ��ʼ��ʾģʽΪ MPU6050

int16_t AX, AY, AZ, GX, GY, GZ;
// MAX30102 ��ر���
extern uint8_t dis_hr;
extern uint8_t dis_spo2;
extern int32_t n_heart_rate;
extern int8_t ch_hr_valid;
extern int32_t n_sp02;
extern int8_t ch_spo2_valid;

void EXTI_Button_Config();
uint32_t GetTick(void); // ������ȡϵͳʱ�ӵĺ���

// --- �����������ڴ洢�ϴη��͵����ʺ�Ѫ��ֵ ---
uint8_t last_sent_hr = 0;
uint8_t last_sent_spo2 = 0;

int main(void) {
    LED_Configuration();
    uart_init(115200);//�ض���printf
	UART2_Configuration(115200); // ��ʼ�� UART2�������� 115200
    I2C_Configuration();
    Tick_Delay_Config();
    EXTI_Button_Config(); // �����жϵİ�������
	BUZZER_Init();//��������ʼ��
    OLED_Init();
    OLED_CLS();
    MPU6050_Init();
    max30102_app_init();
    printf("uart is ok \r\n");
    delay_ms(2000);
    LEDON;
    OLED_ShowStr(32, 0, "Posture:", 1); // ��ʼ��ʾ MPU6050 ��ʾ��Ϣ
//	BUZZER_ON;
    while (1) {
        // ʹ���ж����õ�flag����ⰴť����
        if(button_pressed){
            // ���ð�ť��־
            button_pressed = 0;           
            // �л�LED״̬
            LED_TOGGLE;
            // �л���ʾģʽ
            display_mode = (display_mode+1)%2;           
            // ���OLED��ʾ
            OLED_CLS();            
            // �����µ���ʾģʽ����OLED��ʾ����
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
        
        // ���ݵ�ǰ��ʾģʽִ����Ӧ�Ĳ���
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
				
				// --- ��� MPU6050 �����ϴ���ESP8266�Ĵ��� ---
				char mpu6050_data[100]; // �洢MPU6050���ݵ��ַ��������������󻺳���
				sprintf(mpu6050_data, "AX:%d,AY:%d,AZ:%d,GX:%d,GY:%d,GZ:%d\r\n", AX, AY, AZ, GX, GY, GZ); // ��ʽ������
				UART2_SendString(mpu6050_data); // ͨ��UART2����MPU6050���ݵ�ESP8266
				printf("Sent MPU6050 to ESP8266: %s", mpu6050_data); // ��ӡ���͵�ESP8266��MPU6050���ݣ����ڵ���
				// --- MPU6050 �����ϴ�������� ---
			
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
					 // --- ��������ϴ���ESP8266�Ĵ��� (�������ݱ仯ʱ�ŷ���) ---
                    if (dis_hr != last_sent_hr || dis_spo2 != last_sent_spo2) {
                        char hr_spo2_data[50]; // �洢HR��SpO2���ݵ��ַ���������
                        sprintf(hr_spo2_data, "HR:%d,SpO2:%d\r\n", dis_hr, dis_spo2); // ��ʽ������
                        UART2_SendString(hr_spo2_data); // ͨ��UART2�������ݵ�ESP8266
                        printf("Sent to ESP8266: %s", hr_spo2_data); // ��ӡ���͵�ESP8266�����ݣ����ڵ���

                        // �����ϴη��͵�ֵ
                        last_sent_hr = dis_hr;
                        last_sent_spo2 = dis_spo2;
                    }
                    // --- �����ϴ�������� ---
					
                }
                delay_ms(50);
                break;
        }
    }
}

// ��ȡϵͳʱ�Ӻ�����ʹ��SysTick��ʵ��
uint32_t GetTick(void) {
    return (uint32_t)SysTick->VAL; // ʹ��SysTick��ǰֵ��Ϊʱ���
    // ע��: ʵ��Ӧ����Ӧ��ʹ��һ�����������ļ�����
    // �������ϵͳtick����������ʹ��������˺���
}

// �����ж�����
void EXTI_Button_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ����GPIOAʱ�Ӻ�AFIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    
    // ���ð���GPIO����Ϊ��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // PA1����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // ʹ��GPIOA
    
    // ��EXTI�����ӵ�GPIO����
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1); // PA1
    
    // ����EXTI��
    EXTI_InitStructure.EXTI_Line = EXTI_Line1; // EXTI1��ӦPA1
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش�������ť���£�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    // ���ò�����EXTI�ж����ȼ�
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn; // EXTI1�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// �жϴ����� - ��������߼�
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET) // ���EXTI1��·�Ƿ����ж�
    {
        // ��ȡ��ǰʱ��
        uint32_t current_time = GetTick();
        
        // ʱ����㹻����Ϊ����Ч����
        if((current_time - last_button_time) > DEBOUNCE_TIME || current_time < last_button_time)
        {
            // ���ð�ť��־
            button_pressed = 1;
            // �����ϴΰ���ʱ��
            last_button_time = current_time;
        }
        
        // ����жϱ�־
        EXTI_ClearITPendingBit(EXTI_Line1); // ���EXTI1�жϱ�־λ
    }
}

