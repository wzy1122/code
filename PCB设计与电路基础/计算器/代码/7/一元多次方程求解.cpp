#include <math.h>
#include <iostream>
#include <stdio.h>
#define EPS 1e-6

int solve_quadratic_equation(float p[], float x[])
{

#define ZERO 1.0e-30
    float a, b, c, delta, sqrtDelta;

    a = p[2];
    b = p[1];
    c = p[0];

    if (fabs(a - 0.0) < EPS)
    {
        if (fabs(b - 0.0) < EPS)
        {
            return 0;
        }
        else
        {
            x[0] = -c / b;
            return 1;
        }
    }
    else
    {
        delta = b * b - 4.0 * a * c;
        if (delta > ZERO)
        {
            if (fabs(c - 0.0) < EPS)    //若c = 0,由于计算误差,sqrt(b*b - 4*a*c）不等于b
            {
                x[0] = 0.0;
                x[1] = -b / a;
            }
            else
            {
                sqrtDelta = sqrt(delta);
                if (b > 0.0)
                {
                    x[0] = (-2.0 * c) / (b + sqrtDelta);    //避免两个很接近的数相减,导致精度丢失
                    x[1] = (-b - sqrtDelta) / (2.0 * a);
                }
                else
                {
                    x[0] = (-b + sqrtDelta) / (2.0 * a);
                    x[1] = (-2.0 * c) / (b - sqrtDelta);    //避免两个很接近的数相减,导致精度丢失
                }
            }
            return 2;
        }
        else if (fabs(delta - 0.0) < EPS)
        {
            x[0] = x[1] = -b / (2.0 * a);
        }
        else
        {
            return 0;
        }
    }

#undef ZERO
}

