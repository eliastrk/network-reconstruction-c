# Projet SDD S4

Projet de structures de donnees en C centre sur la reconstruction et l'analyse d'un reseau a partir de chaines de points.

Le projet manipule des fichiers `.cha` decrivant des chaines geometriques, puis reconstruit un reseau selon plusieurs structures de donnees. Il produit aussi des sorties texte et des visualisations HTML/SVG pour observer les donnees d'entree et les reseaux reconstruits.

## Contenu du depot

- `implementation/` : code source C, jeux de donnees, sorties du projet, makefile

## Organisation de l'implementation

- `implementation/src/` : fichiers source C
- `implementation/include/` : fichiers d'en-tete (structures de données, fonctions, description des fonctions, etc.)
- `implementation/data/` : jeux de donnees `.cha`
- `implementation/results/` : sorties texte `.res` et mesures de temps
- `implementation/visualizations/` : visualisations `.html`
- `implementation/Makefile` : compilation des executables principaux

Cette separation permet de distinguer clairement :
- les donnees d'entree
- les resultats calcules
- les affichages graphiques

### Donnees d'entree

Les fichiers `.cha` de `implementation/data/` decrivent un ensemble de chaines de points avec :
- le nombre de chaines
- le parametre `gamma`
- les points de chaque chaine

Jeux de donnees principaux :
- `00014_burma.cha` : petite instance, utile pour tester rapidement
- `05000_USA-road-d-NY.cha` : instance intermediaire
- `07397_pla.cha` : instance volumineuse

### Sorties texte

Les fichiers `.res` de `implementation/results/` decrivent le reseau reconstruit :
- nombre de noeuds
- nombre de liaisons
- nombre de commodites
- valeur de `gamma`
- liste des noeuds `v`
- liste des liaisons `l`
- liste des commodites `k`

### Visualisations

Les programmes generent des fichiers `.html` dans `implementation/visualizations/`. Ces fichiers contiennent du SVG pour afficher les chaines ou les reseaux.

Exemples de sorties visuelles :
- `visualizations/00014_burma.html`
- `visualizations/07397_pla.html`
- `visualizations/05000_USA-road-d-NY.html`

## Compilation

Depuis le dossier `implementation/` :

```bash
make
```

Executables generes :
- `ChaineMain`
- `ReconstitueReseau`
- `TempsCalcul`
- `GrapheMain`

## Methodes de reconstruction

Le programme `ReconstitueReseau` attend un second parametre entier qui choisit la structure de donnees utilisee pour reconstruire le reseau :

- `1` : reconstruction par liste chainee
- `2` : reconstruction par table de hachage
- `3` : reconstruction par arbre quaternaire

Exemple :

```bash
./ReconstitueReseau 00014_burma.cha 2
```

Ici :
- `00014_burma.cha` est cherche dans `data/`
- `2` signifie : reconstruction par table de hachage

## Utilisation detaillee

Toutes les commandes ci-dessous sont a lancer depuis `implementation/`.

### 1. Lire et visualiser un fichier de chaines

```bash
./ChaineMain 00014_burma.cha copie_burma.res
./ChaineMain 05000_USA-road-d-NY.cha copie_road-NY.res
./ChaineMain 07397_pla.cha copie_pla.res
```

Cette commande :
- lit `data/00014_burma.cha`
- reconstruit en memoire la structure `Chaines`
- genere `visualizations/00014_burma.html`
- reecrit les chaines dans `results/copie_00014_burma.res`

Donc :
- entree : `data/00014_burma.cha`
- sortie texte : `results/copie_00014_burma.res`
- sortie visuelle : `visualizations/00014_burma.html`

`ChaineMain` sert surtout a verifier la lecture du fichier `.cha` et a visualiser les chaines d'origine avant reconstruction du reseau.

- `00014_burma.cha` produit `copie_00014_burma.res`
- `07397_pla.cha` produit `copie_07397_pla.res`
- `05000_USA-road-d-NY.cha` produit `copie_05000_USA-road-d-NY.res`

### 2. Reconstruire un reseau avec une liste chainee

```bash
./ReconstitueReseau 00014_burma.cha 1
./ReconstitueReseau 07397_pla.cha 1
./ReconstitueReseau 05000_USA-road-d-NY.cha 1
```

Exemples :
- `00014_burma.cha` : petite instance pour une verification rapide, sortie `visualizations/00014_burma.html` et `results/00014_burma_ReseauListe.res`
- `07397_pla.cha` : instance intermediaire, sortie `visualizations/07397_pla.html` et `results/07397_pla_ReseauListe.res`
- `05000_USA-road-d-NY.cha` : jeu de donnees plus volumineux, sortie `visualizations/05000_USA-road-d-NY.html` et `results/05000_USA-road-d-NY_ReseauListe.res`

