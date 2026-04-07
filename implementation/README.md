# Implementation

Le coeur du projet est organise autour de trois espaces distincts :

- `data/` : jeux de donnees d'entree `.cha`
- `results/` : sorties texte `.res` et mesures de temps
- `visualizations/` : sorties HTML/SVG generees par les programmes

Le code C reste separe entre :
- `src/`
- `include/`

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



