#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<cstring>
#include <math.h>
//system("cls");For screen clear
//Global variable

bool message = false, setPage = false, setCollumn = false; int biaoshifu = 0;
unsigned int page, collumn,cPage=0,cCollumn=0; unsigned int matrix[126][64];
int setups[100];
unsigned int num[19][6] =
{
	{0x00,0x7c,0x82,0x82,0x82,0x7c},//0
	{0x00,0x00,0x82,0xfe,0x80,0x00},//1
	{0x00,0x84,0xc2,0xa2,0x92,0x8c},//2
	{0x00,0x44,0x82,0x92,0x92,0x6c},//3
	{0x00,0x30,0x28,0x24,0xfe,0x20},//4
	{0x00,0x5e,0x8a,0x8a,0x8a,0x72},//5
	{0x00,0x7c,0x92,0x92,0x92,0x74},//6
	{0x00,0x02,0x02,0xe2,0x1a,0x06},//7
	{0x00,0x6c,0x92,0x92,0x92,0x6c},//8
	{0x00,0x4c,0x92,0X92,0X92,0X7C},//9
    {0x00,0X10,0X10,0X7C,0X10,0X10},//+
	{0x00,0X10,0X10,0X10,0X10,0X10},//-
	{0x00,0x44,0x28,0x10,0x28,0x44},//*
	{0x00,0x10,0x10,0x54,0x10,0x10},///
	{0x00,0x00,0x00,0xc0,0x00,0x00},//.
	{0x00,0x3c,0x66,0xc3,0x00,0x00},//(
	{0x00,0x00,0xc3,0x66,0x3c,0x00},//)
	{0X00,0XC0,0XC0,0XC0,0XC0,0XC0},//_
	{0X00,0X28,0X28,0X28,0X28,0X28},// =
};

/*延时：1毫秒的i倍*/
void delay(int i)
{
	int j, k;
	for (j = 0; j < i; j++)
		for (k = 0; k < 110; k++);
}

void draw(unsigned int mtx[126][64])
{
	system("cls");
	for (int j = 0; j < 64; ++j)
	{
		for (int i = 0; i < 126; ++i)
		{
			if (mtx[i][j] == 1)
			{
				printf("**");
			}
			else
			{
				printf("  ");
			}
		}
		printf("|\n");
	}
	for (int i = 0; i < 126; ++i)
	{
		printf("--");
	}
	printf("\n");
}

void draw8px(int a)
{
	int row = page * 8;
	for (int i = 0; i <= 7; ++i)
	{
		matrix[collumn][row + i] = a % 2;
		a = (a - a % 2) / 2;
	}
}


void send(unsigned int a)
{
	if (message)
	{
		if (setPage)
		{
			page = a; setPage = false; message = false;
		}
		else if (setCollumn)
		{
			collumn = a; setCollumn = false; message = false;
		}
		else
		{
			draw8px(a);
			message = false;
		}
	}
	else
	{
		switch (a)
		{
		case 0xb1:
			setPage = true; message = true;
			break;
		case 0x13:
			setCollumn = true; message = true;
			break;
		case 0x1d:
			message = true;
			break;
		}
	}
}

