#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f10x.h"
#include "sys1.h"
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

#define KEY0_PRES	1		//KEY0按下
#define KEY1_PRES	2		//KEY1按下
#define WKUP_PRES	3		//WK_UP按下

#define KEY0  PCin(5)   	//PC5
#define KEY1  PAin(15)	 	//PA15 
#define WK_UP PAin(0)	 	//PA0  WK_UP
	 
void KEY_Init(void);		//IO初始化
u8 KEY_Scan(u8 mode);		//按键扫描函数
#endif
