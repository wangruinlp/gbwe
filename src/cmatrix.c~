#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#define NR_END 1
#define FREE_ARG char*

char **
cmatrix(int nrl, int nrh, int ncl, int nch)
/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */
{
        int i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
         char **m;

        /* allocate pointers to rows */
        m=( char **) malloc((size_t)((nrow+NR_END)*sizeof( char*)));
        if (!m) exit(1);//error("allocation failure 1 in cmatrix()");
	/*
        //m = ( char **)(m+NR_END);
        //m = ( char **)(m-nrl);
	 */
        m += NR_END;
        m -= nrl;


        /* allocate rows and set pointers to them */
        m[nrl]=( char *) malloc((size_t)((nrow*ncol+NR_END)*sizeof( char)));
        if (!m[nrl]) exit(1);//error("allocation failure 2 in cmatrix()");
	/*
        //m[nrl] = ( char *)(m+NR_END);
        //m[nrl] = ( char *)(m+ncl);
	*/
        m[nrl] += NR_END;
        m[nrl] -= ncl;

        for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
	/*
        //for(i=nrl+1;i<=nrh;i++) m[i]=( char *)(m[i-1]+ncol);
	*/

        /* return pointer to array of pointers to rows */
        return m;
}

