#include "Chaine.h"
#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"
#include "SVGwriter.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include <time.h>

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

int main(int argc, char **argv){
    srand(time(NULL)); 
    char inputPath[256];
    char nomHtml[256];
    char nomRes[256];
    
    if (argc != 3){
        printf("Mauvais format, veuillez reessayer");
        return 1;
    }

    char* nomF1 = argv[1];
    int methode = atoi(argv[2]);

    if (strchr(nomF1, '/') == NULL && strchr(nomF1, '\\') == NULL) {
        snprintf(inputPath, sizeof(inputPath), "data/%s", nomF1);
        nomF1 = inputPath;
    }

    nomBaseSansExtension(nomF1, nomHtml, sizeof(nomHtml));

    //Ouverture fichier .cha
    FILE *inputFile = fopen(nomF1, "r");
    if (inputFile == NULL){
        printf("Erreur lors de l'ouverture du fichier d'entree");
        return 2;
    }
    
    //Lecture fichier .cha 
    Chaines* chaines = lectureChaines(inputFile);
    fclose(inputFile);
    if (chaines == NULL){
        printf("Erreur lecture fichier");
        return 3;
    }
    printf("\nLecture de la chaine OK\n");

    Reseau* reseau;

    switch (methode){
        case 1:
            printf("Vous utilisez la methode de reconstitution du reseau avec une liste\n");

            //Creation du reseau a l'aide d'une liste
            reseau = reconstitueReseauListe(chaines);
            if (reseau == NULL){
                printf("Erreur reconstitution reseau (liste)");
                return 4;
            }
            printf("Reconstitution reseau OK\n");

            //Creation du fichier SVG
            afficheReseauSVG(reseau, nomHtml);
            printf("Creation du fichier SVG OK\n");

            //Ecriture du contenu du reseau dans le fichier burma2.res
            snprintf(nomRes, sizeof(nomRes), "results/%s_ReseauListe.res", nomHtml);
            FILE *outputFile1 = fopen(nomRes, "w");
            if (outputFile1 == NULL) {
                printf("Erreur lors de l'ouverture du fichier de sortie (liste)");
                return 5;
            }
            ecrireReseau(reseau, outputFile1);
            printf("Ecriture du contenu du reseau OK\n");
            fclose(outputFile1);

            break;
        case 2:
            printf("Vous utilisez la methode de reconstitution du reseau avec une table de hachage\n");

            //Creation du reseau a l'aide d'une table de hachage
            reseau = reconstitueReseauHachage(chaines, 6000);
            if (reseau == NULL){
                printf("Erreur reconstitution reseau (table de hachage)");
                return 4;
            }
            printf("Reconstitution reseau OK\n");

            //Creation du fichier SVG
            afficheReseauSVG(reseau, nomHtml);
            printf("Creation du fichier SVG OK\n");

            //Ecriture du contenu du reseau dans le fichier burma2.res
            snprintf(nomRes, sizeof(nomRes), "results/%s_ReseauHachage.res", nomHtml);
            FILE *outputFile2 = fopen(nomRes, "w");
            if (outputFile2 == NULL) {
                printf("Erreur lors de l'ouverture du fichier de sortie (table de hachage)");
                return 5;
            }
            ecrireReseau(reseau, outputFile2);
            printf("Ecriture du contenu du reseau OK\n");
            fclose(outputFile2);

            break;
        case 3:
            printf("Vous utilisez la methode de reconstitution du reseau avec un arbre quaternaire\n");

            //Creation du reseau a l'aide d'un arbre quatenaire
            reseau = reconstitueReseauArbre(chaines);
            if (reseau == NULL){
                printf("Erreur reconstitution reseau (arbre quatenaire)");
                return 4;
            }
            printf("Reconstitution reseau OK\n");

            //Creation du fichier SVG
            afficheReseauSVG(reseau, nomHtml);
            printf("Creation du fichier SVG OK\n");

            //Ecriture du contenu du reseau dans le fichier burma2.res
            snprintf(nomRes, sizeof(nomRes), "results/%s_ReseauArbre.res", nomHtml);
            FILE *outputFile3 = fopen(nomRes, "w");
            if (outputFile3 == NULL) {
                printf("Erreur lors de l'ouverture du fichier de sortie (arbre quatenaire)");
                return 5;
            }
            ecrireReseau(reseau, outputFile3);
            printf("Ecriture du contenu du reseau OK\n");
            fclose(outputFile3);

            break;
        default:
            printf("M??thode %d pas reconnue", methode);
            return 6;
    }

    //Liberer la memoire allouee par la chaine et le reseau
    libererChaine(chaines);
    libererReseau(reseau);
    printf("Liberer la memoire allouee par la chaine et le reseau OK\n");

    printf("Fin du programme\n\n");

    return 0;
}
