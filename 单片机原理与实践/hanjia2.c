#include <reg51.h>
#include <intrins.h>
#include <string.h>


#define INT8 	  		char
#define INT16 			int
#define uchar 	unsigned  	char
#define uint 	unsigned 	int

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
sbit note=P3^2;
uchar ReceivedString[20];
uchar ReceivedStringPosition;
uchar IrValue[6];
uchar Time;

uchar DisplayData[8];
uchar code smgduan[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0X76};
//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F、H的显示码


//延时函数,us,最多255 us-------------------------------------------------------
void Delay_us(uchar n)
{
	for(;n>0;n--)
		_nop_();
}


//延时函数,ms------------------------------------------------------------------
void Delay_ms(uint n)
{
   uint i;
    for(i=0; i<n; i++)
    {
	 	Delay_us(250);
		Delay_us(250);
		Delay_us(250);
		Delay_us(250);
    }
}
			
	void DigDisplay()
{
	uint i;
	for(i=0;i<3;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=1;LSB=1;LSC=1; break;//显示第0位	
			case(1):
				LSA=0;LSB=1;LSC=1; break;//显示第1位
			case(2):
				LSA=1;LSB=0;LSC=1; break;//显示第2位 
		}
		P0=DisplayData[i];//发送数据
		Delay_us(100); //间隔一段时间扫描	
		P0=0x00;//消隐
	}		
}			

	   
//串口初始化
void UART_Init(void)
{ 
	SCON = 0x40;                	//串口方式1
	PCON = 0;                   	//SMOD=0
	REN = 1;                     	//允许接收
	TMOD = 0x20;               		//定时器1定时方式2
	TH1 = 0xFD;                		//12MHz 9600波特率
	TL1 = 0xFD;			   	   
	TR1 = 1;                  		//启动定时器
	ES = 1;							//UART中断
	EA = 1;							//中断使能

}

//发送一个Char
void UART_Send_Char(uchar aChar)
{
 	SBUF = aChar;
 	while(TI == 0);//等待，直到发送成功
	TI = 0;
}


void UART_Send_String(uchar *aString, uint StringLength)
{
	uchar i;
	for ( i = 0; i< StringLength; i++ )
	{
		UART_Send_Char( aString[i] );
	}

}


char code Sdata[] = "lxr";
INT8 Receive[1] = "*";
INT16 index =0;
//主函数-----------------------------------------------------------------------

sbit STAT_pinn= P0^0;
uchar *name = "AT+NANE112233\r\n";
uchar *hosten = "AT+HOSTEN0\r\n";
uchar *starten = "AT+STARTEN0\r\n";
uchar *pass = "AT+PASS123456\r\n";
uchar *iscen = "AT+ISCEN\r\n";
uchar *uuid = "AT+UUIDLEN1\r\n";



void main(void)
{ 
	UART_Init();  //初始化UART
	Delay_ms(100);
	UART_Send_String( name,sizeof(name) );//设置蓝牙广播名为112233
	Delay_ms(10);
	UART_Send_String( hosten,sizeof(hosten) );//设置模块为从机模式
	Delay_ms(10);
	UART_Send_String( starten,sizeof(starten) );//设置开机唤醒模式
	Delay_ms(10);
	UART_Send_String( pass,sizeof(pass) );//设置连接密码
	Delay_ms(10);
	UART_Send_String( iscen,sizeof(iscen) );//设置从机连接开关
	Delay_ms(10);
	UART_Send_String( uuid,sizeof(uuid) );//设置UUID为128位
	Delay_ms(10);
	while (1)
	{
		if( STAT_pinn==1 )//如里连接状态为低电平，表示蓝牙模块与APP已经连接
		{
			uchar cnt = 0;
			UART_Send_String( &cnt,1 );//设置开机唤醒模式
			Delay_ms(100);
			if( cnt>100 )cnt=0;
		}
	}	
}

//UART中断服务程序-------------------------------------------------------------
void UART_Receive_Int() interrupt 4
{	
	 uchar receiveData,rec;
   uint i;
        receiveData=SBUF;//出去接收到的数据
        rec=SBUF;
        if(rec==0x43)        //接收到C时
        {for(i=0;i<50;i++)
					{
            DisplayData[0] = smgduan[4];
		        DisplayData[1] = smgduan[3];
		        DisplayData[2] = smgduan[17];  
        }}
				if(rec==0x44)        //接收到D时
        {for(i=0;i<50;i++)
					{
            DisplayData[0] = smgduan[4];
		        DisplayData[1] = smgduan[4];
		        DisplayData[2] = smgduan[17];  
        }}
				
     
        RI = 0;//清除接收中断标志位
        SBUF=receiveData;//将接收到的数据放入到发送寄存器
        while(!TI);                         //等待发送数据完成
        TI=0;                                                 //清除发送完成标志位
	}	

