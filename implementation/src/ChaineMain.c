#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Chaine.h"
#include "SVGwriter.h"

static void nomBaseSansExtension(const char *path, char *dest, size_t size) {
    const char *nom = strrchr(path, '/');
    const char *nomWindows = strrchr(path, '\\');
    const char *base = path;
    const char *point;
    size_t longueur;

    if (nom != NULL) {
        base = nom + 1;
    }
    if (nomWindows != NULL && (nom == NULL || nomWindows > nom)) {
        base = nomWindows + 1;
    }

    point = strrchr(base, '.');
    if (point == NULL) {
        point = base + strlen(base);
    }

    longueur = (size_t)(point - base);
    if (longueur >= size) {
        longueur = size - 1;
    }

    strncpy(dest, base, longueur);
    dest[longueur] = '\0';
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    char inputPath[256];
    char outputPath[256];
    char nomHtml[256];
    char nomCopie[256];

    if (argc != 3){
        printf("Mauvais format, veuillez reessayer");
        return 1;
    }

    char* nomF1 = argv[1];
    char* nomF2 = argv[2];

    if (strchr(nomF1, '/') == NULL && strchr(nomF1, '\\') == NULL) {
        snprintf(inputPath, sizeof(inputPath), "data/%s", nomF1);
        nomF1 = inputPath;
    }

    nomBaseSansExtension(nomF1, nomHtml, sizeof(nomHtml));
    snprintf(nomCopie, sizeof(nomCopie), "copie_%s.res", nomHtml);

    if (strchr(nomF2, '/') == NULL && strchr(nomF2, '\\') == NULL) {
        snprintf(outputPath, sizeof(outputPath), "results/%s", nomCopie);
        nomF2 = outputPath;
    }

    //Ouverture fichier .cha nomF1
    FILE *inputFile = fopen(nomF1, "r");
    if (inputFile == NULL){
        printf("Erreur lors de l'ouverture du fichier d'entree");
        return 2;
    }

    //Lecture fichier .cha 
    Chaines *chaines = lectureChaines(inputFile); 

    //Test fonction generationAleatoire
    //Chaines *chaines = generationAleatoire(100, 10, 100, 100); //Test fonction generationAleatoire

    fclose(inputFile);

    if (chaines == NULL){
        printf("Erreur lecture fichier");
        return 3;
    }
    printf("\nLecture de la chaine OK\n");

    //Cr??ation du fichier SVG
    afficheChainesSVG(chaines, nomHtml);
    printf("Creation du fichier SVG OK\n");

    //Ouverture fichier nomF2
    FILE *outputFile = fopen(nomF2, "w");
    if (outputFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sortie");
        return 4;
    }

    //Ecriture du contenu de la chaine dans le fichier nomF2
    ecrireChaines(chaines, outputFile);
    printf("Ecriture du contenu de la chaine OK\n");
    fclose(outputFile);

    //Liberer la memoire allouee par la chaine
    libererChaine(chaines);
    printf("Liberer la memoire allouee par la chaine OK\n");

    printf("Fin du programme\n\n");

    return 0;
}
