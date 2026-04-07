#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "Chaine.h"
#include "Reseau.h"
#include "Hachage.h"
#include "SVGwriter.h"
#include "ArbreQuat.h"


Noeud *rechercheCreeNoeudListe(Reseau *R, double x, double y) {
    CellNoeud *cn =  R->noeuds;

    while (cn) {
        if (cn->nd->x == x && cn->nd->y == y){
            return cn->nd;
        }
        cn = cn->suiv;
    }

    Noeud* n = (Noeud*)malloc(sizeof(Noeud));
    if (n == NULL) {
        printf("Erreur d'allocation memoire 1\n");
        return NULL;
    }
    n->num = R->nbNoeuds+1;
    n->x = x;
    n->y = y;
    n->voisins = NULL;

    CellNoeud *cn1 = (CellNoeud*)malloc(sizeof(CellNoeud));
    if (cn1 == NULL) {
        printf("Erreur d'allocation memoire 2\n");
        return NULL;
    }
    cn1->nd = n;
    cn1->suiv = R->noeuds;
    R->noeuds = cn1;

    R->nbNoeuds++;

    return n;
}


Reseau *initialiserReseau(int gamma) {
    Reseau *R = (Reseau*)malloc(sizeof(Reseau));
    if (R == NULL) {
        printf("Erreur d'allocation memoire 1\n");
        return NULL;
    }

    R->nbNoeuds = 0;
    R->gamma = gamma;
    R->noeuds = NULL;
    R->commodites = NULL;

    return R;
}

int existeLiaison(Noeud* n1, Noeud* n2) {
    CellNoeud *cn = n1->voisins;
    while (cn) {
        if (cn->nd == n2) {
            return 1;  
        }
        cn = cn->suiv;
    }
    return 0; 
}

Reseau *reconstitueReseauListe(Chaines *C) {
    if (C == NULL){
        printf("La liste des chaines C est vide\n");
        return NULL;
    }

    Reseau *R = initialiserReseau(C->gamma);

    CellChaine *cc = C->chaines;
    while (cc) {
        Noeud *n = NULL;
        CellPoint *cp = cc->points;

        while (cp) {
            Noeud *n1 = rechercheCreeNoeudListe(R, cp->x, cp->y);

            if (n != NULL && n1 != n && !existeLiaison(n, n1)) {
                CellNoeud *cn = (CellNoeud*)malloc(sizeof(CellNoeud));
                if (cn == NULL) {
                    printf("Erreur d'allocation memoire 1\n");
                    return NULL;
                }

                cn->nd = n1;
                cn->suiv = n->voisins;
                n->voisins = cn;


                CellNoeud *cn1 = (CellNoeud*)malloc(sizeof(CellNoeud));
                if (cn1 == NULL) {
                    printf("Erreur d'allocation memoire 2\n");
                    return NULL;
                }
                cn1->nd = n;
                cn1->suiv = n1->voisins;
                n1->voisins = cn1;

            }else if (n == NULL) {
                CellCommodite *cm = (CellCommodite*)malloc(sizeof(CellCommodite));
                if (cm == NULL) {
                    printf("Erreur d'allocation memoire 3\n");
                    return NULL;
                }

                cm->extrA = n1;
                cm->extrB = NULL; 
                cm->suiv = R->commodites;
                R->commodites = cm;
            }

            n = n1;
            cp = cp->suiv;
        }

        if (R->commodites != NULL) {
            R->commodites->extrB = n;
        }

        cc = cc->suiv;
    }

    return R;
}

void libererReseau(Reseau *R) {
    if (R == NULL) {
        return;
    }

    CellNoeud *cn = R->noeuds;
    while (cn) {
        CellNoeud *cn1 = cn;
        cn = cn->suiv;

        CellNoeud *cnV1 = cn1->nd->voisins;
        while (cnV1) {
            CellNoeud *cnV2 = cnV1;
            cnV1 = cnV1->suiv;
            free(cnV2); 
        }
        
        free(cn1->nd);
        free(cn1); 
    }

    CellCommodite *cm = R->commodites;
    while (cm != NULL) {
        CellCommodite *cm1 = cm;
        cm = cm->suiv;
        free(cm1);
    }

    free(R);
}