void writeNum(int n)
{
	for (int i=0; i < 6; ++i)
	{
		send(0xb1); send(cPage);
		send(0x13); send(cCollumn); 
		if (cCollumn != 126) { ++cCollumn; }
		else { cCollumn = 1; cPage++; };
		send(0x1d); send(num[n][i]);
	}

}
void jinwei()
{
	if (cCollumn == 120) { cPage++; cCollumn = 0; }
	else { cCollumn = cCollumn + 6; }
}
void tuiwei()
{
	if (cCollumn == 0) { cPage--; cCollumn = 120; }
	else { cCollumn = cCollumn - 6; }
}
void replaced_front()//向前
{
	if (cCollumn == 0)
	{
		cPage = cPage - 1;
		cCollumn = 120;
		int aq = cCollumn;
		for (int i = 0; i < 6; i++)
		{
			send(0xb1); send(cPage);
			send(0x13); send(cCollumn); cCollumn++;
			send(0x1d); send(num[17][i]);
		}
		cCollumn = 120;
	}
	else
	{
		cCollumn = cCollumn - 6;
		for (int i = 0; i < 6; i++)
		{
			send(0xb1); send(cPage);
			send(0x13); send(cCollumn); cCollumn++;
			send(0x1d); send(num[17][i]);
		}
		cCollumn = cCollumn - 6;
	}
}
void replacedf(int n)
{
	for (int i = 0; i < 6; i++)
	{
		send(0xb1); send(cPage);
		send(0x13); send(cCollumn); cCollumn++;
		send(0x1d); send(num[n][i]);
		}
	cCollumn = cCollumn-6;
}
void replacedh()
{
		for (int i = 0; i < 6; i++)
		{
			send(0xb1); send(cPage);
			send(0x13); send(cCollumn); cCollumn++;
			send(0x1d); send(0X00);
		}
		if (cCollumn == 0) { cPage--; cCollumn = 120; }
		else { cCollumn = cCollumn - 6; }
}
void deleted()//删除键
{
	if (cCollumn == 0)
	{
		cPage = cPage - 1;
		cCollumn = 120;
		int aq = cCollumn;
		for (int i = 0; i < 6; i++)
		{
			send(0xb1); send(cPage);
			send(0x13); send(cCollumn); cCollumn++;
			send(0x1d); send(0X00);
		}
		cCollumn = 120;
		setups[int(cPage * 21 + cCollumn / 6)] = -1;
	}
	else
	{
		cCollumn = cCollumn - 6;
		for (int i = 0; i < 6; i++)
		{
			send(0xb1); send(cPage);
			send(0x13); send(cCollumn); cCollumn++;
			send(0x1d); send(0X00);
		}
		cCollumn = cCollumn - 6;
		setups[int(cPage * 21 + cCollumn / 6)] = -1;
	}
	jinwei();
	if (setups[int(cPage * 21 + cCollumn / 6)] != -1)
	{
		while (setups[int(cPage * 21 + cCollumn / 6)] != -1)
		{
			int asb = cPage; int bsb = cCollumn;
			tuiwei();
			setups[int(cPage * 21 + cCollumn / 6)] = setups[int(asb * 21 + bsb / 6)];
			writeNum(setups[int(cPage * 21 + cCollumn / 6)]);
			setups[int(cPage * 21 + cCollumn / 6)] = -1;
			jinwei();
		}
		tuiwei();
		replacedh();
	}
	else { tuiwei(); }
}

int judge(char id)
{
	switch (id)
	{
	    case '0': {return 0; break; }
	    case '1': {return 1; break; }
		case '2': {return 2; break; }
		case '3': {return 3; break; }
		case '4': {return 4; break; }
		case '5': {return 5; break; }
		case '6': {return 6; break; }
		case '7': {return 7; break; }
		case '8': {return 8; break; }
		case '9': {return 9; break; }
		case '+': {return 10; break; }
		case '-': {return 11; break; }
		case '*': {return 12; break; }
		case '/': {return 13; break; }
		case '.': {return 14; break; }
		case '(': {return 15; break; }
		case ')': {return 16; break; }
		case '=': {return 18; break; }
	}
}

