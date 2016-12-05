#include <stdlib.h>
#include <math.h>
/****************************************************************
*			parameters				*
*	amount:		amount of order				*
*	bigs:		maximum inventory			*
*	initil: 	initial inventory			*
*	invlev:		dynamic inventory			*
*	nevnts: 	number of events			*
*	next:		next event				*
*	nmnths:		simulation time (month)			*
*	smalls:		minimum inventory			*
*	npolcy:		number of policies			*
*	nvalue:		demands					*
*	p1:		cost for stockout - p1/item		*										*
*	pro[]:		probability density for demands		*
*	mdemdt:		average interval time of demands	*
*	setupc:		setup cost				*
*	incrmc:		order cost - incremc/item		*
*	H:		storage cost - h/item/month		*
*	tne[]: 		timeline				*
/***************************************************************/
int amount, bigs, initil = 60, invlev, nevnts = 4, next, nmnths = 120, smalls, I, npolcy = 4;
int nvalue = 4, i, p1 = 5, small1[20], big1[20];
static double pro[99] = {0, 1.0/6, 0.5, 0.8330, 1};
double mdemdt = 0.1, setupc = 32, incrmc = 3, H = 1, z = 1, tne[5], price = 10, money = 0;
double aminus, aplus, tlevent, tordc, time;
double acost[20], ahldc[20], aordc[20], ashrc[20], pri[20];

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
