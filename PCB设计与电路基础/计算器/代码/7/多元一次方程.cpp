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
        printf("�Ƿ��Լ����룿�ǣ�1����0");
        scanf_s("%d", &t);
        if (t == 1)//�ж��Ƿ��Լ���������
        {
            printf("����δ֪���ĸ���");
            scanf_s("%d", &hang);
            for (i = 0; i < hang; i++)//���������������
            {
                for (j = 0; j < hang + 1; j++)
                {
                    printf("��%d�е�%d�е���Ϊ��", i + 1, j + 1);
                    printf("j=%d hang=%d\n", j, hang);
                    scanf_s("%f", &AA[i][j]);
                }
            }
        }
        for (k = 0; k < hang; k++)//�����ѭ������������½�ת��Ϊ0
        {
            while (AA[k][k] == 0)
            {
                for (m = k + 1; m < hang; m++)
                    for (n = k; n < hang + 1; n++) {
                        AA[k][n] += AA[m][n];
                    }
            }
            for (i = k; i < hang; i++)//����K�������Ϊ1
            {
                temp = AA[i][k];
                for (j = k; j < hang + 1; j++)
                {
                    AA[i][j] /= temp;
                }
            }
            for (i = k + 1; i < hang; i++)//�������������������������ʹ������Ϊ0
            {
                for (j = 0; j < hang + 1; j++)
                {
                    AA[i][j] -= AA[k][j];
                }
            }
        }
        for (k = hang - 2; k >= 0; k--)//�����ѭ������������Ͻ�ת��Ϊ0
        {
            for (i = k + 1; i < hang + 1 - 1; i++)/
            {
                AA[k][hang + 1 - 1] -= AA[k][i] * AA[i][hang + 1 - 1];
                AA[k][i] = 0;
            }
        }

        printf("\nδ֪����ֵΪ��\n");
        for (i = 0; i < hang; i++)//����þ���Ҳ���Ƕ�Ԫһ�η����飩�Ľ�
        {
            printf("x(%d)=\t%g\n", i + 1, AA[i][hang + 1 - 1]);
        }
        printf("\n");
        printf("try again���ǣ�1����0");
        scanf_s("%d", &cf);
    } while (cf == 1);//�ж��Ƿ�����
}