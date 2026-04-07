#ifndef LISTEINT_H
#define LISTEINT_H

typedef struct CelluleInt {
    int valeur;
    struct CelluleInt* suivant;
} CelluleInt;

typedef struct {
    CelluleInt* debut;
    int taille;
} ListeInt;

ListeInt* newListeInt(); //Créé une liste d'entiers
void detruireListeInt(ListeInt* liste); //Detruit une liste d'entiers
void ajouterElement(ListeInt* liste, int valeur); //Ajoute un element a la liste
void ajouterElementDebut(ListeInt* liste, int valeur); //Ajoute un element au debut
void supprimerElement(ListeInt* liste, int valeur); //Supprime un element
void afficherListeInt(ListeInt* liste); //Affiche la liste d'entiers
int* listeIntToArray(ListeInt* liste); //Retourne la liste d'entiers en tableau
void libererListeInt(ListeInt* liste); //Libère la memoire allouee par la liste

#endif

