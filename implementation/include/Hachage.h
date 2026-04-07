#ifndef __HACHAGE_H__
#define __HACHAGE_H__

#include "Chaine.h"
#include "Reseau.h"

//Question 4.1

typedef struct{
  int nbElement;
  int tailleMax;
  CellNoeud** T;
}TableHachage;

int f(double x, double y); /*Cette fonction calcule la valeur de hachage à partir des coordonnées (x, y) d'un point. Elle combine les coordonnées x 
et y selon une formule pour générer une clé unique pour chaque point. Cette clé sera utilisée pour indexer les nœuds dans la table de hachage.*/

int h(int k, int M); /* La fonction de hachage h prend en entrée la clé calculée à partir des coordonnées d'un point et le nombre de cases de la table
 de hachage (M). Elle applique une fonction de compression sur la clé pour déterminer l'index où le point sera stocké dans la table de hachage.*/

Noeud *rechercheCreeNoeudHachage(Reseau* R, TableHachage*H, double x, double y); /*Cette fonction recherche un nœud dans la table de hachage 
correspondant aux coordonnées (x, y) d'un point. Si le nœud existe déjà, il est retourné.*/

TableHachage *initialiserTableHachage(int M); // Initialise une nouvelle table de hachage avec une taille spécifiée (M) et alloue sa mémoire.
void libererTableHachage(TableHachage *H); //Libère la mémoire allouée par la table de hachage
Reseau* reconstitueReseauHachage(Chaines *C, int M); //Reconstitue un réseau à partir d'une liste de chaînes données et une table de hachage de taille M.

#endif

