/*   enveloppe_droite.c   */

#include<stdlib.h>
#include<stdio.h>
#include "pilegen.h"
#include "enveloppe_droite.h"
#include "produit_croise.h"
#include "min.h"

static int teste_sommets(int i,pilegen pile_e1,pilegen pile_e2);

void enveloppe_droite(double Ae[], int lig,pilegen pile_e1,pilegen pile_e2)
{
  int i_env_1;
  int i_env_2;
  int i;
  double resu;
  int pbas,phaut;
  int pgauche,pdroite;

  i_env_1 = *(int *) pilegen_sommet(pile_e1);
  i_env_2 = *(int *) pilegen_sommet(pile_e2);

  phaut = i_env_1;
  pbas = i_env_2;
  i = lig-1;

  if ( pbas < phaut )
    {
      pgauche = pbas; pdroite = phaut;
    }
  else
    {
      pgauche = phaut; pdroite = pbas;
    }

  if ( teste_sommets(i,pile_e1,pile_e2))
    {
      i --;
      if ( teste_sommets(i,pile_e1,pile_e2) )
	return;
    }
  do
    {
      if ( teste_sommets(i,pile_e1,pile_e2) )
	{
	  i--;
	  continue;
	}
      resu = produit_croise(Ae[pbas*2],Ae[pbas*2+1],Ae[i_env_1*2],Ae[i_env_1*2+1],Ae[i*2],Ae[i*2+1]);
      if ( resu < 0 )
	i_env_1 = i;

      resu = produit_croise(Ae[phaut*2],Ae[phaut*2+1],Ae[i_env_2*2],Ae[i_env_2*2+1],Ae[i*2],Ae[i*2+1]);
      if ( resu > 0 )
	i_env_2 = i;
      i--;
    } while ( i > pgauche );
  if ( i_env_1 == i_env_2 )
    {
      int * entier = malloc(sizeof(int));
      *entier = i_env_1;
      pilegen_empiler(pile_e1,entier);
      return;
    }
  else
    if ( (i_env_1 == phaut) && (i_env_2 == pbas))
      return;
    else
      {
	int * entier1 = malloc(sizeof(int));
	int * entier2 = malloc(sizeof(int));
	*entier1 = i_env_1;
	*entier2 = i_env_2;
	pilegen_empiler(pile_e1,entier2);
	pilegen_empiler(pile_e2,entier1);
	enveloppe_droite(Ae,lig,pile_e1,pile_e2);
      }
}

int teste_sommets(int i,pilegen pile_e1,pilegen pile_e2)
{
  if ( (i == *(int *)pilegen_sommet(pile_e1)) || (i == *(int *)pilegen_sommet(pile_e2)))
    return 1;
  else
    return 0;
}

