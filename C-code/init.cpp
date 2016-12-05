#include <stdio.h>
#include "first.h"

/****************************************************************
* 函数名： 	init()												*
* 输入参数： void												*
* 输出参数： void												*
* 函数功能：对相关变量进行初始化，对于单个策略仅运行一次。 		*
/***************************************************************/
void init()
{
	time=0;
	invlev=initil;
	tlevent=0;
	amount=0;
	money=0;

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
		printf("next=0,error!");
	return;

}

/****************************************************************
* 函数名： 	ordarv()											*
* 输入参数： void												*
* 输出参数： void												*
* 函数功能：描述订货事件（已完成）。首先更新库存，再更新时间轴	 		*
/***************************************************************/
void ordarv()
{
	update();
	invlev+=amount;
	money+=amount*price;
	tne[1]=999999999;
	//printf("ordarv->invlev	amount : %d  %d\n ",invlev,amount);
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
	update();
	double dsize;
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
	//printf("!!!!!!!enter evalus\n");
	if (invlev >= smalls)
	{
		tne[4]=time+1.0;
		//printf("time to order  %0.f, and amount is %d\n",time,amount);
		return;
	}
	amount=bigs-invlev;
	tordc+=setupc+(incrmc*amount);
	//printf("time to order  %0.f, and amount is %d\n",time,amount);
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
	pri[indx]=money+(initil-invlev)*price-acost[indx]*nmnths;
	if(indx==npolcy)
	{


		printf("==================================report================================\n");
		printf("initil(initial inventory level)= %d\n",initil);
		printf("number of demand %d\n",nvalue);
		for(j=1;j<=nvalue;j++)
			printf("P(%d) == %.4lf  ",j,pro[j]);
		printf("\n");
		printf("mean interdemand time %.2lf\n",mdemdt);
		printf("number of month %d\n",nmnths);
		printf("K=%.2lf  I=%.2lf  H=%.2lf  PI=%d \n",setupc,incrmc,H,p1);
		printf("policy	  ave_cost	aord_cost	ahold_cost	ave_short	profit\n");
		for(j=1;j<=npolcy;j++)
		{
			printf("(%d,%d)%10.2f%14.2f%16.2f%16.2f%16.2f\n",small1[j],big1[j],acost[j],aordc[j],ahldc[j],ashrc[j],pri[j]);
		}
		printf("\n");
	}
		return;
}
