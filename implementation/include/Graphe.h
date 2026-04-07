#ifndef __GRAPHE_H__
#define __GRAPHE_H__

#include <stdlib.h>
#include <stdio.h>
#include "Chaine.h"
#include "Reseau.h"
#include "ListeInt.h"
#include "Struct_File.h"

typedef struct {
    int u, v ; /* Numeros des sommets extremite */
} Arete;

typedef struct cellule_arete {
    Arete * a ; /* pointeur sur l’arete */
    struct cellule_arete *suiv ;
} Cellule_arete;

typedef struct {
    int num ; /* Numero du sommet (le meme que dans T_som) */
    double x , y ;
    Cellule_arete *L_voisin ; /* Liste chainee des voisins */
} Sommet;

typedef struct {
    int e1 , e2 ; /* Les deux extremites de la commodite */
} Commod;

typedef struct {
    int nbsom ; /* Nombre de sommets */
    Sommet ** T_som ; /* Tableau de pointeurs sur sommets */
    int gamma ;
    int nbcommod ; /* Nombre de commoQdites */
    Commod *T_commod ; /* Tableau des commodites */
} Graphe;

Graphe* creerGraphe(Reseau* r); //Créé et alloue un graphe correspondant au reseau R
int distanceMinEntreDeuxSommets(Graphe *g, int u, int v); //Calcule distance min entre deux sommets
int plusPetitNbAretesChaine(Graphe *g, int u, int v); //Calcule le plus petit nombre d'aretes 
ListeInt* cheminEntreDeuxSommets(Graphe *g, int u, int v); //Calcule le chemin le plus cours entre deux sommets
void libererGraphe(Graphe *g); //Libère la mémoire allouée par un graphe
int reorganiseReseau(Reseau* r); /* Créé le graphe correspondant au reseau R, calcule la plus courte chaine 
pour chaque commodité et retourne VRAI si pour toute arete du graphe, le nombre de chaines qui passe par cette
arete est inférieur à gamma*/
# endif

