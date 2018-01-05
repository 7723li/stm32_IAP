#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usmart.h"
#include "rtc.h"

 int main(void)
 { 
	int blink = 10;
	 
  SCB->VTOR = FLASH_BASE | 0x4000; // 内部闪存中的向量表重定向
	 
	delay_init();	    	//延时函数初始化	  
	uart_init(9600);	 	//串口初始化为9600
	LED_Init();					//初始化与LED连接的硬件接口
	 
	printf("Flash App Ready");
	LED0 = 0; LED1 = 0;
	delay_ms(1000);
	LED1 = 1; LED1 = 1;
	 
	while(RTC_Init())		//RTC初始化	，一定要初始化成功
	{ 
		LED0 = 1; LED1 = 1;
		printf("RTC Init ERROR...\n\r");
		delay_ms(800);
		printf("RTC Init Trying Again\n\r");
	}		   
	
	while(blink--){ 		//提示系统初始化完成
		LED0 = 0;LED1 = 0;
		delay_ms(300);
		LED0 = 1;LED1 = 1;
	}
	printf("Done!\n\r");
	printf("calendar.week = %d\n\r" ,calendar.week);
	
	while(1)
	{								    
		printf("begin\n\r");
		LED0 = !LED0;
		
		switch(calendar.week)
		{
			case 0:
				printf("Sunday\n\r");	break;
			case 1:
				printf("Monday\n\r");	break;
			case 2:
				printf("Tuesday\n\r"); break;
			case 3:
				printf("Wednesday\n\r"); break;
			case 4:
				printf("Thursday\n\r"); break;
			case 5:
				printf("Friday\n\r"); break;
			case 6:
				printf("Saturday\n\r"); break;  
			default: 
				printf("nothing"); break;
		}
		
		delay_ms(500);	
	} 
}