int nbCommodites(Reseau *R) {
    CellCommodite *CC = R->commodites;
    int somme = 0;

    while (CC) {
        somme++;
        CC = CC->suiv;
    }

    return somme;
}

int nbLiaisons(Reseau *R) {
    int somme = 0;
    CellNoeud *cn1 = R->noeuds;
    CellNoeud *cn2;

    while (cn1 != NULL) {
        cn2 = cn1->nd->voisins;
        while (cn2 != NULL){
            if (cn1->nd->num < cn2->nd->num){
                somme++;
            }
            cn2 = cn2->suiv;
        }
        cn1 = cn1->suiv;
    }
    return somme;
}

void ecrireReseau(Reseau *R, FILE *f){
    fprintf(f, "NbNoeuds: %d\nNbLiaisons: %d\nNbCommodites: %d\nGamma: %d\n\n", R->nbNoeuds, nbLiaisons(R), nbCommodites(R), R->gamma);

    CellNoeud *cn = R->noeuds;

    while (cn){
        fprintf(f, "v %d %.2lf %.2lf\n", cn->nd->num, cn->nd->x, cn->nd->y);
        cn = cn->suiv;
    }
    fprintf(f,"\n");

    CellNoeud *cn1 = R->noeuds;
    CellNoeud *cn2;

    while (cn1 != NULL){
        cn2 = cn1->nd->voisins;
        while (cn2 != NULL){
            if (cn1->nd->num < cn2->nd->num){
                fprintf(f,"l %d %d\n", cn1->nd->num, cn2->nd->num);
            }
            cn2 = cn2->suiv;
        }
        cn1 = cn1->suiv;
    }
    fprintf(f,"\n");

    CellCommodite *cc = R->commodites;

    while (cc){
        fprintf(f,"k %d %d\n", cc->extrA->num, cc->extrB->num);
        cc = cc->suiv;
    }
}

void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        SVGlineRandColor(&svg);
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

int f(double x, double y){
    return (int)(y + ((x + y) * (x + y + 1)) / 2);
}

int h(int k, int M){
    double A = (sqrt(5) - 1) / 2;
    return (int)(M * (fmod(k * A, 1)));
}

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y){
    int cle = f(x, y);
    int indice = h(cle, H->tailleMax);

    CellNoeud *cn1 = H->T[indice];

    while (cn1){
        if (cn1->nd->x == x && cn1->nd->y == y){
            return cn1->nd;
        }
        cn1 = cn1->suiv;
    }

    Noeud* n = (Noeud*)malloc(sizeof(Noeud));
    if (n == NULL){
        printf("Erreur d'allocation memoire 1\n");
        return NULL;
    }
    n->num = R->nbNoeuds+1;
    n->x = x;
    n->y = y;
    n->voisins = NULL;

    CellNoeud *cn2 = (CellNoeud*)malloc(sizeof(CellNoeud));
    if (cn2 == NULL){
        printf("Erreur d'allocation memoire 2\n");
        return NULL;
    }
    R->nbNoeuds++;
    cn2->nd = n;
    cn2->suiv = R->noeuds;
    R->noeuds = cn2;
    
    CellNoeud *cn3 = (CellNoeud*)malloc(sizeof(CellNoeud));
    cn3->nd = n;
    cn3->suiv = H->T[indice];
    H->T[indice] = cn3;
    if (cn3 == NULL){
        printf("Erreur d'allocation memoire 3\n");
        return NULL;
    }

    return n;
}

TableHachage *initialiserTableHachage(int M){
    TableHachage *H = (TableHachage*)malloc(sizeof(TableHachage));
    if (H == NULL){
        printf("Erreur d'allocation memoire 1\n");
        return NULL;
    }

    H->tailleMax = M;
    H->nbElement = 0;
    H->T = (CellNoeud**)malloc(sizeof(CellNoeud*)*M);

    for (int i=0; i<M; i++){
        H->T[i] = NULL;
    }

    return H;
}