void writesetup(int bp)
{
	setups[int(cPage*21+cCollumn / 6)] = bp;
}
int xiaoshubufen(double n)
{
	biaoshifu = 0; int qudiao = 0;
	int xiaoshu = int((n - int(n))*1000000);
	if (xiaoshu!=0)
	{
		while (xiaoshu % 10 == 0)
		{
			xiaoshu = xiaoshu / 10;
			qudiao++;
		}
		int xiaoshuweishued = int(floor(log10(xiaoshu))) + 1; //算出小数的位数
		biaoshifu = (6 - qudiao) - xiaoshuweishued;
	}
	return(xiaoshu);
}
void jisuan()
{
	double opbuffer[100]; int opcode[100]; memset(opcode, 0, sizeof(int) * 100); memset(opbuffer, 0, sizeof(double) * 100);
	int currentNum = 0, sign, seq = 0;
	bool numDone = false;
	for (int i = 0; i < 99; ++i)
	{
		if (setups[i] != -1)
		{
			switch (setups[i])
			{
			case 0:case 1:case 2:
			case 3:case 4:case 5:
			case 6:case 7:case 8:
			case 9:
				currentNum = currentNum * 10 + setups[i];
				break;
			case 10://  +
				numDone = true; sign = 1;
				break;
			case 11://  -
				numDone = true; sign = 2;
				break;
			case 12://  *
				numDone = true; sign = 3;
				break;
			case 13://  /
				numDone = true; sign = 4;
				break;
			case 14://  .
				numDone = true; sign = 5;
				break;
			case 18://  =
				numDone = true; sign = 6;
				break;
			case 15://  (
				numDone = true; sign = 7;
				break;
			case 16://  )
				numDone = true; sign = 8;
				break;
			default:
				break;
			}
			if (numDone)
			{
				if (sign != 7&&sign !=8)
				{
					opbuffer[seq] = currentNum;
					opcode[seq] = 0; seq++; currentNum = 0;
				}
				if (sign == 6) { break; }
				opbuffer[seq] = 0;
				opcode[seq] = sign; seq++;
				numDone = false;
			}
		}
	}
	int priority[100];
	int layer = 0, topLayer = 0;
	for (int i = 0; i < 99; ++i)
	{
		if (opcode[i] == 7)
		{
			opcode[i] = -1;
			layer++;
			if (layer > topLayer)
			{
				topLayer = layer;
			}
			priority[i] = 0;
		}
		else if (opcode[i] == 8)
		{
			opcode[i] = -1;
			priority[i] = 0;
			layer--;
		}
		else if (opcode[i] == 0)
		{
			priority[i] = 0;
		}
		else
		{
			priority[i] = layer;
		}
	}
	for (int l = topLayer; l >= 0; l--)
	{
		//小数点
		for (int i = 0; i < seq; i++)
		{
			if (priority[i] != l)
			{
				continue;
			}
			if (opcode[i] == 5)
			{
				while (opbuffer[i + 1] >= 1)
				{
					opbuffer[i + 1] = opbuffer[i + 1] / 10;
				}
				opbuffer[i] = opbuffer[i - 1] + opbuffer[i + 1];
				opcode[i - 1] = -1; opcode[i + 1] = -1; opcode[i] = 0;
			}

		}
		//*_/
		for (int i = 0; i < seq; i++)
		{
			if (priority[i] != l)
			{
				continue;
			}
			if (opcode[i] == 3 || opcode[i] == 4)
			{
				int pin1 = 1, pin2 = 1;
				while (opcode[i - pin1] != 0)
				{
					pin1++;
				}
				while (opcode[i + pin2] != 0)
				{
					pin2++;
				}
				if (opcode[i] == 3)
				{
					opbuffer[i] = opbuffer[i - pin1] * opbuffer[i + pin2];
					opcode[i] = 0; opcode[i - pin1] = -1; opcode[i + pin2] = -1;
				}
				else if (opcode[i] == 4)
				{
					opbuffer[i] = opbuffer[i - pin1] / opbuffer[i + pin2];
					opcode[i] = 0; opcode[i - pin1] = -1; opcode[i + pin2] = -1;
				}
			}
		}
		//+-
		for (int i = 0; i < seq; i++)
		{
			if (priority[i] != l)
			{
				continue;
			}
			if (opcode[i] == 1 || opcode[i] == 2)
			{

				int pin1 = 1, pin2 = 1;
				while (opcode[i - pin1] != 0)
				{
					pin1++;
				}
				while (opcode[i + pin2] != 0)
				{
					pin2++;
				}
				if (opcode[i] == 1)
				{
					opbuffer[i] = opbuffer[i - pin1] + opbuffer[i + pin2];
					opcode[i] = 0; opcode[i - pin1] = -1; opcode[i + pin2] = -1;
				}
				else if (opcode[i] == 2)
				{
					opbuffer[i] = opbuffer[i - pin1] - opbuffer[i + pin2];
					opcode[i] = 0; opcode[i - pin1] = -1; opcode[i + pin2] = -1;

				}
			}
		}
	}
	double ans;
	for (int i = 0; i < seq; i++)
	{
		if (opcode[i] == 0)
		{
			ans = opbuffer[i];
			break;
		}
	};
	double ansed = ans; 
	if (ans < 0) { ans = -ans; }
	int zhengshu =int(ans);
	int xiaoshu = xiaoshubufen(ans);
	int paged = 7, collumned = 0;
	while (collumned < 126)//清空输出兰
	{
		for (int i = 0; i < 6; i++)
		{
			send(0xb1); send(paged);
			send(0x13); send(collumned); collumned++;
			send(0x1d); send(0X00);
		}
	}
	paged = 7, collumned = 0;
	int at[6], bt[6];
	int ij, iq, zhengshuweishu,xiaoshuweishu;
	if (zhengshu != 0) { zhengshuweishu = int(floor(log10(zhengshu))) + 1; }
	else { zhengshuweishu = 1; };//算出整数的位数
	if(xiaoshu!=0){xiaoshuweishu = int(floor(log10(xiaoshu))) + 1;}//算出小数的位数
	else { xiaoshuweishu = 1; }
	
	if (zhengshu != 0)
	{
		for (ij = 0; ij < zhengshuweishu; ij++)
		{
			at[ij] = zhengshu % 10;
			zhengshu = int(zhengshu / 10);
		}
	}
	else { at[0] = 0; };
	if (xiaoshu != 0)
	{
		
		for (ij =0; ij < xiaoshuweishu; ij++)
		{
			bt[ij] = xiaoshu % 10;
			xiaoshu =int( xiaoshu / 10);
		}
		if (biaoshifu != 0)
		{
			for (ij = xiaoshuweishu; ij < xiaoshuweishu+biaoshifu; ij++){bt[ij] = 0;}
		};
	}
	else { bt[0] = 0; };
	if (ansed < 0)//负号
	{
		for (int iq = 0; iq < 6; iq++)
		{
			send(0xb1); send(paged);
			send(0x13); send(collumned); collumned++;
			send(0x1d); send(num[11][iq]);
		}
	}
	for (ij = zhengshuweishu-1;ij>=0; ij--)//遍历整数字符串。
	{
		for (iq = 0; iq < 6; iq++)
		{
			send(0xb1); send(paged);
			send(0x13); send(collumned); collumned++;
			send(0x1d); send(num[at[ij]][iq]);
		}

	}
	for (iq = 0; iq < 6; iq++)//遍历小数点。
	{
		send(0xb1); send(paged);
		send(0x13); send(collumned); collumned++;
		send(0x1d); send(num[14][iq]);
	}
	for (ij = xiaoshuweishu+biaoshifu-1;ij>=0; ij--)//遍历小数字符串。
	{
		for (iq = 0; iq < 6; iq++)
		{
			send(0xb1); send(paged);
			send(0x13); send(collumned); collumned++;
			send(0x1d); send(num[bt[ij]][iq]);
		}
	}
}

