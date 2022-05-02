#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//system("cls");For screen clear
//Global variable
bool message = false, setPage = false, setCollumn = false, mathError=false;
unsigned int page, collumn, cPage = 0, cCollumn = 0; unsigned int matrix[128][64];

unsigned int num[10][6] =
{
	{0x00,0x72,0x82,0x82,0x82,0x7c},//0
	{0x00,0x00,0x82,0xfe,0x80,0x00},//1
	{0x00,0x84,0xc2,0xa2,0x92,0x8c},//2
	{0x00,0x44,0x82,0x92,0x92,0x6c},//3
	{0x00,0x30,0x28,0x24,0xfe,0x20},//4
	{0x00,0x5e,0x8a,0x8a,0x8a,0x72},//5
	{0x00,0x7c,0x92,0x92,0x92,0x74},//6
	{0x00,0x02,0x02,0xe2,0x1a,0x06},//7
	{0x00,0x6c,0x92,0x92,0x92,0x6c},//8
	{0x00,0x4c,0x92,0X92,0X92,0X7C}//9
};

/*延时：1毫秒的i倍*/
void delay(int i)
{
	int j, k;
	for (j = 0; j < i; j++)
		for (k = 0; k < 110; k++);
}

void draw(unsigned int mtx[128][64])
{
	system("cls");
	for (int j = 0; j < 64; ++j)
	{
		for (int i = 0; i < 128; ++i)
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
	for (int i = 0; i < 128; ++i)
	{
		printf("--");
	}
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
	for (int i = 0; i < 6; ++i)
	{
		send(0xb1); send(cPage);
		send(0x13); send(cCollumn); ++cCollumn;
		send(0x1d); send(num[n][i]);
	}

}

void initialize(bool anti)
{
	int e;
	if (anti)
	{
		e = 0xFF;
	}
	else
	{
		e = 0x00;
	}
	for (int i = 0; i < 128; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			send(0xb1); send(j);
			send(0x13); send(i);
			send(0x1d); send(e);
		}
	}
}
/*
int main()
{
	initialize(false);
	writeNum(1); writeNum(2);
	draw(matrix);
	return 0;
}
*/

int char2num(char cnum)
{
	switch (cnum)
	{
	case '0':return 0; break;
	case '1':return 1; break;
	case '2':return 2; break;
	case '3':return 3; break;
	case '4':return 4; break;
	case '5':return 5; break;
	case '6':return 6; break;
	case '7':return 7; break;
	case '8':return 8; break;
	case '9':return 9; break;
	}
}

double calculate(double x, int priority[100], int topLayer,int opcode[100],double opbuffer[100],int seq)
{
	for (int i = 0; i < 100; ++i)
	{
		if (opcode[i] == 20)
		{
			opcode[i] = 0;
			opbuffer[i] = x;
		}
	}
	for (int l = topLayer; l >= 0; l--)
	{
		//小数点
		for (int i = 0; i < seq - 1; i++)
		{
			if (opcode[i] != l)
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
		for (int i = 0; i < seq - 1; i++)
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
					if (opbuffer[i + pin2] == 0)
					{
						mathError = true;
						return 0;
					}
					opbuffer[i] = opbuffer[i - pin1] / opbuffer[i + pin2];
					opcode[i] = 0; opcode[i - pin1] = -1; opcode[i + pin2] = -1;
				}
			}
		}
		//+-
		for (int i = 0; i < seq - 1; i++)
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

	for (int i = 0; i < seq; i++)
	{
		if (opcode[i] == 0)
		{
			return opbuffer[i];
		}
	}
	return 0;
}


int main()
{
	while (1)
	{
		char buffer[100], cpbuffer[100]; char ipt; bool ctn = false;
		for (int i = 0; i < 99; ++i)
		{
			scanf_s("%c", &ipt, 1);
			buffer[i] = ipt; cpbuffer[i] = ipt;
			if (buffer[i] == '=')
			{
				break;
			}
		}

		double opbuffer[100],opbuffer1[100]; int opcode[100],opcode1[100];
		int iptNum = 0, currentNum = 0, sign, seq = 0;
		bool numDone = false, empty = true;
		for (int i = 0; i < 99; ++i)
		{
			switch (cpbuffer[i])
			{
			case '0':case '1':case '2':
			case '3':case '4':case '5':
			case '6':case '7':case '8':
			case '9':
				iptNum = char2num(cpbuffer[i]);
				currentNum = currentNum * 10 + iptNum;
				empty = false;
				break;
			case '+':
				numDone = true; sign = 1;
				break;
			case '-':
				numDone = true; sign = 2;
				break;
			case '*':
				numDone = true; sign = 3;
				break;
			case '/':
				numDone = true; sign = 4;
				break;
			case '.':
				numDone = true; sign = 5;
				break;
			case '=':
				numDone = true; sign = 6;
				break;
			case '(':
				numDone = true; sign = 7;
				break;
			case ')':
				numDone = true; sign = 8;
				break;
			case 'x':
				numDone = true; sign = 20;
			default:
				break;
			}
			if (numDone)
			{
				if (empty == false)
				{
					opbuffer[seq] = (double)currentNum;
					opcode[seq] = 0; seq++;
					currentNum = 0; empty = true;
				}
				opbuffer[seq] = 0;
				opcode[seq] = sign; seq++;
				numDone = false;
			}
		}
		int priority[100];
		int layer = 0, topLayer = 0;
		for (int i = 0; i < 100; ++i)
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

		initialize(false);

		double scale = 1; int position[99];
		for (int i = 0; i < 50; ++i)
		{
		spot:;
			double x = i * scale; double ans; int pos;
			for (int j = 0; j < 100; ++j)
			{
				opcode1[j] = opcode[j];
				opbuffer1[j] = opbuffer[j];
			}
			ans = calculate(x, priority, topLayer, opcode1, opbuffer1, seq);
			if (mathError)
			{
				ans = calculate(x+scale*0.001, priority, topLayer, opcode1, opbuffer1, seq);
			}
			if (ans >= 31 * scale)
			{
				pos = 32;
			}
			else if (ans <= -31 * scale)
			{
				pos = -32;
			}
			else
			{
				for (int j = 0; j < 31; ++j)
				{
					int j1 = j + 1;
					if (ans >= j*scale && ans < j1 * scale)
					{
						pos = j; break;
					}
					if (ans <= -j * scale && ans > -j1 * scale)
					{
						pos = -j; break;
					}
				}
			}
			pos += 31;
			position[i + 49] = 62-pos;
			if (i > 0)
			{
				i = -i;
				goto spot;
			}
			i = -i;
		}
		for (int i = 0; i < 99; ++i)
		{
			if (position[i] != -1 && position[i] != 63)
			{
				matrix[i][position[i]] = 1;
			}
		}

		draw(matrix);
		while (!ctn)
		{
			scanf_s("%c", &ipt, 1);
			if (ipt == ' ')
			{
				ctn = true;
				system("cls");
			}
		}
	}
	return 0;
}