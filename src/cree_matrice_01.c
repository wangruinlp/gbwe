/* cree_matrice_01.c */
#include<stdlib.h>
#include"vecteur.h"
#include"cree_matrice_01.h"


/******************************************************************/
/*         fonction de creation de la matrice clique/synonyme     */
/******************************************************************/

void cree_matrice(vecteur v,double **m02,int ligne,int colonne)
{
  int i,j;
  int *val;

  for ( i = 1; i <= ligne ; i++)
    {
      for ( j = 1; j <= colonne ; j++ )
	{
	  m02[i][j] = 0;
	}
    }
  val = malloc(sizeof(int));
  j = 0;
  for ( i = 1 ; i <= ligne ; i ++ )
    {
      val = vecteur_lire(v,j);
      while ( *val != -1 )
	{
	  m02[i][*val] = 1;
	  j++;
	  val = vecteur_lire(v,j);
	}
      j++;
    }
}

