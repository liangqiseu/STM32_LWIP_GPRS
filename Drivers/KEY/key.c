#include "key.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//按键输入 驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/06
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									   
//////////////////////////////////////////////////////////////////////////////////	 
 	    
//按键初始化函数 
//PA0.15和PC5 设置成输入
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;     //使能PORTA时钟
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟
	//JTAG_Set(SWD_ENABLE);	//关闭JTAG,开启SWD
	GPIOA->CRL&=0XFFFFFFF0;	//PA0设置成输入	  
	GPIOA->CRL|=0X00000008;   
	GPIOA->CRH&=0X0FFFFFFF;	//PA15设置成输入	  
	GPIOA->CRH|=0X80000000; 			 
	GPIOA->ODR|=1<<15;	   	//PA15上拉,PA0默认下拉
	GPIOC->CRL&=0XFF0FFFFF;	//PC5设置成输入	  
	GPIOC->CRL|=0X00800000;   
	GPIOC->ODR|=1<<5;	   	//PC5上拉 
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES; 
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	     
	return 0;// 无按键按下
}
