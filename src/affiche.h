/* affiche.h */

#ifndef AFFICHE_H
#define AFFICHE_H

void affiche_temps(struct timeval *temps1);
void affiche_matrice(double **matrice,int ligne,int colonne);
void affiche_marge(double marge[],int colonne);
void affiche_vecteur(double vecteur[],int ligne,int colonne,int position);
void affiche_fermeture(int sommet,int GAMA[],int colonne);

#endif /* AFFICHE_H */
