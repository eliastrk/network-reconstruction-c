#include <stdio.h>
#include <stdlib.h>

#include "Graphe.h"
#include "Reseau.h"
#include "Struct_File.h" 
#include "ListeInt.h"
#include "SVGwriter.h"


Graphe* creerGraphe(Reseau* r){
    Graphe *g = (Graphe*)malloc(sizeof(Graphe));
    if (g == NULL){
        printf("Erreur d'allocation memoire 1\n");
        return NULL;
    }

    g->nbsom = r->nbNoeuds;
    g->gamma = r->gamma;
    g->nbcommod = nbCommodites(r);

    g->T_commod = (Commod*)malloc(g->nbcommod*sizeof(Commod));
    CellCommodite *com = r->commodites;

    for(int i=0; i<g->nbcommod; i++){
        Commod com1 = {com->extrA->num, com->extrB->num};
        g->T_commod[i] = com1;
        com = com->suiv;
    }

    g->T_som = (Sommet**) malloc(g->nbsom * sizeof(Sommet*));
    for(int i=0; i<g->nbsom; i++){
        g->T_som[i] = NULL;
    }

    CellNoeud* cn = r->noeuds;
    while(cn){
        Sommet* s = (Sommet*)malloc(sizeof(Sommet));
        if (s == NULL){
            printf("Erreur d'allocation memoire 2\n");
            return NULL;
        }

        s->x = cn->nd->x;
        s->y = cn->nd->y;
        s->num = cn->nd->num;
        s->L_voisin = NULL; 


        g->T_som[s->num - 1] = s;
        cn = cn->suiv;
    }

    CellNoeud *cn1 = r->noeuds;
    CellNoeud *cn2;

    while (cn1){
        cn2 = cn1->nd->voisins;
        while (cn2){
            if (cn1->nd->num < cn2->nd->num){
                Arete *a1 = (Arete*)malloc(sizeof(Arete));
                if (a1 == NULL){
                    printf("Erreur d'allocation memoire 3\n");
                    return NULL;
                }
                a1->u = cn1->nd->num; 
                a1->v = cn2->nd->num;

                Arete *a2 = (Arete*)malloc(sizeof(Arete));
                if (a2 == NULL){
                    printf("Erreur d'allocation memoire 4\n");
                    return NULL;
                }
                a2->u = cn2->nd->num;
                a2->v = cn1->nd->num;

                Cellule_arete *ca1 = (Cellule_arete*)malloc(sizeof(Cellule_arete));
                if (ca1 == NULL){
                    printf("Erreur d'allocation memoire 5\n");
                    return NULL;
                }
                ca1->a = a1;
                ca1->suiv = g->T_som[cn1->nd->num-1]->L_voisin;
                g->T_som[cn1->nd->num-1]->L_voisin = ca1;

                Cellule_arete *ca2 = (Cellule_arete*)malloc(sizeof(Cellule_arete));
                if (ca2 == NULL){
                    printf("Erreur d'allocation memoire 6\n");
                    return NULL;
                }
                ca2->a = a2;
                ca2->suiv = g->T_som[cn2->nd->num-1]->L_voisin;
                g->T_som[cn2->nd->num-1]->L_voisin = ca2;

            }
            cn2 = cn2->suiv;
        }
        cn1 = cn1->suiv;
    }

    return g;
}
 

// Fonction auxiliaire pour r??cup??rer la distance minimale entre u et v ?? l'aide d'un parcours en largeur
int distanceMinEntreDeuxSommets(Graphe *g, int u, int v) {
    if (u == v) return 0; // Cas sp??cial: si u et v sont les m??mes, la distance est 0
    
    int *distance = (int*)malloc(g->nbsom * sizeof(int)); // Tableau pour stocker les distances depuis u
    if (distance == NULL){
        printf("Erreur d'allocation memoire 1\n");
        return -1;
    }
    
    for (int i = 0; i < g->nbsom; ++i) {
        distance[i] = -1; // Initialiser toutes les distances ?? -1 (non visit??)
    }

    distance[u - 1] = 0; // La distance de u ?? u est 0
    S_file *file = (S_file*)malloc(sizeof(S_file)); // Cr??er une file pour le parcours en largeur
    if (file == NULL){
        printf("Erreur d'allocation memoire 2\n");
        return -1;
    }

    Init_file(file); // Initialiser la file
    enfile(file, u); // Ajouter u ?? la file

    while (!estFileVide(file)) {
        int sommet = defile(file); // Retirer le premier sommet de la file
        Sommet *s = g->T_som[sommet - 1];

        Cellule_arete *voisin = s->L_voisin;
        while (voisin != NULL) {
            int voisinNum = voisin->a->v;
            if (distance[voisinNum - 1] == -1) { // Si le voisin n'a pas ??t?? visit??
                distance[voisinNum - 1] = distance[sommet - 1] + 1; // Mettre ?? jour la distance depuis u
                if (voisinNum == v) { // Si le voisin est v, nous avons trouv?? le plus court chemin
                    free(file); // Lib??rer la m??moire allou??e ?? la file
                    free(distance); // Lib??rer la m??moire allou??e ?? distance
                    return distance[voisinNum - 1];
                }
                enfile(file, voisinNum); // Ajouter le voisin ?? la file
            }
            voisin = voisin->suiv;
        }
    }

    free(file); // Lib??rer la m??moire allou??e ?? la file
    free(distance); // Lib??rer la m??moire allou??e ?? distance
    return -1; // Retourner -1 si aucun chemin n'a ??t?? trouv?? entre u et v
}

