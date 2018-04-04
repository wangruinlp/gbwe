/* calcul_marge.c */

#include"calcul_marge.h"
#include"remplir.h"
#include<math.h>

/******************************************************************/
/*         fonctions de calcul des marges                         */
/******************************************************************/

double calcul_marge(double **m02,int ligne,int colonne,double marge_l[], double marge_c[])
{
  double total;
  int i,j;
  
  total = 0;
  rempli(marge_l,ligne,0);
  rempli(marge_c,colonne,0);
  for ( i = 1 ; i <= ligne ; i ++ )
    {
      for ( j = 1 ; j <= colonne ; j ++ )
	{
	  marge_l[i]=marge_l[i]+m02[i][j];
	  marge_c[j]=marge_c[j]+m02[i][j];
	  total = total +m02[i][j];
	}
    }
  for ( i = 1 ; i <= ligne ; i++)
  { if (fabs(marge_l[i])>0.00001)
      marge_l[i] = 1/(marge_l[i]);
    }
  for ( j = 1 ; j <= colonne ; j++ )
    {if (fabs(marge_c[j])>0.00001)
      marge_c[j] = 1/(marge_c[j]);
    }
  return total;
}
