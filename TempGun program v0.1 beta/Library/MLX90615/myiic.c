#include "myiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//初始化IIC
void IIC_Init(void)
{					     
	P13_Quasi_Mode; IIC_SCL = 1; //IIC_SCL
  	P14_Quasi_Mode; IIC_SDA = 1; //IIC_SDA
}
// //产生IIC起始信号
// void IIC_Start(void)
// {
// 	SDA_OUT();     //sda线输出
// 	IIC_SDA=1;	  	  
// 	IIC_SCL=1;
// 	delay_us(8);
//  	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
// 	delay_us(8);
// 	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
// }	  
// //产生IIC停止信号
// void IIC_Stop(void)
// {
// 	SDA_OUT();//sda线输出
// 	IIC_SCL=0;
// 	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
//  	delay_us(8);
// 	IIC_SCL=1; 
// 	IIC_SDA=1;//发送I2C总线结束信号
// 	delay_us(8);							   	
// }
// //等待应答信号到来
// //返回值：1，接收应答失败
// //        0，接收应答成功
// u8 IIC_Wait_Ack(void)
// {
// 	u8 ucErrTime=0;
// 	SDA_IN();      //SDA设置为输入  
// 	IIC_SDA=1;delay_us(2);	   
// 	IIC_SCL=1;delay_us(2);	 
// 	while(READ_SDA)
// 	{
// 		ucErrTime++;
// 		if(ucErrTime>250)
// 		{
// 			IIC_Stop();
// 			return 1;
// 		}
// 	}
// 	IIC_SCL=0;//时钟输出0 	   
// 	return 0;  
// } 
// //产生ACK应答
// void IIC_Ack(void)
// {
// 	IIC_SCL=0;
// 	SDA_OUT();
// 	IIC_SDA=0;
// 	delay_us(4);
// 	IIC_SCL=1;
// 	delay_us(4);
// 	IIC_SCL=0;
// }
// //不产生ACK应答		    
// void IIC_NAck(void)
// {
// 	IIC_SCL=0;
// 	SDA_OUT();
// 	IIC_SDA=1;
// 	delay_us(4);
// 	IIC_SCL=1;
// 	delay_us(4);
// 	IIC_SCL=0;
// }					 				     
// //IIC发送一个字节
// //返回从机有无应答
// //1，有应答
// //0，无应答			  
// void IIC_Send_Byte(u8 txd)
// {                        
//     u8 t;   
// 	SDA_OUT(); 	    
//     IIC_SCL=0;//拉低时钟开始数据传输
//     for(t=0;t<8;t++)
//     {              
//         //IIC_SDA=(txd&0x80)>>7;
// 		if((txd&0x80)>>7)
// 			IIC_SDA=1;
// 		else
// 			IIC_SDA=0;
// 		txd<<=1; 	  
// 		delay_us(4);   //对TEA5767这三个延时都是必须的
// 		IIC_SCL=1;
// 		delay_us(4); 
// 		IIC_SCL=0;	
// 		delay_us(4);
//     }	 
// } 	    
// //读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
// u8 IIC_Read_Byte(unsigned char ack)
// {
// 	unsigned char i,receive=0;
// 	SDA_IN();//SDA设置为输入
//     for(i=0;i<8;i++ )
// 	{
//         IIC_SCL=0; 
//         delay_us(4);
// 		IIC_SCL=1;
//         receive<<=1;
//         if(READ_SDA)receive++;   
// 		delay_us(2); 
//     }					 
//     if (!ack)
//         IIC_NAck();//发送nACK
//     else
//         IIC_Ack(); //发送ACK   
//     return receive;
// }


 /*************************此部分为I2C总线的驱动程序*************************************/
