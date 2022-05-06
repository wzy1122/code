#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//system("cls");For screen clear

//Global variable
bool message = false, setPage = false, setCollumn = false;
unsigned int page, collumn,cPage=0,cCollumn=0; unsigned int matrix[128][64];

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
	for (int i=0; i < 6; ++i)
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

void two()
{
	for (int i = 30; i < 40; i++)
	{
		for (int j = 2; j < 4; j++)
		{
			send(0xb1); send(j);
			send(0x13); send(i);
			send(0x1d); send(0xFF);
		}
	}
	for (int i = 20; i < 50; ++i)
	{
		for (int j = 6; j < 8; ++j)
		{
			send(0xb1); send(j);
			send(0x13); send(i);
			send(0x1d); send(0xFF);
		}
	}
}

int main()
{
	initialize(false);
	writeNum(1); writeNum(2);
	draw(matrix);
	return 0;
}