int main()
{
	char id; memset(setups, -1, sizeof(int) * 100);
    begin:;
	draw(matrix); scanf_s("%c", &id, 1);
	if (id != '\n' && id != 'B' && id != 'F' && id != 'H' && id != 'D'&&id!='Q')
	{
		int numed = judge(id);
		writesetup(numed); writeNum(numed);
		if (setups[int(cPage * 21 + cCollumn / 6)] != -1)
		{
			writeNum(17);
			if (cCollumn == 0) { cPage--; cCollumn = 120; }
			else { cCollumn = cCollumn - 6; }
		}
	}
	else if (id == 'B') { deleted(); }//删除键
	else if (id == 'F')//向左移动
	{
		if (setups[int(cPage * 21 + cCollumn / 6)] != -1) { replacedf(setups[int(cPage * 21 + cCollumn / 6)]); replaced_front(); }
		else { replacedh(); replaced_front(); }
	}
	else if (id == 'H')
	{
		if (setups[int(cPage * 21 + cCollumn / 6)] != -1)
		{
			writeNum(setups[int(cPage * 21 + cCollumn / 6)]); writeNum(17);
			if (cCollumn == 0) { cPage--; cCollumn = 120; }
			else { cCollumn = cCollumn - 6; }
		}
	}
	else if (id == 'D') {writesetup(18); writeNum(18); jisuan(); }
	else if (id == 'Q')
	{
		memset(setups, -1, sizeof(int) * 100); memset(matrix, 0, sizeof(unsigned int) * 126*64);
		cPage = 0; cCollumn = 0;
	}
	goto begin;
	return 0;
}