/*******************************************************************
                     起动总线函数               
函数原型: void  Start_I2c();  
功能:     启动I2C总线,即发送I2C起始条件.  
********************************************************************/
void IIC_Start(void)
{
  IIC_SDA=1;         /*发送起始条件的数据信号*/
  nop
  IIC_SCL=1;
  nop        /*起始条件建立时间大于4.7us,延时*/
  nop
  nop
  nop
  nop    
  IIC_SDA=0;         /*发送起始信号*/
  nop        /* 起始条件锁定时间大于4μs*/
  nop
  nop
  nop
  nop       
  IIC_SCL=0;       /*钳住I2C总线，准备发送或接收数据 */
  nop
  nop
}

/*******************************************************************
                      结束总线函数               
函数原型: void  Stop_I2c();  
功能:     结束I2C总线,即发送I2C结束条件.  
********************************************************************/
void IIC_Stop(void)
{
  IIC_SDA=0;      /*发送结束条件的数据信号*/
  nop       /*发送结束条件的时钟信号*/
  IIC_SCL=1;      /*结束条件建立时间大于4μs*/
  nop
  nop
  nop
  nop
  nop
  IIC_SDA=1;      /*发送I2C总线结束信号*/
  nop
  nop
  nop
  nop
}

/*******************************************************************
                 字节数据发送函数               
函数原型: void  SendByte(UCHAR c);
功能:     将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
          此状态位进行操作.(不应答或非应答都使ack=0)     
           发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
void  IIC_Send_Byte(unsigned char  c)
{
 unsigned char  BitCnt;
 
 for(BitCnt=0;BitCnt<8;BitCnt++)  /*要传送的数据长度为8位*/
    {
     if((c<<BitCnt)&0x80)IIC_SDA=1;   /*判断发送位*/
       else  IIC_SDA=0;                
     nop
     IIC_SCL=1;               /*置时钟线为高，通知被控器开始接收数据位*/
      nop 
      nop             /*保证时钟高电平周期大于4μs*/
      nop
      nop
      nop         
     IIC_SCL=0; 
    }
    
    nop
    nop
    IIC_SDA=1;                /*8位发送完后释放数据线，准备接收应答位*/
    nop
    nop   
    IIC_SCL=1;
    nop
    nop
    nop
    // if(IIC_SDA==1)ack=0;     
    //    else ack=1;        /*判断是否接收到应答信号*/
    IIC_SCL=0;
    nop
    nop
}

/*******************************************************************
                 字节数据接收函数               
函数原型: UCHAR  RcvByte();
功能:        用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
          发完后请用应答函数应答从机。  
********************************************************************/    
unsigned char   IIC_Read_Byte(unsigned char ack)
{
  unsigned char  retc;
  unsigned char  BitCnt;
  
  retc=0; 
  IIC_SDA=1;                       /*置数据线为输入方式*/
  for(BitCnt=0;BitCnt<8;BitCnt++)
	{
	nop           
	IIC_SCL=0;                  /*置时钟线为低，准备接收数据位*/
	nop
	nop                 /*时钟低电平周期大于4.7μs*/
	nop
	nop
	nop
	IIC_SCL=1;                  /*置时钟线为高使数据线上数据有效*/
	nop
	nop
	retc=retc<<1;
	if(IIC_SDA==1)retc=retc+1;  /*读数据位,接收的数据位放入retc中 */
	nop
	nop 
	}
  IIC_SCL=0;    
  nop
  nop
  return(retc);
}

/********************************************************************
                     应答子函数
函数原型:  void Ack_I2c(bit a);
功能:      主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
********************************************************************/
void IIC_Ack(void)
{
	unsigned char a = 1;
  if(a==0)IIC_SDA=0;              /*在此发出应答或非应答信号 */
  else IIC_SDA=1;
  nop
  nop
  nop      
  IIC_SCL=1;
  nop
  nop                    /*时钟低电平周期大于4μs*/
  nop
  nop
  nop  
  IIC_SCL=0;                     /*清时钟线，钳住I2C总线以便继续接收*/
  nop
  nop    
}
