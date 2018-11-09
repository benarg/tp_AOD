#include "fonctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>




// Declaration des differents tableaux
double *proba;  // tableau des probabilites de chaque elements de l'arbre
int **bst;   // tableau permettant qui represente l'arbre optimal
int **racines; // tableau des racines des arbres optimals pour chaques sous arbres
double **sommeProba; // tableau des sommes de probabilites pour une sous partie des elements
double **nbrComparaisons; // tableau regroupant le nombre de comparasons moyens pour acceder a un element pour chaque sous arbres

/**
 * Fonction qui initiliase les différents tableaux
*/
void initialisation(int n, FILE *freqFile) {

    int i,j;

    // allocation des tableaux
    proba = (double *) malloc(n * sizeof(double));
    bst = (int **) malloc(n * sizeof(int *));
    racines = (int **) malloc(n * sizeof(int *));
    sommeProba = (double **) malloc(n * sizeof(double *));
    nbrComparaisons = (double **) malloc(n * sizeof(double *));

    for (i = 0; i < n; i++) {
        bst[i] = (int *) malloc(2 * sizeof(int));
        racines[i] = (int *) malloc(n * sizeof(int));
        sommeProba[i] = (double *) malloc(n * sizeof(double));
        nbrComparaisons[i] = (double *) malloc(n * sizeof(double));
    }

    // initialisation des probabilites
    int freq;
    int sommeFreq = 0;
    for (i = 0; i < n; i++) {
      fscanf(freqFile, "%d", &freq);
      proba[i] = freq;
      sommeFreq += freq;
      }
    for(i = 0; i < n; i++) {
      proba[i] = proba[i] / sommeFreq;
    }

    for (i = 0; i < n; i++) {
      bst[i][0] = -1;
      bst[i][1] = -1;
      for (j = 0; j < n; j++) {
        racines[i][j] = -1;
        sommeProba[i][j] = 0;
        nbrComparaisons[i][j] = 0;
      }
    }
}

  /**
   * Fonction qui calcule le nombre de comparaisons moyen pour acceder un element compris entre ei et ej du sous arbre correspondant aux elements [ei, ej]
   * Retourne le resultat de l'equation de bellman (voire question 1)
   */
  double bellman(int i, int j) {

    if (j < i)
      return 0;
    else if (j == i) {
      racines[i][j] = i;
      nbrComparaisons[i][j] = proba[i];
      return proba[i];
    }
    else if (nbrComparaisons[i][j] != 0)
      return nbrComparaisons[i][j];
    else {
      double min = DBL_MAX;
      double somme = 0;
      int racine, k;
      if (sommeProba[i][j] !=0)
        somme = sommeProba[i][j];
      else {
        for (k = i; k <= j; k++)
          somme += proba[k];
        sommeProba[i][j] = somme;
      }
      double comp;
      for (k = i; k <= j; k++) {
        comp = bellman(i, k-1) + bellman(k+1, j) + somme;
        if (comp < min) {
          min = comp;
          racine = k;
        }
      }
      nbrComparaisons[i][j] = min;
      racines[i][j] = racine;
      return min;
    }
  }

  /**
   * Fonction qui permet de construire l'arbre optimal en fonctions des couts des differents sous arbres
   */
  void bstree(int i, int j) {

    if(j < i)
      return;

    int racine = racines[i][j];
    if (i <= racine - 1) {
      bst[racine][0] = racines[i][racine - 1];
      bstree(i, racine - 1);
    }

    if (j >= racine + 1) {
      bst[racine][1] = racines[racine + 1][j];
      bstree(racine + 1, j);
    }
  }

  /**
   * Fonction qui permet d'afficher a l'ecran l'arbre optimal contenant les n elements
   */
  void afficher_bst(int n) {
    int i;
    printf("static int BSTroot = %d;\n", racines[0][n-1]);
    printf("static int BSTtree[%d][2] = {", n);
    for (i = 0; i < n; i++) {
        printf(" {%d, %d}", bst[i][0], bst[i][1]);
    }
    printf(" };\n");
    freeTabInt(n, bst);
    freeTabInt(n,racines);
    freeTabDouble(n, nbrComparaisons);
    freeTabDouble(n, sommeProba);
    free(proba);
  }


  void freeTabInt(int n, int **T){
    int i;
    for(i=0;i<n;i++){
      free(T[i]);
      }
    free(T);
  }

  void freeTabDouble(int n, double **T){
    int i;
    for(i=0;i<n;i++){
      free(T[i]);
      }
    free(T);
  }
