#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	int* element;
	int nelements;
}cClique,cFermTrans;

typedef struct
{
	char name[200];
}cSynonyme;

int main(int argc,char** argv) //syntaxe : xx.exe nom_fichier ft1 [ft2 ...] avec fti=numero de la ième fermeture à eliminer. fti<ftj <=> i<j
{
	#define PATH ""
	char FileName[256];
	cClique* tabClique;
	int nCliques=0;
	int ncli=0;
	cFermTrans* tabFermTrans;
	int* tabNumFermTrans;
	int nFermTrans=0;
	int tailleCliqueMax=0;
	int tmptailleclique=0;
	cSynonyme* tabSynonyme;
	int nSynonymes=0;
	char c;
	int i,j,k,num;
	int numft;
	int* tabNewISyn1,* tabNewISyn2,* tmptabNewISyn;
	char* pd;
	cClique* pcli;
	cFermTrans* pft;
	FILE* fi_ft,* fi_cli,* fi_syn,* fo_cli,* fo_syn;

	//ouverture des differents fichiers
	sprintf(FileName,"%s%s.fer",PATH,argv[1]);
	fi_ft=fopen(FileName,"rb");
	sprintf(FileName,"%s%s.clio",PATH,argv[1]);
	fi_cli=fopen(FileName,"rb");
	sprintf(FileName,"%s%s.syno",PATH,argv[1]);
	fi_syn=fopen(FileName,"rb");
	sprintf(FileName,"%s%s.cli",PATH,argv[1]);
	fo_cli=fopen(FileName,"wb");
	sprintf(FileName,"%s%s.syn",PATH,argv[1]);
	fo_syn=fopen(FileName,"wb");

	if (fi_ft==NULL)
		return -2;
	if (fi_cli==NULL)
		return -3;
	if (fi_syn==NULL)
		return -4;

	//lecture de tabClique
	do
	{
		c=fgetc(fi_cli);
		if (c=='\n')
		{
			nCliques++;
			if (tmptailleclique>tailleCliqueMax)
				tailleCliqueMax=tmptailleclique;
			tmptailleclique=0;
		}
		if (c==',')
			tmptailleclique++;
	}while (c!=(char)-1);

	tabClique=(cClique*)malloc(sizeof(cClique)*nCliques);
	fseek(fi_cli,0,0);
	for (i=0;i<nCliques;i++)
	{
		pcli=&tabClique[i];
		pcli->element=(int*)malloc(sizeof(int)*tailleCliqueMax);
		pcli->nelements=0;
		num=0;
		do
		{
			c=fgetc(fi_cli);
			if (c==',')
			{
				pcli->element[pcli->nelements]=num-1;
				if (num>nSynonymes)
					nSynonymes=num;
				num=0;
				pcli->nelements++;
			}
			if ((c>='0') && (c<='9'))
			{
				num=num*10+c-'0';
			}
		}while (c!='\n');
	}


	//lecture de tabSynonyme
	tabSynonyme=(cSynonyme*)malloc(sizeof(cSynonyme)*nSynonymes);
	for (i=0;i<nSynonymes;i++)
	{
		pd=tabSynonyme[i].name;
		do
		{
			(*pd)=c=fgetc(fi_syn);
			pd++;
		}while ((c!=13)&&(c!=10));
		if (c==13)
			fgetc(fi_syn);

		pd--;
		(*pd)=0;
	}

	//on determiner les fermetures transitives à eliminer
	nFermTrans=0;
	while ((c=fgetc(fi_ft))!=-1){if (c=='\n') nFermTrans++;} //saut des lignes innutiles
	nFermTrans-=argc-2;
	tabFermTrans=(cFermTrans*)malloc(sizeof(cFermTrans)*nFermTrans);
	tabNumFermTrans=(int*)malloc(sizeof(int)*nFermTrans);
	if (nFermTrans>0)
	{
		j=0; k=2;
		num=atoi(argv[k]);
		for (i=0;i<nFermTrans;i++)
		{
			while (j==num)
			{
				k++;
				if (k<argc)
					num=atoi(argv[k]);
				j++;
			}
			tabNumFermTrans[i]=j;
			j++;
		}
	}
	//lecture des fermetures transitives à éliminer
	numft=-1;
	fseek(fi_ft,0,0);

	for (i=0;i<nFermTrans;i++)
	{
		for (j=numft;j<tabNumFermTrans[i]-1;j++) while (fgetc(fi_ft)!='\n'); //saut des lignes innutiles

		//lecture d'une ligne utile
		pft=&tabFermTrans[i];
		pft->element=(int*)malloc(sizeof(int)*nSynonymes);
		pft->nelements=0;
		num=0;
		do
		{
			c=fgetc(fi_ft);
			if (c==' ')
			{
				pft->element[pft->nelements]=num;
				num=0;
				pft->nelements++;
			}
			if ((c>='0') && (c<='9'))
			{
				num=num*10+c-'0';
			}
		}while (c!='\n');

		numft=tabNumFermTrans[i];
	}

	tabNewISyn1=(int*)malloc(sizeof(int)*nSynonymes);
	tabNewISyn2=(int*)malloc(sizeof(int)*nSynonymes);
	for (i=0;i<nSynonymes;i++)
	{
		tabNewISyn1[i]=i;
	}

	for (i=0;i<nFermTrans;i++)
	{
		num=0;
		pft=&tabFermTrans[i];
		k=0;
		for (j=0;j<pft->nelements;j++)
		{
			for (;k<pft->element[j];k++)
			{
				if (tabNewISyn1[k]!=-1)
				{
					tabNewISyn2[k]=num;
					num++;
				}
				else
					tabNewISyn2[k]=-1;
			}
			tabNewISyn2[k]=-1;
			k++;
		}
		for (;k<nSynonymes;k++)
		{
			if (tabNewISyn1[k]!=-1)
			{
				tabNewISyn2[k]=num;
				num++;
			}
			else
				tabNewISyn2[k]=-1;
		}

		tmptabNewISyn=tabNewISyn1;
		tabNewISyn1=tabNewISyn2;
		tabNewISyn2=tmptabNewISyn;
	}
	//le resultat final est dans tabNewISyn1.

	//ecriture des cliques modifiés restantes
	for (i=0;i<nCliques;i++)
	{
		pcli=&tabClique[i];
		for (j=0;j<pcli->nelements;j++)
		{
			pcli->element[j]=tabNewISyn1[pcli->element[j]];
			if (pcli->element[j]<0) break; //1 element de la clique doit etre retiré => sortie prematurée
		}
		//sortie normale => tous les elements sont conservés => on garde la clique
		if (j==pcli->nelements)
		{
			for (j=0;j<pcli->nelements;j++)
			{
				fprintf(fo_cli,"%d,",pcli->element[j]+1);
			}
			fprintf(fo_cli,"\n");
      ncli++;
		}
	}

	//ecriture des synonymes restants
	for (i=0;i<nSynonymes;i++)
	{
		if (tabNewISyn1[i]>=0)
		{
			fprintf(fo_syn,"%s\n",tabSynonyme[i].name);
		}
	}


	fclose(fi_ft);
	fclose(fi_syn);
	fclose(fi_cli);
	fclose(fo_syn);
	fclose(fo_cli);

	return 0;
};

