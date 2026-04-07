#include <stdio.h>
#include <stdlib.h>

#include "ListeInt.h"


ListeInt* newListeInt() {
    ListeInt* liste = (ListeInt*)malloc(sizeof(ListeInt));
    if (liste != NULL) {
        liste->debut = NULL;
        liste->taille = 0; // Initialiser la taille ?? z??ro
    }
    return liste;
}

void detruireListeInt(ListeInt* liste) {
    CelluleInt* courant = liste->debut;
    while (courant != NULL) {
        CelluleInt* temp = courant;
        courant = courant->suivant;
        free(temp);
    }
    free(liste);
}

void ajouterElement(ListeInt* liste, int valeur) {
    if (liste != NULL) {
        CelluleInt* nouvelElement = (CelluleInt*)malloc(sizeof(CelluleInt));
        if (nouvelElement != NULL) {
            nouvelElement->valeur = valeur;
            nouvelElement->suivant = liste->debut;
            liste->debut = nouvelElement;
            liste->taille++; // Incr??menter la taille apr??s l'ajout d'un ??l??ment
        }
    }
}

void ajouterElementDebut(ListeInt* liste, int valeur) {
    CelluleInt* nouvelleCellule = (CelluleInt*)malloc(sizeof(CelluleInt));
    nouvelleCellule->valeur = valeur;
    nouvelleCellule->suivant = liste->debut;
    liste->debut = nouvelleCellule;
    liste->taille++;
}

void supprimerElement(ListeInt* liste, int valeur) {
    CelluleInt* courant = liste->debut;
    CelluleInt* precedent = NULL;
    while (courant != NULL && courant->valeur != valeur) {
        precedent = courant;
        courant = courant->suivant;
    }
    if (courant == NULL) {
        return;
    }
    if (precedent == NULL) {
        liste->debut = courant->suivant;
    } else {
        precedent->suivant = courant->suivant;
    }
    free(courant);
    liste->taille--;
}

void afficherListeInt(ListeInt* liste) {
    CelluleInt* courant = liste->debut;
    while (courant != NULL) {
        printf("%d ", courant->valeur);
        courant = courant->suivant;
    }
    printf("\n");
}

int* listeIntToArray(ListeInt* liste) {
    if (liste == NULL || liste->debut == NULL) {
        return NULL; // Si la liste est vide ou non initialis??e, retourner NULL
    }

    // Compter le nombre d'??l??ments dans la liste
    int taille = 0;
    CelluleInt* current = liste->debut;
    while (current != NULL) {
        taille++;
        current = current->suivant;
    }

    // Allouer de la m??moire pour le tableau
    int* tableau = (int*)malloc(taille * sizeof(int));
    if (tableau == NULL) {
        return NULL; // ??chec de l'allocation m??moire
    }

    // Remplir le tableau avec les valeurs de la liste
    current = liste->debut;
    int index = 0;
    while (current != NULL) {
        tableau[index++] = current->valeur;
        current = current->suivant;
    }

    return tableau;
}

void libererListeInt(ListeInt* liste) {
    CelluleInt* courant = liste->debut;
    while (courant != NULL) {
        CelluleInt* temp = courant;
        courant = courant->suivant;
        free(temp);
    }
    free(liste);
}
