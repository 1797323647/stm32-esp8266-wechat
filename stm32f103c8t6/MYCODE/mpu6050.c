#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "mpu6050_Reg.h"
 
#define MPU6050_ADDRESS 0xD0
 
void MyI2C_Init(void)
{
    //  需要完成2个任务：
    //  ① 把SCL和SDA均初始化为开漏输出模式；
    //  ② 把SCL和SDA置高电平。
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//	开漏输出(虽然名字是"输出",但开漏输出模式仍然可以输入)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;//  SCL是PB10，SDA是PB11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);//   置SCL和SDA高电平
}
 
//  对操作端口的库函数进行封装
void MyI2C_W_SCL(uint8_t BitValue)//    写SCL。参数给1或0，就可以释放或拉低SCL
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
    delay_us(10);// 如果单片机主频较快，可以加延时，防止从机跟不上
}
 
void MyI2C_W_SDA(uint8_t BitValue)//    写SDA。参数给1或0，就可以释放或拉低SDA
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
    delay_us(10);
}
 
uint8_t MyI2C_R_SDA(void)// 读SDA
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    delay_us(10);
    return BitValue;
}
 
//  接下来完成I2C的6个时序基本单元
//
//  1、起始条件
//  首先确保SCL和SDA释放，然后先拉低SDA，再拉低SCL
void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);//   释放SDA
    MyI2C_W_SCL(1);//   释放SCL
    
    MyI2C_W_SDA(0);//   先拉低SDA
    MyI2C_W_SCL(0);//   再拉低SCL
}
 
//  2、终止条件
//  SCL高电平期间，SDA从低电平切换到高电平
void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);//   先拉低SDA，避免SDA之前为高电平时释放无法产生上升沿
    MyI2C_W_SCL(1);//   再释放SCL
    MyI2C_W_SDA(1);//   再释放SDA
}
 
//  3、发送一个字节
//  SCL低电平，SDA变换数据；SCL高电平，SDA保持数据稳定。放完1位后，释放SCL，拉低SCL，驱动时钟运转
void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));//  首先趁SCL低电平，把Byte的最高位放在SDA线上。下一轮就是把次高位放在SDA线上
        //  取Byte最高位为Byte&0x80，取次高位为Byte&0x40...在循环中，使用右移i位即可实现按位与数值的变换
        MyI2C_W_SCL(1);//   再释放SCL。释放后从机会立刻把SDA的数据读走
        MyI2C_W_SCL(0);//   拉低SCL
    }
}
 
//  4、接收一个字节
//  SCL低电平，从机把数据放到SDA上，为了防止主机干扰从机写入数据，主机需要先释放SDA，释放SDA也相当于切换为输入模式
//  然后主机在SCL高电平期间读取SDA，再拉低SCL。这样重复8次，主机就能读到一个字节了。
uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);//   主机释放SDA
    for(i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);//   主机释放SCL
        if(MyI2C_R_SDA() == 1)//    主机读取数据
        {
            Byte |= (0x80 >> i);// 如果读取到1，就从高到低把Byte对应位置1
        }
        MyI2C_W_SCL(0);//   读取1位后拉低SCL，这时从机会把下一位数据放到SDA上
    }
    return Byte;
}
 
//  5、发送应答
//  其实就是发送一个字节的简化
void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);//  函数进入时，SCL低电平，主机把AckBit放到SDA上
    MyI2C_W_SCL(1);//   再释放SCL。从机读取应答
    MyI2C_W_SCL(0);//   拉低SCL，进入下一个时序单元
}
 
//  6、接收应答
//  其实就是接收一个字节的简化
uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);//   函数进入时SCL低电平，主机释放SDA防止干扰从机。同时，从机把应答位放在SDA上
    MyI2C_W_SCL(1);//   主机释放SCL，读取应答位
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);//   读取后拉低SCL，进入下一个时序单元
    return AckBit;
}


 
// 指定地址写
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);// 发送从机地址+读写位
    MyI2C_ReceiveAck();//   接收应答
    MyI2C_SendByte(RegAddress);//   指定寄存器地址
    MyI2C_ReceiveAck();
    MyI2C_SendByte(Data);//   发送写入指定寄存器地址下的数据
    MyI2C_ReceiveAck();
    MyI2C_Stop();
}
 
//  指定地址读
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data;
    
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);// 发送从机地址+读写位
    MyI2C_ReceiveAck();//   接收应答
    MyI2C_SendByte(RegAddress);//   指定寄存器地址
    MyI2C_ReceiveAck();
    
    MyI2C_Start();//    重复起始条件
    MyI2C_SendByte(MPU6050_ADDRESS | 0xD1);//   读写位1，读出数据
    MyI2C_ReceiveAck();
    Data = MyI2C_ReceiveByte();//   从机发送数据，主机接收数据
    MyI2C_SendAck(1);//  这里只想要1个字节，所以不给从机应答
    MyI2C_Stop();
    
    return Data;
}
 
void MPU6050_Init(void)
{
    MyI2C_Init();
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//   配置PWR_MGMT_1寄存器，解除睡眠，选择X轴陀螺仪时钟
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//   采样率分频，决定了数据输出的快慢。10分频
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);//   数字低通滤波器给110
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);//  陀螺仪配置，选择最大量程11
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);// 加速度计配置，选择最大量程11
    
}
 
uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}
 
//  由于需要返回6个变量，使用指针的地址传递
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                        int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t Data_H, Data_L;
    
    Data_H = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);//    加速度寄存器X轴高8位
    Data_L = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    *AccX = (Data_H << 8) | Data_L;//   加速度计X轴的16位数据
    
    Data_H = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    Data_L = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (Data_H << 8) | Data_L;
    
    Data_H = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    Data_L = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (Data_H << 8) | Data_L;
    
    Data_H = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);//    陀螺仪寄存器X轴高8位
    Data_L = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    *GyroX = (Data_H << 8) | Data_L;//   陀螺仪X轴的16位数据
    
    Data_H = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    Data_L = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (Data_H << 8) | Data_L;
    
    Data_H = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    Data_L = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (Data_H << 8) | Data_L;
}
