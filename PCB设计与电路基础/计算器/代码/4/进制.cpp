#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;
int digit;//������
int number;//��Ҫ���������
int result = 0;//���
int change_1();
int change_2();
void main() {
	int a;//a=1��ʮ���Ʊ��������ƣ�a=2���������Ʊ�ʮ����,
	printf("��ѡ��ģʽ��1��ʮ���Ʊ��������ƣ�2���������Ʊ�ʮ����");
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
	printf("���ȼ���ʮ��������֮����������ɵĽ��������м��ÿո����:");
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
	printf("���ȼ������ڽ����µ����֣�֮�����˽��ƣ��м��ÿո������");
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
