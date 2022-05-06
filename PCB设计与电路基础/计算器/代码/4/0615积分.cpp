// 0615积分.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <stdio.h>
float Fun1(float x);
float Fun2(float x);
float Integral(float (*f)(float), float a, float b);
int main()
{
    float y1, y2;
    y1 = Integral(Fun1, 0.0, 1.0);
    y2 = Integral(Fun2, 0.0, 3.0);
    printf("y1=%f\ny2=%f\n", y1, y2);
    return 0;
}
/*  函数功能：计算函数1+x*x的函数值 */
float Fun1(float x)
{
    return  1 + x * x;
}

/*  函数功能：计算函数x/(1+x*x)的函数值 */
float Fun2(float x)
{
    return x / (1 + x * x);
}

/*    函数功能：用梯形法计算函数的定积分 */
float Integral(float (*f)(float), float a, float b)
{
    float s, h;
    int n = 100, i;
    s = ((*f)(a) + (*f)(b)) / 2;
    h = (b - a) / n;

    for (i = 1; i < n; i++)
    {
        s += (*f)(a + i * h);
    }

    return s * h;

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
