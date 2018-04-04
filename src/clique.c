/*
clique: This is for clique extraction. 
Usage: ./clique work_e F F work_e
This will create several files, typically:
work_e.syn: all of the Contexonym words.
work_e.doc: the document information of clique extraction.
work_e.cli: The cliques.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dicoscan.h"
#include "Error.h"
#include "constdef.h"
#include "utils.h"
#include "URL.h"
#include "intint.h"
#include "countTokens.h"
//#include "../commun/debug.h"

#define MaxReq 500 //pour la suite, il faut MaxReq>1 (strictement)
#define LFRANCAIS 0
#define LENGLISH 1

//Remarque : une clique est toujours terminée par le chiffre '-1' (de la même façon qu'une chaine de caractères est terminée par '0')

//FILE* flog;
static int nson;
static int **succ;
static int * buffer; //tableau tmp final des cliques
static int *nsucc;
static int *parent;
static int nparent=0;
static int ** tabclique; //tableau final des cliques
static int * ntabclique; //tableau final des cliques
static int ncliques=0;

static char outname[256],filename[256]; //noms des differents fichiers de suavegarde
static char colon[3];
char** syns; //tableau des noms des synonymes
FILE* fdoc; //fichier .doc
FILE* fiv; //fichier .fiv

void printtabclique() //ecriture du tableau de cliques dans le fichier des cliques
{
        FILE* fcli;
        int i,j;
	int n;
        sprintf(filename,"%s.cli",outname);
        fcli=fopen(filename,"wb");
        fprintf(fdoc,"<BR><BR>\n<A NAME=\"cliques\"><I><B>Cliques</B></I></A>%s<BR>",colon);
        for (i=0;i<ncliques;i++)
        {
            fprintf(fdoc,"<BR>\n");
            fprintf(fdoc,"%d%s ",i+1,colon);
	    for(j=0;j<ntabclique[i]-1;j++)
	    {
                n=tabclique[i][j];
                fprintf(fcli,"%d,",n+1);
                fprintf(fdoc,"%s, ",syns[n]);
	    }
            n=tabclique[i][j];
            fprintf(fcli,"%d,\n",n+1);
            fprintf(fdoc,"%s\n",syns[n]);
        }
        fclose(fcli);
        fclose(fdoc);
}


int mystrcmp(char* a,char* b) //prerequis : strlen(a)<strlen(b) !
{
        int l=strlen(a);
        int i;
        for (i=0;i<l;i++)
        {
                if ((*a)<(*b)) return (*a)-(*b);
                a++;

                b++;
        }
        return 0;
};

int
hasPar(int n, int m) //n is index of parent, m is value to compare
{
	int i;
	int np;
	np=parent[n];
	if(n==0)
	{
		for(i=0;i<nsucc[np];i++)
		{
			if(succ[np][i]==m)
				return true;
		}
		return false;
	}
	else
	{
		for(i=0;i<nsucc[np];i++)
		{
			if(succ[np][i]==m)
			{
				return hasPar(n-1, m);
			}
		}
		return false;
	}
}
int
isSurflous()
{
	int i;
	int n,m;
	n=nson+1;
	for(i=0;i<ncliques;i++)
	{
		if((m=ntabclique[i])>n)
		     if(intint(tabclique[i],m,buffer,n))
			return true;
	}
	return false;

}

int
hasSon(int n) // n is index of succ, nparent is index of parent
//hasSon(int n, int par) // n is index of succ, nparent is index of parent
{
	int i;
	int s;
	int isBuffered=false;

	parent[nparent++]=n;
	
	for(i=0;i<nsucc[n];i++)
	{
		s=succ[n][i];
		if(hasPar(nparent-2,s))
		{
			buffer[nson++]=s;
			isBuffered=true;
			

			//if(!hasSon(s,n))
			if(!hasSon(s))
			{
				if(!isSurflous())
				{

					copyint(tabclique[ncliques],buffer,nson+1);
					ntabclique[ncliques]=nson+1;
					ncliques++;
					//fprintf_intt(stdout,tabclique[ncliques-1],nson+1);
				}
			}
		}
	}
	nson--;
	nparent--;
	if(isBuffered)
		return true;
	else
		return false;

}
int
main(int argc,char*argv[])
{
        FILE* fsyn; //fichier de sortie des synonyme
	int ns, son;
	int taillemaxclique;
	int first=true;
	char source_lang[3];
	char display_lang[3];
	int Nsyntotal;
	int nw,nwold;
	int maxlinelen=MAX_LINE_LEN;
	char **syns2;
        FILE *freq; //fichier des requetes
        FILE *ftras; 

        //vedettes
        char trvedette[256],str[256];
        char **vedette;
        char **line;
        int nvedettes;

        //variables de boucle
        int i,j,k,l,m;

        //ligne de dictionnaires
        //char linetr[MaxReq][MaxLine]; 
        char **linetr;

        //variables temporaires
        int premiersyn;
        int nsynreq[MaxReq]; //nombre de synonymes de chaque requete
        int nsyntotal=0; //nombre de synonymes total (sans les doublons)

        //tableau de successeurs pour chaque vedette
	int displayLang;

        //fin des declarations
        if (argc<4)
        {
                printf("\nutilisation : cliques.exe nom_sortie requete1 [requete_n] ");
                printf("\nnombre de requetes limité à %d\n",MaxReq);
                printf("\nles fichiers generés seront nom_sortie.cli, nom_sortie.doc, nom_sortie.syn et nom_sortie.req\n");
                return 0;
        }
        //on copie les arguments dans les vedettes. Attention : chaque nouvel argument represente l'apparition d'un espace dans la ligne de commande; mais les vedettes sont séparées par '+'.
	strcpy(outname,argv[1]);
	strcpy(source_lang, argv[2]);
	strcpy(display_lang, argv[3]);
        if(isEqual(display_lang,"F"))
	{
		strcpy(colon," :");
		displayLang=LFRANCAIS;
	}
	else
	{
		strcpy(colon,":");
		displayLang=LENGLISH;
	}
	nvedettes=countTokens(argv[4],'+');
//printf("nbvedettes %d\n",nvedettes); //sabine
//getchar();
	if(nvedettes>MaxReq)
		exit(1);
	vedette=cmatrix(0,nvedettes-1,0,MAX_WORD_LEN);
	line=cmatrix(0,nvedettes-1,0,maxlinelen);
	linetr=cmatrix(0,nvedettes-1,0,maxlinelen);
	toWords(vedette, argv[4], '+', false);
        //fin de la désignation des vedettes
//printf(" %s\n",outname); //sabine

//getchar();


        //ecriture du fichier de requetes
        sprintf(filename,"%s.req",outname);
        freq = fopen(filename, "wb");
        for (i=0; i<nvedettes; i++)
        {
          fprintf(freq, "%s\n",vedette[i]);
        }
        fclose(freq);
//printf(" %s\n",filename); //sabine

//getchar();

        sprintf(filename,"%s.tras",outname);
        ftras=fopen(filename,"wb");
        for (i=0;i<nvedettes;i++)
        {
	  strcpy(trvedette,source_lang);
          strcat(trvedette, vedette[i]);
          getSynLine(trvedette,linetr[i],source_lang);
          l=strlen(linetr[i]);
          k=0;
          m=0;
          for (j=0;j<l;j++)
            if (linetr[i][j]=='\t')
              {
                if (k>1) // 2 /t avant les traductions dans le dico des traductions
                {
                  strncpy(str, &linetr[i][m+1], (j-m-1));
                  str[j-m-1] = (char)0;
                  fprintf(ftras, "%s\n", str);
                }
                k++;
                m=j+1;
              }
        }
        fclose(ftras);

        tabclique=(int**)malloc(0); //pour ne pas faire planter free(tabclique);

	nsyntotal=0;
        for (i=0;i<nvedettes;i++)
        {
                if(getSynLine(vedette[i],line[i],source_lang)==0)//Mod. Hyung
		{
			if(first)
			{
			   	sprintf(filename,"%s.noentry",outname);
				fsyn=fopen(filename,"wb");
			}
			first=false;
			fprintf(fsyn,"%s\n",vedette[i]);
			continue;
		}

		nsynreq[i]=countTokens(line[i],'\t')-1;
		nsyntotal+=nsynreq[i];
        }
	if(!first)
	{
		fclose(fsyn);
		free_cmatrix(line,0,nvedettes-1,0,maxlinelen);
		free_cmatrix(linetr,0,nvedettes-1,0,maxlinelen);
		free_cmatrix(vedette,0,nvedettes-1,0,MAX_WORD_LEN);
		exit(1);
	}
	Nsyntotal=nsyntotal-1;
//	fprintf(stdout,"%d\n",nsyntotal);
	syns=cmatrix(0,Nsyntotal,0,MAX_WORD_LEN);
	syns2=cmatrix(0,Nsyntotal,0,MAX_WORD_LEN);

        //on copie tous les synonymes de toutes les vedettes dans un unique tableau
	//fprintf_words(stdout,line,nvedettes);
	nsyntotal=toSyns(syns2,line,nvedettes);
	nsyntotal=wsort(syns, syns2, nsyntotal, MAX_WORD_LEN);
	//	fprintf_words(stdout,syns,nsyntotal);
	nwold=Nsyntotal;
//printf("la");getchar();

        /*------------------*\
        | Creation du graphe |
        \*------------------*/
        
        //rmq : on ne considere que les arcs de i vers j, i<j
        //rmq2: line[0] et line[1] servent maintenant de chaine temporaire
	nsucc=ivector(0,nsyntotal-1);
        //nsucc=(int*)malloc(nsyntotal*sizeof(int));
        succ=(int**)malloc(nsyntotal*sizeof(int*));
        for (i=0;i<nsyntotal;i++)
        {
                //on compte le nombre de successeur du mot syns[i]
                nsucc[i]=0;
		//sprint(syns[i]);
                getSynLine(syns[i],line[0],source_lang);
		nw=nsucc[i]=countTokens(line[0],'\t')-2;
                succ[i]=(int*)malloc(nsucc[i]*sizeof(int));
		if(nw>nwold)
		{
			free_cmatrix(syns2,0,nwold,0,MAX_WORD_LEN);
			syns2=cmatrix(0,nw,0,MAX_WORD_LEN);
			nwold=nw;
		}
//		nsyntotal=wsort(syns, syns2, nsynT);
                nsucc[i]=0;
                // on rempli le tableau des successeurs de syns[i]
		//toSyns(syns2,line[0],1);
		toWords(syns2,line[0],'\t',true);
		  for(k=0;k<nw;k++)
	    	  {
                        for (j=i+1;j<nsyntotal;j++) // recherche de l'indice du successeur dans le tableau syns
			{
                                if (isEqual(syns[j],syns2[k]))
                                {
                                        succ[i][nsucc[i]]=j;
                                        nsucc[i]++;
                                        break;
                                }
			}
		}
        }
	
	free_cmatrix(syns2,0,nwold,0,MAX_WORD_LEN);
	

        //ecriture du fichier .syn
        sprintf(filename,"%s.syn",outname);
        fsyn=fopen(filename,"wb");
        if (fsyn==NULL)
        {
                //printf("impossible d'ouvrir le fichier !<BR>\n");
		free_cmatrix(line,0,nvedettes,0,maxlinelen);
		free_cmatrix(linetr,0,nvedettes,0,maxlinelen);
		free_cmatrix(vedette,0,nvedettes,0,MAX_WORD_LEN);
		free_cmatrix(syns,0,Nsyntotal,0,MAX_WORD_LEN);
                return 0;
        }
        for (i=0;i<nsyntotal;i++)
        {
                fprintf(fsyn,"%s\n",syns[i]);
        }
        fclose(fsyn);

        /*------------------*\
        | Calcul des cliques |
        \*------------------*/
        
        taillemaxclique= Min(nsyntotal*nsyntotal,MAX_CLIQUES);
	
	if((tabclique=imatrix(0, taillemaxclique, 0, nsyntotal))==NULL)
	{
		free_ivector(nsucc,0,nsyntotal-1);
        	sprintf(filename,"cp %s.req %serror.%s",outname,Datadir,"cliquetr");
		system(filename);
		return ALLOCATION_FAILURE;
	}
	buffer=ivector(0, nsyntotal-1);
	ntabclique=ivector(0, taillemaxclique);
        parent=ivector(0,taillemaxclique);
	j=0;i=0;l=0;nson=0;
	nparent=0;
	ncliques=0;
	nson=0;
