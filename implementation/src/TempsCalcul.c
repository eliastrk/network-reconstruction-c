#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Reseau.h"
#include "Chaine.h"
#include "Hachage.h"
#include "ArbreQuat.h"
#include "SVGwriter.h"

#define NB_ITERATIONS 10000
#define HACHAGE1 50
#define HACHAGE2 100
#define HACHAGE3 500
#define HACHAGE4 1000

#define NB_CHAINES_MIN 500
#define NB_CHAINES_MAX 5000
#define PAS 500
#define NB_POINTS_CHAINE 100
#define XMAX 80
#define YMAX 80

//Fonctions pour mesure les temps de calcul de ReconstitueReseauListe/Hachage/arbre
double mesureTemps(Reseau *(*fonction)(Chaines*), Chaines* chaines) {  
    clock_t begin, end;
    double temps;

    begin = clock();
    fonction(chaines);
    end = clock();

    temps = (double)(end - begin) / CLOCKS_PER_SEC;

    return temps;
}

double mesureTempsHachage(Reseau *(*fonction)(Chaines*, int), Chaines* chaines, int m) {
    clock_t debut, fin;
    double temps;

    debut = clock();
    fonction(chaines, m);
    fin = clock();

    temps = (double)(fin - debut) / CLOCKS_PER_SEC;

    return temps;
}

//Fonctions pour afficher puis sauvegarder un graphe global de comparaison
void ecrireDonneesGraphe(FILE *graphe, int *x, double *y1, double *y2, double *y3, double *y4, double *y5, double *y6, int taille) {
    fprintf(graphe, "set title 'Comparaison des temps de calcul sur des chaines aleatoires'\n");
    fprintf(graphe, "set xlabel 'Nombre total de points des chaines'\n");
    fprintf(graphe, "set ylabel 'Temps de calcul (secondes)'\n");
    fprintf(graphe, "set grid\n");
    fprintf(graphe, "set key left top\n");
    fprintf(graphe, "set xrange [%d:%d]\n", NB_CHAINES_MIN * NB_POINTS_CHAINE, NB_CHAINES_MAX * NB_POINTS_CHAINE);
    fprintf(graphe, "$data << EOD\n");
    for (int i = 0; i < taille; i++) {
        fprintf(graphe, "%d %f %f %f %f %f %f\n", x[i], y1[i], y2[i], y3[i], y4[i], y5[i], y6[i]);
    }
    fprintf(graphe, "EOD\n");
    fprintf(graphe, "plot '$data' using 1:2 with linespoints title 'Hachage %d', ", HACHAGE1);
    fprintf(graphe, "'$data' using 1:3 with linespoints title 'Hachage %d', ", HACHAGE2);
    fprintf(graphe, "'$data' using 1:4 with linespoints title 'Hachage %d', ", HACHAGE3);
    fprintf(graphe, "'$data' using 1:5 with linespoints title 'Hachage %d', ", HACHAGE4);
    fprintf(graphe, "'$data' using 1:6 with linespoints title 'Arbre', ");
    fprintf(graphe, "'$data' using 1:7 with linespoints title 'Liste'\n");
}

void generateGrapheComparaison(int *x, double *y1, double *y2, double *y3, double *y4, double *y5, double *y6, int taille) {
    FILE *graphe = popen("gnuplot -persistent", "w");
    if (graphe == NULL) {
        printf("Erreur ouverture graphe\n");
        return;
    }

    ecrireDonneesGraphe(graphe, x, y1, y2, y3, y4, y5, y6, taille);
    fflush(graphe);
    pclose(graphe);

    graphe = popen("gnuplot", "w");
    if (graphe == NULL) {
        printf("Erreur sauvegarde graphe\n");
        return;
    }

    fprintf(graphe, "set terminal pngcairo size 1400,800\n");
    fprintf(graphe, "set output 'results/TempsCalculComparaison.png'\n");
    ecrireDonneesGraphe(graphe, x, y1, y2, y3, y4, y5, y6, taille);
    fprintf(graphe, "unset output\n");

    fflush(graphe);
    pclose(graphe);
}


