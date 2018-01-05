#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stmflash.h"
#include "iap.h" 

iapfun jump2app; //最后通过一个虚拟的函数（jump2app）跳转到 APP 程序执行代码，实现 IAP?APP 的跳转
u32 iapbuf[512]; 	//2K字节缓存  

// appxaddr:应用程序的起始地址
// appbuf:应用程序CODE.
// appsize:应用程序大小(字节).
// iap_write_appbin 函数用于将存放在串口接收 buf 里面的APP 程序写入到 FLASH 中
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	u32 t;
	u16 i=0;
	u32 temp;
	u32 fwaddr = appxaddr;//当前写入的地址
	u8 *dfu = appbuf;
	for(t=0; t<appsize; t+=4) 
	{						   
		temp =(u32)dfu[3] << 24;   // 
		temp|=(u32)dfu[2] << 16;    
		temp|=(u32)dfu[1] << 8;
		temp|=(u32)dfu[0];	  
		dfu += 4;// 偏移4个字节
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0; 
			STMFLASH_Write(fwaddr,iapbuf,512);
			fwaddr += 2048;//偏移2048  512*4=2048 = 2Kb
		}
	} 
	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
}

// iap_load_app 函数，用于跳转到 APP 程序运行
// appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr)
{
	if( ( ( STMFLASH_ReadWord(appxaddr) ) & 0x2FFE0000) == 0x20000000)	//检查栈顶地址是否合法.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
	}
}		 














