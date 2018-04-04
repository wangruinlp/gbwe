#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "getopt.h"
#include "dicoscan.h"
//#include "../commun/dicoscan1.h"
//#include "../commun/constants.h"
//#include "constdef.h"
#include "utils.h"
#include "URL.h"
#include "Error.h"

extern char **cmatrix(int nrl, int nrh, int ncl, int nch);
extern void free_cmatrix( char **m, int nrl, int nrh, int ncl, int nch);
extern int countTokens( const char *s, const char v);
extern int toWords( char **t, const char *s, const char v, int dico);

#define MaxReq 20 //pour la suite, il faut MaxReq>1 (strictement)
#define FRANCAIS 0
#define ENGLISH 1


static int nson;
static int **succ;
static int *nsucc;
static int *parent;
static int nparent=0;
static int ** tabclique; //tableau final des cliques
static int * ntabclique; //tableau final des cliques
static int ncliques=0;

static char outname[256]; //noms des differents fichiers de suavegarde

char** syns; //tableau des noms des synonymes
FILE* fdoc; //fichier .doc
FILE* fiv; //fichier .fiv

int main(int argc,char*argv[])
{
	printf("test1\n");
        FILE* fsyn; //fichier de sortie des synonyme
	int n; 
	
	char source_lang[3];
	char display_lang[3];
	
        FILE *dict,*test; //fichier des requetes
      
	int found; //Modif. Hyung - found=1 : found  at leat a word that has no synonym but itself


        char terme[1000],filename[256],filename1[256],v[256];
	char terme1[1000];
        char  commande0[512],commande1[512],commande2[512],commande3[512];

      
       
        char c=0;
        //variables de boucle
        int i,j,k,l,jj,n1,nv; //ii,m;
       
             snprintf(filename,255,"liste1700F");
               

                       
    dict = fopen(filename, "r");
    if (dict==NULL)
        {
                //write_html_internal_error("A005");
                printf("non");
        return 0;
        }
    else
        { 
        while(!feof(dict)) //fin de fichier
            { i=0;
                        do
                {
                terme[i]=fgetc(dict);
                i++;                 }
            while ((terme[i-1]!='\n')&&(!feof(dict)));
            terme[i-1]='\0';
                      printf("terme %s  %d \n",terme,strlen(terme));
printf("test2\n");            
               if (argc>1)
		snprintf(terme1,510,argv[1]);
		else
		snprintf(terme1,510,terme);
		

    
printf("test3\n");       
     
                               snprintf(commande0,510,"./clique %s F F %s",terme1,terme);
printf("%s",commande0);
                		system(commande0);
printf("test\n");
			/*snprintf(filename1,255,"%s\"%s\".clio",Datadir,terme1);

			test = fopen(filename1, "r");
    			if (test==NULL) 
			{printf("%s\n",filename1);}

			else{
				fclose(test);*/

		                snprintf(commande2,510,"mv \"%s\".clio \"%s\".cli", terme1, terme1);
                		system(commande2);                                

               snprintf(commande3,510,"mv \"%s\".syno \"%s\".syn", terme1, terme1);
		if (system(commande3)==0)
		
  		{
//snprintf(commande1,510,"./cliq_poly_liste \"%s\"",terme1); system(commande1);
}

		
                  }
//}
                     fclose(dict);   
                        
                     }
                    return(0);
                  }
	