int main() {
    srand(time(NULL));


    //Ouverture fichier .cha
    FILE* inputFile = fopen("data/00014_burma.cha", "r");
    if (inputFile == NULL){
        printf("Erreur lors de l'ouverture du fichier d'entree");
        return 2;
    }

    //Lecture fichier .cha 
    Chaines* chaines = lectureChaines(inputFile);
    printf("\nLecture de la chaine pour 00014_burma.cha OK\n");
    fclose(inputFile);

    if (chaines == NULL){
        printf("Erreur lecture fichier");
        return 3;
    }

    //Calcul et ecriture du temps des calculs des fonctions ReconstitueReseau dans un fichier txt
    FILE* outputFile = fopen("results/TempsCalcul.txt", "w");
    if (outputFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sortie");
        return 4;
    }

    //Pour 00014_burma.cha (boucle qui fait une moyenne des temps de calculs car la chaine est tres petite et affiche souvent 0s)
    double tempsTotal1 = 0, tempsTotal2 = 0, tempsTotal3 = 0, tempsTotal4 = 0, tempsTotal5 = 0, tempsTotal6 = 0;
    for (int i = 0; i<NB_ITERATIONS; i++){
        tempsTotal1 += mesureTemps(reconstitueReseauListe, chaines);
        tempsTotal2 += mesureTemps(reconstitueReseauArbre, chaines);
        tempsTotal3 += mesureTempsHachage(reconstitueReseauHachage, chaines, HACHAGE1);
        tempsTotal4 += mesureTempsHachage(reconstitueReseauHachage, chaines, HACHAGE2);
        tempsTotal5 += mesureTempsHachage(reconstitueReseauHachage, chaines, HACHAGE3);
        tempsTotal6 += mesureTempsHachage(reconstitueReseauHachage, chaines, HACHAGE4);
    }

    double tempsListe = tempsTotal1/NB_ITERATIONS;
    double tempsArbre = tempsTotal2/NB_ITERATIONS;
    double tempsHachage1 = tempsTotal3/NB_ITERATIONS; double tempsHachage2 = tempsTotal4/NB_ITERATIONS; double tempsHachage3 = tempsTotal5/NB_ITERATIONS; double tempsHachage4 = tempsTotal6/NB_ITERATIONS;

    fprintf(outputFile, "ReconstitueReseauListe: %f secondes\n", tempsListe);
    fprintf(outputFile, "ReconstitueReseauArbre: %f secondes\n", tempsArbre);
    fprintf(outputFile, "ReconstitueReseauHachage %d: %f secondes\n", HACHAGE1, tempsHachage1);
    fprintf(outputFile, "ReconstitueReseauHachage %d: %f secondes\n", HACHAGE2, tempsHachage2);
    fprintf(outputFile, "ReconstitueReseauHachage %d: %f secondes\n", HACHAGE3, tempsHachage3);
    fprintf(outputFile, "ReconstitueReseauHachage %d: %f secondes\n\n", HACHAGE4, tempsHachage4);
    fprintf(outputFile, "Comparaison sur des chaines aleatoires\n\n");

    printf("Ecriture des temps de calcul OK\n");

    //Liberer la memoire allouee par les chaines
    libererChaine(chaines);
    printf("Liberer la memoire allouee par la chaine OK\n\n");
    
    
    int nbChainesMin = NB_CHAINES_MIN;
    
    int nbElements = ((NB_CHAINES_MAX - nbChainesMin) / PAS) + 1;
    double *tabTempsListe = (double *)malloc(nbElements * sizeof(double));
    double *tabTempsHachage1 = (double *)malloc(nbElements * sizeof(double));
    double *tabTempsHachage2 = (double *)malloc(nbElements * sizeof(double));
    double *tabTempsHachage3 = (double *)malloc(nbElements * sizeof(double));
    double *tabTempsHachage4 = (double *)malloc(nbElements * sizeof(double));
    double *tabTempsArbre = (double *)malloc(nbElements * sizeof(double));
    int *tabNbPointsTotal = (int *)malloc(nbElements * sizeof(int));

    int j = 0;

    //Temps de calculs selon une chaine aleatoire
    for (;nbChainesMin <= NB_CHAINES_MAX; nbChainesMin += PAS){
        Chaines *C = generationAleatoire(nbChainesMin, NB_POINTS_CHAINE, XMAX, YMAX);
        
        tempsTotal1 = 0, tempsTotal2 = 0, tempsTotal3 = 0, tempsTotal4 = 0, tempsTotal5 = 0, tempsTotal6 = 0;

        tempsTotal1 = mesureTemps(reconstitueReseauListe, C);
        tempsTotal2 = mesureTemps(reconstitueReseauArbre, C);
        tempsTotal3 = mesureTempsHachage(reconstitueReseauHachage, C, HACHAGE1);
        tempsTotal4 = mesureTempsHachage(reconstitueReseauHachage, C, HACHAGE2);
        tempsTotal5 = mesureTempsHachage(reconstitueReseauHachage, C, HACHAGE3);
        tempsTotal6 = mesureTempsHachage(reconstitueReseauHachage, C, HACHAGE4);

        tabTempsListe[j] = tempsTotal1;
        tabTempsArbre[j] = tempsTotal2;
        tabTempsHachage1[j] = tempsTotal3;
        tabTempsHachage2[j] = tempsTotal4;
        tabTempsHachage3[j] = tempsTotal5;
        tabTempsHachage4[j] = tempsTotal6;
        tabNbPointsTotal[j] = nbChainesMin*NB_POINTS_CHAINE;

        printf("nbChaines : %d, , nbPoints : %d\nHachage %d : %.3fs, Hachage %d : %.3fs, Hachage %d : %.3fs, Hachage %d : %.3fs, Arbre : %.3fs, Liste : %.3fs\n\n", nbChainesMin, tabNbPointsTotal[j], HACHAGE1, tabTempsHachage1[j], HACHAGE2, tabTempsHachage2[j], HACHAGE3, tabTempsHachage3[j], HACHAGE4, tabTempsHachage4[j], tabTempsArbre[j], tabTempsListe[j]);
        fprintf(outputFile, "nbChaines : %d, , nbPoints : %d\n", nbChainesMin, tabNbPointsTotal[j]);
        fprintf(outputFile, "Hachage %d : %.3fs, Hachage %d : %.3fs, Hachage %d : %.3fs, Hachage %d : %.3fs, Arbre : %.3fs, Liste : %.3fs\n\n", HACHAGE1, tabTempsHachage1[j], HACHAGE2, tabTempsHachage2[j], HACHAGE3, tabTempsHachage3[j], HACHAGE4, tabTempsHachage4[j], tabTempsArbre[j], tabTempsListe[j]);
        
        j++;

        libererChaine(C);
    }

    fclose(outputFile);

    printf("\nTemps de calcul OK\n");

    //Graphe global de comparaison
    generateGrapheComparaison(tabNbPointsTotal, tabTempsHachage1, tabTempsHachage2, tabTempsHachage3, tabTempsHachage4, tabTempsArbre, tabTempsListe, nbElements);
    printf("Graphe de comparaison OK\n");

    //Liberer la memoire allouee par les tableaux
    free(tabTempsListe);
    free(tabTempsArbre);
    free(tabTempsHachage1);
    free(tabTempsHachage2);
    free(tabTempsHachage3);
    free(tabTempsHachage4);
    free(tabNbPointsTotal);
    printf("Liberer la memoire allouee par les tableaux OK\n");

    printf("Fin du programme\n\n");

    return 0;
}