void libererTableHachage(TableHachage *H){
    for (int i = 0; i<H->tailleMax; i++) {
        CellNoeud *cn = H->T[i];
        while (cn){
            CellNoeud *cn1 = cn;
            cn = cn->suiv;
            free(cn1);
        }
    }

    free(H->T);
    free(H);
}

Reseau *reconstitueReseauHachage(Chaines *C, int M) {
    if (C == NULL){
        printf("La liste chainee C est vide\n");
        return NULL;
    }

    Reseau *R = initialiserReseau(C->gamma);
    TableHachage *H = initialiserTableHachage(M);

    CellChaine *cc = C->chaines;
    while (cc != NULL){
        Noeud *n1 = NULL;
        CellPoint *cp = cc->points;

        while (cp != NULL){
            Noeud *n2 = rechercheCreeNoeudHachage(R, H, cp->x, cp->y);

            if (n1 != NULL && n2 != n1 && !existeLiaison(n1, n2)){
                CellNoeud *cn = (CellNoeud*)malloc(sizeof(CellNoeud));
                if (cn == NULL){
                    printf("Erreur d'allocation memoire 1\n");
                    return NULL;
                }
                cn->nd = n2;
                cn->suiv = n1->voisins;
                n1->voisins = cn;

                CellNoeud *cn1 = (CellNoeud*)malloc(sizeof(CellNoeud));
                if (cn1 == NULL){
                    printf("Erreur d'allocation memoire 2\n");
                    return NULL;
                }
                cn1->nd = n1;
                cn1->suiv = n2->voisins;
                n2->voisins = cn1;
            }else if (n1 == NULL){
                CellCommodite *cm = (CellCommodite*)malloc(sizeof(CellCommodite));
                if (cm == NULL){
                    printf("Erreur d'allocation memoire 3\n");
                    return NULL;
                }
                cm->extrA = n2;
                cm->extrB = NULL; 
                cm->suiv = R->commodites;
                R->commodites = cm;
            }

            n1 = n2;
            cp = cp->suiv;
        }

        if (R->commodites != NULL){
            R->commodites->extrB = n1;
        }

        cc = cc->suiv;
    }

    libererTableHachage(H);
    return R;
}

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    if (C == NULL){
        printf("La liste des chaines C est vide\n");
        return;
    }

    CellChaine *cc = C->chaines;
    *xmin = cc->points->x; *xmax = cc->points->x;
    *ymin = cc->points->y; *ymax = cc->points->y;
    while (cc){
        CellPoint *cp = cc->points;

        while (cp){
            if (cp->x < *xmin){
                *xmin = cp->x;
            }else if (cp->x > *xmax){
                *xmax = cp->x;
            }else if (cp->y > *ymax){
                *ymax = cp->y;
            }else if (cp->y < *ymin){
                *ymin = cp->y;
            }

            cp = cp->suiv;
        }

        cc = cc->suiv;
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
    ArbreQuat *AQ = (ArbreQuat*)malloc(sizeof(ArbreQuat));
    if (AQ == NULL){
        printf("Erreur d'allocation memoire\n");
        return NULL;
    }

    AQ->xc = xc;
    AQ->yc = yc;
    AQ->coteX = coteX;
    AQ->coteY= coteY;

    AQ->noeud = NULL;
    AQ->so = NULL;   
    AQ->se = NULL;   
    AQ->no = NULL;   
    AQ->ne = NULL;

    return AQ;
}

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){
    if (*a == NULL){
        if (parent == NULL){
            *a = creerArbreQuat(n->x, n->y, 0, 0);
        }else{
            double coteX = fabs(parent->coteX / 2);
            double coteY = fabs(parent->coteY / 2); 
            double xc, yc;

            if (n->x < parent->xc){
                xc = parent->xc - coteX/2;
            }else{
                xc = parent->xc + coteX/2;
            }

            if (n->y < parent->yc){
                yc = parent->yc - coteY/2;
            }else{
                yc = parent->yc + coteY/2;
            }

            *a = creerArbreQuat(xc, yc, coteX, coteY);
        }
        (*a)->noeud = n;
    } else if ((*a)->noeud != NULL){
        if ((*a)->noeud->x == n->x && (*a)->noeud->y == n->y){
            return;
        }else{
            ArbreQuat *a1 = *a;
            *a = creerArbreQuat(a1->xc, a1->yc, a1->coteX, a1->coteY);
            (*a)->noeud = NULL;
            insererNoeudArbre(a1->noeud, a, parent);
            insererNoeudArbre(n, a, parent);
        }
    }else{
        if (n->x < (*a)->xc){
            if (n->y < (*a)->yc){
                insererNoeudArbre(n, &((*a)->so), *a);
            }else{
                insererNoeudArbre(n, &((*a)->no), *a);
            }
        } else{
            if (n->y < (*a)->yc){
                insererNoeudArbre(n, &((*a)->se), *a);
            }else{
                insererNoeudArbre(n, &((*a)->ne), *a);
            }
        }
    }
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
    if (*a == NULL){
        Noeud* n = rechercheCreeNoeudListe(R, x, y);
        insererNoeudArbre(n, &(*a), parent);
        return n;
    }else if ((*a)->noeud != NULL){
        if ((*a)->noeud->x == x && (*a)->noeud->y == y){
            return (*a)->noeud;
        }else{
            Noeud* n = rechercheCreeNoeudListe(R, x, y);
            insererNoeudArbre(n, a, parent);
            return n;
        }
    }else{
        if (x < (*a)->xc){
            if (y < (*a)->yc){
                return rechercheCreeNoeudArbre(R, &((*a)->so), *a, x, y);
            }else{
                return rechercheCreeNoeudArbre(R, &((*a)->no), *a, x, y);
            }
        } else{
            if (y < (*a)->yc){
                return rechercheCreeNoeudArbre(R, &((*a)->se), *a, x, y);
            }else{
                return rechercheCreeNoeudArbre(R, &((*a)->ne), *a, x, y);
            }
        }
    }

}

