#include <stdio.h>
#include "init.cpp"
int main()
{
	int i,flag=0;
	double a,sum=0;
//  可以人工输入各项参数，为调试方便，各参数在头文件中设置好了
//	printf("Enter Initial Inventory:\n");
//	scanf("%d",initil); //初始库存量
//	printf("Enter Simulation Months:\n");
//	scanf("%d",nmnths);	//仿真运行时间
//	printf("Enter number of policies:\n");
//	scanf("%d",npolcy);	//策略数
//	printf("Enter number of demand sizes:\n");
//	scanf("%d",nvalue);	//需求量
//	printf("Enter mean Interdemand Time:\n");
//	scanf("%d",mdemdt);	//平均需求间隔时间
//	printf("Enter cost of item to prepaer:\n");
//	scanf("%d",setupc);	//订货附加费
//	printf("Enter order cost per item:\n");
//	scanf("%d",incrmc);	//每件订货费
//	printf("Enter mean maitaining cost:\n");
//	scanf("%d",H);		//每件货每月保管费
//	printf("Enter mean cost per item for loss:\n");
//	scanf("%d",p1);		//每件货损失费
////	printf("Enter distributionof demand sizes:\n");
////	scanf();


	for(i=1;i<=npolcy;i++)
	{
		printf("Enter smalls and bigs:\n");
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
			default:	printf("default error");
				break;
			}
		}

	}

	return 0;
}
