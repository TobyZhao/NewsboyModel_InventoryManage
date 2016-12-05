#include <stdio.h>
#include "init.cpp"
int main()
{
	int i, flag = 0;
	double a, sum = 0;
//  manually input parameters
//	printf("Enter Initial Inventory:\n");
//	scanf("%d",initil);	// initial inventory
//	printf("Enter Simulation Months:\n");
//	scanf("%d",nmnths);	// simulation time
//	printf("Enter number of policies:\n");
//	scanf("%d",npolcy);	// number of policies
//	printf("Enter number of demand sizes:\n");
//	scanf("%d",nvalue);	// demands
//	printf("Enter mean Interdemand Time:\n");
//	scanf("%d",mdemdt);	// mean interdemand time
//	printf("Enter cost of item to prepaer:\n");
//	scanf("%d",setupc);	// setup cost
//	printf("Enter order cost per item:\n");
//	scanf("%d",incrmc);	// order cost per item
//	printf("Enter mean maitaining cost:\n");
//	scanf("%d",H);		// mean maitaining cost
//	printf("Enter mean cost per item for loss:\n");
//	scanf("%d",p1);		// mean cost per item for loss
////	printf("Enter distributionof demand sizes:\n");
////	scanf();

	for(i = 1; i <= npolcy; i++)
	{
		printf("Enter smalls and bigs:\n");
		scanf("%d%d", &smalls, &bigs);
		flag = 0;
		init();
		while(1)
		{
			if(flag == 1)
				break;
			timing();
			switch(next)
			{
			case 1:	ordarv();
				break;
			case 2:	demand();
				break;
			case 3:	report(i); flag = 1;
				break;
			case 4:	evalus();
				break;
			default: printf("default error");
				break;
			}
		}
	}
	return 0;
}
