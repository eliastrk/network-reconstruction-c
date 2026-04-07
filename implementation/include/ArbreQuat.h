#ifndef __ARBRE_QUAT_H__
#define __ARBRE_QUAT_H__

#include "Chaine.h"
#include "Reseau.h"

/* Arbre quaternaire contenant les noeuds du reseau */
typedef struct arbreQuat{
    double xc, yc;          /* Coordonnees du centre de la cellule */	
    double coteX;           /* Longueur de la cellule */
    double coteY;           /* Hauteur de la cellule */
    Noeud* noeud;           /* Pointeur vers le noeud du reseau */
    struct arbreQuat *so;   /* Sous-arbre sud-ouest, pour x < xc et y < yc */
    struct arbreQuat *se;   /* Sous-arbre sud-est, pour x >= xc et y < yc */
    struct arbreQuat *no;   /* Sous-arbre nord-ouest, pour x < xc et y >= yc */
    struct arbreQuat *ne;   /* Sous-arbre nord-est, pour x >= xc et y >= yc */
} ArbreQuat;

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax); /*Détermine les coordonnées minimales et maximales des points 
constituant les différentes chaînes du réseau*/
ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY); /*Crée une cellule de l'arbre quaternaire avec le centre, la longueur et la
hauteur spécifiés*/
void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent); /*Insère un nœud du réseau dans l'arbre quaternaire. Elle prend en compte trois cas : 
arbre vide, feuille et cellule interne*/
Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y); /*recherche ou crée un nœud du réseau correspondant aux 
coordonnées spécifiées dans l'arbre quaternaire*/
void libererArbreQuat(ArbreQuat **a); //Libere la memoire allouée par un Arbre
Reseau* reconstitueReseauArbre(Chaines* C); /*Reconstruit le réseau à partir des chaînes en utilisant l'arbre quaternaire. Elle parcourt chaque point, 
recherche ou crée le nœud correspondant, puis établit les liaisons entre les nœuds*/


#endif

