#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "mpu6050_Reg.h"
 
#define MPU6050_ADDRESS 0xD0
 
void MyI2C_Init(void)
{
    //  ��Ҫ���2������
    //  �� ��SCL��SDA����ʼ��Ϊ��©���ģʽ��
    //  �� ��SCL��SDA�øߵ�ƽ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//	��©���(��Ȼ������"���",����©���ģʽ��Ȼ��������)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;//  SCL��PB10��SDA��PB11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);//   ��SCL��SDA�ߵ�ƽ
}
 
//  �Բ����˿ڵĿ⺯�����з�װ
void MyI2C_W_SCL(uint8_t BitValue)//    дSCL��������1��0���Ϳ����ͷŻ�����SCL
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
    delay_us(10);// �����Ƭ����Ƶ�Ͽ죬���Լ���ʱ����ֹ�ӻ�������
}
 
void MyI2C_W_SDA(uint8_t BitValue)//    дSDA��������1��0���Ϳ����ͷŻ�����SDA
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
    delay_us(10);
}
 
uint8_t MyI2C_R_SDA(void)// ��SDA
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    delay_us(10);
    return BitValue;
}
 
//  ���������I2C��6��ʱ�������Ԫ
//
//  1����ʼ����
//  ����ȷ��SCL��SDA�ͷţ�Ȼ��������SDA��������SCL
void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);//   �ͷ�SDA
    MyI2C_W_SCL(1);//   �ͷ�SCL
    
    MyI2C_W_SDA(0);//   ������SDA
    MyI2C_W_SCL(0);//   ������SCL
}
 
//  2����ֹ����
//  SCL�ߵ�ƽ�ڼ䣬SDA�ӵ͵�ƽ�л����ߵ�ƽ
void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);//   ������SDA������SDA֮ǰΪ�ߵ�ƽʱ�ͷ��޷�����������
    MyI2C_W_SCL(1);//   ���ͷ�SCL
    MyI2C_W_SDA(1);//   ���ͷ�SDA
}
 
//  3������һ���ֽ�
//  SCL�͵�ƽ��SDA�任���ݣ�SCL�ߵ�ƽ��SDA���������ȶ�������1λ���ͷ�SCL������SCL������ʱ����ת
void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));//  ���ȳ�SCL�͵�ƽ����Byte�����λ����SDA���ϡ���һ�־��ǰѴθ�λ����SDA����
        //  ȡByte���λΪByte&0x80��ȡ�θ�λΪByte&0x40...��ѭ���У�ʹ������iλ����ʵ�ְ�λ����ֵ�ı任
        MyI2C_W_SCL(1);//   ���ͷ�SCL���ͷź�ӻ������̰�SDA�����ݶ���
        MyI2C_W_SCL(0);//   ����SCL
    }
}
 
//  4������һ���ֽ�
//  SCL�͵�ƽ���ӻ������ݷŵ�SDA�ϣ�Ϊ�˷�ֹ�������Ŵӻ�д�����ݣ�������Ҫ���ͷ�SDA���ͷ�SDAҲ�൱���л�Ϊ����ģʽ
//  Ȼ��������SCL�ߵ�ƽ�ڼ��ȡSDA��������SCL�������ظ�8�Σ��������ܶ���һ���ֽ��ˡ�
uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);//   �����ͷ�SDA
    for(i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);//   �����ͷ�SCL
        if(MyI2C_R_SDA() == 1)//    ������ȡ����
        {
            Byte |= (0x80 >> i);// �����ȡ��1���ʹӸߵ��Ͱ�Byte��Ӧλ��1
        }
        MyI2C_W_SCL(0);//   ��ȡ1λ������SCL����ʱ�ӻ������һλ���ݷŵ�SDA��
    }
    return Byte;
}
 
//  5������Ӧ��
//  ��ʵ���Ƿ���һ���ֽڵļ�
void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);//  ��������ʱ��SCL�͵�ƽ��������AckBit�ŵ�SDA��
    MyI2C_W_SCL(1);//   ���ͷ�SCL���ӻ���ȡӦ��
    MyI2C_W_SCL(0);//   ����SCL��������һ��ʱ��Ԫ
}
 
//  6������Ӧ��
//  ��ʵ���ǽ���һ���ֽڵļ�
uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);//   ��������ʱSCL�͵�ƽ�������ͷ�SDA��ֹ���Ŵӻ���ͬʱ���ӻ���Ӧ��λ����SDA��
    MyI2C_W_SCL(1);//   �����ͷ�SCL����ȡӦ��λ
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);//   ��ȡ������SCL��������һ��ʱ��Ԫ
    return AckBit;
}


 
// ָ����ַд
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);// ���ʹӻ���ַ+��дλ
    MyI2C_ReceiveAck();//   ����Ӧ��
    MyI2C_SendByte(RegAddress);//   ָ���Ĵ�����ַ
    MyI2C_ReceiveAck();
    MyI2C_SendByte(Data);//   ����д��ָ���Ĵ�����ַ�µ�����
    MyI2C_ReceiveAck();
    MyI2C_Stop();
}
 
//  ָ����ַ��
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data;
    
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);// ���ʹӻ���ַ+��дλ
    MyI2C_ReceiveAck();//   ����Ӧ��
    MyI2C_SendByte(RegAddress);//   ָ���Ĵ�����ַ
    MyI2C_ReceiveAck();
    
    MyI2C_Start();//    �ظ���ʼ����
    MyI2C_SendByte(MPU6050_ADDRESS | 0xD1);//   ��дλ1����������
    MyI2C_ReceiveAck();
    Data = MyI2C_ReceiveByte();//   �ӻ��������ݣ�������������
    MyI2C_SendAck(1);//  ����ֻ��Ҫ1���ֽڣ����Բ����ӻ�Ӧ��
    MyI2C_Stop();
    
    return Data;
}
 
void MPU6050_Init(void)
{
    MyI2C_Init();
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);//   ����PWR_MGMT_1�Ĵ��������˯�ߣ�ѡ��X��������ʱ��
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);//   �����ʷ�Ƶ����������������Ŀ�����10��Ƶ
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);//   ���ֵ�ͨ�˲�����110
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);//  ���������ã�ѡ���������11
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);// ���ٶȼ����ã�ѡ���������11
    
}
 
uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}
 
//  ������Ҫ����6��������ʹ��ָ��ĵ�ַ����
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                        int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
    uint8_t Data_H, Data_L;
    
    Data_H = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);//    ���ٶȼĴ���X���8λ
    Data_L = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    *AccX = (Data_H << 8) | Data_L;//   ���ٶȼ�X���16λ����
    
    Data_H = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    Data_L = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (Data_H << 8) | Data_L;
    
    Data_H = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    Data_L = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (Data_H << 8) | Data_L;
    
    Data_H = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);//    �����ǼĴ���X���8λ
    Data_L = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    *GyroX = (Data_H << 8) | Data_L;//   ������X���16λ����
    
    Data_H = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    Data_L = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (Data_H << 8) | Data_L;
    
    Data_H = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    Data_L = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (Data_H << 8) | Data_L;
}
