#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "add_accent.h"
void
getNewWord(char* t, char* t2, const char *s, const char *program_name)
{
	char str[256];
	char *p=t;
	
	while (*s && *s++!=':')
		;
	s++;
      	while (*s && *s !='\t')
		*p++ = *s++;
	*p='\0';
	//toHtml(t2,t,'\t');
	toHref(t2,t);
	if(*s!=0 && *(++s)!=0)
	{
		toHref(str,s);
		printf("<A HREF=\"%p?r=%s\"><font size=+2>%s</font></A>&nbsp;", program_name, str,s);
	}
}
/**
 *  making possible search words such as "garcon+velo+inferieur" by converting "gar�on+v�lo+inf�rieur"
 **/
void add_accent( FILE *fp, const char *msg, const char **V, int nv, const char *v, const char *bg, int nw, const char *dico_ext, const char *program_name )
{
/*
//	extern char program_name[];
//	extern char **V;
	extern char nv;
	extern char *vedettes;
	extern char bg_image[];
	//extern char ImageURL[];
*/
        char str[512]="",strhtml[512]="",strtmp[512],strhtmltmp[512];
        int i,j;
      	char line[256];
	int notfirst=false;
	int hasEntry[nv];
	char words[nv][nw];
	int n=0;
	int nvalid=0;
	char *p;
	int isFirstFind=true;

	for(i=0;i<nv;i++)
		hasEntry[i]=false;
	for(i=0;i<nv;i++)
	{
		if(fgetline(words[n++],nw,fp)<0)
			break;
	}
	for(i=0;i<nv;i++)
		for(j=0;j<n;j++)
			if(isEqual(V[i],words[j]))
				hasEntry[i]=true;//no entry word  e.g. garcon
		
	i=0;
	for (i=0;i<nv;i++)
	{
	   if(hasEntry[i]) //no entry word  e.g. garcon
	   {  // .sa
		if(notfirst)
		{
			strcat(str,"+");
			strcat(strhtml,"%2B");
		}
		if (getSynLine(V[i],line,dico_ext)<=0) //e.g. "sa"
		{
			notfirst=false;
			continue;
		}
		if(isFirstFind)
		{
			printf("%s",msg);
			isFirstFind=false;
		}

		getNewWord(strtmp,strhtmltmp,line,program_name);
		strcat(str,strtmp);
		strcat(strhtml,strhtmltmp);
		notfirst=true;
		nvalid++;
	   }//if
	   else
	   {
		if(notfirst)
		{
			strcat(str,"+");
			strcat(strhtml,"%2B");
		}
		strcat(str,V[i]);
		strcat(strhtml,V[i]);
		notfirst=true; 
		nvalid++;
	   }
	}//for
	//print_noentry();
	if(nvalid==0)
	{
		return;
	}
	p=str+strlen(str)-1;
	if(*p == '+')
		*p=0;
	p=strhtml+strlen(strhtml)-1;
	if(*p == '+')
		*p=0;

	printf("<A HREF=\"%s?r=%s\"><font size=+2>%s</font></A><BR><BR>", program_name, strhtml,str);
	//printf("%s <A HREF=\"%s?r=%s\"><font size=+2> %s </font></A><BR><BR>",msg, program_name, strhtml,str);
}
