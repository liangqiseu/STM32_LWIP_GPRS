#include "NVIC.h"
#include "usart1.h"
#include "usart2.h"
#include "led.h"
#include "delay.h"
#include "stm32f10x.h"
#include "ppp.h"

extern unsigned char AT_AT;
extern int pd;  //在main函数处定义
int time=0;

void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//2bit抢占优先级，2bit子优先级

}

//软件中断 使用中断4
//该中断函数用于将数据通过USART2发送到SIM900模块
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4) != RESET)   
    {
				EXTI_ClearITPendingBit(EXTI_Line4);
				pppos_input(pd,USART2_RX_BUF,USART2_RX_STA_BACKUP&0x3fff);
		}
}


//LED闪烁
void EXTI3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line3);
			//	LED0=!LED0;
    }
}


//void EXTI2_IRQHandler(void)
//{
//    if(EXTI_GetITStatus(EXTI_Line2) != RESET)
//    {
//            EXTI_ClearITPendingBit(EXTI_Line2);
//    }
////		LED0=!LED0;
//}


//USART1向PC发送数据
void EXTI1_IRQHandler(void)
{
			u8 t;
	//LED1=!LED1;
    if(EXTI_GetITStatus(EXTI_Line1) != RESET)   
    {
			EXTI_ClearITPendingBit(EXTI_Line1);  //清除挂起位
			
			//发送数据
			//p_usart1_send_data=&AT_AT;
			p_usart1_send_data=USART2_RX_BUF;
			USART1_SEND_LEN=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<USART1_SEND_LEN;t++)
			{
				USART1->DR=*p_usart1_send_data;
				p_usart1_send_data++;
				while((USART1->SR&0X40)==0);//等待发送结束
			}			
			USART2_RX_STA=0;
			USART1_SEND_LEN=0;
			p_usart1_send_data=NULL;
		}	
	
}

//软件中断配置函数
void EXIT_init()
{
  EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//配置软件中断
  EXTI_InitStructure.EXTI_Line=EXTI_Line4|EXTI_Line3|EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd=ENABLE;
  EXTI_Init(&EXTI_InitStructure);  	
	
 
	
  
 //已经配置为2bit抢占优先级，2bit子优先级
  NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
  NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel=EXTI2_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
//  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

