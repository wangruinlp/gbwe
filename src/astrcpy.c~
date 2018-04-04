/**
 * xstrcqy(to,"gar%E7on");
 * == strcqy(to,"garcon");
 * xstrcqy(to,"gar%E7on%2Bfille");
 * == strcqy(to,"garçon+fille");
 * xstrcqy(to,"+gar%E7on+%2B++%2B+++fille%2B%2B+++%2B++");
 * == strcqy(to,"garçon+fille");
 *
 * CAUTION : this does not examine whether the length of "t" is too short or not. 
 * 
 * */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int astrcpy(char * t, const char *s)
{
	register char *q=t;
	register const char *p=s;
	char str[6];
        int c=0;
	*q++ = '_';
	while(*p)
	{
		c = *p;
		if(*p=='%')
		{
                	//conversion de %xx en le caractère corresqondant
			p++;
                	str[0]= *p++;str[1]= *p; str[2]=0;
                	sscanf(str,"%x",&c);
		}
		if((isalpha(c)))
			*q++ = c;
		else
		{
			switch(c)
			{
				case 'ç':
					c='c';
					break;
				case 'à':
				case 'â':
					c='a';
					break;
				case 'é':
				case 'è':
				case 'ê':
				case 'ë':
					c='e';
					break;
				case 'ï':
				case 'î':
					c='i';
					break;
				case 'ô':
				case 'ö':
					c='o';
					break;
				case 'ù':
				case 'û':
				case 'ü':
					c='u';
					break;
				default:
					c='_';
					break;
			}
			*q++ = c;
		}
		p++;

	}
	*q = '\0';
	return 1;
}
