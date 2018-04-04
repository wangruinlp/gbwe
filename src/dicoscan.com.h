#define MaxLine 10000

#define	 MAX_word_len 255
#define	 MAX_line_len 1023

int getSynLine(char* word,char* line,char *base); //copie dans la chaine 'line', la ligne du dictionnaire associée au mot 'word'
// retourne un nombre positif en cas de succes, negatif en cas d'echec
