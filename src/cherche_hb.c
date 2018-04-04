/* cherche_hb.c  */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include "pilegen.h"
#include "cherche_hb.h"
#define PRECISION 0.00001


void cherche_hb(double Ae[],int lig,pilegen pile_e1,pilegen pile_e2)
{
  double max, min;
  int i;
  int *maxi;
  int *mini;

  max = Ae[1];
  min = Ae[lig*2-1];

  maxi = malloc(sizeof(int));
  mini = malloc(sizeof(int));
  *maxi = 0;
  *mini = lig-1;

  for ( i = 0; i < lig ; i ++ )
    {
      if ( Ae[i*2+1] > max+PRECISION )
	{
	  max = Ae[i*2+1];
	  *maxi = i;
	}
      if ( Ae[i*2+1] < min-PRECISION )
	{
	  min = Ae[i*2+1];
	  *mini = i;
	}
    }
  pilegen_empiler(pile_e1,mini);
  pilegen_empiler(pile_e2,maxi);
}





