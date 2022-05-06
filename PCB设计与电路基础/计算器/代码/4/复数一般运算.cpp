#include "ComplexCalculation.h"
11 #include "math.h"
12 #include "stdio.h"
complexType    complexAdd(complexType a, complexType b)
24 {
	25     complexType result;
	26
		27     result.Real = a.Real + b.Real;
	28     result.Imag = a.Imag + b.Imag;
	29
		30     return result;
	31 }
/*函数名：complexSubtract
 34  *说明：复数减法
 35  *输入：minuend被减数，subtrahend减数
 36  *输出：
 37  *返回：a － b
 38  *调用：
 39  *其它：
 40  */
41 complexType    complexSubtract(complexType minuend, complexType subtrahend)
42 {
	43     complexType result;
	44
		45     result.Real = minuend.Real - subtrahend.Real;
	46     result.Imag = minuend.Imag - subtrahend.Imag;
	47
		48     return result;
	49 }
50
51 /*函数名：complexMultiply
52  *说明：复数乘法
53  *输入：a,b两个复数
54  *输出：
55  *返回：a * b
56  *调用：
57  *其它：
58  */
59 complexType complexMultiply(complexType a, complexType b)
60 {
	61     complexType result;
	62
		63     result.Real = a.Real * b.Real - a.Imag * b.Imag;
	64     result.Imag = a.Imag * b.Real + a.Real * b.Imag;
	65
		66     return result;
	67 }
68
69
70 /*函数名：complexDivision
71  *说明：复数除法
72  *输入：dividend被除数，divisor除数
73  *输出：
74  *返回：a / b
75  *调用：
76  *其它：divisor的实部和虚部不能同时为0
77  */
78 complexType complexDivision(complexType dividend, complexType divisor)
79 {
	80     complexType result;
	81
		82     /*断言，被除数的实部和虚部不能同时为零*/
		83     assert_param(IS_COMPLEX_DIVISOR_CORRENT(divisor.Real, divisor.Imag));
	84
		85     result.Real = (mathDouble)(dividend.Real * divisor.Real + dividend.Imag * divisor.Imag) / \
		86                   (divisor.Real * divisor.Real + divisor.Imag * divisor.Imag);
	87     result.Imag = (mathDouble)(dividend.Imag * divisor.Real - dividend.Real * divisor.Imag) / \
		88                   (divisor.Real * divisor.Real + divisor.Imag * divisor.Imag);
	89     return result;
	90 }
91
92 /*函数名：complexAbs
93  *说明：复数取模
94  *输入：a复数
95  *输出：
96  *返回：复数的模
97  *调用：
98  *其它：
99  */
100 mathDouble    complexAbs(complexType a)
101 {
	102     return (sqrt(pow(a.Real, 2) + pow(a.Imag, 2)));
	103 }
104
105
106 /*函数名：complexAngle
107  *说明：复数取相角
108  *输入：a复数
109  *输出：
110  *返回：复数的相角
111  *调用：
112  *其它：
113  */
114 mathDouble    complexAngle(complexType a)
115 {
	116     /*是atan2而非atan，（-PI,PI] */
		117     return (atan2(a.Imag, a.Real));
	118 }
119
120 /*函数名：complexByAbsAngle
121  *说明：通过模和相角合成复数
122  *输入：r 模， theta 相角
123  *输出：
124  *返回：复数
125  *调用：
126  *其它：
127  */
128 complexType complexByAbsAngle(mathDouble r, mathDouble theta)
129 {
	130     complexType tmp_1, tmp_2;
	131
		132     tmp_1.Real = 0;
	133     tmp_1.Imag = theta;
	134     tmp_2 = complexExp(tmp_1);
	135     tmp_2.Real *= r;
	136     tmp_2.Imag *= r;
	137
		138     return tmp_2;
	139 }
140
141 /*函数名：complexExp
142  *说明：复指数运算
143  *输入：a 复指数
144  *输出：
145  *返回：e的a次方
146  *调用：
147  *其它：使用欧拉公式 e^(jw) = cos(w) + j * sin(w)
148  */
149 complexType complexExp(complexType a)
150 {
	151     complexType result;
	152
		153     result.Real = exp(a.Real) * cos(a.Imag);
	154     result.Imag = exp(a.Real) * sin(a.Imag);
	155
		156     return result;
	157 }
158
159
160 #if ASSERT_ENABLE
161 /*函数名：assert_failed
162  *说明：断言函数
163  *输入：
164  *输出：打印出错的位置
165  *返回：
166  *调用：
167  *其它：
168  */
169 void assert_failed(mathUint_8 * file, mathUint_32 line)
170 {
	171     printf("Assert Error in File: %s \r\nLine: %d \r\n", file, line);
	172 }
173
174 #endif
