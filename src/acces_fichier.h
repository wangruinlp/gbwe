/* acces_fichier.h */

#ifndef ACCES_FICHIER_H
#define ACCES_FICHIER_H

FILE * ouvre_fichier(const char *vedette, char *extension, char *mode);
void ecrit_fichier(const char *vedette, char *extension, char *mode,double **C,int ligne,int colonne);

#endif /* ACCES_FICHIER_H */
