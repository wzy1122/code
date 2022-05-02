// calculator2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>

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

int main()
{
begin:;
	char buffer[100],cpbuffer[100];
	for (int i = 0; i < 99; ++i)
	{
		char ipt;
		scanf_s("%c", &ipt, 1);
		buffer[i] = ipt; cpbuffer[i] = ipt;
		if (buffer[i] == '=')
		{
			break;
		}
	}

	double opbuffer[100];int opcode[100];
	int iptNum=0, currentNum=0,sign,seq=0;
	bool numDone = false;
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
		default:
			break;
		}
		if (numDone)
		{
			if (sign != 7 && sign != 8)
			{
				opbuffer[seq] = (double)currentNum;
				opcode[seq] = 0; seq++; currentNum = 0;
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




	//------------------

	double ans;
	for (int i = 0; i < seq; i++)
	{
		if (opcode[i] == 0)
		{
			ans = opbuffer[i];
			break;
		}
	}
	printf("%f", ans); printf("\n");
	goto begin;
	return 0;
}