Pour `./ReconstitueReseau 00014_burma.cha 1` :
- lit `data/00014_burma.cha`
- reconstruit le reseau avec une liste chainee
- genere `visualizations/00014_burma.html`
- ecrit `results/00014_burma_ReseauListe.res`

### 3. Reconstruire un reseau avec une table de hachage

```bash
./ReconstitueReseau 00014_burma.cha 2
./ReconstitueReseau 07397_pla.cha 2
./ReconstitueReseau 05000_USA-road-d-NY.cha 2
```

Exemples :
- `00014_burma.cha` : sortie `visualizations/00014_burma.html` et `results/00014_burma_ReseauHachage.res`
- `07397_pla.cha` : sortie `visualizations/07397_pla.html` et `results/07397_pla_ReseauHachage.res`
- `05000_USA-road-d-NY.cha` : sortie `visualizations/05000_USA-road-d-NY.html` et `results/05000_USA-road-d-NY_ReseauHachage.res`

Pour `./ReconstitueReseau 00014_burma.cha 2` :
- lit `data/00014_burma.cha`
- reconstruit le reseau avec une table de hachage de taille `6000`
- genere `visualizations/00014_burma.html`
- ecrit `results/00014_burma_ReseauHachage.res`

### 4. Reconstruire un reseau avec un arbre quaternaire

```bash
./ReconstitueReseau 00014_burma.cha 3
./ReconstitueReseau 07397_pla.cha 3
./ReconstitueReseau 05000_USA-road-d-NY.cha 3
```

Exemples :
- `00014_burma.cha` : sortie `visualizations/00014_burma.html` et `results/00014_burma_ReseauArbre.res`
- `07397_pla.cha` : sortie `visualizations/07397_pla.html` et `results/07397_pla_ReseauArbre.res`
- `05000_USA-road-d-NY.cha` : sortie `visualizations/05000_USA-road-d-NY.html` et `results/05000_USA-road-d-NY_ReseauArbre.res`

Pour `./ReconstitueReseau 00014_burma.cha 3` :
- lit `data/00014_burma.cha`
- reconstruit le reseau avec un arbre quaternaire
- genere `visualizations/00014_burma.html`
- ecrit `results/00014_burma_ReseauArbre.res`

### 5. Mesurer les temps de calcul

```bash
./TempsCalcul
```

Cette commande :
- commence par lire `data/00014_burma.cha`
- ecrit dans `results/TempsCalcul.txt` une premiere comparaison courte sur cette petite instance
- genere ensuite plusieurs chaines aleatoires de tailles croissantes
- compare sur ces chaines aleatoires les temps de reconstruction pour la liste, l'arbre et plusieurs tailles de table de hachage
- ecrit aussi ces comparaisons detaillees dans `results/TempsCalcul.txt`
- construit un graphe a partir de ces mesures sur chaines aleatoires
- affiche ce graphe a la fin
- enregistre aussi ce graphe dans `results/TempsCalculComparaison.png`

Donc :
- entree initiale : `data/00014_burma.cha`
- travail principal : comparaison experimentale sur des chaines aleatoires
- sortie texte : `results/TempsCalcul.txt`
- sortie graphique affichee : fenetre `gnuplot`
- sortie graphique enregistree : `results/TempsCalculComparaison.png`

### 6. Tester la reorganisation sur plusieurs instances

```bash
./GrapheMain
```

Cette commande :
- lit `data/00014_burma.cha`
- lit `data/07397_pla.cha`
- lit `data/05000_USA-road-d-NY.cha`
- reconstruit les reseaux avec l'arbre quaternaire
- teste la fonction `reorganiseReseau` pour plusieurs valeurs de `gamma`
- affiche les resultats dans le terminal

Donc :
- entrees : plusieurs fichiers de `data/`
- sortie : affichage console
- pas de nouveau `.res` ni de nouveau `.html` produits par ce programme

## Difference entre `ChaineMain` et `ReconstitueReseau`

`ChaineMain` travaille au niveau des chaines d'entree : il lit un fichier `.cha`, verifie sa structure, l'affiche et le reecrit.

`ReconstitueReseau` va plus loin : il part du meme type de fichier `.cha`, mais reconstruit ensuite un vrai reseau avec noeuds, liaisons et commodites, puis produit une visualisation du reseau et un fichier `.res`.

En bref :
- `ChaineMain` visualise les donnees d'entree
- `ReconstitueReseau` visualise le resultat de la reconstruction
