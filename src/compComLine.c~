#define MAX_LINE_LEN_COM 840
#define MAX_WORD_LEN_COM 56
int compComLine(char *line, char *mot, char *com)
{
	int i,c,pos,lenM;

	lenM=strlen(mot);
	i=0;
	com[0]=0;
//printf("<BR>");
	while (line[i]!=':') i++;
	do
	{
		do
		{
			c=line[i];
			i++;
			if (c=='\n') return 0;
			if (c=='\t') break;
		} while (i<MAX_LINE_LEN_COM);
//printf("*%s* *%s*<BR>\n",line+i,mot);
		if (!strncmp(line+i,mot,lenM))
		{
//printf("COMP OK *%s* *%s* %d<BR>\n",line+i,mot,lenM);
			pos=0;
			while (line[i]!='(' && i<MAX_LINE_LEN_COM)
				if (line[i]=='\t') break;
				else i++;
			com[pos]='(';
			while (line[i]!='(') i++;
			while (line[i]!=')' && i<MAX_LINE_LEN_COM)
			{
				if (pos>MAX_WORD_LEN_COM-2)
					break;
				com[pos]=line[i];
				pos++;
				i++;
			}
			com[pos]=')';
			com[pos+1]=0;
			return 1;
		}
	} while (i<MAX_LINE_LEN_COM);
	return 0;
}