int solve_cubic_equation(float p[], float x[])
{

#define ZERO 1.0e-30
    float a, b, c, d, A, B, C, delta;
    float Y1, Y2, Z1, Z2, K, parm[3], roots[2], theta, T;

    a = p[3];
    b = p[2];
    c = p[1];
    d = p[0];

    if (fabs(a - 0.0) < EPS)
    {
        parm[2] = b;
        parm[1] = c;
        parm[0] = d;

        return solve_quadratic_equation(parm, x);
    }
    else
    {
        A = b * b - 3.0 * a * c;
        B = b * c - 9.0 * a * d;
        C = c * c - 3.0 * b * d;

        delta = B * B - 4.0 * A * C;

        if (fabs(A - 0.0) < EPS && fabs(B - 0.0) < EPS)
        {
            x[0] = x[1] = x[2] = -b / (3.0 * a);
            return 3;
        }

        if (delta > ZERO)
        {
            parm[2] = 1.0;
            parm[1] = B;
            parm[0] = A * C;

            solve_quadratic_equation(parm, roots);
            Z1 = roots[0];
            Z2 = roots[1];

            Y1 = A * b + 3.0 * a * Z1;
            Y2 = A * b + 3.0 * a * Z2;

            if (Y1 < 0.0 && Y2 < 0.0)    //pow函数的底数必须为非负数,必须分类讨论
            {
                x[0] = (-b + pow(-Y1, 1.0 / 3.0) + pow(-Y2, 1.0 / 3.0)) / (3.0 * a);
            }
            else if (Y1 < 0.0 && Y2 > 0.0)
            {
                x[0] = (-b + pow(-Y1, 1.0 / 3.0) - pow(Y2, 1.0 / 3.0)) / (3.0 * a);
            }
            else if (Y1 > 0.0 && Y2 < 0.0)
            {
                x[0] = (-b - pow(Y1, 1.0 / 3.0) + pow(-Y2, 1.0 / 3.0)) / (3.0 * a);
            }
            else
            {
                x[0] = (-b - pow(Y1, 1.0 / 3.0) - pow(Y2, 1.0 / 3.0)) / (3.0 * a);
            }
            return 1;
        }
        else if (fabs(delta - 0.0) < EPS)
        {
            if (fabs(A - 0.0) > EPS)
            {
                K = B / A;
                x[0] = -b / a + K;
                x[1] = x[2] = -0.5 * K;
                return 3;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            if (A > 0.0)
            {
                T = (2.0 * A * b - 3.0 * a * B) / (2.0 * pow(A, 3.0 / 2.0));
                if (T > 1.0)    //由于计算误差,T的值可能略大于1(如1.0000001)
                {
                    T = 1.0;
                }
                if (T < -1.0)
                {
                    T = -1.0;
                }
                theta = acos(T);
                x[0] = (-b - 2.0 * sqrt(A) * cos(theta / 3.0)) / (3.0 * a);
                x[1] = (-b + sqrt(A) * (cos(theta / 3.0) + sqrt(3.0) * sin(theta / 3.0))) / (3.0 * a);
                x[2] = (-b + sqrt(A) * (cos(theta / 3.0) - sqrt(3.0) * sin(theta / 3.0))) / (3.0 * a);
                return 3;
            }
            else
            {
                return 0;
            }
        }
    }

#undef ZERO
}


int solve_quartic_equation(float p[], float x[])
{


    float a, b, c, d, e;
    float parm[4], roots[3];
    float y, M, N;
    float x1[2], x2[2];
    int rootCount1, rootCount2, rootCount, i;
    float MSquareTemp, MSquare, yTemp;

    a = p[4];
    b = p[3];
    c = p[2];
    d = p[1];
    e = p[0];

    if (fabs(a - 0.0) < EPS)
    {
        if (fabs(b - 0.0) < EPS)
        {
            parm[2] = c;
            parm[1] = d;
            parm[0] = e;
            return solve_quadratic_equation(parm, x);
        }
        else
        {
            parm[3] = b;
            parm[2] = c;
            parm[1] = d;
            parm[0] = e;
            return solve_cubic_equation(parm, x);
        }
    }
    else
    {
        b = b / a;
        c = c / a;
        d = d / a;
        e = e / a;

        parm[3] = 8.0;
        parm[2] = -4.0 * c;
        parm[1] = 2.0 * (b * d - 4.0 * e);
        parm[0] = -e * (b * b - 4.0 * c) - d * d;

        if (rootCount = solve_cubic_equation(parm, roots))
        {
            y = roots[0];
            MSquare = 8.0 * y + b * b - 4.0 * c;
            for (i = 1; i < rootCount; i++)
            {
                yTemp = roots[i];
                MSquareTemp = 8.0 * yTemp + b * b - 4.0 * c;
                if (MSquareTemp > MSquare)
                {
                    MSquare = MSquareTemp;
                    y = yTemp;
                }
            }

            if (MSquare > 0.0)
            {
                M = sqrt(MSquare);
                N = b * y - d;
                parm[2] = 2.0;
                parm[1] = b + M;
                parm[0] = 2.0 * (y + N / M);
                rootCount1 = solve_quadratic_equation(parm, x1);

                parm[2] = 2.0;
                parm[1] = b - M;
                parm[0] = 2.0 * (y - N / M);
                rootCount2 = solve_quadratic_equation(parm, x2);

                if (rootCount1 == 2)
                {
                    x[0] = x1[0];
                    x[1] = x1[1];
                    x[2] = x2[0];
                    x[3] = x2[1];
                }
                else
                {
                    x[0] = x2[0];
                    x[1] = x2[1];
                    x[2] = x1[0];
                    x[3] = x1[1];
                }
                return rootCount1 + rootCount2;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }

}

int quadratic_equation()
{

    float a, b, c, disc, p, q;

    printf("一元二次方程，请按顺序输入二次项系数，一次项系数，常数，用空格隔开");
    scanf_s("%f%f%f", &a, &b, &c);
    if (fabs(a) <= EPS)
    {
        printf("No\n");
        exit(0);
    }
    disc = b * b - 4 * a * c;
    if (fabs(disc) <= EPS)
    {
        printf("x1=x2=%.2f\n", -b / (2 * a));
    }
    else
    {
        p = -b / (2 * a);
        q = sqrt(fabs(disc)) / (2 * a);
        if (disc > 1e-6)
        {
            printf("Two:x1=%.2f,x2=%.2f\n", p + q, p - q);
        }
        else
        {
            printf("复数根");
            printf("x1=%.2f+%.2fi\n", p, q);
            printf("x2=%.2f-%.2fi\n", p, q);
        }
    }
}
int cubic_equation()
{
    int z;
    for (z = 0;; ++z)
    {
        float a, b, c, d;
        int ab, g, bc, cd, de, ef;
        printf("\n一元三次方程求实数解近似值\n\n");
        printf("aX^3+bX^2+cX+d=0\n\n请输入系数:\n\na=");
        bc = scanf_s("%f", &a);
        for (g = 0;; ++g)
        {
            if (bc == 1)
            {
                break;
            }
            if (bc != 1)
            {
                fflush(stdin);
                printf("\n格式错误,请重新输入\n\na=");
                bc = scanf_s("%f", &a);
            }
        }
        for (g = 0;; ++g)
        {
            if (a == 0)
            {
                printf("\na不能为0,不然就不是三次方程了\n\n请重新输入一次吧!\n\na=");
                scanf_s("%f", &a);
            }
            if (a != 0)
            {
                break;
            }
        }
        printf("\nb=");
        cd = scanf_s("%f", &b);
        for (g = 0;; ++g)
        {
            if (cd == 1)
            {
                break;
            }
            if (cd != 1)
            {
                fflush(stdin);
                printf("\n格式错误，请重新输入\n\nb=");
                cd = scanf_s("%f", &b);
            }
        }
        printf("\nc=");
        de = scanf_s("%f", &c);
        for (g = 0;; ++g)
        {
            if (de == 1)
            {
                break;
            }
            if (de != 1)
            {
                fflush(stdin);
                printf("\n格式错误,请重新输入\n\nc=");
                de = scanf_s("%f", &c);
            }
        }
        printf("\nd=");
        ef = scanf_s("%f", &d);
        for (g = 0;; ++g)
        {
            if (ef == 1)
            {
                break;
            }
            if (ef != 1)
            {
                fflush(stdin);
                printf("\n格式错误，请重新输入\n\nd=");
                ef = scanf_s("%f", &d);
            }
        }//这之前的代码全部是用来纠错的
        if (a < 0)
        {
            a = -a;
            b = -b;
            c = -c;
            d = -d;
        }//系数正负转化，方便后面的讨论
        double i, k, p, q;
        int o, j, h;


        if (b * b <= 3 * a * c)
        {
            if (b * b == 3 * a * c)//讨论各种情况
            {
                k = -b / (3 * a);
                p = a * k * k * k + b * k * k + c * k + d;
                o = 1;
                if (p > 0)
                {
                    for (j = 0;; ++j)
                    {
                        i = -b / (3 * a) - o;
                        q = a * i * i * i + b * i * i + c * i + d;
                        o = o * 2;
                        if (q <= 0)
                        {
                            break;
                        }
                    }
                }
                if (p < 0)
                {
                    for (j = 0;; ++j)
                    {
                        i = -b / (3 * a) + o;
                        q = a * i * i * i + b * i * i + c * i + d;
                        o = o * 2;
                        if (q >= 0)
                        {
                            break;
                        }
                    }
                }
                for (h = 0; h < 50; ++h)
                {
                    i = i - (a * i * i * i + b * i * i + c * i + d) / (3 * a * i * i + 2 * b * i + c);
                }
                if (p == 0)
                {
                    i = -b / (3 * a);
                }
            }
            if (b * b < 3 * a * c)
            {
                i = -b / (3 * a);
                for (h = 0; h < 100; ++h)
                {
                    i = i - (a * i * i * i + b * i * i + c * i + d) / (3 * a * i * i + 2 * b * i + c);
                }
            }
            printf("\n这个方程对应的函数是单调的\n\n所以只有一个实数解\n\n即\n\nX=%.16f", i);
        }
        if (b * b > 3 * a * c)
        {
            double r, s, t, u;
            int w, y;
            r = (-b - sqrt(b * b - 3 * a * c)) / (3 * a);
            s = (-b + sqrt(b * b - 3 * a * c)) / (3 * a);
            t = a * r * r * r + b * r * r + c * r + d;
            u = a * s * s * s + b * s * s + c * s + d;
            if (t * u >= 0)
            {
                if (d > 0)
                {
                    o = 1;
                    for (w = 0;; ++w)
                    {
                        i = r - o;
                        o = o * 2;
                        if (a * i * i * i + b * i * i + c * i + d <= 0)
                        {
                            break;
                        }
                    }
                }
                if (d < 0)
                {
                    o = 1;
                    for (w = 0;; ++w)
                    {

                        i = s + o;
                        o = o * 2;
                        if (a * i * i * i + b * i * i + c * i + d >= 0)
                        {
                            break;
                        }
                    }
                }
                if (d == 0) {
                    float M[3], X[2];
                    int rootCount, breakPointHere;
                    M[0] = c;
                    M[1] = b;
                    M[2] = a;
                    rootCount = solve_quadratic_equation(M, X);

                    breakPointHere = 1.0;
                    printf("x1=0\n");
                    printf("x2=%f\n", X[0]);
                    printf("x3=%f\n", X[1]);
                    M[0] = 0;
                    M[1] = 0;
                    M[2] = 0;
                    return 0;
                    break;
                 
                
                    
                }
                for (h = 0; h < 100; ++h)
                {
                    i = i - (a * i * i * i + b * i * i + c * i + d) / (3 * a * i * i + 2 * b * i + c);
                }
            }
            if (t * u > 0)
            {
                printf("\n这个方程只有一个实数解\n\n即\n\nX=%.16f", i);//函数极小值大于零，只有一个解
            }
            if (t * u == 0)
            {
                if (t == 0)
                {
                    printf("\n这个方程刚好有两个解\n\n分别是\n\nX1=%.16f\n\nX2=%.16f", r, i);//极小值等于零，两个解
                }
                if (u == 0)
                {
                    printf("\n这个方程刚好有两个解\n\n分别是\n\nX1=%.16f\n\nX2=%.16f", i, s);//极大值等于零，两个解
                }
            }
            if (t * u < 0)
            {
                double m, n, v;
                o = 1;
                for (w = 0;; ++w)
                {

                    m = r - o;
                    o = o * 2;
                    if (a * m * m * m + b * m * m + c * m + d <= 0)
                    {
                        break;
                    }
                }
                for (h = 0; h < 100; ++h)
                {
                    m = m - (a * m * m * m + b * m * m + c * m + d) / (3 * a * m * m + 2 * b * m + c);
                }
                o = 1;
                for (w = 0;; ++w)
                {

                    n = s + o;
                    o = o * 2;
                    if (a * n * n * n + b * n * n + c * n + d >= 0)
                    {
                        break;
                    }
                }
                for (h = 0; h < 100; ++h)
                {
                    n = n - (a * n * n * n + b * n * n + c * n + d) / (3 * a * n * n + 2 * b * n + c);
                }
                v = (r + s) / 2;
                for (h = 0; h < 100; ++h)
                {
                    if (a * v * v * v + b * v * v + c * v + d > 0)
                    {
                        r = v;
                        v = (r + s) / 2;
                    }
                    if (a * v * v * v + b * v * v + c * v + d < 0)
                    {
                        s = v;
                        v = (r + s) / 2;
                    }
                    if (a * v * v * v + b * v * v + c * v + d == 0)
                    {
                        break;
                    }
                }
                printf("\n这个方程有三个解\n\n分别是\n\nX1=%.16f\n\nX2=%.16f\n\nX3=%.16f", m, v, n);
            }
        }
    }
}

void main()
{
    float x[2], xx[3], xxx[4], p[5];
    int rootCount;
    float breakPointHere, x1, x2, a, b, c, d, e;
    int mode;
    printf("请选择模式，2代表一元二次方程，3代表一元三次方程，4代表一元四次方程：");
    scanf_s("%d",&mode,128);
    if (mode == 2) {
        quadratic_equation();
    }

    else if (mode == 3) {
        cubic_equation();
    }


    else if (mode == 4) {
        printf("一元四次方程，请按顺序输入四次项系数，三次项系数，二次项系数，一次项系数，常数，用空格隔开");
        scanf_s("%f%f%f%f%f", &a, &b, &c, &d, &e);
        p[0] = e;
        p[1] = d;
        p[2] = c;
        p[3] = b;
        p[4] = a;
        rootCount = solve_quartic_equation(p, xxx);

        breakPointHere = 1.0;
        printf("x1=%f\n", xxx[0]);
        printf("x2=%f\n", xxx[1]);
        printf("x3=%f\n", xxx[2]);
        printf("x4=%f\n", xxx[3]);
    }
    else {
        printf("请输入正确的格式。");
    }
    getchar();
    getchar();
}