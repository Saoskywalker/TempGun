#include "myiic.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 
//��ʼ��IIC
void IIC_Init(void)
{					     
	P13_Quasi_Mode; IIC_SCL = 1; //IIC_SCL
  	P14_Quasi_Mode; IIC_SDA = 1; //IIC_SDA
}
// //����IIC��ʼ�ź�
// void IIC_Start(void)
// {
// 	SDA_OUT();     //sda�����
// 	IIC_SDA=1;	  	  
// 	IIC_SCL=1;
// 	delay_us(8);
//  	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
// 	delay_us(8);
// 	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
// }	  
// //����IICֹͣ�ź�
// void IIC_Stop(void)
// {
// 	SDA_OUT();//sda�����
// 	IIC_SCL=0;
// 	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
//  	delay_us(8);
// 	IIC_SCL=1; 
// 	IIC_SDA=1;//����I2C���߽����ź�
// 	delay_us(8);							   	
// }
// //�ȴ�Ӧ���źŵ���
// //����ֵ��1������Ӧ��ʧ��
// //        0������Ӧ��ɹ�
// u8 IIC_Wait_Ack(void)
// {
// 	u8 ucErrTime=0;
// 	SDA_IN();      //SDA����Ϊ����  
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
// 	IIC_SCL=0;//ʱ�����0 	   
// 	return 0;  
// } 
// //����ACKӦ��
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
// //������ACKӦ��		    
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
// //IIC����һ���ֽ�
// //���شӻ�����Ӧ��
// //1����Ӧ��
// //0����Ӧ��			  
// void IIC_Send_Byte(u8 txd)
// {                        
//     u8 t;   
// 	SDA_OUT(); 	    
//     IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
//     for(t=0;t<8;t++)
//     {              
//         //IIC_SDA=(txd&0x80)>>7;
// 		if((txd&0x80)>>7)
// 			IIC_SDA=1;
// 		else
// 			IIC_SDA=0;
// 		txd<<=1; 	  
// 		delay_us(4);   //��TEA5767��������ʱ���Ǳ����
// 		IIC_SCL=1;
// 		delay_us(4); 
// 		IIC_SCL=0;	
// 		delay_us(4);
//     }	 
// } 	    
// //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
// u8 IIC_Read_Byte(unsigned char ack)
// {
// 	unsigned char i,receive=0;
// 	SDA_IN();//SDA����Ϊ����
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
//         IIC_NAck();//����nACK
//     else
//         IIC_Ack(); //����ACK   
//     return receive;
// }


 /*************************�˲���ΪI2C���ߵ���������*************************************/
/*******************************************************************
                     �����ߺ���               
����ԭ��: void  Start_I2c();  
����:     ����I2C����,������I2C��ʼ����.  
********************************************************************/
void IIC_Start(void)
{
  IIC_SDA=1;         /*������ʼ�����������ź�*/
  nop
  IIC_SCL=1;
  nop        /*��ʼ��������ʱ�����4.7us,��ʱ*/
  nop
  nop
  nop
  nop    
  IIC_SDA=0;         /*������ʼ�ź�*/
  nop        /* ��ʼ��������ʱ�����4��s*/
  nop
  nop
  nop
  nop       
  IIC_SCL=0;       /*ǯסI2C���ߣ�׼�����ͻ�������� */
  nop
  nop
}

/*******************************************************************
                      �������ߺ���               
����ԭ��: void  Stop_I2c();  
����:     ����I2C����,������I2C��������.  
********************************************************************/
void IIC_Stop(void)
{
  IIC_SDA=0;      /*���ͽ��������������ź�*/
  nop       /*���ͽ���������ʱ���ź�*/
  IIC_SCL=1;      /*������������ʱ�����4��s*/
  nop
  nop
  nop
  nop
  nop
  IIC_SDA=1;      /*����I2C���߽����ź�*/
  nop
  nop
  nop
  nop
}

/*******************************************************************
                 �ֽ����ݷ��ͺ���               
����ԭ��: void  SendByte(UCHAR c);
����:     ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
          ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0)     
           ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
********************************************************************/
void  IIC_Send_Byte(unsigned char  c)
{
 unsigned char  BitCnt;
 
 for(BitCnt=0;BitCnt<8;BitCnt++)  /*Ҫ���͵����ݳ���Ϊ8λ*/
    {
     if((c<<BitCnt)&0x80)IIC_SDA=1;   /*�жϷ���λ*/
       else  IIC_SDA=0;                
     nop
     IIC_SCL=1;               /*��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ*/
      nop 
      nop             /*��֤ʱ�Ӹߵ�ƽ���ڴ���4��s*/
      nop
      nop
      nop         
     IIC_SCL=0; 
    }
    
    nop
    nop
    IIC_SDA=1;                /*8λ��������ͷ������ߣ�׼������Ӧ��λ*/
    nop
    nop   
    IIC_SCL=1;
    nop
    nop
    nop
    // if(IIC_SDA==1)ack=0;     
    //    else ack=1;        /*�ж��Ƿ���յ�Ӧ���ź�*/
    IIC_SCL=0;
    nop
    nop
}

/*******************************************************************
                 �ֽ����ݽ��պ���               
����ԭ��: UCHAR  RcvByte();
����:        �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
          ���������Ӧ����Ӧ��ӻ���  
********************************************************************/    
unsigned char   IIC_Read_Byte(unsigned char ack)
{
  unsigned char  retc;
  unsigned char  BitCnt;
  
  retc=0; 
  IIC_SDA=1;                       /*��������Ϊ���뷽ʽ*/
  for(BitCnt=0;BitCnt<8;BitCnt++)
	{
	nop           
	IIC_SCL=0;                  /*��ʱ����Ϊ�ͣ�׼����������λ*/
	nop
	nop                 /*ʱ�ӵ͵�ƽ���ڴ���4.7��s*/
	nop
	nop
	nop
	IIC_SCL=1;                  /*��ʱ����Ϊ��ʹ��������������Ч*/
	nop
	nop
	retc=retc<<1;
	if(IIC_SDA==1)retc=retc+1;  /*������λ,���յ�����λ����retc�� */
	nop
	nop 
	}
  IIC_SCL=0;    
  nop
  nop
  return(retc);
}

/********************************************************************
                     Ӧ���Ӻ���
����ԭ��:  void Ack_I2c(bit a);
����:      ����������Ӧ���ź�(������Ӧ����Ӧ���źţ���λ����a����)
********************************************************************/
void IIC_Ack(void)
{
	unsigned char a = 1;
  if(a==0)IIC_SDA=0;              /*�ڴ˷���Ӧ����Ӧ���ź� */
  else IIC_SDA=1;
  nop
  nop
  nop      
  IIC_SCL=1;
  nop
  nop                    /*ʱ�ӵ͵�ƽ���ڴ���4��s*/
  nop
  nop
  nop  
  IIC_SCL=0;                     /*��ʱ���ߣ�ǯסI2C�����Ա��������*/
  nop
  nop    
}
