#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/time.h>
#include <time.h>
#include "../commun/dicofile.h"
#include "../commun/dicoaccess.h"
#include "../commun/utils.h"
#include "../commun/URL.h"


/* DEBUG */
//#define ICI printf("%d\n",__LINE__);


/* GLOBALES */
int total_req=0;
int total_req_serveur=0;
int id_status=ID_ok;
int req_status=REQ_ok;
char vedettes[MAX_query_len+1];
char Erm[512];  // message d'erreur pour page HTML
char *EMPTY="***";

int
isCracker(const char *ip)
{
	char str[256];
	FILE *fp;
        sprintf(str,"%scracker_list",Controldir);
	fp=fopen(str,"r");
	while(fgetline(str,256,fp)>=0)
	{
		if(isEqual(ip,str))
		{
			fclose(fp);
			return true;
		}
	}
	fclose(fp);
	return false;
}

int vcheckid(const char *controldir)
{
        FILE *f,*f_remote;   //fichier temporaire
        char str[512];  //chaîne temporaire
        char* remote_id;
        char controle[MAX_ip_len+1];
        int i, len, cumul, first;
        int remote_num;
// special
int i2,cumul2;
char controle2[MAX_query_len+1];
int special;
// special

        special=0;
        total_req=0;
        total_req_serveur=0;
        remote_id=getenv("REMOTE_ADDR");
        if (remote_id==NULL)
        {
            strcpy(Erm,"RA001");
            return ID_ko_misc;
        }
	if(isCracker(remote_id))
		return ID_CRACKER;
        len=strlen(remote_id);
        if (len>MAX_ip_len) len=MAX_ip_len;   // soyons parano
        first = 0;
        for (i=0; i<len; i++)
        {
            if (remote_id[i]=='.') first++;
            if (first==3) break;
        }
        strncpy(controle, remote_id, len);
        controle[i] = 0;
// special
first = 0;
for (i2=0; i2<len; i2++)
{
   if (remote_id[i2]=='.') first++;
   if (first==2) break;
}
strncpy(controle2, remote_id, len);
controle2[i2] = 0;
if (    !strcmp(controle2, "213.131") ||
         !strcmp(controle2, "195.67") ||
         !strcmp(controle2, "195.42") ||
         !strcmp(controle2, "195.198") )
  special=1;
// special
        sprintf(str,"%strusted",Controldir);
        if ((f=dicofopen(str,"r+b"))==NULL)
        {
            strcpy(Erm,"T001");
            return ID_ko_misc;
        }

        while (fscanf(f,"%s\n",str)!=EOF)
        {
                if (!strcmp(str,remote_id))
                {
                        dicofclose(f);
                        return ID_trusted;
                }
        }
        dicofclose(f);

        sprintf(str,"%sremote",Controldir);
        if ((f_remote=dicofopen(str,"r+"))==NULL)
        {
            strcpy(Erm,"T002");
            return ID_ko_misc;
        }

        cumul = 0;
        cumul2 = 0;
        while (fscanf(f_remote,"%s",str)!=EOF)
        {
                fscanf(f_remote,"%d",&remote_num);
                if (!strcmp(str,remote_id))
                        total_req=remote_num;
                fscanf(f_remote,"%d",&total_req_serveur);
                if (strncmp(str, controle, i) == NULL)
                        cumul++;
// special
if (    special &&
         ( !strncmp(str, "213.131", i2) ||
           !strncmp(str, "195.67", i2) ||
           !strncmp(str, "195.42", i2) ||
           !strncmp(str, "195.198", i2) ) )
  cumul2++;
// special
        }
        dicofclose(f_remote);

        if (total_req>=MAX_NUM_REQ)
                return ID_ko_num_req;
        if (total_req_serveur>=MAX_NUM_REQ_SERVEUR)
                return ID_ko_num_req_serveur;
        if (cumul>=MAX_NUM_REQ_DOMAINE)
                return ID_ko_num_req_domaine;
if (cumul2>=MAX_NUM_REQ)
  return ID_ko_special;
        return ID_ok;
}

void get_fcompt_name(char *controldir, char *outname)
{
        FILE* fcomp;
        int filenum;
        int c;
        int i;
        time_t tim;
        struct tm *ttim;
        char str[512];

        sprintf(str,"%scompteur",controldir);
        while ((fcomp=dicofopen(str,"r+b"))==NULL);
        filenum=0;
        fseek(fcomp,0,0);
        for (i=0;i<8;i++)
        {
                c=fgetc(fcomp);
                if (c>'9') c=c-'A'+10; else c=c-'0'; //transformation d'un caractère hexa en nombre décimal
                filenum=filenum*16+c;
        }
        filenum+=1;

        tim=time(NULL);
        ttim=localtime(&tim);
        sprintf(outname,"%2.2d%2.2d%2.2d%2.2d_%08X",((*ttim).tm_year % 100),((*ttim).tm_mon+1),(*ttim).tm_mday,(*ttim).tm_hour,filenum);
        fseek(fcomp,0,0);
        fprintf(fcomp,"%08X",filenum);
        dicofclose(fcomp);
}

void write_log(const char *controldir, char *query)
{
       char *str,*referrer;
       char str2[512];
       FILE *f_log,*f_remote;
       time_t tim;

       sprintf(str2,"%sremote",controldir);
       if ((f_remote=dicofopen(str2,"a+"))!=NULL)
       {
               str=getenv("REMOTE_ADDR");
               if (id_status != ID_trusted)
               {
                       if (str==NULL) str=EMPTY;
                       fprintf(f_remote,"%s\t",str);
                       fprintf(f_remote,"%d\t%d\n",total_req,total_req_serveur);
               }
               dicofclose(f_remote);
        }
       sprintf(str2,"%slog",controldir);
       if ((f_log=dicofopen(str2,"a+"))!=NULL)
       {
              tim=time(NULL);
              strftime(str2, sizeof(str2), "%a %x %X",localtime( &tim));
              fprintf(f_log,"%s \t", str2);
              fprintf(f_log,"(%s) \t", str);
	      referrer=getenv("HTTP_REFERER");
              if (referrer && strlen(referrer)>7)   // supprime http://
              {
                  char *ps,*pd;

                  pd=str2;
                  ps=referrer+7;
                  while (ps[0]!=0)
                  {
                      *pd=*ps;
                      if (*pd=='+')
                        *pd=' ';
                      if (*pd=='%')
                      {
                          int i;
                          char tmpstr[6];
                          //conversion de %xx en le caractère correspondant
                          tmpstr[0]=ps[1]; tmpstr[1]=ps[2]; tmpstr[2]=0;
                          sscanf(tmpstr,"%x",&i);
                          *pd=(char)i;
                          ps+=2;
                      }
                      pd++;
                      ps++;
                 }
                 *pd=0;
              }
              else
                  strcpy(str2,EMPTY);
              str=getenv("REMOTE_HOST");
              if (str==NULL) str=EMPTY;
              fprintf(f_log,"%s \t%s \t%s \t%d.%d.%d\n",str,str2,query,id_status,req_status,total_req);
              dicofclose(f_log);
       }
}




