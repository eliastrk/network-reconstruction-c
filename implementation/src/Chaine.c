#include <stdlib.h>
#include <math.h>

#include "Chaine.h"
#include "SVGwriter.h"



Chaines* lectureChaines(FILE *f) {
    Chaines *chaines = (Chaines*)malloc(sizeof(Chaines));
    if (chaines == NULL) {
        printf("Erreur d'allocation memoire 1\n");
        return NULL;
    }

    fscanf(f, "NbChain: %d\n", &(chaines->nbChaines));
    fscanf(f, "Gamma: %d\n", &(chaines->gamma));
    //printf("NbChain: %d\nGamma: %d\n", chaines->nbChaines, chaines->gamma);


    chaines->chaines = NULL;
    for (int i = 0; i < chaines->nbChaines; i++) {
        CellChaine *cc = (CellChaine*)malloc(sizeof(CellChaine));
        if (cc == NULL) {
            printf("Erreur d'allocation memoire 2\n");
            return NULL;
        }

        int nbPoints;
        fscanf(f, "%d %d", &(cc->numero), &nbPoints);
        //printf("%d %d ", cc->numero, nbPoints);

        cc->points = NULL;
        cc->suiv = NULL;


        for (int j = 0; j < nbPoints; j++) {
            CellPoint *cp = (CellPoint*)malloc(sizeof(CellPoint));
            if (cp == NULL) {
                printf("Erreur d'allocation memoire 3\n");
                return NULL;
            }
            
            if (fscanf(f, "%lf %lf", &(cp->x), &(cp->y)) != 2) {
                printf("Erreur lecture coordonnees du point\n");
                return NULL;
            }
            
            //printf("%.2lf %.2lf ",(cp->x),(cp->y));
            cp->suiv = cc->points;
            cc->points = cp;
        }

        //printf("\n");
        cc->suiv = chaines->chaines;
        chaines->chaines = cc;
    }

    return chaines;
}

int comptePointsDansChaine(CellChaine *chaine) {
    int count = 0;

    CellPoint *currentPoint = chaine->points;
    while (currentPoint != NULL) {
        count++;
        currentPoint = currentPoint->suiv;
    }

    return count;
}

void ecrireChaines(Chaines *C, FILE *f) {
    fprintf(f, "NbChain: %d\n", C->nbChaines);
    fprintf(f, "Gamma: %d\n", C->gamma);

    CellChaine *cc = C->chaines;
    while (cc != NULL) {
        fprintf(f, "%d %d ", cc->numero, comptePointsDansChaine(cc));
        CellPoint *cp = cc->points;

        while (cp != NULL) {
            fprintf(f, "%.2lf %.2lf ", cp->x, cp->y);
            cp = cp->suiv;
        }

        fprintf(f, "\n");
        cc = cc->suiv;
    }
}

void afficheChainesSVG(Chaines *C, char* nomInstance){
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}



// Calcule la distance euclidienne entre deux points
double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Calcule la longueur d'une cha??ne
double longueurChaine(CellChaine *c) {
    double longueur = 0.0;
    CellPoint *currentPoint = c->points;
    CellPoint *nextPoint = NULL;

    // Parcours de tous les points de la cha??ne
    while (currentPoint != NULL && currentPoint->suiv != NULL) {
        nextPoint = currentPoint->suiv;
        // Calcul de la distance entre les points cons??cutifs et ajout ?? la longueur totale
        longueur += distance(currentPoint->x, currentPoint->y, nextPoint->x, nextPoint->y);
        currentPoint = nextPoint;
    }

    return longueur;
}

// Calcule la longueur totale de toutes les cha??nes dans l'ensemble
double longueurTotale(Chaines *C) {
    double longueurTotale = 0.0;
    CellChaine *currentChaine = C->chaines;

    // Parcours de toutes les cha??nes dans l'ensemble
    while (currentChaine != NULL) {
        // Ajout de la longueur de chaque cha??ne ?? la longueur totale
        longueurTotale += longueurChaine(currentChaine);
        currentChaine = currentChaine->suiv;
    }

    return longueurTotale;
}

int comptePointsTotal(Chaines *C) {
    int totalPoints = 0;
    CellChaine *currentChaine = C->chaines;

    // Parcourir toutes les cha??nes
    while (currentChaine != NULL) {
        // Compter les points dans chaque cha??ne en appelant la fonction comptePointsDansChaine
        totalPoints += comptePointsDansChaine(currentChaine);
        currentChaine = currentChaine->suiv;
    }

    return totalPoints;
}

void libererChaine(Chaines *C) {
    CellChaine *cc = C->chaines;
    while (cc) {
        CellPoint *cp = cc->points;

        while (cp) {
            CellPoint *cp1 = cp;
            cp = cp->suiv;
            free(cp1);
        }

        CellChaine *cc1 = cc;
        cc = cc->suiv;
        free(cc1);
    }
    free(C);
}

// Question 6.2
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax) {
    Chaines* chaines = (Chaines*)malloc(sizeof(Chaines));
    if (chaines == NULL) {
        printf("Erreur d'allocation memoire 1\n");
        return NULL;
    }
    chaines->nbChaines = nbChaines;
    chaines->gamma = (int)(nbChaines/3);
    chaines->chaines = NULL;

    for (int i = 0; i < nbChaines; i++) {
        CellChaine *cc = (CellChaine*)malloc(sizeof(CellChaine));
        if (cc == NULL) {
            printf("Erreur d'allocation memoire 2\n");
            return NULL;
        }

        cc->numero = i;
        cc->points = NULL;
        cc->suiv = NULL;

        for (int j = 0; j < nbPointsChaine; j++) {
            CellPoint *cp = (CellPoint*)malloc(sizeof(CellPoint));
            if (cp == NULL) {
                printf("Erreur d'allocation memoire 3\n");
                return NULL;
            }

            cp->x = (int)(rand()%(xmax+1));
            cp->y = (int)(rand()%(ymax+1));

            cp->suiv = cc->points;
            cc->points = cp;
        }

        cc->suiv = chaines->chaines;
        chaines->chaines = cc;
    }

    return chaines;
}