// Fonction principale pour trouver le plus court chemin entre deux sommets u et v dans le graphe
int plusPetitNbAretesChaine(Graphe *g, int u, int v) {
    if (u < 1 || u > g->nbsom || v < 1 || v > g->nbsom) {
        printf("Sommet(s) invalide(s).\n");
        return -1;
    }
    return distanceMinEntreDeuxSommets(g, u, v);
}

ListeInt* cheminEntreDeuxSommets(Graphe *g, int u, int v) {
    if (u == v) {
        ListeInt* chemin = newListeInt();
        ajouterElementDebut(chemin, u);
        return chemin;
    }

    int *distance = (int*)malloc(g->nbsom * sizeof(int));
    int *predecesseur = (int*)malloc(g->nbsom * sizeof(int));

    for (int i = 0; i < g->nbsom; ++i) {
        distance[i] = -1;
        predecesseur[i] = -1;
    }

    distance[u - 1] = 0;
    S_file *file = creerFile();
    enfile(file, u);

    while (!estFileVide(file)) {
        int sommet = defile(file);
        Sommet *s = g->T_som[sommet - 1];
        Cellule_arete *voisin = s->L_voisin;
        
        while (voisin != NULL) {
            int voisinNum = voisin->a->v;
            if (distance[voisinNum - 1] == -1) {
                distance[voisinNum - 1] = distance[sommet - 1] + 1;
                predecesseur[voisinNum - 1] = sommet;
                if (voisinNum == v) {
                    libererFile(file);
                    free(distance);
                    ListeInt* chemin = newListeInt();
                    int current = v;
                    while (current != u) {
                        ajouterElementDebut(chemin, current);
                        current = predecesseur[current - 1];
                    }
                    ajouterElementDebut(chemin, u);
                    free(predecesseur);
                    return chemin;
                }
                enfile(file, voisinNum);
            }
            voisin = voisin->suiv;
        }
    }

    libererFile(file);
    free(distance);
    free(predecesseur);
    return NULL; // Retourner NULL si aucun chemin n'a ??t?? trouv?? entre u et v
}

void libererGraphe(Graphe *g){
    free(g->T_commod);

    Sommet **tabS = g->T_som;
    for (int i=0; i<g->nbsom; i++){
        Sommet *s = tabS[i];

        Cellule_arete *ca = s->L_voisin;
        while(ca){
            Cellule_arete *ca1 = ca;
            ca = ca->suiv;
            free(ca1->a);
            free(ca1);
        }

        free(s);
    }

    free(tabS);
}

int reorganiseReseau(Reseau* r) {
    //printf("\nOK\n");
    Graphe* g = creerGraphe(r); 
    
    int nbAretes = g->nbsom;
    //printf("nbAretes : %d OK\n", nbAretes);

    int **compteurAretes = (int**)malloc(nbAretes * sizeof(int*));
    for (int i = 0; i < nbAretes; ++i) {
        compteurAretes[i] = (int*)malloc(nbAretes * sizeof(int));
        for (int j=0; j<nbAretes; j++){
            compteurAretes[i][j] = 0;
        }
    }

    //printf("Allocation compteur OK %d\n", g->nbcommod);

    for (int i=0; i<g->nbcommod; i++){
        ListeInt *tab = cheminEntreDeuxSommets(g, (g->T_commod[i]).e1, (g->T_commod[i]).e2);
        //afficherListeInt(tab);
        //printf("taille : %d", tab->taille);

        if (tab != NULL){
            CelluleInt *ci = tab->debut;
            for (int j=0; j<(tab->taille-1); j++){
                if (ci->valeur <= ci->suivant->valeur){
                    compteurAretes[ci->valeur-1][ci->suivant->valeur-1]++;
                }else{
                    compteurAretes[ci->suivant->valeur-1][ci->valeur-1]++;
                }
                ci = ci->suivant;
                //printf("%d ", j);
            }
        }

        libererListeInt(tab);
    }

    //printf("Compteur OK\n");

    for (int i=0; i<nbAretes; i++){
        for (int j=0; j<nbAretes; j++){
            if (compteurAretes[i][j] >= g->gamma){
                return 0;
            }
        }
        free(compteurAretes[i]);
    }
    free(compteurAretes);

    //printf("Resolution OK\n");

    libererGraphe(g);

    //printf("FIN PROGRAMME\n");
    return 1;
}
