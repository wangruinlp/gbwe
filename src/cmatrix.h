#include <stdlib.h>
#define NR_END 1
#define FREE_ARG char*

char ** cmatrix(int nrl, int nrh, int ncl, int nch);
/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */
