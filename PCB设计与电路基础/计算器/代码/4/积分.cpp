// 微分积分.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<iostream>
#include<cmath>
using namespace std; 
int main()
{
	float integral(float(*p)(float),float a,float b,int n);	
	float fsin(float);	float a1,b1,c,(*p)(float);	
	int n;	
	cout<<"输入精确度数n："<<endl;
	cin>>n;	cout<<"input a1,b1:"<<endl;
	cin>>a1>>b1;
	p=fsin;	c=integral(p,a1,b1,n);
	cout<<"the integral of sin(x) is:"<<c<<endl;
	return 0;
}//声明了一个指针p,p指向一个具有一个float类型形参的函数,这个函数返回一个float型值.float integral(float(*p)(float),float a,float b,int n){	int i;	float x,h,s;	h=(b-a)/2;	x=a;//积分下限	s=0;	for(i=0;i<=n;i++)	{		x=x+h;//对函数的自变量x进行迭代		s=s+(*p)(x)*h;对每一个小矩形累加求和	}	return(s);}float fsin(float x){	return sin(x);}

float integral(float(*p)(float), float a, float b, int n) 
{
	int i;	float x, h, s;	
	h = (b - a) / 2;
	x = a;//积分下限	
	s=0;
	for(i=0;i<=n;i++)
	{	
		x=x+h;//对函数的自变量x进行迭代	
		s=s+(*p)(x)*h;//对每一个小矩形累加求和	
	}
	return(s);
}

float fsin(float x)
{   
	return  sin(x);
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
