#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double Derivativefunction(double x, double y)    //���������ʽ
{
    return y - 2 * x / y;
}

double function(double x)    //ԭ����
{
    return sqrt(1 + 2 * x);
}

void Euler(double a, double b, double initialValue, int n)
//Euler�㷨��������[a, b]�ϣ���ֵf��a�� = initialValue, n�ȷ�
{
    int i = 0;
    double x = a;
    double y = initialValue;
    double h = (b - a) / n;    //����
    for (i = 1; i <= n; i++)
    {
        printf("x = %lf:\t", x + h);
        y = y + h * Derivativefunction(x, y);
        printf("y(Euler) = %lf\t", y);
        x += h;
        printf("y(׼ȷ) = %lf\n", function(x));
    }
}

void ImprovementEuler(double a, double b, double initialValue, int n)
//ImprovementEuler�㷨�����������Euler����ͬ��
//��Euler�㷨���Ʋ������޸�
{
    int i = 0;
    double x = a;
    double yForecast;    //yԤ��
    double temp = initialValue;
    double yCorrecting;    //У��
    double h = (b - a) / n;    //����
    for (i = 1; i <= n; i++)
    {
        printf("x = %lf:\t", x + h);
        yForecast = temp + h * Derivativefunction(x, temp);
        yCorrecting = temp + (Derivativefunction(x, temp) + \
            Derivativefunction(a + h * i, yForecast)) * h / 2;
        temp = yCorrecting;
        printf("y(ImprovementEuler) = %lf\t", temp);
        x += h;
        printf("y(׼ȷ) = %lf\n", function(x));
    }
}

void FourOrderRung_Kutta(double a, double b, double initialValue, int n)    //�Ľ�Rung_Kutta�㷨
{
    int i = 0;
    double x = a;
    double y = initialValue;
    double h = (b - a) / n;    //����
    double K1, K2, K3, K4;
    for (i = 1; i <= 10; i++)
    {
        printf("x = %lf:\t", x + h);
        K1 = Derivativefunction(x, y);
        K2 = Derivativefunction(x + h / 2, y + K1 * h / 2);
        K3 = Derivativefunction(x + h / 2, y + K2 * h / 2);
        K4 = Derivativefunction(x + h, y + K3 * h);
        y += (K1 + 2 * K2 + 2 * K3 + K4) * h / 6;
        printf("y(FourOrderRung_Kutta) = %lf\t", y);
        x += h;
        printf("y(׼ȷ) = %lf\n", function(x));
    }
}

int main()
{
    printf("------------------------Euler----------------------\n");
    Euler(0, 1, 1, 10);
    printf("\n-------------------ImprovementEuler----------------\n");
    ImprovementEuler(0, 1, 1, 10);
    printf("\n-----------------FourOrderRung_Kutta---------------\n");
    FourOrderRung_Kutta(0, 1, 1, 10);
    return 0;
}
