#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "stmflash.h"   
#include "iap.h"

// 例程使用的是stm32f103rct6  实际应用平台为 stm32f103rbt6 二者区别主要为前者是256Kbd大容量产品 后者为128Kb中容量产品

// FLASH分区: 0X0800 0000~0x0800 4000,给IAP使用,共16K字节,FLASH APP还有128-16=112KB可用.

int main(void)
 {
	u16 t = 0;						//用于计时
	u8 key;
	u16 oldcount=0;	//老的串口接收数据值
	u8 applenth=0;	//接收到的app代码长度
	 
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  		//初始化与LED连接的硬件接口
 	KEY_Init();				//按键初始化 	
	 
	uart_init(9600);	 	//串口初始化为9600
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	 
	printf("begin to the loop");
	
	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount == USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("用户程序接收完成!\r\n");
				printf("代码长度:%dBytes\r\n",applenth);
			}
			else oldcount=USART_RX_CNT;			
		}

		t++;
		delay_ms(10);
		if(t % 30 == 0)
		{
			LED0 = !LED0;
			printf("the system is running\r\n");
			t = 0;
		}	  	 
		
		key=KEY_Scan(0);
		
		if(key==WKUP_PRES)			//WK_UP按键按下  更新flash程序
		{
			if(applenth)
			{
				printf("开始更新固件...\r\n");	
 				if( ( ( *(vu32*)(0X20001000+4) ) & 0xFF000000) == 0x08000000)//判断是否为0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//更新FLASH代码   
					printf("固件更新完成!\r\n");	
				}else 
				{ 
					printf("非FLASH应用程序!\r\n");
				}
 			}else 
			{
				printf("没有可以更新的固件!\r\n");
			}								 
		} 
		
		if(key==KEY1_PRES)			//KEY1按键按下  执行新的flash程序
		{
			printf("开始执行FLASH用户代码!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
			}else 
			{
				printf("非FLASH应用程序,无法执行!\r\n");
			}									 
		}		   
		 
	}
}

/*
int main(void){
	delay_init();
	LED_Init();
	while(1){
		LED0 = 0;
		delay_ms(300);
		LED0 =1;
		delay_ms(300);
	}
}
*/
