/* affiche.c */

#include<sys/time.h> 
#include<time.h> 
#include<stdio.h>
#include<stdlib.h>
//#include<time.h>

#include "affiche.h"


/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */
void affiche_temps(struct timeval *temps1)
{
  double tdiff;   /* variables pour */
  long tdiffm;
  struct timeval *temps2; /* le temps d'execution */
  struct timezone *tps2;

  temps2 = malloc(sizeof(struct timeval));
  tps2 = malloc(sizeof(struct timezone));

  gettimeofday(temps2,tps2);
  tdiff = difftime(temps2->tv_sec,temps1->tv_sec);
  tdiffm = temps2->tv_usec-temps1->tv_usec;
  printf("\ttemps ecoule : %.0f seconde(s) %ld microseconde(s) \n",tdiff,tdiffm);
  tdiff = tdiff*1000000 + tdiffm;
  printf("\ttemps total ecoule en microsec.: %.0f\n",tdiff);

}


/******************************************************************/
/*         fonctions d'affichage des  V E C T E U R S             */
/******************************************************************/

void affiche_vecteur(double vecteur[],int ligne,int colonne,int position)
{
  int i;
  for ( i = 0 ; i < ligne ; i++ )
    {
      printf("%6.3f ",vecteur[i*colonne+position]);
    }
  printf("\n");
}


/******************************************************************/
/*         fonctions d'affichage  des M A T R I C E S             */
/******************************************************************/

void affiche_matrice(double **matrice,int ligne,int colonne)
{
  int i,j;
  for ( i = 1 ; i <= ligne ; i++ )
    {
      printf("%4d: ",i);
      for ( j = 1 ; j <= colonne ; j++ )
	{
	  printf("%6.3f ",matrice[i][j]);
	}
      printf("\n");
    }
}

/******************************************************************/
/*         fonction d'affichage  des marges                       */
/******************************************************************/

void affiche_marge(double marge[],int colonne)
{
  int j;
  printf("\n     ");
  for ( j = 0 ; j < colonne ; j ++ )
    {	
      printf("%2.0f",marge[j]);
    }
  printf("\n");
}

/******************************************************************/
/*         fonction fermeture transitive                          */
/******************************************************************/

void affiche_fermeture(int sommet,int GAMA[],int colonne)
{
  int i;
  printf("composante %d : ", sommet);
  for( i = 0 ; i < colonne ; i++ )
    {
      if ( GAMA[i] == sommet )
	{
	  printf("%3d ", i);
	}
    }
  printf("\n");
}

