#include "mlx90615.h"
#include "myiic.h"

#define SLAVE_ADDR 0x00 // MLX90615默认地址
#define IIC_WRITE 0x00  // 写
#define IIC_READ 0x01   // 读
#define RAM_TA 0x26		// 环境温度RAM地址
#define RAM_TO 0x27		// 目标温度RAM地址

void mlx90615Init(void)
{
	IIC_Init();
}

// 读取一个字(两个字节)
u16 mlxReadWord(u8 ramAddr)
{
	u16 temp = 0;
	u8 buff[3] = {0, 0, 0};

	IIC_Start();
	IIC_Send_Byte(SLAVE_ADDR + IIC_WRITE); //发送器件地址0X00,写数据
	IIC_Wait_Ack();						   //
	IIC_Send_Byte(ramAddr);				   // 0x26环境温度，0x27被测物温度
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(SLAVE_ADDR + IIC_READ); //进入接收模式
	IIC_Wait_Ack();
	buff[0] = IIC_Read_Byte(1); // 发送ACK;
	buff[1] = IIC_Read_Byte(1); // 发送ACK;
	buff[2] = IIC_Read_Byte(0); // 不发送ACK;  PEC 校验码
	IIC_Stop();					//产生一个停止条件

	temp = (u16)buff[1] * 256 + buff[0];
	return temp;
}

// 将RAM数据转换为摄式温度数据。结果放大10位，返回值最后一位为小数。
u16 mlxConvert(u16 ram)
{
	ram /= 5;
	ram -= 2731; // TO[℃] = RAM(7h)*0.02-273.15
	return ram;
}

// 获取环境温度。
u16 mlxGetTaTemp(void)
{
	u16 temp = 0;
	temp = mlxReadWord(RAM_TA);
	temp = mlxConvert(temp);
	return temp;
}

// 获取被测目标温度。
u16 mlxGetToTemp(void)
{
	u16 temp = 0;
	temp = mlxReadWord(RAM_TO);
	temp = mlxConvert(temp);
	return temp;
}
