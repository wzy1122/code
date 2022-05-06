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
	double C = 1;
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
	float buffer[100][2];
	int currentNum = 0, iptNum = 0, cnt = 0, sign = 0, cal = 0;
	bool save = false ;
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
			break;
		case '+':
			save = true; sign = 1; cal++;
			break;
		case '-':
			save = true; sign = 2; cal++;
			break;
		case '*':
			save = true; sign = 3; cal++;
			break;
		case '/':
			save = true; sign = 4; cal++;
			break; 
		case '.':
			save = true; sign = 5; cal++;
			break;
		case '=':
			save = true; sign = 6; 
			break;
		case 'A':
			save = true; sign = 7; cal++;
		case 'C':
			save = true; sign = 8; cal++;
		}
		if (save)
		{
			buffer[cnt][0] = currentNum;
			buffer[cnt][1] = 0;
			currentNum = 0;
			cnt++;
			save = false;
		}
		if (sign != 0)
		{
			if (sign == 6)
			{
				break;
			}
			else
			{
				buffer[cnt][0] = 0;
				buffer[cnt][1] = sign;
				cnt++; sign = 0;
			}
			
		}
	}
	while (cal != 0)
	{
		for (int i = 0; i < cnt-1; i++)//小数点
		{
			if (buffer[i][1] == 5)
			{
				while (buffer[i + 1][0] > 1)
				{
					buffer[i + 1][0] = buffer[i + 1][0] / 10;
				}
				buffer[i][0]=buffer[i - 1][0] + buffer[i + 1][0];
				buffer[i-1][1] = -1; buffer[i+1][1] = -1; buffer[i][1] = 0;
				cal--;
			}
			
		}
		for (int i = 0; i < cnt - 1; i++)//*_/
		{
			if (buffer[i][1] == 3|| buffer[i][1] == 4)
			{
				if (buffer[i][1] == 3)
				{
					int pin1 = 1,pin2 = 1;
					while(buffer[i-pin1][1] !=0)
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
		for (int i = 0; i < cnt - 1; i++)//+-
		{
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
		for (int i = 0; i < cnt - 1; i++)//A C/
		{
			if (buffer[i][1] == 7 || buffer[i][1] == 8)
			{
				if (buffer[i][1] == 7)
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
				if (buffer[i][1] == 8)
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
					buffer[i][0] = C(a,b);
					buffer[i][1] = 0; buffer[i - pin1][1] = -1; buffer[i + pin2][1] = -1;
					cal--;
				}
			}
		}
	}
	
	float ans;
	for (int i = 0; i < cnt - 1; i++)
	{
		if (buffer[i][1] == 0)
		{
			ans = buffer[i][0];
		}
	}
	printf("%f",ans);
	//Wait for end of typing
	//Scan typingzone
	//Do calculation
	//Print/save the result
	getchar();	getchar();	

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
