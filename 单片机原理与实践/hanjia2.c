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
//0��1��2��3��4��5��6��7��8��9��A��b��C��d��E��F��H����ʾ��


//��ʱ����,us,���255 us-------------------------------------------------------
void Delay_us(uchar n)
{
	for(;n>0;n--)
		_nop_();
}


//��ʱ����,ms------------------------------------------------------------------
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
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA=1;LSB=1;LSC=1; break;//��ʾ��0λ	
			case(1):
				LSA=0;LSB=1;LSC=1; break;//��ʾ��1λ
			case(2):
				LSA=1;LSB=0;LSC=1; break;//��ʾ��2λ 
		}
		P0=DisplayData[i];//��������
		Delay_us(100); //���һ��ʱ��ɨ��	
		P0=0x00;//����
	}		
}			

	   
//���ڳ�ʼ��
void UART_Init(void)
{ 
	SCON = 0x40;                	//���ڷ�ʽ1
	PCON = 0;                   	//SMOD=0
	REN = 1;                     	//�������
	TMOD = 0x20;               		//��ʱ��1��ʱ��ʽ2
	TH1 = 0xFD;                		//12MHz 9600������
	TL1 = 0xFD;			   	   
	TR1 = 1;                  		//������ʱ��
	ES = 1;							//UART�ж�
	EA = 1;							//�ж�ʹ��

}

//����һ��Char
void UART_Send_Char(uchar aChar)
{
 	SBUF = aChar;
 	while(TI == 0);//�ȴ���ֱ�����ͳɹ�
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
//������-----------------------------------------------------------------------

sbit STAT_pinn= P0^0;
uchar *name = "AT+NANE112233\r\n";
uchar *hosten = "AT+HOSTEN0\r\n";
uchar *starten = "AT+STARTEN0\r\n";
uchar *pass = "AT+PASS123456\r\n";
uchar *iscen = "AT+ISCEN\r\n";
uchar *uuid = "AT+UUIDLEN1\r\n";



void main(void)
{ 
	UART_Init();  //��ʼ��UART
	Delay_ms(100);
	UART_Send_String( name,sizeof(name) );//���������㲥��Ϊ112233
	Delay_ms(10);
	UART_Send_String( hosten,sizeof(hosten) );//����ģ��Ϊ�ӻ�ģʽ
	Delay_ms(10);
	UART_Send_String( starten,sizeof(starten) );//���ÿ�������ģʽ
	Delay_ms(10);
	UART_Send_String( pass,sizeof(pass) );//������������
	Delay_ms(10);
	UART_Send_String( iscen,sizeof(iscen) );//���ôӻ����ӿ���
	Delay_ms(10);
	UART_Send_String( uuid,sizeof(uuid) );//����UUIDΪ128λ
	Delay_ms(10);
	while (1)
	{
		if( STAT_pinn==1 )//��������״̬Ϊ�͵�ƽ����ʾ����ģ����APP�Ѿ�����
		{
			uchar cnt = 0;
			UART_Send_String( &cnt,1 );//���ÿ�������ģʽ
			Delay_ms(100);
			if( cnt>100 )cnt=0;
		}
	}	
}

//UART�жϷ������-------------------------------------------------------------
void UART_Receive_Int() interrupt 4
{	
	 uchar receiveData,rec;
   uint i;
        receiveData=SBUF;//��ȥ���յ�������
        rec=SBUF;
        if(rec==0x43)        //���յ�Cʱ
        {for(i=0;i<50;i++)
					{
            DisplayData[0] = smgduan[4];
		        DisplayData[1] = smgduan[3];
		        DisplayData[2] = smgduan[17];  
        }}
				if(rec==0x44)        //���յ�Dʱ
        {for(i=0;i<50;i++)
					{
            DisplayData[0] = smgduan[4];
		        DisplayData[1] = smgduan[4];
		        DisplayData[2] = smgduan[17];  
        }}
				
     
        RI = 0;//��������жϱ�־λ
        SBUF=receiveData;//�����յ������ݷ��뵽���ͼĴ���
        while(!TI);                         //�ȴ������������
        TI=0;                                                 //���������ɱ�־λ
	}	

