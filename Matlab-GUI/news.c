//mex
#include <stdlib.h>
#include <math.h>
#include "F:\Program Files\MATLAB\R2012a\extern\include\mex.h"
int amount,bigs,initil=60,invlev,nevnts=4,next,nmnths=120,smalls,I,npolcy=1;
int nvalue=4,i,p1=5,small1[20],big1[20];
static double pro[99]={0,1.0/6,0.5,0.8330,1}; 
double mdemdt=0.1,setupc=32,incrmc=3,H=1,z=1,tne[5];
double aminus,aplus,tlevent,tordc,time;
double acost[20],ahldc[20],aordc[20],ashrc[20];

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
	//mexprintf("update->  aplus=%f,	aminus=%f\n",aplus,aminus);
	return;
}
/**************************************************************** 
* �������� 	init()												*
* ��������� void												*
* ��������� void												*
* �������ܣ�����ر������г�ʼ�������ڵ������Խ�����һ�Ρ� 		*			
/***************************************************************/
void init()
{
	time=0;
	invlev=initil;	//????????
	tlevent=0;	//????��????????
	amount=0;
	
	tordc=0;
	aplus=0;
	aminus=0;
	tne[1]=999999999;
	tne[2]=expon(mdemdt);
	tne[3]=nmnths;
	tne[4]=0;
	return;
	 
}

/**************************************************************** 
* �������� 	timing()											*
* ��������� void												*
* ��������� void												*
* �������ܣ��ҵ���һ�̷������¼���������ʱ��			 		*			
/***************************************************************/
void timing()
{
	double rmin;
	rmin=1e+29;
	next=0;
	for (i=1;i<=nevnts;i++)
	{
		if(tne[i]>=rmin)
			continue;
		rmin=tne[i];
		next=i;
	}
	if(next>0)
		time=tne[next];
	else
		mexPrintf("next=0,error!");
	return;
	
}

/**************************************************************** 
* �������� 	ordarv()											*
* ��������� void												*
* ��������� void												*
* �������ܣ����������¼������ȸ��¿�棬�ٸ���ʱ����	 		*			
/***************************************************************/
void ordarv()
{
	update();
	invlev+=amount;
	tne[1]=999999999;
	//mexprintf("ordarv->invlev	amount : %d  %d\n ",invlev,amount);
	return;
}

/**************************************************************** 
* �������� 	demand()											*
* ��������� void												*
* ��������� void												*
* �������ܣ����������¼�������������������ٸ���ʱ���� 		*			
/***************************************************************/
void demand()
{
    double dsize;
	update();
	dsize=irandi(z);
	invlev-=dsize;
	tne[2]=time+expon(mdemdt);
	return;
}

/**************************************************************** 
* �������� 	evalus()											*
* ��������� void												*
* ��������� void												*
* �������ܣ����������¼��������ж��Ƿ�Ҫ������Ҫ����㶩���� 	*
*           �ڸ���ʱ���ᣬ���л��ﵽ��ʱ��Ϊ�������			*			
/***************************************************************/
void evalus()
{
	//mexprintf("!!!!!!!enter evalus\n");
	if (invlev >= smalls)
	{
		tne[4]=time+1.0;
		//mexprintf("time to order  %0.f, and amount is %d\n",time,amount);
		return;
	}
	amount=bigs-invlev;
	tordc+=setupc+(incrmc*amount);
	//mexprintf("time to order  %0.f, and amount is %d\n",time,amount);
	tne[4]=time+1.0;
	tne[1]=time+unifrm(0.5,1.0);
	
	return;
}

