/* cherche_doublons.c */

#include<stdio.h>
#include "cherche_doublons.h"
#define PRECIS 1000000

int cherche_doublons(int *Ax,double *Ae,int nbcli)
{
  int i;
  int j;

  for ( i = 0 ; i < nbcli-1 ; i ++ )
    {
      if ( (int) (Ae[i*2]*PRECIS) == (int) (Ae[(i+1)*2]*PRECIS) )
	{
	  if ((int) (Ae[i*2+1]*PRECIS) == (int) (Ae[(i+1)*2+1]*PRECIS) )
	    {
	      if ( i != ( nbcli - 2 ))
		{
		  for ( j = i ; j < nbcli-1 ; j ++ )
		    {
		      Ae[j*2] = Ae[(j+1)*2];
		      Ae[j*2+1] = Ae[(j+1)*2+1];
		      Ax[j] = Ax[j+1];
		    }
		}
	      i--;
	      nbcli = nbcli -1;	      
	    }
	}
    }
  return nbcli;
}