//printf("nsyntotal %d",nsyntotal);getchar();
	if(nsyntotal>2)
	{
		for(i=0;i<nsyntotal;i++)
		{
		//	dprint(i);
			nparent=0; nson=0;
			ns=nsucc[i]-1;
		//	dprint(ns);
	
			parent[nparent++]=i;
			buffer[nson++]=i;
			for(j=0;j<=ns;j++)
			{
				nson=1;
				nparent=1;
				//printf("     ");
				//dprint(j);
				son=succ[i][j];
				buffer[nson++]=son;
				//if(!hasSon(son,i) || true)
				if(!hasSon(son) || true)
				{
					if(!isSurflous())
					{
						copyint(tabclique[ncliques],buffer,nson+1);
						ntabclique[ncliques]=nson+1;
						ncliques++;
					}
					//printf("tabclique[%d] : ",ncliques);getchar();
					//fprintf_intt(stdout,tabclique,ncliques,nson-1);
				}
			}
		}
		if(ncliques>1)
	    for(i=0;i<ncliques-1;i++)
		if(intint(tabclique[i],ntabclique[i],tabclique[ncliques-1],ntabclique[ncliques-1]))
		{
			ncliques--;
			break;
		}
	}
	else //nsyntotal==2
	{
		ncliques=1;
		ntabclique[0]=2;
		tabclique[0][0]=0;
		tabclique[0][1]=1;
	}
	

        //on ecrit le fichier .doc
        sprintf(filename,"%s.doc",outname);
        fdoc=fopen(filename,"wb");
	if(displayLang==LFRANCAIS)
        	fprintf(fdoc,"Votre requête était : ");
	else
        	fprintf(fdoc,"You made the following query: ");
        for (i=0;i<nvedettes;i++)
        {
                fprintf(fdoc,"<B>%s </B> ",vedette[i]);
        }
	if(displayLang==LFRANCAIS)
        	fprintf(fdoc,"<BR>\nIl y a %d termes et %d cliques\n<BR><BR><A NAME=\"syno\"><B><I>Termes : </I></B></A>",nsyntotal-nvedettes,ncliques);
	else
        	fprintf(fdoc,"<BR>\n%d related words and %d cliques were found.\n<BR><BR><A NAME=\"syno\"><B><I>Related words: </I></B></A>",nsyntotal-nvedettes,ncliques);
       
        sprintf(filename,"%s.iv",outname);
        for (i=0;i<nvedettes;i++)
        {
                fprintf(fdoc,"<BR><BR>\n<FONT SIZE=+1><I>%s</I></FONT>%s ",vedette[i],colon);
                premiersyn=1;
                for (j=0;j<nsyntotal;j++)
                {
                        if (!strcmp(syns[j],vedette[i]))
			{
				fiv=fopen(filename,"wb");
				fprintf(fiv,"%d",j);
				fclose(fiv);
                                break;
			}
                }
                for (k=0;k<j;k++)
                {
                        for (l=0;l<nsucc[k];l++)
                                if (succ[k][l]==j)
                                {
                                        if (premiersyn)
                                        {
                                                fprintf(fdoc,"%s",syns[k]);
                                                premiersyn=0;
                                        }
                                        else
                                        {
                                                fprintf(fdoc,", %s",syns[k]);
                                        }
                                }
                }
                for (k=0;k<nsucc[j];k++)
                        if (premiersyn)
                        {
                                fprintf(fdoc,"%s",syns[succ[j][k]]);
                                premiersyn=0;
                        }
                        else
                        {
                                fprintf(fdoc,", %s",syns[succ[j][k]]);
                        }
        }
        printtabclique();
	
	free_cmatrix(line,0,nvedettes-1,0,maxlinelen);
	free_cmatrix(linetr,0,nvedettes-1,0,maxlinelen);
	free_cmatrix(vedette,0,nvedettes-1,0,MAX_WORD_LEN);
	free_cmatrix(syns,0,Nsyntotal,0,MAX_WORD_LEN);
	free_ivector(nsucc,0,nsyntotal-1);
  	free_ivector(parent,0,taillemaxclique);
  	free_imatrix(tabclique,0,taillemaxclique,0,nsyntotal);
  	free_ivector(ntabclique,0,taillemaxclique);
	free_ivector(buffer, 0, nsyntotal-1);

        return 0;
}

