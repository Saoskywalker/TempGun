#include "mlx90615.h"
#include "myiic.h"

#define SLAVE_ADDR 0x00 // MLX90615Ĭ�ϵ�ַ
#define IIC_WRITE 0x00  // д
#define IIC_READ 0x01   // ��
#define RAM_TA 0x26		// �����¶�RAM��ַ
#define RAM_TO 0x27		// Ŀ���¶�RAM��ַ

void mlx90615Init(void)
{
	IIC_Init();
}

// ��ȡһ����(�����ֽ�)
u16 mlxReadWord(u8 ramAddr)
{
	u16 temp = 0;
	u8 buff[3] = {0, 0, 0};

	IIC_Start();
	IIC_Send_Byte(SLAVE_ADDR + IIC_WRITE); //����������ַ0X00,д����
	IIC_Wait_Ack();						   //
	IIC_Send_Byte(ramAddr);				   // 0x26�����¶ȣ�0x27�������¶�
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(SLAVE_ADDR + IIC_READ); //�������ģʽ
	IIC_Wait_Ack();
	buff[0] = IIC_Read_Byte(1); // ����ACK;
	buff[1] = IIC_Read_Byte(1); // ����ACK;
	buff[2] = IIC_Read_Byte(0); // ������ACK;  PEC У����
	IIC_Stop();					//����һ��ֹͣ����

	temp = (u16)buff[1] * 256 + buff[0];
	return temp;
}

// ��RAM����ת��Ϊ��ʽ�¶����ݡ�����Ŵ�10λ������ֵ���һλΪС����
u16 mlxConvert(u16 ram)
{
	ram /= 5;
	ram -= 2731; // TO[��] = RAM(7h)*0.02-273.15
	return ram;
}

// ��ȡ�����¶ȡ�
u16 mlxGetTaTemp(void)
{
	u16 temp = 0;
	temp = mlxReadWord(RAM_TA);
	temp = mlxConvert(temp);
	return temp;
}

// ��ȡ����Ŀ���¶ȡ�
u16 mlxGetToTemp(void)
{
	u16 temp = 0;
	temp = mlxReadWord(RAM_TO);
	temp = mlxConvert(temp);
	return temp;
}
