#include <STC12C5A60S2.H>
#include <stdio.h>
#include <string.h>
#include <vector>
#define FOSC 11059200L
#define BAUD 9600
#define S2RI 0x01;	 //����2����bit
#define S2TI 0x02;	 //����2����bit
char a[101];int l=0;//�������
int cnt,rec,cnt1,cnt2,cnt3,len;
unsigned int tmp,num;
int nums[10]={0Xc0,0xF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90},numm[5];
unsigned char rec2,sndflag=0,numflag=0;  //sndflag:�Ƿ���С���򷵻ش�ӡ��״̬,numflag�Ƿ�ı�num
bit New_rec = 0, Send_ed = 1; 
void InitUart2()
{
    S2CON=0x50; //����2�����ڷ�ʽ1  10λ�첽�շ� S2REN=1�������
    AUXR|=0x14;
    BRT=-(FOSC/32/BAUD);
    S2CON|=S2TI;
	  IE2 =0x01;    //������2�ж�  ES2=1
}
//�������ʾ
void Timer0Init(void)		//20΢��@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xAE;		//���ö�ʱ��ֵ
	TH0 = 0xFB;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
}
void USART_Init()
{
	PCON &= 0x7F;		//�����ʲ����� SMOD=0
	SCON = 0x50;		//����Ϊ��ʽ1,8λ����,�ɱ䲨����,��������
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	TMOD = 0X20;		//��ʱ��1:ģʽ2,8λ�Զ���װģʽ,���ڲ��������� 
	TL1 = 0XFD;		    //�趨��ʱ��ֵ������������Ϊ115200
	TH1 = 0XFD;		    //�趨��ʱ����װֵ
	TR1 = 1;			//������ʱ��1
	ES = 1;				//�������ж�
	EA = 1;				//�����ж�
	TI=0;
	//EA��ES��1�����д��ڽ��ջ��߷��ͣ������ִ�д����жϷ������void USART_Interrupt(void)  interrupt 4	 
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
void Init(void)  //�����м��ΪĬ��ֵ 6 X 0.125mm  ESC2
{
	SBUF=0x1b;Pause();SBUF=0x32;Pause();
}
void print(void)  //��ӡ����ֽ��1����λ�м�ࣩ
{
	SBUF=0x0a;Pause();
}
void printhz(void)  //�������
{
	l=0;
	while(a[l]){
		SBUF=a[l++];Pause();
		}
	memset(a,0,sizeof(a));
	
}
void InitPrinter(void)  //��ʼ���������ӡ�����������ݣ���λ��ӡ�����ã�
{
	SBUF=0x1b;Pause();SBUF=0x40;Pause();
}
void setLine(int n)  //�����м��Ϊ n x 0.125mm  (Ĭ��n=6) 255
{
	SBUF=0x1b;Pause();SBUF=0x33;Pause();SBUF=n;Pause();
}// nΪʮ��������
void setMode(int n,int m)  //���ô�ӡģʽ 60:���߱��� 40������ 20������
{
	int ctl=0;
	if(n==1) ctl+=20;if(m==1) ctl+=40;
	SBUF=0x1b;Pause();SBUF=0x21;Pause();SBUF=ctl;Pause();
}
void setLeft(int m,int n)
{
	SBUF=0x1b;Pause();SBUF=0x24;Pause();SBUF=m;Pause();SBUF=n;Pause();
}//���������ַ� ������߾� m:255,n:1(m=n=0)(m+n x 256) x 0.125mm��Ҫ����
 // n,mΪʮ��������
void tranStatus(void)//(���飩
{
	SBUF=0x1b;Pause();SBUF=0x76;Pause();SBUF=0x00;Pause();
}
void setAline(int n)//(���飩
{
	SBUF=0x1b;Pause();SBUF=0x61;Pause();SBUF=n;Pause();
}
void setEnglishLine(int n)//���������Ҳ��ַ����Ϊn x 0.125mm ������������
{
	SBUF=0x1b;Pause();SBUF=0x20;Pause();SBUF=n;Pause();
}
void setAlign(int n) //�����ַ����뷽ʽ n:0�� 1:�� 2:��
{
	SBUF=0x1b;Pause();SBUF=0x61;Pause();SBUF=n;Pause();
}
void setPicLeft(int m,int n)//m:255,n:1 ����ͼƬ��߾�Ϊ(m+n*256)��
{
	SBUF=0x1b;Pause();SBUF=0x50;Pause();SBUF=m;Pause();SBUF=n;Pause();
}
void setPicMode(void)
{
	SBUF=0x1b;Pause();SBUF=0x2a;Pause();
}//m:0,1,32,33:��ͼ��ʽ ˮƽ�������:nl+nh*256(nl:255,nh:1)
//d1..k:255 d[i]=0/1: ��ӡ/����ӡ ��������һ�У������������Ĳ��ֱ�����
//ָ�������������PrintPic()����ӡͼƬ
void PrintPic(void)
{
	SBUF=0x1b;Pause();SBUF=0x4a;Pause();SBUF=0x00;Pause();
}
void PrintHRI(int n)//n:0����ӡ 1�������Ϸ� 2�·� 3���·�
{
	SBUF=0x1d;Pause();SBUF=0x48;Pause();SBUF=n;Pause();
}
void setChinesemargin(int m,int n)
{
	SBUF=0x1c;Pause();SBUF=0x53;Pause();SBUF=m;Pause();SBUF=n;Pause();
	}//m:��߾� n:�ұ߾� ֻ�Ժ�����Ч
	void setBarHeight(int n) //default:n=50 n:10-240
{
	SBUF=0x1d;Pause();SBUF=0x68;Pause();SBUF=n;Pause();
}
void setBarWidth(int n)//default:n=2 n:2-4
{
	SBUF=0x1d;Pause();SBUF=0x77;Pause();SBUF=n;Pause();
}
void setBarLeft(int n)//���õ���������߾�
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
		strcpy(a,"ShukeEnglish ѧϰ�ܱ�");printhz();
		print();
//		InitPrinter();setAlign(1);
//		strcpy(a,"*�͹���*");printhz();
//		print();
		InitPrinter();print();
		strcpy(a,"-------------------------------");printhz();print();
		InitPrinter();
		strcpy(a,"�µ�ʱ��: mm-dd mm-ss");printhz();print();  //
		InitPrinter();strcpy(a,"-------------------------------");printhz();print();
		InitPrinter();setMode(1,1);
		strcpy(a,"��ע:3���ò�");printhz();print();  //
		InitPrinter();setMode(1,1);
		strcpy(a,"������ע:����������С��ע�ⰲȫ ���ż�");printhz();print();  //
		InitPrinter();
		strcpy(a,"********************************");printhz();print();
		InitPrinter();
		strcpy(a,"------------1�ſڴ�------------");printhz();print();
		InitPrinter();setAlign(0);
		strcpy(a,"��Ʒ1          *1          �۸�1");printhz();print();  //
		InitPrinter();setAlign(0);
		strcpy(a,"��Ʒ2          *1          �۸�2");printhz();print();  //
		InitPrinter();
		strcpy(a,"-------------����-------------");printhz();print();
		InitPrinter();
		strcpy(a,"�ͺз�                     �۸�3");printhz();print();  //
		InitPrinter();
		strcpy(a,"���ͷ�                     �۸�4");printhz();print();  //
		InitPrinter();
		strcpy(a,"[�Ż� ��20.0Ԫ��5.0Ԫ]      -5.0");printhz();print();  //
		InitPrinter();
		strcpy(a,"********************************");printhz();print();
		InitPrinter();setAlign(2);
		strcpy(a,"ԭ��:�۸�5");printhz();print();  //
		InitPrinter();setAlign(2);setMode(1,1);
		strcpy(a,"�ּ�:�۸�6");printhz();print();  //
		InitPrinter();setAlign(2);
		strcpy(a,"֧����ʽ:����֧��");printhz();print();
		InitPrinter();strcpy(a,"-------------------------------");printhz();print();
		InitPrinter();
		strcpy(a,"��ַ:Ϊ�����˿���˽������С�������ֶ˲鿴��ϸ��ַ");printhz();print();
		InitPrinter();setAlign(0);setMode(1,1);
		strcpy(a,"�˿�:������");printhz();print();  //
		signalBar();
		InitPrinter();setAlign(1);setMode(1,1);
		strcpy(a,"ף����θ��!");printhz();print();
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
		P37=1;            //����ֹͣ����
    beepdelay(2000);
}
void beep(int n)
{
	unsigned char i,j;
	for(j=1;j<=n;j++){
  for(i=0;i<200;i++)//���ȷ�����ʱ��ѭ�����ı��С���Ըı䷢��ʱ�䳤��
  {
    beepdelay(1);     //��������������Ƶ��
		P37=0;
		beepdelay(1);
		P37=1;
  }
		P37=1;            //����ֹͣ����
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
	IPH|=0x01;IP|=0x01;//����1��Ϊ������ȼ������ȼ�3��
	IPH2|=0x01;IP2|=0x01;//����2��Ϊ������ȼ������ȼ�3��
	while(1){
		if(P04==0){while(P04==0){;}Delay50ms();cnt1++;cnt1%=2;}//cnt1 �������
		if(P05==0){while(P05==0){;}Delay50ms();cnt2++;cnt2%=2;}//cnt2 ��ѯ״̬
		if(cnt1==1&&!cnt2){ //�������
			InitPrinter();example1();
			cnt1++;cnt1%=2;
		}
		if(!cnt1&&cnt2==1){  //��ѯ״̬
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
	if(P04==0){while(P04==0){;}Delay50ms();cnt1++;cnt1%=2;}//cnt1 �������
	if(P05==0){while(P05==0){;}Delay50ms();cnt2++;cnt2%=2;}//cnt2 ��ѯ״̬
	TL0 = 0xAE;		//���ö�ʱ��ֵ
	TH0 = 0xFB;		//���ö�ʱ��ֵ
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