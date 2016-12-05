// mex
#include <stdlib.h>
#include <math.h>
#include "F:\Program Files\MATLAB\R2012a\extern\include\mex.h"
int amount, bigs, initil = 60, invlev, nevnts = 4, next, nmnths = 120, smalls, I, npolcy = 1;
int nvalue = 4, i, p1 = 5, small1[20], big1[20];
static double pro[99]={0, 1.0/6, 0.5, 0.8330, 1}; 
double mdemdt = 0.1, setupc = 32, incrmc = 3, H = 1, z = 1, tne[5];
double aminus, aplus, tlevent, tordc, time;
double acost[20], ahldc[20], aordc[20], ashrc[20];

/****************************************************************
* function:		drand()					*
* input:		double z				*
* output:		double					*
* description:		create random number between 0 to z	*
/***************************************************************/
double drand(double z)
{
	double a = rand() % 10000;
	a = a / 10000;
	return a * z;
}

/****************************************************************
* function:		irandi()				*
* input:		int z					*
* output:		int					*
* description:		create random demands			*
/***************************************************************/
int irandi(int z)
{
	int i, n1, res = 1;
	double u = drand(z);
	n1 = nvalue - 1;
	for (i = 1; i <= n1; i++)
	{
		if(u >= pro[i])
		{
			res = i + 1;
		}
	}
	return res;
}

/****************************************************************
* function:		unifrm()				*
* input:		double a, double b			*
* output:		double					*
* description:		create random number between a to b	*
/***************************************************************/
double unifrm(double a, double b)
{
	double u, aa;
	u = drand(z);
	aa = a + u * (b - a);
	return aa;
}

/****************************************************************
* function:		expon()					*
* input:		double rmean				*
* output:		double					*
* description:		create random exponential number whose	* 
			means equals to rmean			*
/***************************************************************/
double expon(double rmean)
{
	return -1 * rmean * log(drand(1));
}

/****************************************************************
* function:		update()				*
* input:		void					*
* output:		void					*
* description:		compute integration of the inventory	*
			or the stockout to obtain relative cost	*
/***************************************************************/
void update()
{
	double tsle;
	tsle = time - tlevent;
	tlevent = time;
	if(invlev < 0)
	{
		aminus = aminus + (-1 * invlev * tsle);
	}

	else if(invlev > 0)
		aplus = aplus + (invlev * tsle);
	//printf("update -> aplus = %f, aminus = %f\n", aplus, aminus);
	return;
}

/****************************************************************
* funtion:		init()					*
* input:		void					*
* output:		void					*
* description:		initialization for variables		*
/***************************************************************/
void init()
{
	time = 0;
	invlev = initil;
	tlevent = 0;
	amount = 0;
	money = 0;
	tordc = 0;
	aplus = 0;
	aminus = 0;
	tne[1] = 999999999;
	tne[2] = expon(mdemdt);
	tne[3] = nmnths;
	tne[4] = 0;
	return;
}

/****************************************************************
* funtion:		timing()				*
* input:		void					*
* output:		void					*
* description:		find next event and update timeline	*
/***************************************************************/
void timing()
{
	double rmin;
	rmin = 1e+29;
	next = 0;
	for (i = 1; i <= nevnts; i++)
	{
		if(tne[i] >= rmin)
			continue;
		rmin = tne[i];
		next = i;
	}
	if(next > 0)
		time = tne[next];
	else
		printf("next = 0, error!");
	return;
}

/****************************************************************
* funtion:		ordarv()				*
* input:		void					*
* output:		void					*
* description:		order event. first update inventory,	* 
			then update timeline			*
/***************************************************************/
void ordarv()
{
	update();
	invlev += amount;
	money += amount * price;
	tne[1] = 999999999;
	//printf("ordarv -> invlev  amount: %d  %d\n", invlev, amount);
	return;
}

/****************************************************************
* funtion:		demand()				*
* input:		void					*
* output:		void					*
* description:		demand event. first create random	*
			demands, then update timeline		*
/***************************************************************/
void demand()
{
	update();
	double dsize;
	dsize = irandi(z);
	invlev -= dsize;
	tne[2] = time + expon(mdemdt);
	return;
}

/****************************************************************
* funtion:		evalus()				*
* input:		void					*
* output:		void					*
* description:		order event. first decide whether to	*
			order. If yes, compute order amount	*
			and update timeline			*
/***************************************************************/
void evalus()
{
	//printf("!!!!!!!enter evalus\n");
	if (invlev >= smalls)
	{
		tne[4] = time + 1.0;
		//printf("time to order %0.f, and amount is %d\n", time, amount);
		return;
	}
	amount = bigs - invlev;
	tordc += setupc + (incrmc * amount);
	//printf("time to order %0.f, and amount is %d\n", time, amount);
	tne[4] = time + 1.0;
	tne[1] = time + unifrm(0.5, 1.0);
	
	return;
}

/****************************************************************
* funtion:		report()				*
* input:		int indx				*
* output:		void					*
* description:		compute costs and printout when		*
			simulation finishes			*
/***************************************************************/
void report(int indx)
{

	int j;
	update();
	aordc[indx] = tordc / nmnths;
	ahldc[indx] = H * (aplus / nmnths);
	ashrc[indx] = p1 * (aminus / nmnths);
	acost[indx] = aordc[indx] + ahldc[indx] + ashrc[indx];
	small1[indx] = smalls;
	big1[indx] = bigs;
	pri[indx] = money + (initil - invlev) * price - acost[indx] * nmnths;
	if (indx == npolcy)
	{ 


		printf("================================== report ================================\n");
		printf("initil(initial inventory level) = %d\n", initil);
		printf("number of demand %d\n", nvalue);
		for(j = 1; j <= nvalue; j++)
			printf("P(%d) == %.4lf  ", j, pro[j]);
		printf("\n");
		printf("mean interdemand time %.2lf\n", mdemdt);
		printf("number of month %d\n", nmnths);
		printf("K=%.2lf  I=%.2lf  H=%.2lf  PI=%d \n", setupc, incrmc, H, p1);
		printf("policy	  ave_cost	aord_cost	ahold_cost	ave_short	profit\n");
		for(j = 1; j <= npolcy; j++)
		{
			printf("(%d,%d)%10.2f%14.2f%16.2f%16.2f%16.2f\n",small1[j],big1[j],acost[j],aordc[j],ahldc[j],ashrc[j],pri[j]);
		}
		printf("\n");
	}
		return;
}

int test()
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

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	int i, flag = 0;
	double a, sum = 0;
    	nlhs = 0;
    	smalls = 20;
    	bigs = 40;
	plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    	if (nrhs == 2)
	{
        	smalls = mxGetScalar(prhs[0]);
        	bigs = mxGetScalar(prhs[1]);
    	}
	for(i=1;i<=npolcy;i++)
	{
		//mexPrintf("Enter smalls bigs:\n");
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
			default: mexPrintf("default error");
				break;
			}
		}	
	}  
}
