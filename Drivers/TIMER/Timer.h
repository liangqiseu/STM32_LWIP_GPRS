#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"

extern u16 USART2_RX_STA;
extern u16 USART1_RX_STA;
extern u32 lwip_time; //自系统启动就计时的时间，每10毫秒加1

void TIM3_Int_Init(void);
//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM4_Set(u8 sta);

//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM4_Init(u16 arr,u16 psc);

void TIM2_Init(u16 arr,u16 psc);		//10ms中断
void TIM2_Set(u8 sta);			//关闭定时器4
#endif