void libererArbreQuat(ArbreQuat **a) {
    if (a == NULL || *a == NULL) return;

    libererArbreQuat(&((*a)->ne));  
    libererArbreQuat(&((*a)->no));  
    libererArbreQuat(&((*a)->se)); 
    libererArbreQuat(&((*a)->so));

    free(*a);
    *a = NULL;
}

Reseau* reconstitueReseauArbre(Chaines* C){
    if (C == NULL){
        fprintf(stderr, "La liste des chaines C est vide\n");
        return NULL;
    }

    Reseau *R = initialiserReseau(C->gamma);

    double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);

    ArbreQuat *a = creerArbreQuat((xmin+xmax)/2, (ymin+ymax)/2, xmax-xmin, ymax-ymin);

    CellChaine *cc = C->chaines;
    while (cc != NULL){
        Noeud *n1 = NULL;
        CellPoint *cp = cc->points;

        while (cp != NULL){
            Noeud *n2 = rechercheCreeNoeudArbre(R, &a, a, cp->x, cp->y);

            if (n1 != NULL && n2 != n1 && !existeLiaison(n1, n2)){
                CellNoeud *cn = (CellNoeud*)malloc(sizeof(CellNoeud));
                if (cn == NULL){
                    printf("Erreur d'allocation memoire 1\n");
                    return NULL;
                }

                cn->nd = n2;
                cn->suiv = n1->voisins;
                n1->voisins = cn;


                CellNoeud *cn1 = (CellNoeud*)malloc(sizeof(CellNoeud));
                if (cn1 == NULL){
                    printf("Erreur d'allocation memoire 2\n");
                    return NULL;
                }

                cn1->nd = n1;
                cn1->suiv = n2->voisins;
                n2->voisins = cn1;
            }else if (n1 == NULL){
                CellCommodite *cm = (CellCommodite*)malloc(sizeof(CellCommodite));
                if (cm == NULL){
                    printf("Erreur d'allocation memoire 3\n");
                    return NULL;
                }

                cm->extrA = n2;
                cm->extrB = NULL; 
                cm->suiv = R->commodites;
                R->commodites = cm;
            }

            n1 = n2;
            cp = cp->suiv;
        }

        if (R->commodites != NULL){
            R->commodites->extrB = n1;
        }

        cc = cc->suiv;
    }

    libererArbreQuat(&a);
    return R;
}
