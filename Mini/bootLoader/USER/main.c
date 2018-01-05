#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "stmflash.h"   
#include "iap.h"

// ����ʹ�õ���stm32f103rct6  ʵ��Ӧ��ƽ̨Ϊ stm32f103rbt6 ����������ҪΪǰ����256Kbd��������Ʒ ����Ϊ128Kb��������Ʒ

// FLASH����: 0X0800 0000~0x0800 4000,��IAPʹ��,��16K�ֽ�,FLASH APP����128-16=112KB����.

int main(void)
 {
	u16 t = 0;						//���ڼ�ʱ
	u8 key;
	u16 oldcount=0;	//�ϵĴ��ڽ�������ֵ
	u8 applenth=0;	//���յ���app���볤��
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
 	KEY_Init();				//������ʼ�� 	
	 
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	 
	printf("begin to the loop");
	
	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount == USART_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("�û�����������!\r\n");
				printf("���볤��:%dBytes\r\n",applenth);
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
		
		if(key==WKUP_PRES)			//WK_UP��������  ����flash����
		{
			if(applenth)
			{
				printf("��ʼ���¹̼�...\r\n");	
 				if( ( ( *(vu32*)(0X20001000+4) ) & 0xFF000000) == 0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//����FLASH����   
					printf("�̼��������!\r\n");	
				}else 
				{ 
					printf("��FLASHӦ�ó���!\r\n");
				}
 			}else 
			{
				printf("û�п��Ը��µĹ̼�!\r\n");
			}								 
		} 
		
		if(key==KEY1_PRES)			//KEY1��������  ִ���µ�flash����
		{
			printf("��ʼִ��FLASH�û�����!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			}else 
			{
				printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
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
