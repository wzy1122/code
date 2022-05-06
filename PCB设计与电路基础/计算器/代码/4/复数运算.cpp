/*file        ComplexCalculation.h
 2  *author    Vincent Cui
 3  *e-mail    whcui1987@163.com
 4  *version    0.1
 5  *data        20-Oct-2014
 6  *brief        用于复数运算的一些函数头和定义
 7 */
8
9
10
11 #ifndef _COMPLEXCALCULATION_H_
12 #define _COMPLEXCALCULATION_H_
13
14 #define ASSERT_ENABLE 1
15
16 #define IS_COMPLEX_DIVISOR_CORRENT(DIVISOR_REAL, DIVISOR_IMAG)        ((DIVISOR_REAL != 0) || (DIVISOR_IMAG != 0))
17
18 typedef double                    mathDouble;
19 typedef unsigned char            mathUint_8;
20 typedef unsigned short int        mathUint_16;
21 typedef unsigned int            mathUint_32;
22
23
24 typedef struct _ReDefcomplex
25 {
	26     mathDouble    Real;
	27     mathDouble    Imag;
	28 }complexType;
29
30
31 complexType    complexAdd(complexType a, complexType b);
32 complexType    complexSubtract(complexType minuend, complexType subtrahend);
33 complexType complexMultiply(complexType a, complexType b);
34 complexType complexDivision(complexType dividend, complexType divisor);
35 mathDouble    complexAbs(complexType a);
36 mathDouble    complexAngle(complexType a);
37 complexType complexByAbsAngle(mathDouble r, mathDouble theta);
38 complexType complexExp(complexType a);
39
40 #if ASSERT_ENABLE
41   #define assert_param(expr) ((expr) ? (void)0 : assert_failed((mathUint_8*)__FILE__, __LINE__))
42   void assert_failed(mathUint_8 * file, mathUint_32 line);
43 #else
44   #define assert_param(expr) ((void)0)
45 #endif
46
47#include "ComplexCalculation.h"
2 #include "stdio.h"
3
4 int main(void)
5 {
	6     complexType a, b, c;
	7     a.Imag = 0.5;
	8     a.Real = 2.5;
	9     b.Real = 1;
	10     b.Imag = -5;
	11
		12     c = complexAdd(a, b);
	13     printf("complexAdd: c.Real %f, c.Imag %f \r\n", c.Real, c.Imag);
	14     c = complexSubtract(a, b);
	15     printf("complexSubtract: c.Real %f, c.Imag %f \r\n", c.Real, c.Imag);
	16     c = complexMultiply(a, b);
	17     printf("complexMultiply: c.Real %f, c.Imag %f \r\n", c.Real, c.Imag);
	18     c = complexDivision(a, b);
	19     printf("complexDivision: c.Real %f, c.Imag %f \r\n", c.Real, c.Imag);
	20     printf("Abs(c): %f\r\n", complexAbs(a));
	21     printf("Angle(c): %f\r\n", complexAngle(a));
	22     c = complexByAbsAngle(complexAbs(a), complexAngle(a));
	23     printf("complexByAbsAngle: a.Real %f, a.Imag %f \r\n", c.Real, c.Imag);
	24
		25     while (1);
	26 }
48
49 #endif