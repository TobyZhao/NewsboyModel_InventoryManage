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
* 函数名： 	drand()												*
* 输入参数： double z											*
* 输出参数： double												*
* 函数功能：产生0~Z的连续随机数							 		*			
/***************************************************************/
double drand(double z)
{
	double a=rand()%10000;
	a=a/10000;
	return a*z;
}

/**************************************************************** 
* 函数名： 	irandi()											*
* 输入参数： int												*
* 输出参数： int												*
* 函数功能：产生随机需求。根据0~1的连续随机数结合分布概率生成	*			
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
* 函数名： 	unifrm()											*
* 输入参数： double a，double b									*
* 输出参数： double												*
* 函数功能：产生[a,b]连续随机数 								*			
/***************************************************************/
double unifrm(double a,double b)
{
	double u,aa;
	u=drand(z);
	aa=a+u*(b-a);
	return aa;
	
}

/**************************************************************** 
* 函数名： 	expon()												*
* 输入参数： double rmean										*
* 输出参数： double												*
* 函数功能：生成均值为rmean的指数随机数 						*			
/***************************************************************/
double expon( double rmean)
{
	return -1*rmean*log(drand(1));
}

/**************************************************************** 
* 函数名： 	update()											*
* 输入参数： void												*
* 输出参数： void												*
* 函数功能：计算库存积分量或者缺货积分量，用于计算相关费用		*			
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
* 函数名： 	init()												*
* 输入参数： void												*
* 输出参数： void												*
* 函数功能：对相关变量进行初始化，对于单个策略仅运行一次。 		*			
/***************************************************************/
void init()
{
	time=0;
	invlev=initil;	//????????
	tlevent=0;	//????θ????????
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
* 函数名： 	timing()											*
* 输入参数： void												*
* 输出参数： void												*
* 函数功能：找到下一刻发生的事件，并更新时间			 		*			
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
* 函数名： 	ordarv()											*
* 输入参数： void												*
* 输出参数： void												*
* 函数功能：描述订货事件。首先更新库存，再更新时间轴	 		*			
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
* 函数名： 	demand()											*
* 输入参数： void												*
* 输出参数： void												*
* 函数功能：描述需求事件。首先生成随机需求，再更新时间轴 		*			
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
* 函数名： 	evalus()											*
* 输入参数： void												*
* 输出参数： void												*
* 函数功能：描述订货事件。首先判断是否要定，若要则计算订货数 	*
*           在更新时间轴，其中货物到达时间为随机数。			*			
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
* 函数名： 	report()											*
* 输入参数： void												*
* 输出参数： void												*
* 函数功能：当仿真时间结束时，计算各项花费等，并格式化输出 		*			
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
//  可以人工输入各项参数，为调试方便，各参数在头文件中设置好了 
//	mexprintf("Enter Initial Inventory:\n");
//	scanf("%d",initil); //初始库存量 
//	mexprintf("Enter Simulation Months:\n");
//	scanf("%d",nmnths);	//仿真运行时间 
//	mexprintf("Enter number of policies:\n");
//	scanf("%d",npolcy);	//策略数 
//	mexprintf("Enter number of demand sizes:\n");
//	scanf("%d",nvalue);	//需求量 
//	mexprintf("Enter mean Interdemand Time:\n");
//	scanf("%d",mdemdt);	//平均需求间隔时间 
//	mexprintf("Enter cost of item to prepaer:\n");
//	scanf("%d",setupc);	//订货附加费 
//	mexprintf("Enter order cost per item:\n");
//	scanf("%d",incrmc);	//每件订货费 
//	mexprintf("Enter mean maitaining cost:\n");
//	scanf("%d",H);		//每件货每月保管费 
//	mexprintf("Enter mean cost per item for loss:\n");
//	scanf("%d",p1);		//每件货损失费 
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