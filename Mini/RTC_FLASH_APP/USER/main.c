#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usmart.h"
#include "rtc.h"

 int main(void)
 { 
	int blink = 10;
	 
  SCB->VTOR = FLASH_BASE | 0x4000; // �ڲ������е��������ض���
	 
	delay_init();	    	//��ʱ������ʼ��	  
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();					//��ʼ����LED���ӵ�Ӳ���ӿ�
	 
	printf("Flash App Ready");
	LED0 = 0; LED1 = 0;
	delay_ms(1000);
	LED1 = 1; LED1 = 1;
	 
	while(RTC_Init())		//RTC��ʼ��	��һ��Ҫ��ʼ���ɹ�
	{ 
		LED0 = 1; LED1 = 1;
		printf("RTC Init ERROR...\n\r");
		delay_ms(800);
		printf("RTC Init Trying Again\n\r");
	}		   
	
	while(blink--){ 		//��ʾϵͳ��ʼ�����
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
