CC = gcc
current_dir = $(shell pwd)
all:
	echo $(current_dir)
	$(CC) -w -Wall -o $(current_dir)/index index.c
	g++ -w -Wall -o $(current_dir)/sym sym.cpp
	$(CC) -w -Wall -o $(current_dir)/clique $(current_dir)/clique.c $(current_dir)/dicoscan.c $(current_dir)/countTokens.c $(current_dir)/cmatrix.c $(current_dir)/free_cmatrix.c $(current_dir)/toWords.c $(current_dir)/wsort.c $(current_dir)/indexx.c $(current_dir)/ivector.c $(current_dir)/free_ivector.c $(current_dir)/toSyns.c $(current_dir)/cvector.c $(current_dir)/free_cvector.c $(current_dir)/intint.c $(current_dir)/copyint.c $(current_dir)/imatrix.c $(current_dir)/free_imatrix.c
	$(CC) -w -Wall -lm -o $(current_dir)/ca  $(current_dir)/cliq_poly.c $(current_dir)/acces_fichier.c $(current_dir)/vecteur.c $(current_dir)/lecture_fichier.c $(current_dir)/nrutils.c $(current_dir)/cree_matrice_01.c $(current_dir)/minimum.c $(current_dir)/calcul_marge.c $(current_dir)/matrice_khi2.c $(current_dir)/produit_matrice.c $(current_dir)/tred2.c $(current_dir)/tqli.c $(current_dir)/eigsrt.c $(current_dir)/pilegen.c $(current_dir)/tri_rapide.c $(current_dir)/cherche_doublons.c $(current_dir)/cherche_hb.c $(current_dir)/enveloppe_gauche.c $(current_dir)/transvase.c $(current_dir)/enveloppe_droite.c $(current_dir)/min.c $(current_dir)/exit_if.c $(current_dir)/erreurs.c $(current_dir)/remplir.c $(current_dir)/produit_croise.c
	echo "finished"
 
