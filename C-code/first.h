#include <stdlib.h>
#include <math.h>
/****************************************************************
*								�����б�						*
*	amount��	�������� 										*
*	bigs��		�ֿ�����										*
*	initil�� 	ԭʼ���										*
*	invlev:		��̬���										*
*	nevnts��	�¼���Ŀ										*
*	next��		��һ���¼�										*
*	nmnths��	����ʱ�䣨�£�									*
*	smalls��	���趩���Ŀ������								*
*	npolcy��	������Ŀ										*
*	nvalue��	������											*
*	p1:			ÿ��ȱ����ʧ��									*
*	i��			ѭ������										*
*	pro[]��		��������ܶ�									*
*	mdemdt��	ƽ��������ʱ��								*
*	setupc��	�������ӷ���									*
*	incrmc��	ÿ��������										*
*	H��			ÿ��ÿ�±��ܷ�									*
*	tne[]��		ʱ���ᣬ�ֱ��¼�¼�������ʱ��					*
/***************************************************************/
int amount,bigs,initil=60,invlev,nevnts=4,next,nmnths=120,smalls,I,npolcy=4;
int nvalue=4,i,p1=5,small1[20],big1[20];
static double pro[99]={0,1.0/6,0.5,0.8330,1};
double mdemdt=0.1,setupc=32,incrmc=3,H=1,z=1,tne[5],price=10,money=0;
double aminus,aplus,tlevent,tordc,time;
double acost[20],ahldc[20],aordc[20],ashrc[20],pri[20];

/****************************************************************
* �������� 	drand()												*
* ��������� double z											*
* ��������� double												*
* �������ܣ�����0~Z�����������							 		*
/***************************************************************/
double drand(double z)
{
	double a=rand()%10000;
	a=a/10000;
	return a*z;
}

/****************************************************************
* �������� 	irandi()											*
* ��������� int												*
* ��������� int												*
* �������ܣ�����������󡣸���0~1�������������Ϸֲ���������	*
/***************************************************************/
int irandi(int z)
{
	int i,n1,res=1;
	double u=drand(z);
	n1=nvalue-1;
	for (i=1;i<=n1;i++)
	{
		if(u>= pro[i])
		{
			res=i+1;
		}

	}
	return res;
}

/****************************************************************
* �������� 	unifrm()											*
* ��������� double a��double b									*
* ��������� double												*
* �������ܣ�����[a,b]��������� 								*
/***************************************************************/
double unifrm(double a,double b)
{
	double u,aa;
	u=drand(z);
	aa=a+u*(b-a);
	return aa;

}

/****************************************************************
* �������� 	expon()												*
* ��������� double rmean										*
* ��������� double												*
* �������ܣ����ɾ�ֵΪrmean��ָ������� 						*
/***************************************************************/
double expon( double rmean)
{
	return -1*rmean*log(drand(1));
}

/****************************************************************
* �������� 	update()											*
* ��������� void												*
* ��������� void												*
* �������ܣ����������������ȱ�������������ڼ�����ط���		*
/***************************************************************/
void update()
{
	double tsle;
	tsle=time-tlevent;
	tlevent=time;
	if(invlev<0)
	{
		aminus=aminus+(-1*invlev*tsle);
	}

	else if(invlev >0)
		aplus=aplus+(invlev*tsle);
	//printf("update->  aplus=%f,	aminus=%f\n",aplus,aminus);
	return;
}
