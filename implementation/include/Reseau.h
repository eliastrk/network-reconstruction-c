#ifndef __RESEAU_H__
#define __RESEAU_H__

#include "Chaine.h"

typedef struct noeud Noeud;

/* Liste chainee de noeuds (pour la liste des noeuds du reseau ET les listes des voisins de chaque noeud) */
typedef struct cellnoeud {
    Noeud *nd;               /* Pointeur vers le noeud stock\'e */
    struct cellnoeud *suiv;         /* Cellule suivante dans la liste */
} CellNoeud;

/* Noeud du reseau */
struct noeud{
   int num;                        /* Numero du noeud */
   double x, y;                    /* Coordonnees du noeud*/
   CellNoeud *voisins;             /* Liste des voisins du noeud */
};

/* Liste chainee de commodites */
typedef struct cellCommodite {
    Noeud *extrA, *extrB;       /* Noeuds aux extremites de la commodite */
    struct cellCommodite *suiv;     /* Cellule suivante dans la liste */
} CellCommodite;

/* Un reseau */
typedef struct {
    int nbNoeuds;                   /* Nombre de noeuds du reseau */
    int gamma;                      /* Nombre maximal de fibres par cable */
    CellNoeud *noeuds;              /* Liste des noeuds du reseau */
    CellCommodite *commodites;      /* Liste des commodites a relier */
} Reseau;

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y); //Recherche un nœud dans le réseau à partir de ses coordonnées (x, y) 
Reseau *initialiserReseau(int gamma); //Initialise une structure de réseau avec un nombre maximal de fibres par câble (gamma) donné
int existeLiaison(Noeud* n1, Noeud* n2); //Vérifie s'il existe une liaison entre deux nœuds donnés
Reseau* reconstitueReseauListe(Chaines *C); //Reconstitue un réseau à partir d'une liste de chaînes données
int nbCommodites(Reseau *R); //Nombre commodités Reseau
int nbLiaisons(Reseau *R); //Nombre liaison Reseau
void libererReseau(Reseau *R); //Libère la mémoire allouée pour le réseau
void ecrireReseau(Reseau *R, FILE *f); //Ecris un Reseau
void afficheReseauSVG(Reseau *R, char* nomInstance); //Representation graphique d'un reseau (html)

#endif
