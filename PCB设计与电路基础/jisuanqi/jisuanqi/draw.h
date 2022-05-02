#include<math.h>
#include<stdio.h>

int max(int a, int b)
{
    return a > b ? a : b;
}

int sishewuru(float a);
void plot(double a[],double b[], int width , int hight)//画图 
{
    int n = 320;
    int i, j;
    int A[500], B[500];
    for (i = 0; i < n; i++)
    {
        a[i] = width * a[i];
        b[i] = hight * b[i];
        A[i] = sishewuru(a[i]);
        B[i] = sishewuru(b[i]);
    }
    for (i = 240; i >= 0; i--)
    {
        for (j = 0; j < n; j++)
        {
            if (j == 160) printf("|");
            else if (i == 120) printf("-");
            else if (i == B[j] + 120)
            {
                printf("*");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}
void draw_linear(double a,double b,int width,int height)//一次方程 
{
	int n=320;
	double x[500],y[500],X;
	for (int i = 0; i < n; i++)
    {
        x[i] = x[i - 1] + (double) 1 / (double) width;
        X= x[i] - (double) 160 / (double) width;
        y[i] = a * X + b;
    }
    plot(x,y,width,height);
}
void draw_quadratic(double a,double b,double c,int width,int height)//二次方程 
{
	int n=320;
	double x[500],y[500],X;
	for (int i = 0; i < n; i++)
    {
        x[i] = x[i - 1] + (double) 1 / (double) width;
        X= x[i] - (double) 160 / (double) width;
        y[i] = (a * X + b)* X  +c;
    }
    plot(x,y,width,height);
}
void draw_cubic(double a,double b,double c,double d,int width,int height)//三次方程 
{
	int n=320;
	double x[500],y[500],X;
	for (int i = 0; i < n; i++)
    {
        x[i] = x[i - 1] + (double) 1 / (double) width;
        X= x[i] - (double) 160 / (double) width;
        y[i] = ((a * X + b) * X  + c) * X + d;
    }
    plot(x,y,width,height);
}
void draw_cos(double a,double b,double c,double d,int width,int height)//cos
{
	int n=320;
	double x[500],y[500],X;
	for (int i = 0; i < n; i++)
    {
        x[i] = x[i - 1] + (double) 1 / (double) width;
        X= x[i] - (double) 160 / (double) width;
        y[i] = a * (cos( b * X + c )) + d;
    }
    plot(x,y,width,height);
}
void draw_sin(double a,double b,double c,double d,int width,int height)//sin
{
	int n=320;
	double x[500],y[500],X;
	for (int i = 0; i < n; i++)
    {
        x[i] = x[i - 1] + (double) 1 / (double) width;
        X= x[i] - (double) 160 / (double) width;
        y[i] = a * (sin( b * X + c )) + d;
    }
    plot(x,y,width,height);
}
void draw_log(double k,double a,double b,double c,double d,int width,int height)// k 底数  
{
	int n=320;
	double x[500],y[500],X;
	for (int i = 0; i < n; i++)
    {
        x[i] = x[i - 1] + (double) 1 / (double) width;
        X= x[i] - (double) 160 / (double) width;
        y[i] = a * (log( b * X + c )/log(k)) + d;
    }
    plot(x,y,width,height);
}
void draw_inverse(double a,double b,double c,double d,int width,int height)// 反比例函数 
{
	int n=320;
	double x[500],y[500],X;
	for (int i = 0; i < n; i++)
    {
        x[i] = x[i - 1] + (double) 1 / (double) width;
        X= x[i] - (double) 160 / (double) width;
        y[i] = a / ( b*X + c )  + d;
    }
    plot(x,y,width,height);
}
void draw_exponential(double k,double a,double b,double c,double d,int width,int height)// k 的 bx+c 次方 
{
	int n=320;
	double x[500],y[500],X;
	for (int i = 0; i < n; i++)
    {
        x[i] = x[i - 1] + (double) 1 / (double) width;
        X= x[i] - (double) 160 / (double) width;
        y[i] = a * pow( k , b*X + c )  + d;
    }
    plot(x,y,width,height);
}
int sishewuru(float a)//四舍五入 
{
    int m;
    if (a - floor(a) < 0.5)
    {
        m = a;
        return m;
    }
    else
    {
        m = a + 1;
        return m;
    }
}

//int main()
//{
//    draw_exponential(2,1,1,0,0,50,50);//2^x
//    draw_inverse(1,1,0,0,50,50);//1/x 
//    draw_log(2,1,1,0,0,50,50);//以2为低，log2x 
//    draw_sin(1,1,0,0,50,50);//sinx
//    draw_cos(1,1,0,0,50,50);//cosx
//	draw_linear(1,0,50,50);//x
//	draw_quadratic(1,0,0,50,50);//x^2
//	draw_cubic(1,0,0,0,50,50);//x^3
//}

