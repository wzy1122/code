// Calculator.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>
using namespace std;

int A(int n, int m) {
	int i, A = 1;
	int q = n - m + 1;
	for (i = n; i >= q; i--) {
		A = A * i;
	}
	return A;
}

int C(int n, int m) {
	int i1, i2 = 1;
	int C = 1;
	int q = n - m + 1;
	for (i1 = n; i1 >= q; i1--) {
		C = C * i1;
	}
	for (i2 = m; i2 >= 1; i2--) {
		C = C / i2;
	}

	return C;

}
int char2num(char cnum);

int main()
{
	begin:;
	float buffer[100][2];
	int currentNum = 0, iptNum = 0, cnt = 0, sign = 0, cal = 0,
		pi=0;
	bool numFlag = false,signFlag=false,multPi=false,notSpecialSign=true;
	for (int i = 0; i < 99; i++)
	{
		char ipt;
		scanf_s("%c", &ipt,1);
		switch (ipt)
		{
		case '0':case '1':case '2':
		case '3':case '4':case '5':
		case '6':case '7':case '8':
		case '9':
			iptNum = char2num(ipt);
			currentNum = currentNum * 10 + iptNum;
			numFlag = true;
			break;
		case 'p':case 'i':
			signFlag = false;
			if (ipt == 'p'&& pi == 0)
			{pi++;}
			if(ipt == 'i' && pi == 1)
			{pi++;}
			if(pi==2)
			{
				if(numFlag==true)
				{ 
					multPi = true;
					signFlag = true;
				}
				sign = 11;
				pi = 0; 
			}
			break;
		case '+':
			signFlag = true; sign = 1; cal++;
			break;
		case '-':
			signFlag = true; sign = 2; cal++;
			break;
		case '*':
			signFlag = true; sign = 3; cal++;
			break;
		case '/':
			signFlag = true; sign = 4; cal++;
			break; 
		case '.':
			signFlag = true; sign = 5; cal++;
			break;
		case '=':
			signFlag = true; sign = 6;
			break;
		case '(':
			sign = 7;
			break;
		case ')':
			sign = 8;
			break;
		case 'A':
			signFlag = true; sign = 9; cal++;
			break;
		case 'C':
			signFlag = true; sign = 10; cal++;
			break;
		default:
			break;
		
		}
		if (signFlag&&notSpecialSign)
		{
			buffer[cnt][0] = currentNum;
			buffer[cnt][1] = 0;
			currentNum = 0;
			cnt++;
			signFlag = false;
		}
		if (sign != 0)
		{
			if (sign == 6)
			{
				break;
			}
			else if (sign == 11)
			{
				if (multPi == true)
				{
					buffer[cnt][0] = 0;
					buffer[cnt][1] = 3;
					cnt++;
					multPi = false;
				}
				buffer[cnt][0] = 0;
				buffer[cnt][1] = sign;
				cnt++; sign = 0;
				numFlag = true;
				notSpecialSign = false;
			}
			else
			{
				buffer[cnt][0] = 0;
				buffer[cnt][1] = sign;
				cnt++; sign = 0;
				numFlag = false;
				notSpecialSign = true;
			}
		}
	}
	
	//pi
	for (int i = 0; i < cnt; i++)
	{
		if (buffer[i][1] == 11)
		{
			buffer[i][1] = 0;
			buffer[i][0] = 3.1415;
		}
	}
	
	int priority[100];
	int layer=0,topLayer=0;
	for (int i = 0; i < 100; i++)
	{
		if(buffer[i][1]==7)
		{
			buffer[i][1] = -1;
			layer++;
			if(layer>topLayer)
			{
				topLayer=layer;
			}
			priority[i]=0;
		}
		else if(buffer[i][1]==8)
		{
			buffer[i][1] = -1;
			priority[i]=0;
			layer--;
		}
		else if (buffer[i][1] == 0)
		{
			priority[i] = 0;
		}
		else
		{
			priority[i]=layer;
		}
	}

	for(int l=topLayer;l>=0;l--)
	{
		//小数点
		for (int i = 0; i < cnt - 1; i++)
		{
			if (priority[i] != l)
			{
				continue;
			}
			if (buffer[i][1] == 5)
			{
				while (buffer[i + 1][0] >= 1)
				{
					buffer[i + 1][0] = buffer[i + 1][0] / 10;
				}
				buffer[i][0] = buffer[i - 1][0] + buffer[i + 1][0];
				buffer[i - 1][1] = -1; buffer[i + 1][1] = -1; buffer[i][1] = 0;
			}

		}
		//A C
		for (int i = 0; i < cnt - 1; i++)
		{
			if (priority[i] != l)
			{
				continue;
			}
			if (buffer[i][1] == 9 || buffer[i][1] == 10)
			{
				if (buffer[i][1] == 9)
				{
					int pin1 = 1, pin2 = 1;
					while (buffer[i - pin1][1] != 0)
					{
						pin1++;
					}
					while (buffer[i + pin2][1] != 0)
					{
						pin2++;
					}
					int a = buffer[i - pin1][0];
					int b = buffer[i + pin2][0];
					buffer[i][0] = A(a, b);
					buffer[i][1] = 0; buffer[i - pin1][1] = -1; buffer[i + pin2][1] = -1;
					cal--;
				}
				if (buffer[i][1] == 10)
				{
					int pin1 = 1, pin2 = 1;
					while (buffer[i - pin1][1] != 0)
					{
						pin1++;
					}
					while (buffer[i + pin2][1] != 0)
					{
						pin2++;
					}
					int a = buffer[i - pin1][0];
					int b = buffer[i + pin2][0];
					buffer[i][0] = C(a, b);
					buffer[i][1] = 0; buffer[i - pin1][1] = -1; buffer[i + pin2][1] = -1;
					cal--;
				}
			}
		}
		//*_/
		for (int i = 0; i < cnt - 1; i++)
		{
			if (priority[i] != l)
			{
				continue;
			}
			if (buffer[i][1] == 3 || buffer[i][1] == 4)
			{
				if (buffer[i][1] == 3)
				{
					int pin1 = 1, pin2 = 1;
					while (buffer[i - pin1][1] != 0)
					{
						pin1++;
					}
					while (buffer[i + pin2][1] != 0)
					{
						pin2++;
					}
					buffer[i][0] = buffer[i - pin1][0] * buffer[i + pin2][0];
					buffer[i][1] = 0; buffer[i - pin1][1] = -1; buffer[i + pin2][1] = -1;
					cal--;
				}
				if (buffer[i][1] == 4)
				{
					int pin1 = 1, pin2 = 1;
					while (buffer[i - pin1][1] != 0)
					{
						pin1++;
					}
					while (buffer[i + pin2][1] != 0)
					{
						pin2++;
					}
					buffer[i][0] = buffer[i - pin1][0] / buffer[i + pin2][0];
					buffer[i][1] = 0; buffer[i - pin1][1] = -1; buffer[i + pin2][1] = -1;
					cal--;
				}
			}
		}
		//+-
		for (int i = 0; i < cnt - 1; i++)
		{
			if (priority[i] != l)
			{
				continue;
			}
			if (buffer[i][1] == 1 || buffer[i][1] == 2)
			{
				if (buffer[i][1] == 1)
				{
					int pin1 = 1, pin2 = 1;
					while (buffer[i - pin1][1] != 0)
					{
						pin1++;
					}
					while (buffer[i + pin2][1] != 0)
					{
						pin2++;
					}
					buffer[i][0] = buffer[i - pin1][0] + buffer[i + pin2][0];
					buffer[i][1] = 0; buffer[i - pin1][1] = -1; buffer[i + pin2][1] = -1;
					cal--;
				}
				if (buffer[i][1] == 2)
				{
					int pin1 = 1, pin2 = 1;
					while (buffer[i - pin1][1] != 0)
					{
						pin1++;
					}
					while (buffer[i + pin2][1] != 0)
					{
						pin2++;
					}
					buffer[i][0] = buffer[i - pin1][0] - buffer[i + pin2][0];
					buffer[i][1] = 0; buffer[i - pin1][1] = -1; buffer[i + pin2][1] = -1;
					cal--;
				}
			}
		}
		
	}


	//------------------
	
	float ans;
	for (int i = 0; i < cnt; i++)
	{
		if (buffer[i][1] == 0)
		{
			ans = buffer[i][0];
			break;
		}
	}
	printf("%f",ans);
	

	goto begin;
	return 0;
}

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


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
