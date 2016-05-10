#include "Timer.h"
#include "led.h"
#include "usart2.h"
#include <stdio.h>

extern int pd;
extern unsigned int usart2_flag;
extern unsigned int LED1_flag;

u32 len,t;
int ttt=0;
u32 lwip_time=0; //自系统启动就计时的时间，每10毫秒加1


//定时器3中断服务程序		    
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			lwip_time++;
			if(lwip_time%100==0)
			{
				LED0=!LED0;
				if(LED1_flag==1)
				{	
					LED1=!LED1;
				}
			}
		}
}

//定时器4中断服务程序		    
void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0X01)//是更新中断
	{	 			   
		if(USART2_RX_STA!=0)     //说明是USART2在接受数据
		{
			USART2_RX_STA|=1<<15;	//标记接收完成
			USART2_RX_STA_BACKUP=USART2_RX_STA;
			TIM4->SR&=~(1<<0);		//清除中断标志位		   
			TIM4_Set(0);			//关闭TIM4  
			
			len=USART2_RX_STA_BACKUP&0x3fff;
			for(t=0;t<len;t++)
			{
				USART1->DR=USART2_RX_BUF[t];
				
				while((USART1->SR&0X40)==0);//等待发送结束
			}	
			USART2_RX_STA=0;			

			if(usart2_flag==1)
			{
					EXTI_GenerateSWInterrupt(EXTI_Line4);  //开启软件中断 调用pppos_input处理数据
			}
			
		}
		else  //否则就是USART1在接受数据
		{
			USART2_RX_STA|=0;	//标记接收完成			
			TIM4->SR&=~(1<<0);		//清除中断标志位		   
			TIM4_Set(0);			//关闭TIM4  
		}
	}	    
}
//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM4_Set(u8 sta)
{
	if(sta)
	{
    	TIM4->CNT=0;         //计数器清空
		TIM4->CR1|=1<<0;     //使能定时器4
	}else TIM4->CR1&=~(1<<0);//关闭定时器4	   
}
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM4_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC->APB1ENR|=1<<2;	//TIM4时钟使能    
 	TIM4->ARR=arr;  	//设定计数器自动重装值   
	TIM4->PSC=psc;  	//预分频器
 	TIM4->DIER|=1<<0;   //允许更新中断				
 	TIM4->CR1|=0x01;  	//使能定时器4	  	   
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器						 
}


//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = 99; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM3
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}

//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM2_Set(u8 sta)
{
	if(sta)
	{
    	TIM2->CNT=0;         //计数器清空
		TIM2->CR1|=1<<0;     //使能定时器4
	}else TIM2->CR1&=~(1<<0);//关闭定时器4	   
}
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM2_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC->APB1ENR|=1<<2;	//TIM4时钟使能    
 	TIM2->ARR=arr;  	//设定计数器自动重装值   
	TIM2->PSC=psc;  	//预分频器
 	TIM2->DIER|=1<<0;   //允许更新中断				
 	TIM2->CR1|=0x01;  	//使能定时器4	  	   
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器						 
}


