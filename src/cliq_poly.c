/*
ca: The Correspondence Analysis (CA) dimension reduction method. 
Usage: ./ca work_e
This will create mainly two files:
work_e.cpc: The vectors for cliques
work_e.cps: The vectors for words
Typically, the 6-dimension vectors of work_e.cpc and work_e.cps are corresponding to the clique/word order in work_e.cli and work_e.syn.
*/

/* ----------------------------------------------------------- */
/*    VERSION NUMERICAL PRINCIPES                              */
/*    lecture du fichiers .cli  des cliques                    */
/*    mise en forme de matrice                                 */
/*    calcul de valeurs propres metrique du khi-deux           */
/*    calcul des composantes principales                       */
/*    calcul des coordonnees des synonymes                     */
/*    calcul des coordonnees des cliques                       */
/*    calcul des coordonnees des ellipses                      */
/* ----------------------------------------------------------- */
/* cliq_poly.c    par Francis BOUDIN pour l'isc    le 8/11/99  */

/****************************************************************
  le nom du fichier de la vedette doit etre passe en argument
  lorsque on lance le programme. Ne pas mettre l'extension :
  cliq animal
  ****************************************************************/

#include<sys/types.h>
//#include<sys/time.h> 
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
//#include<unistd.h>
#include<math.h>
#include<string.h>

#include"taille_tampon.h"
#include"exit_if.h"
#include"erreurs.h"
#include"remplir.h"
#include"nrutils.h" 
#include"tred2.h"
#include"tqli.h"
#include"eigsrt.h"
#include"vecteur.h"
#include"pilegen.h"
#include"affiche.h"
#include"calcul_marge.h"
#include"minimum.h"
#include"cree_matrice_01.h"
#include"acces_fichier.h"
#include"matrice_khi2.h"
#include"produit_matrice.h"
#include"cherche_hb.h"
#include"enveloppe_gauche.h"
#include"enveloppe_droite.h"
#include"transvase.h"
#include"produit_croise.h"
#include"tri_rapide.h"
#include"cherche_doublons.h"
#include"lecture_fichier.h"


//#define CLEAR() printf("\033[2J")
#define NB_MAX_COORD 6


void analyse_fact(int nf,int ligne,int colonne,double **m02, double **ccli,double **csyn);
void polygone(int nf,int ligne,int colonne,double **m02,const char *vedette, double **CC);
char * ajoute(char * test_chaine,char * test_doublon);
char *  ecrit_env(FILE *flot,char **nouvelle_chaine, char *test_chaine_temoin,int *cpt_doublon);
char * enveloppe(FILE *flot,int *nbcli, double *Ae, int *Ax,int *cpt_doublon,char **nouvelle_chaine,char *test_chaine_temoin,pilegen pile_e1,pilegen pile_e2);
char ** ajouter(char **nouvelle_chaine,char * test_doublon);
void vide_compteur(FILE *flot,int *cpt_doublon);



/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */ 
int main ( int argc, char *argv[] )
{
  struct timeval *temps1; /* mesurer */
  struct timezone *tps1;

  coord coordi; /* coordi->i : nb de cliques, coordi->j : nb de synonymes */
  FILE *flot;
	FILE *fnb;
	char nbname[256];
  int nf;
  int nbdim;

  double ** m02;
  double ** ccli;
  double ** csyn;
  vecteur v;


  temps1 = malloc(TAILLE_TAMPON);
  tps1 = malloc(TAILLE_TAMPON);


  if ( argc>3 || argc<2 )
    {
      fprintf(stderr, "Usage : %s <<nom de la vedette>>  [dimensions]\n",argv[0]);
      return EXIT_FAILURE;
    }
  if (argc==3)
  {
          nbdim=atoi(argv[2]);
          if (nbdim>6) nbdim=6;
  }
  else 
          nbdim=6;

//  gettimeofday(temps1,tps1); /* prise du temps avant lecture du fichier des cliques */

  /* ****************************************************************
     ouverture et lecture du fichier "*.cli" qui contient les cliques
     ****************************************************************** */
  
  flot = malloc(sizeof(FILE *));
  flot = ouvre_fichier(argv[1],".cli", "r");


  v = vecteur_faire();
  lecture_fichier_num(v,flot,",",&coordi); 
  fclose(flot);


  /* ************************************************************
     mise en forme de la matrice correspondante A[]
     la matrice theorique a coordi.i lignes et coordi.j colonnes
     la matrice reelle est ecrite sous forme d'un vecteur
     dont les elements se reperent par : i * coordi.j + j
     *********************************************************** */

  m02 = dmatrix(1,coordi.i,1,coordi.j);
  cree_matrice(v,m02,coordi.i,coordi.j);  
  nf = minimum( coordi.i, coordi.j, NB_MAX_COORD );
      
  ccli = dmatrix(1,coordi.i,1,nf+1);
  csyn = dmatrix(1,coordi.j,1,nf+1);


  analyse_fact(nf+1, coordi.i, coordi.j,m02,ccli,csyn);
  ecrit_fichier(argv[1],".cpc", "w",ccli,coordi.i,nf+1);
  ecrit_fichier(argv[1],".cps", "w",csyn,coordi.j,nf+1);

  sprintf(nbname, "%s.nb", argv[1]);
	fnb = fopen(nbname, "w");
	fprintf(fnb,"%d\n",nf);
	fprintf(fnb,"%d\n",coordi.i);
	fprintf(fnb,"%d\n",coordi.j);
	fclose(fnb);
	

  polygone(nf+1,coordi.i,coordi.j,m02,argv[1],ccli);
/*      printf("nom : %s\n",argv[1]);  */
/*      printf("nb axes : %d\n",nf); */ 

/*      affiche_temps(temps1);  */ 
/*      affiche_temps(temps1);   */ 
/*    printf("fin des enveloppes\n");  */
/*    affiche_temps(temps1);  */
  return 0;
}

