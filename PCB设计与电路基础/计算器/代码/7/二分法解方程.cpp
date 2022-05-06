#include <stdio.h>
#include <math.h>
//�˷���ֻ�����ڿ��ö��ַ���ķ��̣�������ֻ��һ���⣩,������Χ
double f(double x) {

    return sin(x)-0.5;
}
double bisection(double l, double r) {
    double mid;
    if (f(l) * f(r) > 0) {
        printf("error");
        return 0;
    }

    while (fabs(r - l) > 0.0000001) {
        mid = (l + r) / 2;
        if (f(l) * f(mid) < 0) {
            r = mid;
        }
        else if(f(l) * f(mid) > 0) {
            l = mid;
        }
        else {
            return mid;
        }
    }

    return l;
}

int main() {
    printf("%f", bisection(-1,1));
    getchar();
    getchar();
    return 0;

}