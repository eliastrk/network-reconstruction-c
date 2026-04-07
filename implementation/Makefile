CFLAGS = -g -Wno-unused-parameter -Iinclude
CC = gcc

PROGRAMS = ChaineMain ReconstitueReseau TempsCalcul GrapheMain
SRC_DIR = src
INC_DIR = include

.PHONY: all clean

all: $(PROGRAMS)

ChaineMain: Chaine.o SVGwriter.o ChaineMain.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

ReconstitueReseau: Chaine.o SVGwriter.o Reseau.o ReconstitueReseau.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

TempsCalcul: Chaine.o SVGwriter.o Reseau.o TempsCalcul.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

GrapheMain: Chaine.o SVGwriter.o Reseau.o Graphe.o Struct_File.o ListeInt.o GrapheMain.o
	$(CC) -o $@ $(CFLAGS) $^ -lm

Chaine.o: $(SRC_DIR)/Chaine.c $(INC_DIR)/Chaine.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Chaine.c -o $@

SVGwriter.o: $(SRC_DIR)/SVGwriter.c $(INC_DIR)/SVGwriter.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/SVGwriter.c -o $@

Reseau.o: $(SRC_DIR)/Reseau.c $(INC_DIR)/Reseau.h $(INC_DIR)/Chaine.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Reseau.c -o $@

ChaineMain.o: $(SRC_DIR)/ChaineMain.c $(INC_DIR)/Chaine.h $(INC_DIR)/SVGwriter.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/ChaineMain.c -o $@

ReconstitueReseau.o: $(SRC_DIR)/ReconstitueReseau.c $(INC_DIR)/Reseau.h $(INC_DIR)/Chaine.h $(INC_DIR)/SVGwriter.h $(INC_DIR)/Hachage.h $(INC_DIR)/ArbreQuat.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/ReconstitueReseau.c -o $@

TempsCalcul.o: $(SRC_DIR)/TempsCalcul.c $(INC_DIR)/Chaine.h $(INC_DIR)/SVGwriter.h $(INC_DIR)/Reseau.h $(INC_DIR)/Hachage.h $(INC_DIR)/ArbreQuat.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/TempsCalcul.c -o $@

Struct_File.o: $(SRC_DIR)/Struct_File.c $(INC_DIR)/Struct_File.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Struct_File.c -o $@

ListeInt.o: $(SRC_DIR)/ListeInt.c $(INC_DIR)/ListeInt.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/ListeInt.c -o $@

Graphe.o: $(SRC_DIR)/Graphe.c $(INC_DIR)/Graphe.h $(INC_DIR)/Reseau.h $(INC_DIR)/Chaine.h $(INC_DIR)/SVGwriter.h $(INC_DIR)/Struct_File.h $(INC_DIR)/ListeInt.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Graphe.c -o $@

GrapheMain.o: $(SRC_DIR)/GrapheMain.c $(INC_DIR)/Graphe.h $(INC_DIR)/Reseau.h $(INC_DIR)/Chaine.h $(INC_DIR)/SVGwriter.h $(INC_DIR)/Struct_File.h $(INC_DIR)/ListeInt.h $(INC_DIR)/Hachage.h $(INC_DIR)/ArbreQuat.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/GrapheMain.c -o $@

clean:
	rm -f *.o *~ $(PROGRAMS)
