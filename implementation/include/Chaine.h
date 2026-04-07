#ifndef __CHAINE_H__
#define __CHAINE_H__	

#include<stdio.h>
#include<stdlib.h>

/* Liste chainee de points */
typedef struct cellPoint{
  double x,y;                   /* Coordonnees du point */
  struct cellPoint *suiv;       /* Cellule suivante dans la liste */
} CellPoint;

/* Celllule d une liste (chainee) de chaines */
typedef struct cellChaine{
  int numero;                   /* Numero de la chaine */
  CellPoint *points;            /* Liste des points de la chaine */
  struct cellChaine *suiv;      /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
  int gamma;                    /* Nombre maximal de fibres par cable */
  int nbChaines;                /* Nombre de chaines */
  CellChaine *chaines;          /* La liste chainee des chaines */
} Chaines;

Chaines* lectureChaines(FILE *f); //Lire les données à partir d'un fichier et les stocker dans une structure Chaines
int comptePointsDansChaine(CellChaine *chaine); //Compter le nombre de point dans une chaine
void ecrireChaines(Chaines *C, FILE *f); //Ecrire les données d'une structure Chaines dans un fichier
void afficheChainesSVG(Chaines *C, char* nomInstance); //Representation graphique d'une chaine
double distance(double x1, double y1, double x2, double y2); //Distance euclidienne entre deux points
double longueurChaine(CellChaine *c); // Prototype de la fonction longueurChaine
double longueurTotale(Chaines *C); //Calculer la longueur totale de toutes les chaînes dans l'ensemble
int comptePointsTotal(Chaines *C); //Compter le nombre total de points dans toutes les chaînes
void libererChaine(Chaines *C); //Liberer la memoire d'une chaine
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax); //Generer une chaine aleatoirement (ex6)

#endif