/**************************************************************** 
* �������� 	report()											*
* ��������� void												*
* ��������� void												*
* �������ܣ�������ʱ�����ʱ���������ѵȣ�����ʽ����� 		*			
/***************************************************************/
void report(int indx)
{

	int j;
	update();
	aordc[indx]=tordc/nmnths;
	ahldc[indx]=H*(aplus/nmnths);
	ashrc[indx]=p1*(aminus/nmnths);
	acost[indx]=aordc[indx]+ahldc[indx]+ashrc[indx];
	small1[indx]=smalls;
	big1[indx]=bigs;
	if(indx==npolcy)
	{
		
	
		mexPrintf("==================================report================================\n");
		mexPrintf("initil(initial inventory level)= %d\n",initil);
		mexPrintf("number of demand %d\n",nvalue);
		for(j=1;j<=nvalue;j++)
			mexPrintf("P(%d) == %.4lf  ",j,pro[j]);
		mexPrintf("\n");
		mexPrintf("mean interdemand time %.2lf\n",mdemdt);
		mexPrintf("number of month %d\n",nmnths);
		mexPrintf("K=%.2lf  I=%.2lf  H=%.2lf  PI=%d \n",setupc,incrmc,H,p1);
		mexPrintf("policy		ave_cost	ave_ord_cost	avr_hold_cost	ave_short\n");
		for(j=1;j<=npolcy;j++)
		{
			mexPrintf("(%d,%d)%16.2f%16.2f%16.2f%16.2f\n",small1[j],big1[j],acost[j],aordc[j],ahldc[j],ashrc[j]);
		}
		mexPrintf("\n");
	}
		return;
}


int test()
{
	int i,flag=0;
	double a,sum=0;
//  �����˹�������������Ϊ���Է��㣬��������ͷ�ļ������ú��� 
//	mexprintf("Enter Initial Inventory:\n");
//	scanf("%d",initil); //��ʼ����� 
//	mexprintf("Enter Simulation Months:\n");
//	scanf("%d",nmnths);	//��������ʱ�� 
//	mexprintf("Enter number of policies:\n");
//	scanf("%d",npolcy);	//������ 
//	mexprintf("Enter number of demand sizes:\n");
//	scanf("%d",nvalue);	//������ 
//	mexprintf("Enter mean Interdemand Time:\n");
//	scanf("%d",mdemdt);	//ƽ��������ʱ�� 
//	mexprintf("Enter cost of item to prepaer:\n");
//	scanf("%d",setupc);	//�������ӷ� 
//	mexprintf("Enter order cost per item:\n");
//	scanf("%d",incrmc);	//ÿ�������� 
//	mexprintf("Enter mean maitaining cost:\n");
//	scanf("%d",H);		//ÿ����ÿ�±��ܷ� 
//	mexprintf("Enter mean cost per item for loss:\n");
//	scanf("%d",p1);		//ÿ������ʧ�� 
////	mexprintf("Enter distributionof demand sizes:\n");
////	scanf();	


	for(i=1;i<=npolcy;i++)
	{
		mexPrintf("Enter smalls bigs:\n");
		scanf("%d%d",&smalls,&bigs);
		flag=0;
		init();
		while(1)
		{	
			if(flag==1)
				break;
			timing();
			switch(next)
			{
			case 1:	ordarv();
				break;
			case 2:	demand();
				break;
			case 3:	report(i);flag=1;
				break;
			case 4:	evalus();
				break;
			default:	mexPrintf("default error");
				break;
			}
		}
	
	}
	
	return 0;
}

void mexFunction(int nlhs,mxArray *plhs[], int nrhs,const mxArray *prhs[])
{
    int i,flag=0;
	double a,sum=0;
    nlhs=0;
    smalls=20;
    bigs=40;
	plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    if (nrhs ==2)
    {
        smalls=mxGetScalar(prhs[0]);
        bigs=mxGetScalar(prhs[1]);
    }
 	

	
		for(i=1;i<=npolcy;i++)
	{
		//mexPrintf("Enter smalls bigs:\n");
		
        
		flag=0;
		init();
		while(1)
		{	
			if(flag==1)
				break;
			timing();
			switch(next)
			{
			case 1:	ordarv();
				break;
			case 2:	demand();
				break;
			case 3:	report(i);flag=1;
				break;
			case 4:	evalus();
				break;
			default:	mexPrintf("default error");
				break;
			}
		}
	
	}  
}