/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */




void analyse_fact(int nf,int ligne,int colonne,double **m02, double **ccli, double **csyn)
{
  /* ************************************************************
   *              A N A L Y S E    F A C T O R I E L L E 
   * ************************************************************

   * ***********************************************************
   *    ouverture du fichier "nom de la vedette".cpr
   *     pour rentrer les coordonnees des synonymes
   *         dans le repere forme par les six premieres 
   *                       composantes principales.
   * ************************************************************* */

  double valeur;
  int n;
  int i,j,ii;
  double total; /* somme des elements de la matrice m01 */
  double **mv;
  double *d;
  double *e;
  double * D1;  /* vecteur D1 : total des lignes */
  double * D2;  /* vecteur D2 : total des colonnes */
  double **m01;

  m01 = dmatrix(1,ligne,1,colonne);

  mv = dmatrix(1,colonne,1,colonne);
  d = malloc((colonne+1)*sizeof(double));
  e = malloc((colonne+1)*sizeof(double));

  D1 = malloc((ligne+1)*sizeof(double));
  D2 = malloc((colonne+1)*sizeof(double));

  total = calcul_marge(m02,ligne,colonne,D1,D2);
  matrice_khi2(m01,m02,D1,D2,ligne,colonne);
  produit_matrice(mv,m01,ligne,colonne);
  tred2(mv,colonne,d,e); /* tridiagonalisation de la matrice mv */
  tqli(d,e,colonne,mv); /* diagonalisation de la matrice mv */
  eigsrt(d,mv,colonne);/* tri des valeurs propres et des vecteurs propres
                          dans l'ordre decroissant des valeurs propres. */

  /* *******************************************
     calcul des coordonnees 
     ******************************************* */ 
  for ( n = 1 ; n <= nf ; n++ )
    {
      for ( i = 1 ; i <= ligne ; i++ )
        {
          valeur = 0;
          for( j = 1 ; j <= colonne ; j++ )
            {
              valeur = valeur + m01[i][j]*mv[j][n];
            }
          valeur = valeur*sqrt(total*fabs(D1[i]));
          ccli[i][n] = valeur;
        }      
      for ( i = 1 ; i <= colonne ; i ++ ) /* il s'agit de tA */
        {
          valeur = 0;
          for ( ii = 1 ; ii <= ligne ; ii ++ )
            {
              valeur = valeur + m02[ii][i]*ccli[ii][n];
            }
          valeur = valeur*fabs(D2[i]);
          csyn[i][n] = valeur;
        }
    }
}



/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */
/* XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX */

void polygone(int nf,int ligne,int colonne,double **m02,const char * vedette, double **ccli)
{
  /*************************************************************/
  /*                   P O L Y G O N E S                       */
  /*************************************************************/
  
  int i;
  int j;
  int c,cc;
  double d;
  FILE *flot;
  int nbcli;
  double * Ae; /* matrices des coord des cliques contenant un synonyme donne */
  int *Ax; /* vecteur memorisant le numero des cliques dans la matrice de depart */
/*    char *test_chaine; */
  char *test_chaine_temoin;
  char **nouvelle_chaine;
  int cpt_doublon;

  pilegen pile_e1 = pilegen_faire();
  pilegen pile_e2 = pilegen_faire();
  pilegen_mode_bavard(pile_e1,1);
  pilegen_mode_bavard(pile_e2,1);
  
  Ae = malloc(ligne*2*sizeof(double));
  Ax = malloc(ligne*sizeof(int));
  nouvelle_chaine = malloc(sizeof(char *));
  *nouvelle_chaine = malloc(7*sizeof(char));
  test_chaine_temoin = malloc(7*sizeof(char));
  **nouvelle_chaine = '\0';
  *test_chaine_temoin = '\0';
  cpt_doublon = 0;
  
  flot = ouvre_fichier(vedette,".env", "w");  
  
  for( j = 1; j <= colonne ; j++ ) /* ------------------  choix du synonyme */
    {
      for( c = 2; c <= nf-1; c++ ) /* ------------------  choix 1ere coord */
        {
          for( cc = c + 1 ; cc <= nf ; cc++ ) /* -------  choix 2eme coord */
            {
              d = -1;
              nbcli = -1;
              for( i = 1; i <= ligne; i++ ) /* ---------  choix de clique */
                {
                  if ( m02[i][j] == 1 )
                    {
                      nbcli ++;
                      Ae[nbcli] = ccli[i][c];
                      Ax[nbcli/2] =i;
                      nbcli ++;
                      Ae[nbcli] = ccli[i][cc];                    
                    }
                }
              test_chaine_temoin = enveloppe(flot,&nbcli,Ae,Ax,&cpt_doublon,nouvelle_chaine,test_chaine_temoin,pile_e1,pile_e2);
            }
        }
      vide_compteur(flot,&cpt_doublon);
      **nouvelle_chaine = '\0';
      *test_chaine_temoin = '\0';
    }
  fclose(flot);
}

