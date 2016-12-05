#include <stdio.h>
#include "first.h"

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
