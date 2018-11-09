#include <stdio.h>


void initialisation(int n, FILE *freqFile);
double bellman(int i, int j);
void bstree(int i, int j);
void afficher_bst(int n);
void freeTabInt(int n, int **T);
void freeTabDouble(int n, double **T);