char * enveloppe(FILE *flot,int *nbcli, double *Ae, int *Ax,int *cpt_doublon,char **nouvelle_chaine,char *test_chaine_temoin,pilegen pile_e1,pilegen pile_e2)
{
  int *mini;
  char *test_doublon;
  int i;
  

  test_doublon = malloc(7*sizeof(char));
  *test_doublon = '\0';
  mini = malloc(sizeof(int));

  *nbcli = (*nbcli+1)/2;
  if ( *nbcli > 2  ) /* si plus de trois cliques 
                       alors recherche de polygone */
    {
      tri_rapide(Ax,Ae,1,*nbcli); /* tri du tableau Ae
                                    selon les abscisses
                                    croissantes des cliques */
      *nbcli = cherche_doublons(Ax,Ae,*nbcli);
    }
  if ( *nbcli > 2 )
    {
      cherche_hb(Ae,*nbcli,pile_e1,pile_e2);
      mini = pilegen_sommet(pile_e1);
      enveloppe_gauche(Ae,*nbcli,pile_e1,pile_e2);
      transvase(pile_e1,pile_e2); /* remplissage de pile1 
                                     avec le vidage de pile2 */
      pilegen_empiler(pile_e2,mini);
      enveloppe_droite(Ae,*nbcli,pile_e1,pile_e2);
      transvase(pile_e1,pile_e2);
      sprintf(*nouvelle_chaine,"%d",Ax[*(int *)pilegen_depiler(pile_e1)]-1);
       while ( ! pilegen_vide(pile_e1))
        {
          sprintf(test_doublon," %d",Ax[*(int *)pilegen_depiler(pile_e1)]-1);
          nouvelle_chaine = ajouter(nouvelle_chaine,test_doublon);
        } while ( ! pilegen_vide(pile_e1));
    }
  else
    {
      for ( i = 0 ; i < *nbcli ; i ++ )
        {
          if ( i == *nbcli-1 )
            sprintf(test_doublon, "%d",Ax[i]-1);
          else
            sprintf(test_doublon,"%d ",Ax[i]-1);
          nouvelle_chaine = ajouter(nouvelle_chaine,test_doublon);        
        }
    }
  test_chaine_temoin = ecrit_env(flot,nouvelle_chaine,test_chaine_temoin,cpt_doublon);
  return test_chaine_temoin;
}

char ** ajouter(char **nouvelle_chaine,char * test_doublon)
{
  *nouvelle_chaine = realloc(*nouvelle_chaine,(strlen(*nouvelle_chaine)+strlen(test_doublon)+1)*sizeof(char));
  strcat(*nouvelle_chaine,test_doublon);
  return nouvelle_chaine;
}

char *  ecrit_env(FILE *flot,char **nouvelle_chaine,char *test_chaine_temoin,int *cpt_doublon)
{
  if ( strcmp(*nouvelle_chaine,test_chaine_temoin) == 0 )
    {
      if (*cpt_doublon == 0 )
        {
          fprintf(flot, "%%");
        }
      *cpt_doublon= *cpt_doublon + 1;
    }
  else
    {
      vide_compteur(flot,cpt_doublon);
      fprintf(flot, "%s\n",*nouvelle_chaine);
      test_chaine_temoin = realloc(test_chaine_temoin,strlen(*nouvelle_chaine)+1);
      strcpy(test_chaine_temoin,*nouvelle_chaine);
      *cpt_doublon = 0;
    }
  **nouvelle_chaine = '\0';
  return test_chaine_temoin;
}

void vide_compteur(FILE *flot,int *cpt_doublon)
{
  if (*cpt_doublon > 0)
    {
      if (*cpt_doublon > 1)
        fprintf(flot, "%d\n",*cpt_doublon);
      else
        fprintf(flot, "\n");
    }
  *cpt_doublon = 0;
}


