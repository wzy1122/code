#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h> 
struct Decimal{
	int num;   //���� 
	int den;   //��ĸ 
};   //��������ṹ�� 
void simplify(int *num,int *den);   //����һ�������ķ��Ӻͷ�ĸ
int GCD(int a,int b);    //�������������Լ�� 
int main()
{
	Begin:;
	char str1[100],str2[100],int1[10];  //��������������ַ����������ٴ���  
	float f1; 
	int integer,dec1,num,den;
	//integer������С�����������֣�dec1�����޲��ֵ�С�����֣�����С����ģ�,num��den�ֱ������ս�������ķ��Ӻͷ�ĸ
	struct Decimal Dec,Cir;//С�����ֶ�Ӧ�ķ����ṹ�壨���޲���+ѭ�����֣� 
	printf("*******С��ת��Ϊ������ʵ��*********\n");
	printf("������һ��С��������ѭ���ڣ�������޲����ÿո��������û��ѭ���ڣ�����0���档\n");
	printf("����0.2 34����0.234343434......,3.8753 0����3.8753\n");//�������һ����˵��
	scanf_s("%s%s",str1,str2);  //���ﲻ����%fɨ�룬���򽫲�֪��С������ ,���ַ�������ʽ���� 
	f1=atof(str1);      //��������ַ�ת��Ϊ������ 
	integer=(int)f1;   //ȡ��������
	dec1=atoi(strstr(str1,".")+1);
	//atoi���ַ�����������ַ�ת��Ϊ������,strstr() ��������һ���ַ���"."����str1�еĵ�һ�γ���λ�ã�������С����С������ 
	itoa(integer,int1,10);   //��integerתΪ�ַ���Int1��������㳤��             itoa ��һ����ת��Ϊ�ַ���
	                        //char *itoa(int value, char *string, int radix);
                           //int value ��ת����������char *string ת���󴢴���ַ����飬int radix ת������������2,8,10,16 ���Ƶȡ�
	Dec.num=dec1;         //����С����С�����֣�ע�������εģ�ָ���ǲ���С�����С�����֣�
	Dec.den=pow(10,strlen(str1)-strlen(int1)-1);  //pow������a��b�η���strlen�Ǽ����ַ����ĳ��� ���ټ�һ��ΪҪ��ȥС���� 
	Cir.num=atoi(str2);        //atoi���ַ���ת����������,��Ϊ������ѭ���ڱ���Ϊ���� 
	Cir.den=pow(10,strlen(str2))-1;
	//����0.234343434�������޲���Ϊ0.2=1/5�����޲���Ϊ0.0343434��=0.034/(1-0.01)=34/990=17/495;0.2343434��=1/5+17/495=116/495 
	Cir.den*=pow(10,(int)(log10(dec1)+1));
	if(Cir.num==0)//���ѭ��С����0��ֱ��ת�����޲��ּ��ɣ�������Ҫ��ѭ�����ּ���ת�����������޲��ֵķ������ 
	{
		simplify(&Dec.num,&Dec.den);
		num=Dec.num+integer*Dec.den;
		den=Dec.den;
		simplify(&num,&den);
		if(integer!=0)printf("ת��Ϊ�������Ľ��Ϊ:%d+%d/%d\n",integer,Dec.num,Dec.den);
		printf("ת��Ϊ�����Ľ��Ϊ:%d/%d\n",num,den); 
		goto Begin;
		return 0;
	} 
	simplify(&Dec.num,&Dec.den);
	simplify(&Cir.num,&Cir.den);
	den=Dec.den*Cir.den;
	num=Dec.num*Cir.den+Dec.den*Cir.num;
	simplify(&num,&den);
	if(integer!=0)printf("ת��Ϊ���������Ϊ:%d+%d/%d\n",integer,num,den);
	num+=den*integer;
	printf("ת��Ϊ�������Ϊ:%d/%d\n",num,den); 
	goto Begin;
	return 0;
	
} 
int GCD(int a,int b)
{
	//����շת������������������Լ�� 
	if(a==1||b==1)return 1;
	int t;
	if(a<b){t=a;a=b;b=t;}//��֤a�Ǵ���
	while(b>0)
	{
		t=b;//t��ʱ���С��
		b=a%b;//С������һ�����������������
		a=t;//��������һ�������С��
	}
	return a;
	
}
void simplify(int *num,int *den)
{
	int gcd=GCD(*num,*den);  // �������Լ�� 
	*num=*num/gcd;
	*den=*den/gcd;
}

