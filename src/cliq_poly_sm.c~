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

	coord coord_clis, coord_clic, coord_trad; /* coord->i : nb de cliques, coord->j : nb de synonymes */
	FILE *fclis, *fclic, *ftrad,  *fsyns, *freq, *fnb;
	FILE *fcoord, *fclis2, *fclic2,*fsyns2, *ftras, *fsync;
	char coordname[256],tradname[256],clisname[256],clicname[256],syns0name[256], syns2name[256], trasname[256], syncname[256];
	char line[1024];
  	char paralang[2],paralangC[2];
	int nsync;
//	int c;
	//FILE *fo_erreur;//fichier d'erreur

	int nf;
	int nbdim;
	int nreq, nsyns;
  char synsname[256], reqname[256], str[1024], nbname[256];
  int *req;
  //int  nsyns;
  //char synsname[256];//,  str[1024];
  //char errename[256];
  
	typedef char tsynonyme[1024];
  tsynonyme *syns;
  char *pd;
	//char c;
	int c;

	// matrices pour determiner la matrice de travail
	double **TFF, **TFE, **TEE, **mEE, **mEE1,**mFF,**mFF1,**mFE1,**mFE;
	double **matrice_travail, **matrice_tmp;
	double *D1,*D2;

	int i, j, k;

	double **cliS, **cliC;
	double **synsS, **synsC;
	vecteur v_clis, v_clic, v_trad;
	double total;
	double somme;


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

	// ****************************************************************
	//   ouverture et lecture des fichier "*.clis" "*.clir" et "*.trad" 
	//	 qui contiennent les cliques
	// ****************************************************************** 


	//lecture fichier synonymes langage source
  sprintf(synsname, "%s.syns", argv[1]);
  sprintf(nbname, "%s.nb", argv[1]);
	fnb = fopen(nbname, "wb");

	fsyns = fopen(synsname, "rb");
	if (fsyns == NULL)
		{ return(-2); }

	//comptage nombre d'entrees du fichier
	nsyns=0;
	do
	{
		c=fgetc(fsyns);
		if (c=='\n')
		{
			nsyns++;
		}
	}while (!feof(fsyns));

    if(nsyns==0)
    {
	fclose(fsyns);
  if (argv[2][0]=='F')
    {
      paralang[0] = 'F';
      paralang[1] = '\0';
      paralangC[0]='E';
      paralangC[1]='\0';
    }
  else
    {
      paralang[0] = 'E';
      paralang[1] = '\0';
      paralangC[0]='F';
      paralangC[1]='\0';
    }
  	sprintf(syns2name, "%s.syns", argv[1]);
	fsyns2=fopen(syns2name, "wb");
  	sprintf(syns0name, "%s.syns0", argv[1]);
	fsyns=fopen(syns0name, "rb");

	nsyns=0;
	do
	{
		c=fgetc(fsyns);
		if (c=='\n')
		{
			nsyns++;
		}
	}while (!feof(fsyns));



	//stockage des entrees
	syns = (tsynonyme *)calloc(nsyns, sizeof(tsynonyme));
	fseek(fsyns,0,0);
	for (i=0;i<nsyns;i++)
		{
		pd=syns[i];
		do
			{
			(*pd)=c=fgetc(fsyns);
			pd++;
			}while ((c!=13)&&(c!=10));
		if (c==13)
			fgetc(fsyns);
		pd--;
		(*pd)=(char)0;
		}
	
	fclose(fsyns);
	for(i=0;i<nsyns;i++)
		fprintf(fsyns2,"%s%s\n",paralang,syns[i]);
	fclose(fsyns2);
 //lecture fichier des requetes
  	sprintf(reqname, "%s.req", argv[1]);
	freq = fopen(reqname, "rb");
	if (freq == NULL)
		{ return(-2); }

	//comptage nombre d'entrees du fichier
	nreq=0;
	do
		{
		c=fgetc(freq);
		if (c=='\n')
			{
			nreq++;
			}
		}while (!feof(freq));


	//stockage des entrees
	req = (int *)calloc(nreq, sizeof(int));
	fseek(freq,0,0);
	for (i=0;i<nreq;i++)
	{
		pd=str;
		do
		{
			(*pd)=c=fgetc(freq);
			pd++;
		}while ((c!=13)&&(c!=10));
		if (c==13)
			fgetc(freq);
		pd--;
		(*pd)=(char)0;

    	    for (j=0; j<nsyns; j++)
    	    {
      		if (strcmp(str, &syns[j][0])==0)
      		{
       		 req[i]=j;
       		 break;
      		}
    	    }
	}
	fclose(freq);
  		sprintf(clisname, "%s.clis", argv[1]);
		fclis2=fopen(clisname, "wb");
		for(i=0;i<nsyns;i++)
		{
			fprintf(fclis2,"%d,",i+1);
		}

  		sprintf(syncname, "%s.synr", argv[1]);
		fsync=fopen(syncname, "wb");
  		sprintf(trasname, "%s.tras", argv[1]);
		ftras=fopen(trasname, "rb");
		nsync=0;
		while((c=fgetc(ftras))!=EOF)
		{
			i=0;
			line[i++]=c;
			while((c=fgetc(ftras))!='\n')
				line[i++]=c;
			nsync++;
			line[i]='\0';
			fprintf(fsync,"%s%s\n",paralangC,line);
		}
		fclose(ftras);
		fclose(fsync);
  		sprintf(clicname, "%s.clir", argv[1]);
		fclic2=fopen(clicname, "wb");
		for(i=0;i<nsync;i++)
		{
			fprintf(fclic2,"%d,",i+1);
		}

  		sprintf(tradname, "%s.trad", argv[1]);
		ftrad=fopen(tradname,"wb");
		for(i=0;i<nsyns;i++)
		{
	    	    if(req[0]==i)
	    	    {
			for(j=0;j<nsync;j++)
				fprintf(ftrad,"%d,",j+1);
	    	    }
		    fprintf(ftrad,"\n");
		}
		fclose(ftrad);


  		sprintf(coordname, "%s.cpcs", argv[1]);
		fcoord=fopen(coordname,"w");
		fprintf(fcoord,"%f %f",0.0,0.0);
		fclose(fcoord);

  		sprintf(coordname, "%s.cpcr", argv[1]);
		fcoord=fopen(coordname,"w");
		fprintf(fcoord,"%f %f",0.0,0.0);
		fclose(fcoord);


  		sprintf(coordname, "%s.cpss", argv[1]);
		fcoord=fopen(coordname,"w");
		fprintf(fcoord,"%f %f",0.0,0.0);
		fclose(fcoord);


  		sprintf(coordname, "%s.cpsr", argv[1]);
		fcoord=fopen(coordname,"w");
		fprintf(fcoord,"%f %f",0.0,0.0);
		fclose(fcoord);

	fprintf(fnb,"%d\n",1);
	fprintf(fnb,"%d\n",1);
	fprintf(fnb,"%d\n",1);
	fprintf(fnb,"%d\n",nsyns);
	fprintf(fnb,"%d\n",nsync);
















		return 1;
    }
    else
    {

	//stockage des entrees
	syns = (tsynonyme *)calloc(nsyns, sizeof(tsynonyme));
	fseek(fsyns,0,0);
	for (i=0;i<nsyns;i++)
		{
		pd=syns[i];
		do
			{
			(*pd)=c=fgetc(fsyns);
			pd++;
			}while ((c!=13)&&(c!=10));
		if (c==13)
			fgetc(fsyns);
		pd--;
		(*pd)=(char)0;
		}
	
	fclose(fsyns);






	fclis = malloc(sizeof(FILE *));
	fclis = ouvre_fichier(argv[1],".clis", "r");
	v_clis = vecteur_faire();
	lecture_fichier_num(v_clis,fclis,",",&coord_clis); 
	fclose(fclis);
	fclic = malloc(sizeof(FILE *));
	fclic = ouvre_fichier(argv[1],".clir", "r");
	v_clic = vecteur_faire();
	lecture_fichier_num(v_clic,fclic,",",&coord_clic); 
	fclose(fclic);
	ftrad = malloc(sizeof(FILE *));
	ftrad = ouvre_fichier(argv[1],".trad", "r");
	v_trad = vecteur_faire();
	lecture_fichier_num(v_trad,ftrad,",",&coord_trad); 
	fclose(ftrad);


	//creation de la matrice sur laquelle on va faire les calculs
	TFF = dmatrix(1, coord_clis.i, 1, coord_clis.j);
	cree_matrice(v_clis, TFF, coord_clis.i, coord_clis.j);

	TEE = dmatrix(1, coord_clic.i, 1, coord_clic.j);
	cree_matrice(v_clic, TEE, coord_clic.i, coord_clic.j);

	TFE = dmatrix(1, coord_trad.i, 1, coord_trad.j);
	cree_matrice(v_trad, TFE, coord_trad.i, coord_trad.j);



 //lecture fichier des requetes
  	sprintf(reqname, "%s.req", argv[1]);
	freq = fopen(reqname, "rb");
	if (freq == NULL)
		{ return(-2); }

	//comptage nombre d'entrees du fichier
	nreq=0;
	do
		{
		c=fgetc(freq);
		if (c=='\n')
			{
			nreq++;
			}
		}while (!feof(freq));


	//stockage des entrees
	req = (int *)calloc(nreq, sizeof(int));
	fseek(freq,0,0);
	for (i=0;i<nreq;i++)
		{
		pd=str;
		do
			{
			(*pd)=c=fgetc(freq);
			pd++;
			}while ((c!=13)&&(c!=10));
		if (c==13)
			fgetc(freq);
		pd--;
		(*pd)=(char)0;

    for (j=0; j<nsyns; j++)
    {
      if (strcmp(str, &syns[j][1])==0)
      {
        req[i]=j;
        break;
      }
    }
		}
	
	fclose(freq);

  //modification de la matrice TFE
  for (i=0; i<nreq; i++)
  {
    //recherche des cliques qui utilisent ce terme
    somme = (double)0.0;
    for (j=1; j<=coord_clis.i; j++)
    {
      somme += TFF[j][req[i]+1];
    }

    somme *= (double)1000.0;

    //division de la ligne de TFE par le nombre de cliques qui utilisent ce terme
    for (j=1; j<=coord_trad.j; j++)
    {
    TFE[req[i]+1][j] /= somme;
    }
  }



	



	matrice_travail = dmatrix(1, coord_clis.i, 1, coord_clic.i);
	matrice_tmp = dmatrix(1, coord_clis.i, 1, coord_trad.j);


	  //Khi2 pour la matrice TFF
	mFF1 = dmatrix(1,coord_clis.i,1,coord_clis.j);
	mFF = dmatrix(1,coord_clis.i,1,coord_clis.j);
	cree_matrice(v_clis, mFF1, coord_clis.i, coord_clis.j);

  D1 = malloc((coord_clis.i+1)*sizeof(double));
  D2 = malloc((coord_clis.j+1)*sizeof(double));
  total = calcul_marge(mFF1,coord_clis.i,coord_clis.j,D1,D2);

  matrice_khi2(mFF,mFF1,D1,D2,coord_clis.i,coord_clis.j);
  //Khi2 pour la matrice TEE
	 free(D1);free(D2);
  mEE1 = dmatrix(1,coord_clic.i,1,coord_clic.j);
  mEE = dmatrix(1,coord_clic.i,1,coord_clic.j);
cree_matrice(v_clic, mEE1, coord_clic.i, coord_clic.j);
  D1 = malloc((coord_clic.i+1)*sizeof(double));
  D2 = malloc((coord_clic.j+1)*sizeof(double));
  total = calcul_marge(mEE1,coord_clic.i,coord_clic.j,D1,D2);
  matrice_khi2(mEE,mEE1,D1,D2,coord_clic.i,coord_clic.j);
  //fclose(fo_erreur);
	free(D1);free(D2);
  // Khi2 pour la matrice TFE
 mFE1 = dmatrix(1,coord_trad.i,1,coord_trad.j);
 mFE = dmatrix(1,coord_trad.i,1,coord_trad.j);
cree_matrice(v_trad, mFE1, coord_trad.i, coord_trad.j);


//modification de la matrice TFE
  for (i=0; i<nreq; i++)
  {
    //recherche des cliques qui utilisent ce terme
    somme = (double)0.0;
    for (j=1; j<=coord_clis.i; j++)
    {
      somme += TFF[j][req[i]+1];
    }

    somme *= (double)1000.0;

    //division de la ligne de TFE par le nombre de cliques qui utilisent ce terme
    for (j=1; j<=coord_trad.j; j++)
    {
    mFE1[req[i]+1][j] /= somme;
    }
  }


  D1 = malloc((coord_trad.i+1)*sizeof(double));
  D2 = malloc((coord_trad.j+1)*sizeof(double));

  total = calcul_marge(mFE1,coord_trad.i,coord_trad.j,D1,D2);
 matrice_khi2(mFE,mFE1,D1,D2,coord_trad.i,coord_trad.j);


	   free(D1);free(D2);


	//calcul des valeurs de la matrice de travail = TFF*TFE*TEE'
	for (i=1; i<=coord_clis.i; i++)
	{
		for (j=1; j<=coord_trad.j; j++)
		{
			matrice_tmp[i][j] = (double)0;
			for (k=1; k<=coord_clis.j; k++)
			{
				matrice_tmp[i][j] += mFF[i][k]*mFE[k][j];//modif
			}
		}
	}
	for (i=1; i<=coord_clis.i; i++)
	{
		for (j=1; j<=coord_clic.i; j++)
		{
			matrice_travail[i][j] = (double)0;
			for (k=1; k<=coord_trad.j; k++)
			{
				matrice_travail[i][j] += matrice_tmp[i][k]*mEE[j][k];
			}
		}
	}


  nf = minimum(coord_clis.i, coord_clic.i, NB_MAX_COORD );
  cliS = dmatrix(1,coord_clis.i,1,nf+1);
  cliC = dmatrix(1,coord_clic.i,1,nf+1);
  analyse_fact(nf+1, coord_clis.i, coord_clic.i,matrice_travail,cliS,cliC);
  ecrit_fichier(argv[1],".cpcs", "w",cliS,coord_clis.i,nf+1);
  ecrit_fichier(argv[1],".cpcr", "w",cliC,coord_clic.i,nf+1);


	fprintf(fnb,"%d\n",nf);
	fprintf(fnb,"%d\n",coord_clis.i);
	fprintf(fnb,"%d\n",coord_clic.i);

  //calcul des coordonnees des synonymes (centres de gravite des cliques qui les contiennent)
  synsS=dmatrix(1, coord_clis.j, 1, nf+1);
  for (i=1; i<=coord_clis.j; i++)
  {
	  for (j=1; j<=(nf+1); j++)
	  {
		  synsS[i][j] = (double)0.0;
		  somme = (double)0.0;
		  for (k=1; k<=coord_clis.i; k++)
		  {
			  synsS[i][j] += TFF[k][i]*cliS[k][j];
			  somme += TFF[k][i];          
		  }
		  synsS[i][j] /= somme;
	  }
  }

  synsC=dmatrix(1, coord_clic.j, 1, nf+1);
  for (i=1; i<=coord_clic.j; i++)
  {
	  for (j=1; j<=(nf+1); j++)
	  {
		  synsC[i][j] = (double)0.0;
		  somme = (double)0.0;
		  for (k=1; k<=coord_clic.i; k++)
		  {
			  synsC[i][j] += TEE[k][i]*cliC[k][j];
			  somme += TEE[k][i];
		  }
		  synsC[i][j] /= somme;
	  }
  }
  ecrit_fichier(argv[1],".cpss", "w",synsS,coord_clis.j,nf+1);
  ecrit_fichier(argv[1],".cpsr", "w",synsC,coord_clic.j,nf+1);
  /*
  if(coord_clis.j==0)
  {
  	sprintf(syns2name, "%s.syns", argv[1]);
	fsyns2=fopen(syns2name, "wb");
  	sprintf(syns0name, "%s.syns", argv[1]);
	fsyns0=fopen(syns0name, "rb");
	fprintf(fsyns2,"test");
   }
   */



	fprintf(fnb,"%d\n",coord_clis.j);
	fprintf(fnb,"%d\n",coord_clic.j);
	fclose(fnb);
    }

	return 1;
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


/*
	while((c=fgetc(fsyns0))!=EOF)
	{
		i=0;
		line[i++]=c;
		while((c=fgetc(fsyns0))!='\n')
			line[i++]=c;
		nsync++;
		line[i]='\0';
		if(strcmp(line,
		fprintf(fsyns2,"%s\n",line);
	}
	*/
