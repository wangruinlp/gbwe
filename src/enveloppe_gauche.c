/*   enveloppe_gauche.c   */
#include<stdlib.h>
#include<stdio.h>
#include "pilegen.h"
#include "enveloppe_gauche.h"
#include "produit_croise.h"
#include "max.h"

static int teste_sommets(int i,pilegen pile_e1,pilegen pile_e2);

void enveloppe_gauche(double Ae[], int lig,pilegen pile_e1,pilegen pile_e2)
{
  int i_env_1;
  int i_env_2;
  int i;
  double resu;
  int  pbas,phaut;
  int pgauche,pdroite;

  i_env_1 = *(int *) pilegen_sommet(pile_e2);
  i_env_2 = *(int *) pilegen_sommet(pile_e1);

  pbas = i_env_2;
  phaut = i_env_1;
  if ( pbas < phaut )
    {
      pgauche = pbas; pdroite = phaut;
    }
  else
    {
      pgauche = phaut; pdroite = pbas;
    }
  i = 0;
  if ( teste_sommets(i,pile_e1,pile_e2) )
    {
      i++;
      if ( teste_sommets(i,pile_e1,pile_e2) )
	return;
    }
  do
    {
      if (  teste_sommets(i,pile_e1,pile_e2) )
	{
	  i++;
	  continue;
	}
      resu = produit_croise(Ae[pbas*2],Ae[pbas*2+1],Ae[i_env_1*2],Ae[i_env_1*2+1],Ae[i*2],Ae[i*2+1]);
      if ( resu > 0 )
	i_env_1 = i;
      resu = produit_croise(Ae[phaut*2],Ae[phaut*2+1],Ae[i_env_2*2],Ae[i_env_2*2+1],Ae[i*2],Ae[i*2+1]);
      if ( resu < 0 )
	i_env_2 = i;
      i++;
    } while ( i < pdroite);

  if ( i_env_1 == i_env_2 )
    {
      int * entier = malloc(sizeof(int));
      *entier = i_env_1;
      pilegen_empiler(pile_e1,entier);
      return;
    }
  else 
    if ( (i_env_2 == pbas) && (i_env_1 == phaut))
      return;
    else
      {
	int * entier1 = malloc(sizeof(int));
	int * entier2 = malloc(sizeof(int));
	*entier1 = i_env_1;
	pilegen_empiler(pile_e1,entier1);

	*entier2 = i_env_2;
	pilegen_empiler(pile_e2,entier2);

	enveloppe_gauche(Ae,lig,pile_e1,pile_e2);
      }
}

int teste_sommets(int i,pilegen pile_e1,pilegen pile_e2)
{
  if ( (i == *(int *)pilegen_sommet(pile_e1)) || (i == *(int *)pilegen_sommet(pile_e2)))
    return 1;
  else
    return 0;
}




