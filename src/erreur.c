/* erreur.c */

#include <stdio.h>
#include <stdlib.h>

#include "erreur.h"


/******************************************************************/
/*         fonction de gestion des erreurs                        */
/******************************************************************/

void perror_and_exit(char *location)
{
  perror(location);
  exit(EXIT_FAILURE);
}
