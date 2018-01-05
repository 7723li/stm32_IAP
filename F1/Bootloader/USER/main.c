#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "stmflash.h"
#include "iap.h"

int main(void)
{		
	u8 t;
	u8 key;
	u16 oldcount=0;				//�ϵĴ��ڽ�������ֵ
	u16 applenth=0;				//���յ���app���볤��
	u8 clearflag=0;  

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	//���ڳ�ʼ��Ϊ115200
	delay_init();	   	 	//��ʱ��ʼ�� 
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	LCD_Init();			   		//��ʼ��LCD  
	
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(30,50,200,16,16,"Warship STM32");	
	LCD_ShowString(30,70,200,16,16,"IAP TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/27");  
	LCD_ShowString(30,130,200,16,16,"KEY_UP:Copy APP2FLASH");
	LCD_ShowString(30,150,200,16,16,"KEY2:Erase SRAM APP");
	LCD_ShowString(30,170,200,16,16,"KEY1:Run FLASH APP");
	LCD_ShowString(30,190,200,16,16,"KEY0:Run SRAM APP");
	POINT_COLOR=BLUE;
	//��ʾ��ʾ��Ϣ
	
	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
			{
				applenth=USART_RX_CNT;
				oldcount=0;
				USART_RX_CNT=0;
				printf("�û�����������!\r\n");
				printf("���볤��:%dBytes\r\n",applenth);
			}else oldcount=USART_RX_CNT;			
		}
		t++;
		delay_ms(10);
		if(t==30)
		{
			LED0=!LED0; // 300���뷴תһ�κ�� ��ʾ��������
			t=0;
			if(clearflag)
			{
				clearflag--;
				if(clearflag==0)LCD_Fill(30,210,240,210+16,WHITE);//�����ʾ
			}
		}	  	 
		
		key=KEY_Scan(0);
		
		if(key==WKUP_PRES)
		{
			if(applenth)
			{
				printf("��ʼ���¹̼�...\r\n");	
				LCD_ShowString(30,210,200,16,16,"Copying APP2FLASH...");
 				if(((*(vu32*)(0X20001000+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
				{	 
					iap_write_appbin(FLASH_APP1_ADDR,USART_RX_BUF,applenth);//����FLASH����   
					LCD_ShowString(30,210,200,16,16,"Copy APP Successed!!");
					printf("�̼��������!\r\n");	
				}else 
				{
					LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!  ");	   
					printf("��FLASHӦ�ó���!\r\n");
				}
 			}else 
			{
				printf("û�п��Ը��µĹ̼�!\r\n");
				LCD_ShowString(30,210,200,16,16,"No APP!");
			}
			clearflag=7;//��־��������ʾ,��������7*300ms�������ʾ									 
		}
		
		if(key==KEY1_PRES)
		{
			printf("��ʼִ��FLASH�û�����!!\r\n");
			if(((*(vu32*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//�ж��Ƿ�Ϊ0X08XXXXXX.
			{	 
				iap_load_app(FLASH_APP1_ADDR);//ִ��FLASH APP����
			}else 
			{
				printf("��FLASHӦ�ó���,�޷�ִ��!\r\n");
				LCD_ShowString(30,210,200,16,16,"Illegal FLASH APP!");	   
			}									 
			clearflag=7;//��־��������ʾ,��������7*300ms�������ʾ	  
		}		   
		 
	}   	   
}












