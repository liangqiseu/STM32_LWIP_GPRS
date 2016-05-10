/***********************BSP**************************/
#include "led.h"
#include "NVIC.h"
#include "usart1.h"
#include "usart2.h"
#include "Timer.h"
#include "string.h"
#include "key.h"
#include "delay.h"

/**********************LWIP***********************/
#include "ppp.h"
#include "init.h"
#include "lwip/inet.h"
#include "tcp.h"

/******************UC/OS**************************/
//#include "usos-ii.h"


#define LWIP_DEBUG

//AT命令集
 char AT_AT[]="AT\r\n";
 char AT_CGCLASS[]="AT+CGCLASS=\"B\"\r\n";
 char AT_CGDOCNT[]="AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n";
 char AT_ATD[]="ATD*99#\r\n";
 char PPP_LCP_RE[]={0X7E,0XFF,0X7D,0X23,0XC0,0X21,0X7D,0X21,0X7D,0X21,0X7D,\
										0X20,0X7D,0X2A,0X7D,0X22,0X7D,0X26,0X7D,0X20,0X7D,0X2A,\
										0X7D,0X20,0X7D,0X20,0X22,0X7D,0X28,0X7E};
 
 char PPP_LCP_ACK[]={0X7E,0XFF,0X7D,0X23,0XC0,0X21,0X7D,0X22,0X7D,0X21,0X7D,\
										 0X2E,0X7D,0X22,0X7D,0X26,0X7D,0X20,0X7D,0X2A,0X7D,0X20,\
										 0X7D,0X20,0X7D,0X23,0X7D,0X24,0XC0,0X23,0XCD,0XCE,0X7E};

char PPP_IPCP_REQ[]={0X7E,0XFF,0X03,0X80,0X21,0X01,0X02,0X00,0X0A,0X03,0X06,\
											0X00,0X00,0X00,0X00,0X14,0XFE,0X7E};
										 

const static char TCP_TestData[]="This is LwIP TCP Client test\r\n";		

//PPP回调函数											
static void linkStatusCB(void* ctx, int errCode, void* arg);				
void TCP_Client_Init();
				

	unsigned int usart2_flag=0;
	int pd; //PPP连接句柄
	unsigned int LED1_flag=0;
										 
 int main(void)
 {	
	sio_fd_t usart1; 
	int connected = 0;
	volatile int setup = 0;
	
	 

	delay_init(72);
	NVIC_Configuration();// 设置中断优先级分组
	usart1_init(9600);	 //串口初始化为9600
	usart2_init(115200);  //usart2的初始化包含了定时器TIM4的初始化
	LED_Init();		  	 //初始化与LED连接的硬件接口 
	KEY_Init();          	//初始化与按键连接的硬件接口
  EXIT_init();         //初始化EXIT外部中断。这里用的是软件中断
	TIM3_Int_Init();   //10ms中断一次，lwip_time加1
	TIM4_Init(99,7199);		//10ms中断
	TIM4_Set(0);			//关闭定时器4
	USART1_RX_STA=0;
	USART2_RX_STA=0;
	
 //在使用pengding位之前清除很重要，不然会有莫名其妙的错误，CM3权威指南诚不欺我也~	 
	EXTI_ClearITPendingBit(EXTI_Line1);  
	//EXTI_ClearITPendingBit(EXTI_Line2);    //这个EXIT2是不是有毒啊，USART2发送数据之后总是会自动进入中断
																					 //可能是和某个IO口冲突了
	EXTI_ClearITPendingBit(EXTI_Line3);
	EXTI_ClearITPendingBit(EXTI_Line4);
	
	lwip_init();
	pppInit();
	
	

		while(KEY_Scan(0)!=KEY0_PRES); 
		delay_ms(2000);
		USART2_SEND(AT_AT,strlen(AT_AT));
		delay_ms(200);   //200ms比较合适 太短有问题
		USART2_SEND(AT_AT,strlen(AT_AT));
		delay_ms(200);
		USART2_SEND(AT_CGCLASS,strlen(AT_CGCLASS));
		delay_ms(200);
		USART2_SEND(AT_CGDOCNT,strlen(AT_CGDOCNT));
		delay_ms(200);
		USART2_SEND(AT_ATD,strlen(AT_ATD));
		delay_ms(300);

	
		//按下之后进行PPP连接
		while(KEY_Scan(0)!=KEY0_PRES);  
		delay_ms(1000);	
		usart2_flag=1; //USART2的数据同时发到PC和LWIP协议栈
		pppSetAuth(PPPAUTHTYPE_ANY,NULL,NULL);
		pd=pppOverSerialOpen(usart1, linkStatusCB, &connected);
		
		
		delay_ms(300);
		
    if (pd >= 0) {
			
      // the thread was successfully started.
      while (!connected ) 
			{
				//waitiing;
			}
			TCP_Client_Init();
			while(1)
			{
				/* create some socket connections, 
         * do writes and reads on the sockets, close them, etc */
			}
        
     
				
		  /* calling pppClose will end the pppInputThread associated with pd*/
      pppClose(pd);
    }
}
 

//PPP连接成功的回调函数
static void linkStatusCB(void *ctx, int errCode, void *arg) {
   int *connected = (int *) ctx;

   struct ppp_addrs *addrs = arg;
	 addrs=addrs;
	 *connected = 1;

   if (errCode ==PPPERR_NONE) {
       /* We are connected */
       *connected = 1;
				LED1_flag=1;
//      syslog(LOG_DEBUG, "ip_addr = %s", inet_ntoa(addrs->our_ipaddr));
//       syslog(LOG_DEBUG, "netmask = %s", inet_ntoa(addrs->netmask));
//       syslog(LOG_DEBUG, "dns1    = %s", inet_ntoa(addrs->dns1));
//       syslog(LOG_DEBUG, "dns2    = %s", inet_ntoa(addrs->dns2));
   } else {
       /* We have lost connection */
   }
}

//TCP连接成功的回调函数
err_t TcpCli_Connected(void *arg,struct tcp_pcb *pcb,err_t err)
{
tcp_write(pcb,TCP_TestData,sizeof(TCP_TestData),0); 
tcp_close(pcb);
return ERR_OK;
}

void TCP_Client_Init()
{
struct tcp_pcb *Clipcb;
struct ip_addr ipaddr;
IP4_ADDR(&ipaddr,121,42,49,56); // 绑定远程主机IP
Clipcb = tcp_new();  //建立通信的TCP控制块
tcp_bind(Clipcb,IP_ADDR_ANY,8000);  //绑定本地地址和端口号
tcp_connect(Clipcb,&ipaddr,8080,TcpCli_Connected);
}


