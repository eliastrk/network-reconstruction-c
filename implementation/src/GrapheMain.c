#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Reseau.h"
#include "Chaine.h"
#include "Hachage.h"
#include "ArbreQuat.h"
#include "SVGwriter.h"
#include "Graphe.h"

int main(){

    //Ouverture et lecture fichiers .cha
    FILE* inputFile = fopen("data/00014_burma.cha", "r");
    if (inputFile == NULL){
        printf("Erreur lors de l'ouverture du fichier d'entree");
        return 2;
    }

    Chaines* chaines = lectureChaines(inputFile);
    printf("\nLecture de la chaine pour 00014_burma.cha OK\n");
    fclose(inputFile);

    if (chaines == NULL){
        printf("Erreur lecture fichier");
        return 3;
    }

    FILE* inputFile1 = fopen("data/07397_pla.cha", "r");
    if (inputFile1 == NULL){
        printf("Erreur lors de l'ouverture du fichier d'entree");
        return 2;
    }

    Chaines* chaines1 = lectureChaines(inputFile1);
    printf("Lecture de la chaine pour 07397_pla.cha OK\n");
    fclose(inputFile1);

    if (chaines1 == NULL){
        printf("Erreur lecture fichier");
        return 3;
    }

    FILE* inputFile2 = fopen("data/05000_USA-road-d-NY.cha", "r");
    if (inputFile2 == NULL){
        printf("Erreur lors de l'ouverture du fichier d'entree");
        return 2;
    }

    Chaines* chaines2 = lectureChaines(inputFile2);
    printf("Lecture de la chaine pour 05000_USA-road-d-NY.cha OK\n");
    fclose(inputFile2);

    if (chaines2 == NULL){
        printf("Erreur lecture fichier");
        return 3;
    }
    
    //Reconstitutiond des reseaux a l'aide des chaines
    Reseau *R = reconstitueReseauArbre(chaines);
    Reseau *R1 = reconstitueReseauArbre(chaines1);
    Reseau *R2 = reconstitueReseauArbre(chaines2);

    printf("Reconstitution des reseaux OK\n");

    //Reorganisation des reseaux
    int x = reorganiseReseau(R);
    int x1 = reorganiseReseau(R1);
    int x2 = reorganiseReseau(R2);

    printf("Reorganisation des reseaux OK\n\n");

    printf("ReorganiseReseau avec 00014_burma.cha et gamma = 3 : %s\nReorganiseReseau avec 07397_pla.cha et gamma = 908 : %s\nReorganiseReseau avec 05000_USA-road-d-NY.cha et gamma = 143 : %s\n\n", (x == 1) ? "VRAI" : "FAUX", (x1 == 1) ? "VRAI" : "FAUX", (x2 == 1) ? "VRAI" : "FAUX");

    //Variation de gamma
    printf("On fait varier gamma pour en deduire quand ReorganiseReseau est vrai\n\n");
    
    for(int j=0; j<=chaines->nbChaines; j++){
    	R->gamma = j;
        int x = reorganiseReseau(R);
    	if (x == 1){
			printf("ReorganisationReseau avec 00014_burma.cha pour gamma = %d : VRAI\n", j);
		}else{
			printf("ReorganisationReseau avec 00014_burma.cha pour gamma = %d : FAUX\n", j);
		}
	}

    printf("\n");

    for(int j=1315; j<=1325; j++){
    	R1->gamma = j;
        int x = reorganiseReseau(R1);
    	if (x == 1){
			printf("ReorganisationReseau avec 07397_pla.cha pour gamma = %d : VRAI\n", j);
		}else{
			printf("ReorganisationReseau avec 07397_pla.cha pour gamma = %d : FAUX\n", j);
		}
	}

    printf("\n");

    for(int j=275; j<=285; j++){
    	R2->gamma = j;
        int x = reorganiseReseau(R2);
    	if (x == 1){
			printf("ReorganisationReseau avec 05000_USA-road-d-NY.cha pour gamma = %d : VRAI\n", j);
		}else{
			printf("ReorganisationReseau avec 05000_USA-road-d-NY.cha pour gamma = %d : FAUX\n", j);
		}
	}

    //Liberer la memoire allouee par les chaine et les reseaux
    libererChaine(chaines); libererChaine(chaines1); libererChaine(chaines2);
    libererReseau(R); libererReseau(R1); libererReseau(R2);
    printf("\nLiberer la memoire allouee par les chaines et les reseaux OK\n");

    printf("Fin du programme\n\n");

    return 0;
}
