 #include <reg52.h> 
 #include <stdio.h>
#include <string.h>
 typedef unsigned char uchar;
 typedef unsigned int uint;
 sbit beep=P1^5;
 sbit LSA=P2^2;
 sbit LSB=P2^3;
 sbit LSC=P2^4;
  uchar code KEY_TABLE[] =
  {
      0xEE, 0xDE, 0xBE, 0x7E,
      0xED, 0xDD, 0xBD, 0x7D,
      0xEB, 0xDB, 0xBB, 0x7B,
      0xE7, 0xD7, 0xB7, 0x77
 };
uchar code number[]={
	0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uchar LedBuff[]={0x00,0x00,0x00,0x00};
uchar UART_buff;
 void init_com( void ) //115200bps
{ 
   SCON=0x50;   
   TH2=0xFF;           
   TL2=0xFD;   
   RCAP2H=0xFF;   
   RCAP2L=0xFD; 
   TCLK=1;   
   RCLK=1;   
   C_T2=0;   
   EXEN2=0;
   TR2=1 ;
   TI=0;
}
void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Pause()//        Pause();
{
	while(!TI){;}
	TI=0;
}

void Init(void)  //t width as default 6 X 0.125mm  ESC2
{
	SBUF=0x1b;Pause();SBUF=0x32;Pause();
}
void Print(void)  //start to print a line LF
{
	SBUF=0x0a;Pause();Delay100ms();
}
void konghang(void)  //打印空行
{  SBUF=0x1b;Pause();SBUF=0x62;Pause();SBUF=0x01;Pause();SBUF=0x30;Pause();}
void way(void)   //居中
{SBUF=0x1b;Pause();SBUF=0X61;Pause();SBUF=0x01;Pause();SBUF=0x31;Pause();}
void weight(void)   //一维条形码宽度
{SBUF=0x1D;Pause();SBUF=0X77;Pause();SBUF=0X02;Pause();}
void tiaoxingma(void)   //打印条码
{
	SBUF=0X1D;Pause();SBUF=0x6B;Pause();SBUF=0X49;Pause();SBUF=0x08;Pause();
	SBUF=0X31;Pause();SBUF=0x32;Pause();SBUF=0X33;Pause();SBUF=0x34;Pause();
	SBUF=0X35;Pause();SBUF=0x36;Pause();SBUF=0X37;Pause();SBUF=0x38;Pause();
}
 void Delay(uchar m)
 {
     --m;
 }
 void fengming(uchar ad)
 {uchar s;
	 for(s=1;s<50;s++)
	 {beep=~beep;
		 Delay(ad);
	 }
 }
 void jieshou(void){
	 SBUF=0X1D;Pause();SBUF=0x72;Pause();SBUF=0X01;Pause();  
	if(RI == 1) {  //如果收到.  
      RI = 0;      //清除标志.    
      UART_buff = SBUF;  //接收.  
			if(UART_buff == 0x04)
			{fengming(10);}
		}
	}
 void ShowNumber(unsigned long j)
 {
	 signed char i;
	 uchar buf[4];
	 for(i=0;i<4;i++)
	 {buf[i]=j%10;
		 j=j/10;
	 }
	 for(i=3;i>=1;i--)
	 {if(buf[i]==0)
		    LedBuff[i]=0x00;
		 else
			 break;
	 }
	 for(;i>=0;i--)
	 {
		 LedBuff[i]=number[buf[i]];
	 }}
		 
 void shumaguan(void)
 { static uchar i=0;
	 P0=0X00;
   switch(i)
		 { case 0:LSA=0;LSB=0;LSC=0;i++;P0=LedBuff[0];break;
			 case 1:LSA=1;LSB=0;LSC=0;i++;P0=LedBuff[1];break;
			 case 2:LSA=0;LSB=1;LSC=0;i++;P0=LedBuff[2];break;
			 case 3:LSA=1;LSB=1;LSC=0;i=0;P0=LedBuff[3];break;
			 default:break;}
		 }
 void main()
 {
   uchar temp, key, i;
	 char a[101],b[]="黄焖鸡米饭",c[]="爆炒鸡丁",d[]="西红柿炒鸡蛋",e[]="水饺",f[]="冒菜",g[]="麻辣香锅",h[]="海底捞",mm[]="Cream Pie";
	 uint l=0;
	 unsigned long j=0;
  init_com();
	Init();
	 way();
	 
     while(1)
     {
			   jieshou();
         P1 = 0xF0;
         if (P1 != 0xF0)
         {
             Delay(2000);
             if (P1 != 0xF0)
             {
                 temp = P1;
                 P1 = 0x0F;  
                 key = temp | P1;
							   j++;
                 for (i = 0; i < 16; ++i)
                     if (key == KEY_TABLE[i])
                         break;
								switch(i)
								{
									case 0:strcpy(a,b);while(a[l]){SBUF=a[l++];Pause();}Print();memset(a, 0, 101);l=0;konghang();tiaoxingma();break;
									case 1:strcpy(a,c);while(a[l]){SBUF=a[l++];Pause();}Print();memset(a, 0, 101);l=0;konghang();tiaoxingma();break;
									case 2:strcpy(a,d);while(a[l]){SBUF=a[l++];Pause();}Print();memset(a, 0, 101);l=0;konghang();tiaoxingma();break;
									case 3:strcpy(a,e);while(a[l]){SBUF=a[l++];Pause();}Print();memset(a, 0, 101);l=0;konghang();tiaoxingma();break;
									case 4:strcpy(a,f);while(a[l]){SBUF=a[l++];Pause();}Print();memset(a, 0, 101);l=0;konghang();tiaoxingma();break;
									case 5:strcpy(a,g);while(a[l]){SBUF=a[l++];Pause();}Print();memset(a, 0, 101);l=0;konghang();tiaoxingma();break;
									case 6:strcpy(a,h);while(a[l]){SBUF=a[l++];Pause();}Print();memset(a, 0, 101);l=0;konghang();tiaoxingma();break;
									case 7:strcpy(a,mm);while(a[l]){SBUF=a[l++];Pause();}Print();memset(a, 0, 101);l=0;konghang();tiaoxingma();break;
									default:break;
                }
             }
         }
				 ShowNumber(j);
				 shumaguan();
     }
 }