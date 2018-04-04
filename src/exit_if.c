/*          exit_if.c     du 26 juin 98    */

#include <stdlib.h>
#include <stdio.h>
#include "exit_if.h"

char *exit_message = NULL;

void exit_if(char *fichier, int ligne, int condition,
	     char *code, char *message)
{
  if (condition == 0)
    return;
  if (message == NULL)
    fprintf(stderr,
	    "%s:%d:condition d'arret: < %s >\n",
	    fichier, ligne, code);
  else
    fprintf(stderr, "%s:%d: %s\n", fichier, ligne, message);

  exit_message = message;
  exit(EXIT_FAILURE);
}

void perror_and_exit1(char *localisation)
{
  perror(localisation);
  exit(EXIT_FAILURE);
}
