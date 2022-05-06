#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;
int digit;//进制数
int number;//需要做计算的数
int result = 0;//结果
int change_1();
int change_2();
void main() {
	int a;//a=1，十进制变其他进制，a=2，其他进制变十进制,
	printf("请选择模式：1，十进制变其他进制，2，其他进制变十进制");
	scanf_s("%d", &a);
	if (a == 1) {
		change_1();
		printf("%d", result);
	}
	if (a == 2) {
		change_2();
		printf("%d", result);
	}
//	if (a == 3) {

	//}
	getchar(); getchar(); getchar();
}

int change_1() {
	printf("首先键入十进制数，之后键入你想变成的进制数，中间用空格隔开:");
	scanf_s("%d %d", &number, &digit);
	int i = 50;
	int b, c, e = 0;
	while (number != 0) {
		b = pow(digit, i);
		c = number / b;
		number = number - b * c;
		e = pow(10, i);
		result = c * e + result;
		i--;
	}

		return result;
}
int change_2() {
	printf("首先键入所在进制下的数字，之后键入此进制，中间用空格隔开：");
	scanf_s("%d %d", &number, &digit);
	int i = 50;
	int b, c, e = 0;
	while (number != 0) {
		e = pow(10, i);
		c = number / e;
		number = number - e * c;
		b = pow(digit, i);
		result = c * b + result;
		i--;
	}

		return result;

}
