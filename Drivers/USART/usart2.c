#include "Timer.h"
#include "usart2.h" 
#include "stm32f10x.h"
#include "string.h"
#include "led.h"
#include "delay.h"
 

//串口发送缓存区 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	//发送缓冲,最大USART2_MAX_SEND_LEN字节
 	  
//串口接收缓存区 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u8 USART2_SEND_LEN;
unsigned  char * p_usart2_send_data=NULL;



//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART2_RX_STA=0;   	 
u16 USART2_RX_STA_BACKUP=0;   	

//USART2中断处理函数 函数名是规定好的，不可以更改
void USART2_IRQHandler(void)
{
	u8 res;	
	if(USART2->SR&(1<<5))//接收到数据  位5是RXNE，标志着是否有数据收到
	{	 
		res=USART2->DR; 			 
		if(USART2_RX_STA<USART2_MAX_RECV_LEN)		//还可以接收数据
		{
			TIM4->CNT=0;         					//计数器清空
			if(USART2_RX_STA==0) TIM4_Set(1);	 	//使能定时器4的中断 
			USART2_RX_BUF[USART2_RX_STA++]=res;		//记录接收到的值	 
		}else 
		{
			USART2_RX_STA|=1<<15;					//强制标记接收完成
			//EXTI_GenerateSWInterrupt(EXTI_Line1);  //开启软件中断
		} 
	}  											 
}


//USART2发送数据函数
void USART2_SEND( char *p_data, unsigned int len)
{
	u8 t;
	char *pdata;
	pdata=p_data;
	for(t=0;t<len;t++)
	{
			USART2->DR=*pdata;
			pdata++;
			while((USART2->SR&0X40)==0);//等待发送结束
	}	
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//初始化IO 串口2
//
//bound:波特率	  
void usart2_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	 
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART1，GPIOA时钟
 	  USART_DeInit(USART2);  //复位串口1
	 //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
   
    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3

   //Usart2 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级2
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级2
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	 USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	 USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	 USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	 USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

   USART_Init(USART2, &USART_InitStructure); //初始化串口
   USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
   USART_Cmd(USART2, ENABLE);                    //使能串口 
		
		
}


