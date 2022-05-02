#include <STC12C5A60S2.H>
#include <stdio.h>
#include <string.h>
#include <vector>
#define FOSC 11059200L
#define BAUD 9600
#define S2RI 0x01;	 //串口2接收bit
#define S2TI 0x02;	 //串口2发送bit
char a[101];int l=0;//输出汉字
int cnt,rec,cnt1,cnt2,cnt3,len;
unsigned int tmp,num;
int nums[10]={0Xc0,0xF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90},numm[5];
unsigned char rec2,sndflag=0,numflag=0;  //sndflag:是否向小程序返回打印机状态,numflag是否改变num
bit New_rec = 0, Send_ed = 1; 
void InitUart2()
{
    S2CON=0x50; //串口2工作在方式1  10位异步收发 S2REN=1允许接收
    AUXR|=0x14;
    BRT=-(FOSC/32/BAUD);
    S2CON|=S2TI;
	  IE2 =0x01;    //开串口2中断  ES2=1
}
//数码管显示
void Timer0Init(void)		//20微秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xAE;		//设置定时初值
	TH0 = 0xFB;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
}
void USART_Init()
{
	PCON &= 0x7F;		//波特率不倍速 SMOD=0
	SCON = 0x50;		//设置为方式1,8位数据,可变波特率,接收允许
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	TMOD = 0X20;		//定时器1:模式2,8位自动重装模式,用于产生波特率 
	TL1 = 0XFD;		    //设定定时初值，波特率设置为115200
	TH1 = 0XFD;		    //设定定时器重装值
	TR1 = 1;			//启动定时器1
	ES = 1;				//开串行中断
	EA = 1;				//开总中断
	TI=0;
	//EA、ES置1后，若有串口接收或者发送，则进入执行串口中断服务程序void USART_Interrupt(void)  interrupt 4	 
}
void Delay50ms()		//@11.0592MHz
{
	unsigned char i, j, k;
	i = 3;
	j = 26;
	k = 223;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay20us()		//@11.0592MHz
{
	unsigned char i;
	i = 52;
	while (--i);
}
void Pause()//        Pause();
{
	while(!TI){;}
	TI=0;
}
void Init(void)  //设置行间距为默认值 6 X 0.125mm  ESC2
{
	SBUF=0x1b;Pause();SBUF=0x32;Pause();
}
void print(void)  //打印并走纸（1个单位行间距）
{
	SBUF=0x0a;Pause();
}
void printhz(void)  //输出汉字
{
	l=0;
	while(a[l]){
		SBUF=a[l++];Pause();
		}
	memset(a,0,sizeof(a));
	
}
void InitPrinter(void)  //初始化（清除打印缓冲区中数据，复位打印机配置）
{
	SBUF=0x1b;Pause();SBUF=0x40;Pause();
}
void setLine(int n)  //设置行间距为 n x 0.125mm  (默认n=6) 255
{
	SBUF=0x1b;Pause();SBUF=0x33;Pause();SBUF=n;Pause();
}// n为十六进制数
void setMode(int n,int m)  //设置打印模式 60:倍高倍宽 40：倍宽 20：倍高
{
	int ctl=0;
	if(n==1) ctl+=20;if(m==1) ctl+=40;
	SBUF=0x1b;Pause();SBUF=0x21;Pause();SBUF=ctl;Pause();
}
void setLeft(int m,int n)
{
	SBUF=0x1b;Pause();SBUF=0x24;Pause();SBUF=m;Pause();SBUF=n;Pause();
}//仅适用于字符 设置左边距 m:255,n:1(m=n=0)(m+n x 256) x 0.125mm不要过大
 // n,m为十六进制数
void tranStatus(void)//(试验）
{
	SBUF=0x1b;Pause();SBUF=0x76;Pause();SBUF=0x00;Pause();
}
void setAline(int n)//(试验）
{
	SBUF=0x1b;Pause();SBUF=0x61;Pause();SBUF=n;Pause();
}
void setEnglishLine(int n)//设置西文右侧字符间距为n x 0.125mm 仅适用于西文
{
	SBUF=0x1b;Pause();SBUF=0x20;Pause();SBUF=n;Pause();
}
void setAlign(int n) //设置字符对齐方式 n:0左 1:中 2:右
{
	SBUF=0x1b;Pause();SBUF=0x61;Pause();SBUF=n;Pause();
}
void setPicLeft(int m,int n)//m:255,n:1 设置图片左边距为(m+n*256)点
{
	SBUF=0x1b;Pause();SBUF=0x50;Pause();SBUF=m;Pause();SBUF=n;Pause();
}
void setPicMode(void)
{
	SBUF=0x1b;Pause();SBUF=0x2a;Pause();
}//m:0,1,32,33:点图方式 水平方向点数:nl+nh*256(nl:255,nh:1)
//d1..k:255 d[i]=0/1: 打印/不打印 点数超过一行，超过最大点数的部分倍忽略
//指令结束后必须调用PrintPic()来打印图片
void PrintPic(void)
{
	SBUF=0x1b;Pause();SBUF=0x4a;Pause();SBUF=0x00;Pause();
}
void PrintHRI(int n)//n:0不打印 1条形码上方 2下方 3上下方
{
	SBUF=0x1d;Pause();SBUF=0x48;Pause();SBUF=n;Pause();
}
void setChinesemargin(int m,int n)
{
	SBUF=0x1c;Pause();SBUF=0x53;Pause();SBUF=m;Pause();SBUF=n;Pause();
	}//m:左边距 n:右边距 只对汉字有效
	void setBarHeight(int n) //default:n=50 n:10-240
{
	SBUF=0x1d;Pause();SBUF=0x68;Pause();SBUF=n;Pause();
}
void setBarWidth(int n)//default:n=2 n:2-4
{
	SBUF=0x1d;Pause();SBUF=0x77;Pause();SBUF=n;Pause();
}
void setBarLeft(int n)//设置单条形码左边距
{
	SBUF=0x1d;Pause();SBUF=0x78;Pause();SBUF=n;Pause();
}
void signalBar(void)//code128
{
	  InitPrinter();setBarLeft(30);
	  SBUF=0x1d;Pause();SBUF=0x6b;Pause();
		SBUF=73;Pause();SBUF=0x08;Pause();
		SBUF=0x31;Pause();SBUF=0x32;Pause();
		SBUF=0x33;Pause();SBUF=0x34;Pause();
		SBUF=0x35;Pause();SBUF=0x36;Pause();
		SBUF=0x37;Pause();SBUF=0x38;Pause();print();
}
void example1(void)
{
	InitPrinter();
	setMode(1,1);
	strcpy(a,"  ShukeEnglish");printhz();print();
	InitPrinter();strcpy(a,"    Example of Weekly data");printhz();print();
	InitPrinter();print();
	
	InitPrinter();strcpy(a,"-----------STATISTICS----------");printhz();print();
	InitPrinter();strcpy(a,"                               ");printhz();print();
	InitPrinter();strcpy(a,"        |  WORDS  |  GRAMMAR  | ");printhz();print();
	InitPrinter();strcpy(a,"-------------------------------");printhz();print();
	InitPrinter();strcpy(a,"  10-20 |  15/15  |   18/20   |");printhz();print();
	InitPrinter();strcpy(a,"-------------------------------");printhz();print();
	InitPrinter();strcpy(a,"  10-21 |  14/15  |   15/20   |");printhz();print();
	InitPrinter();strcpy(a,"-------------------------------");printhz();print();
	InitPrinter();strcpy(a,"  10-22 |  15/15  |   17/20   |");printhz();print();
	InitPrinter();strcpy(a,"-------------------------------");printhz();print();
	InitPrinter();strcpy(a,"  10-23 |  15/15  |   20/20   |");printhz();print();
	InitPrinter();strcpy(a,"-------------------------------");printhz();print();
	InitPrinter();strcpy(a,"  10-24 |  10/15  |   15/20   |");printhz();print();
	InitPrinter();strcpy(a,"-------------------------------");printhz();print();
	InitPrinter();strcpy(a,"  10-25 |  12/15  |   14/20   |");printhz();print();
	InitPrinter();strcpy(a,"-------------------------------");printhz();print();
	InitPrinter();strcpy(a,"  10-26 |  14/15  |   19/20   |");printhz();print();
	InitPrinter();strcpy(a,"-------------------------------");printhz();print();
	InitPrinter();strcpy(a,"  TOTAL |  15/105 |   18/140  |");printhz();print();
	InitPrinter();strcpy(a,"-------------------------------");printhz();print();
	InitPrinter();strcpy(a,"              PRINT TIME: 10-26");printhz();print();
	print();print();
	
/*
	setMode(1,1);
//		strcpy(a," #0000");printhz(); //
		strcpy(a,"ShukeEnglish 学习周报");printhz();
		print();
//		InitPrinter();setAlign(1);
//		strcpy(a,"*餐馆名*");printhz();
//		print();
		InitPrinter();print();
		strcpy(a,"-------------------------------");printhz();print();
		InitPrinter();
		strcpy(a,"下单时间: mm-dd mm-ss");printhz();print();  //
		InitPrinter();strcpy(a,"-------------------------------");printhz();print();
		InitPrinter();setMode(1,1);
		strcpy(a,"备注:3人用餐");printhz();print();  //
		InitPrinter();setMode(1,1);
		strcpy(a,"其他备注:下雨天外卖小哥注意安全 不着急");printhz();print();  //
		InitPrinter();
		strcpy(a,"********************************");printhz();print();
		InitPrinter();
		strcpy(a,"------------1号口袋------------");printhz();print();
		InitPrinter();setAlign(0);
		strcpy(a,"商品1          *1          价格1");printhz();print();  //
		InitPrinter();setAlign(0);
		strcpy(a,"商品2          *1          价格2");printhz();print();  //
		InitPrinter();
		strcpy(a,"-------------其他-------------");printhz();print();
		InitPrinter();
		strcpy(a,"餐盒费                     价格3");printhz();print();  //
		InitPrinter();
		strcpy(a,"配送费                     价格4");printhz();print();  //
		InitPrinter();
		strcpy(a,"[优惠 满20.0元减5.0元]      -5.0");printhz();print();  //
		InitPrinter();
		strcpy(a,"********************************");printhz();print();
		InitPrinter();setAlign(2);
		strcpy(a,"原价:价格5");printhz();print();  //
		InitPrinter();setAlign(2);setMode(1,1);
		strcpy(a,"现价:价格6");printhz();print();  //
		InitPrinter();setAlign(2);
		strcpy(a,"支付方式:线上支付");printhz();print();
		InitPrinter();strcpy(a,"-------------------------------");printhz();print();
		InitPrinter();
		strcpy(a,"地址:为保护顾客隐私，请在小程序骑手端查看详细地址");printhz();print();
		InitPrinter();setAlign(0);setMode(1,1);
		strcpy(a,"顾客:李先生");printhz();print();  //
		signalBar();
		InitPrinter();setAlign(1);setMode(1,1);
		strcpy(a,"祝您好胃口!");printhz();print();
		*/
}
void beepdelay(unsigned int t)
{
  unsigned char n; 
  for(;t>0;t--)
   for(n=0;n<125;n++)
   {;}
}
void beep_order()
{
	unsigned long int i;
	for(i=0;i<1800;i++)
	{
		beepdelay(1);
		P37=0;
		beepdelay(1);
		P37=1;
	}
		P37=1;            //喇叭停止工作
    beepdelay(2000);
}
void beep(int n)
{
	unsigned char i,j;
	for(j=1;j<=n;j++){
  for(i=0;i<200;i++)//喇叭发声的时间循环，改变大小可以改变发声时间长短
  {
    beepdelay(1);     //参数决定发声的频率
		P37=0;
		beepdelay(1);
		P37=1;
  }
		P37=1;            //喇叭停止工作
    beepdelay(2000);
}
}
void SHOWSTATUS(void)
{
	unsigned char rec3;
	if(rec==0x00) rec3=0x61;
	if(rec==0x04) rec3=0x62;
	if(rec==0x80) rec3=0x63;
	if(rec==0x84) rec3=0x64;
	S2BUF=rec3;
	while(1){
	//tranStatus();
	if(P05==0){while(P05==0){;}Delay50ms();cnt2++;cnt2%=2;}
	if(cnt2==0){TR0=1;return;}
	if(rec==0x04){//pape
		P00=1;P01=1;P02=1;P03=0;
		P2=0x86;Delay20us();P2=0xFF;
		P00=1;P01=1;P02=0;P03=1;
		P2=0x8c;Delay20us();P2=0xFF;
		P00=1;P01=0;P02=1;P03=1;
		P2=0xa0;Delay20us();P2=0xFF;
		P00=0;P01=1;P02=1;P03=1;
	  P2=0x8c;Delay20us();P2=0xFF;
		
	}
	if(rec==0x00){//good
		P00=1;P01=1;P02=1;P03=0;
		P2=0xa1;Delay20us();P2=0xFF;
		P00=1;P01=1;P02=0;P03=1;
		P2=0xa3;Delay20us();P2=0xFF;
		P00=1;P01=0;P02=1;P03=1;
		P2=0xa3;Delay20us();P2=0xFF;
		P00=0;P01=1;P02=1;P03=1;
	  P2=0x82;Delay20us();P2=0xFF;
	}
	if(rec==0x80){//hot
		P00=1;P01=1;P02=1;P03=0;
		P2=0x87;Delay20us();P2=0xFF;
		P00=1;P01=1;P02=0;P03=1;
		P2=0xa3;Delay20us();P2=0xFF;
		P00=1;P01=0;P02=1;P03=1;
		P2=0x8b;Delay20us();P2=0xFF;
		P00=0;P01=1;P02=1;P03=1;
	  P2=0xFF;Delay20us();P2=0xFF;
	}
	if(rec==0x84){//ph
		P00=1;P01=1;P02=1;P03=0;
		P2=0x8b;Delay20us();P2=0xFF;
		P00=1;P01=1;P02=0;P03=1;
		P2=0xFF;Delay20us();P2=0xFF;
		P00=1;P01=0;P02=1;P03=1;
		P2=0x8c;Delay20us();P2=0xFF;
		P00=0;P01=1;P02=1;P03=1;
	  P2=0xFF;Delay20us();P2=0xFF;
	}
}
}

void main()
{
	int i,t=0;
	cnt1=cnt2=cnt3=0;num=0;
	memset(numm,0,sizeof(numm));
  USART_Init();InitUart2();Init();InitPrinter();Timer0Init();
	IPH|=0x01;IP|=0x01;//串口1设为最高优先级（优先级3）
	IPH2|=0x01;IP2|=0x01;//串口2设为最高优先级（优先级3）
	while(1){
		if(P04==0){while(P04==0){;}Delay50ms();cnt1++;cnt1%=2;}//cnt1 输出样例
		if(P05==0){while(P05==0){;}Delay50ms();cnt2++;cnt2%=2;}//cnt2 查询状态
		if(cnt1==1&&!cnt2){ //输出样例
			InitPrinter();example1();
			cnt1++;cnt1%=2;
		}
		if(!cnt1&&cnt2==1){  //查询状态
			TR0=0;
			tranStatus();
			TR0=1;
		}
		if(numflag){
			num=0;numflag=0;
		for(i=0;i<4;i++){
			if(numm[i]==0x30) t=0;
			if(numm[i]==0x31) t=1;
			if(numm[i]==0x32) t=2;
			if(numm[i]==0x33) t=3;
			if(numm[i]==0x34) t=4;
			if(numm[i]==0x35) t=5;
			if(numm[i]==0x36) t=6;
			if(numm[i]==0x37) t=7;
			if(numm[i]==0x38) t=8;
			if(numm[i]==0x39) t=9;
			if(i==0) num+=t*1000;
			if(i==1) num+=t*100;
			if(i==2) num+=t*10;
			if(i==3) num+=t;
		}
	}
		//while(1){;}
	}
}

void Timer0() interrupt 1
{
	if(P04==0){while(P04==0){;}Delay50ms();cnt1++;cnt1%=2;}//cnt1 输出样例
	if(P05==0){while(P05==0){;}Delay50ms();cnt2++;cnt2%=2;}//cnt2 查询状态
	TL0 = 0xAE;		//设置定时初值
	TH0 = 0xFB;		//设置定时初值
	P2=0xFF;
	tmp=num;
	if(cnt==0)
	{
	  P00=1;P01=1;P02=1;P03=0;P2=nums[tmp%10];
	}
	if(cnt==1)
	{
		tmp/=10;
	  P00=1;P01=1;P02=0;P03=1;P2=nums[tmp%10];
	}
	if(cnt==2)
	{
		tmp/=100;
	  P00=1;P01=0;P02=1;P03=1;P2=nums[tmp%10];
	}
	if(cnt==3)
	{
		tmp/=1000;
	  P00=0;P01=1;P02=1;P03=1;P2=nums[tmp];
	}
	cnt++;cnt%=4;
}
void USART_Interrupt(void)  interrupt 4
{
	if(RI)
	{
		rec=SBUF;Delay50ms();RI=0;
		if(rec==0x80) beep(2);
		if(rec==0x00) beep(1);
		if(rec==0x04) beep(4);
		if(rec==0x84) beep(6);
		SHOWSTATUS();
	}
}
void USART2_Interrupt(void) interrupt 8
{
	if(S2CON&0x01) //S2RI==1
	{
		S2CON&=~S2RI;//S2RI=0
    rec2 = S2BUF;
		if(rec2==0x01){
			cnt1++;cnt1%=2;
			SBUF=0x01;
		}
		else if(rec2==0x02){
			cnt2++;cnt2%=2;
		}
		else if(rec2==0x03){
			if(cnt3==1)
				numflag=1;
			cnt3++;cnt3%=2;len=0;
		}
		else if(rec2==0x04){
			beep_order();
			InitPrinter();
		}
		else{
			if(cnt3==0){
				SBUF=rec2;Pause();
			}
			else{
				numm[len]=rec2;len++;
			}
		}
	}
	else{
		S2CON&=~S2TI;
	}
}