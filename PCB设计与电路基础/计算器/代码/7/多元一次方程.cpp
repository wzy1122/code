#include <stdio.h>
#include <math.h>
#define Han 200
#define JUZHEN {},{},{},{}
int main()
{
    int i, j, k, m, n, t, cf, hang = 4;
    float temp;
    float AA[Han][Han + 1] = { JUZHEN };
    do {

        printf("\n");
        printf("是否自己输入？是：1；否：0");
        scanf_s("%d", &t);
        if (t == 1)//判断是否自己输入数组
        {
            printf("输入未知数的个数");
            scanf_s("%d", &hang);
            for (i = 0; i < hang; i++)//输入所定义的数组
            {
                for (j = 0; j < hang + 1; j++)
                {
                    printf("第%d行第%d列的数为：", i + 1, j + 1);
                    printf("j=%d hang=%d\n", j, hang);
                    scanf_s("%f", &AA[i][j]);
                }
            }
        }
        for (k = 0; k < hang; k++)//这个大循环将数组的左下角转化为0
        {
            while (AA[k][k] == 0)
            {
                for (m = k + 1; m < hang; m++)
                    for (n = k; n < hang + 1; n++) {
                        AA[k][n] += AA[m][n];
                    }
            }
            for (i = k; i < hang; i++)//将第K列下面变为1
            {
                temp = AA[i][k];
                for (j = k; j < hang + 1; j++)
                {
                    AA[i][j] /= temp;
                }
            }
            for (i = k + 1; i < hang; i++)//将下面的数列与上面的数列相减使其下面为0
            {
                for (j = 0; j < hang + 1; j++)
                {
                    AA[i][j] -= AA[k][j];
                }
            }
        }
        for (k = hang - 2; k >= 0; k--)//这个大循环将数组的右上角转化为0
        {
            for (i = k + 1; i < hang + 1 - 1; i++)/
            {
                AA[k][hang + 1 - 1] -= AA[k][i] * AA[i][hang + 1 - 1];
                AA[k][i] = 0;
            }
        }

        printf("\n未知数的值为：\n");
        for (i = 0; i < hang; i++)//输出该矩阵（也就是多元一次方程组）的解
        {
            printf("x(%d)=\t%g\n", i + 1, AA[i][hang + 1 - 1]);
        }
        printf("\n");
        printf("try again？是：1；否：0");
        scanf_s("%d", &cf);
    } while (cf == 1);//判断是否重试
}