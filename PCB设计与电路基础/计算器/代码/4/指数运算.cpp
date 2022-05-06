#include<stdio.h>


double power(int n, int m)
{
    double ans;
    int i;
    int flag;

    if (n == 0)
        return 0;
    else if (m == 0)
        return 1;
    else if (m < 0)
    {
        m = -m;
        ans = 1;
        for (i = 1; i <= m; i++)
            ans *= 1 / n;
    }
}


int main()
{
    int i, n, m ;
    scanf_s("%d%d", &n, &m);

    for (i = 0; i < 10; ++i)
        printf("%d ", power(n, m) );
    return 0;
}
