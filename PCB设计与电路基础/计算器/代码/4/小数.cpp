#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h> 
struct Decimal{
	int num;   //分子 
	int den;   //分母 
};   //定义分数结构体 
void simplify(int *num,int *den);   //化简一个分数的分子和分母
int GCD(int a,int b);    //求两个数的最大公约数 
int main()
{
	Begin:;
	char str1[100],str2[100],int1[10];  //存放两个分数的字符串，后续再处理  
	float f1; 
	int integer,dec1,num,den;
	//integer是整个小数的整数部分，dec1是有限部分的小数部分（不含小数点的）,num和den分别是最终结果分数的分子和分母
	struct Decimal Dec,Cir;//小数部分对应的分数结构体（有限部分+循环部分） 
	printf("*******小数转换为分数的实验*********\n");
	printf("请输入一串小数，如有循环节，请和有限部分用空格隔开，如没有循环节，请用0代替。\n");
	printf("例如0.2 34代表0.234343434......,3.8753 0代表3.8753\n");//对输入的一个简单说明
	scanf_s("%s%s",str1,str2);  //这里不能用%f扫入，否则将不知道小数长度 ,以字符串的形式输入 
	f1=atof(str1);      //将输入的字符转换为浮点数 
	integer=(int)f1;   //取整数部分
	dec1=atoi(strstr(str1,".")+1);
	//atoi将字符串里的数字字符转化为整形数,strstr() 函数搜索一个字符串"."在另str1中的第一次出现位置，即有限小数的小数部分 
	itoa(integer,int1,10);   //将integer转为字符串Int1，方便计算长度             itoa 把一整数转换为字符串
	                        //char *itoa(int value, char *string, int radix);
                           //int value 被转换的整数，char *string 转换后储存的字符数组，int radix 转换进制数，如2,8,10,16 进制等。
	Dec.num=dec1;         //有限小数的小数部分（注意是整形的，指的是不含小数点的小数部分）
	Dec.den=pow(10,strlen(str1)-strlen(int1)-1);  //pow就是求a的b次方，strlen是计算字符串的长度 ，再减一因为要减去小数点 
	Cir.num=atoi(str2);        //atoi把字符串转换成整型数,因为这里是循环节必须为整形 
	Cir.den=pow(10,strlen(str2))-1;
	//例如0.234343434…，有限部分为0.2=1/5，无限部分为0.0343434…=0.034/(1-0.01)=34/990=17/495;0.2343434…=1/5+17/495=116/495 
	Cir.den*=pow(10,(int)(log10(dec1)+1));
	if(Cir.num==0)//如果循环小数是0，直接转换有限部分即可，否则需要将循环部分加以转换，并与有限部分的分数相加 
	{
		simplify(&Dec.num,&Dec.den);
		num=Dec.num+integer*Dec.den;
		den=Dec.den;
		simplify(&num,&den);
		if(integer!=0)printf("转换为带分数的结果为:%d+%d/%d\n",integer,Dec.num,Dec.den);
		printf("转换为分数的结果为:%d/%d\n",num,den); 
		goto Begin;
		return 0;
	} 
	simplify(&Dec.num,&Dec.den);
	simplify(&Cir.num,&Cir.den);
	den=Dec.den*Cir.den;
	num=Dec.num*Cir.den+Dec.den*Cir.num;
	simplify(&num,&den);
	if(integer!=0)printf("转换为带分数结果为:%d+%d/%d\n",integer,num,den);
	num+=den*integer;
	printf("转换为分数结果为:%d/%d\n",num,den); 
	goto Begin;
	return 0;
	
} 
int GCD(int a,int b)
{
	//利用辗转相除法求两个数的最大公约数 
	if(a==1||b==1)return 1;
	int t;
	if(a<b){t=a;a=b;b=t;}//保证a是大数
	while(b>0)
	{
		t=b;//t临时存放小数
		b=a%b;//小数是上一次两个数相除的余数
		a=t;//大数是上一次相除的小数
	}
	return a;
	
}
void simplify(int *num,int *den)
{
	int gcd=GCD(*num,*den);  // 除以最大公约数 
	*num=*num/gcd;
	*den=*den/gcd;
}

