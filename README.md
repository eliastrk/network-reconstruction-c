# Network Reconstruction Project in C

[Version française](./README.md)

C data structures project focused on reconstructing and analysing a network from chains of points.

The project works with `.cha` files describing geometric chains, then reconstructs a network using several data structures. It also produces text outputs and HTML/SVG visualizations to inspect both the input data and the reconstructed networks.

## Repository Contents

- `implementation/`: C source code, datasets, project outputs, and the `Makefile`

## Implementation Layout

- `implementation/src/`: C source files
- `implementation/include/`: header files
- `implementation/data/`: `.cha` input datasets
- `implementation/results/`: `.res` outputs and timing measurements
- `implementation/visualizations/`: `.html` visualizations
- `implementation/Makefile`: build file for the main executables

This separation makes it easier to distinguish:
- input data
- computed results
- visual outputs

### Input Data

The `.cha` files in `implementation/data/` describe a set of point chains with:
- the number of chains
- the `gamma` parameter
- the points of each chain

Main datasets:
- `00014_burma.cha`: small instance, useful for quick checks
- `05000_USA-road-d-NY.cha`: medium-to-large road network instance
- `07397_pla.cha`: another large instance used in the project

### Text Outputs

The `.res` files in `implementation/results/` describe the reconstructed network:
- number of nodes
- number of links
- number of commodities
- `gamma`
- node list `v`
- link list `l`
- commodity list `k`

### Visualizations

The programs generate `.html` files in `implementation/visualizations/`. These files embed SVG content to display chains or reconstructed networks.

Examples:
- `visualizations/00014_burma.html`
- `visualizations/07397_pla.html`
- `visualizations/05000_USA-road-d-NY.html`

## Build

From the `implementation/` directory:

```bash
make
```

Generated executables:
- `ChaineMain`
- `ReconstitueReseau`
- `TempsCalcul`
- `GrapheMain`

## Reconstruction Methods

`ReconstitueReseau` expects a second integer parameter that selects the data structure used to reconstruct the network:

- `1`: linked-list based reconstruction
- `2`: hash-table based reconstruction
- `3`: quadtree based reconstruction

Example:

```bash
./ReconstitueReseau 00014_burma.cha 2
```

Here:
- `00014_burma.cha` is looked up in `data/`
- `2` means reconstruction using a hash table

## Detailed Usage

All commands below are meant to be run from `implementation/`.

### 1. Read and visualize a chain file

```bash
./ChaineMain 00014_burma.cha copie_burma.res
./ChaineMain 05000_USA-road-d-NY.cha copie_road-NY.res
./ChaineMain 07397_pla.cha copie_pla.res
```

This command:
- reads `data/00014_burma.cha`
- reconstructs the `Chaines` structure in memory
- generates `visualizations/00014_burma.html`
- writes the chain copy to `results/copie_00014_burma.res`

So:
- input: `data/00014_burma.cha`
- text output: `results/copie_00014_burma.res`
- visual output: `visualizations/00014_burma.html`

`ChaineMain` is mainly used to verify `.cha` parsing and visualize the original chains before network reconstruction.

Automatic copy naming:
- `00014_burma.cha` -> `copie_00014_burma.res`
- `07397_pla.cha` -> `copie_07397_pla.res`
- `05000_USA-road-d-NY.cha` -> `copie_05000_USA-road-d-NY.res`

### 2. Reconstruct a network with linked lists

```bash
./ReconstitueReseau 00014_burma.cha 1
./ReconstitueReseau 07397_pla.cha 1
./ReconstitueReseau 05000_USA-road-d-NY.cha 1
```

Examples:
- `00014_burma.cha`: quick verification instance, outputs `visualizations/00014_burma.html` and `results/00014_burma_ReseauListe.res`
- `07397_pla.cha`: intermediate case, outputs `visualizations/07397_pla.html` and `results/07397_pla_ReseauListe.res`
- `05000_USA-road-d-NY.cha`: larger dataset, outputs `visualizations/05000_USA-road-d-NY.html` and `results/05000_USA-road-d-NY_ReseauListe.res`

For `./ReconstitueReseau 00014_burma.cha 1`:
- reads `data/00014_burma.cha`
- reconstructs the network with linked lists
- generates `visualizations/00014_burma.html`
- writes `results/00014_burma_ReseauListe.res`

### 3. Reconstruct a network with a hash table

```bash
./ReconstitueReseau 00014_burma.cha 2
./ReconstitueReseau 07397_pla.cha 2
./ReconstitueReseau 05000_USA-road-d-NY.cha 2
```

Examples:
- `00014_burma.cha`: outputs `visualizations/00014_burma.html` and `results/00014_burma_ReseauHachage.res`
- `07397_pla.cha`: outputs `visualizations/07397_pla.html` and `results/07397_pla_ReseauHachage.res`
- `05000_USA-road-d-NY.cha`: outputs `visualizations/05000_USA-road-d-NY.html` and `results/05000_USA-road-d-NY_ReseauHachage.res`

For `./ReconstitueReseau 00014_burma.cha 2`:
- reads `data/00014_burma.cha`
- reconstructs the network using a hash table of size `6000`
- generates `visualizations/00014_burma.html`
- writes `results/00014_burma_ReseauHachage.res`

### 4. Reconstruct a network with a quadtree

```bash
./ReconstitueReseau 00014_burma.cha 3
./ReconstitueReseau 07397_pla.cha 3
./ReconstitueReseau 05000_USA-road-d-NY.cha 3
```

Examples:
- `00014_burma.cha`: outputs `visualizations/00014_burma.html` and `results/00014_burma_ReseauArbre.res`
- `07397_pla.cha`: outputs `visualizations/07397_pla.html` and `results/07397_pla_ReseauArbre.res`
- `05000_USA-road-d-NY.cha`: outputs `visualizations/05000_USA-road-d-NY.html` and `results/05000_USA-road-d-NY_ReseauArbre.res`

For `./ReconstitueReseau 00014_burma.cha 3`:
- reads `data/00014_burma.cha`
- reconstructs the network with a quadtree
- generates `visualizations/00014_burma.html`
- writes `results/00014_burma_ReseauArbre.res`

### 5. Measure execution times

```bash
./TempsCalcul
```

This command:
- first reads `data/00014_burma.cha`
- writes an initial short comparison for this small instance into `results/TempsCalcul.txt`
- then generates random chains with increasing sizes
- compares reconstruction times on these random chains for linked lists, quadtrees, and several hash-table sizes
- writes these detailed comparisons into `results/TempsCalcul.txt`
- builds a graph from those random-chain measurements
- displays the graph at the end
- also saves it as `results/TempsCalculComparaison.png`

So:
- initial input: `data/00014_burma.cha`
- main workload: experimental comparison on random chains
- text output: `results/TempsCalcul.txt`
- displayed graph: `gnuplot` window
- saved graph: `results/TempsCalculComparaison.png`

### 6. Test network reorganization on multiple instances

```bash
./GrapheMain
```

This command:
- reads `data/00014_burma.cha`
- reads `data/07397_pla.cha`
- reads `data/05000_USA-road-d-NY.cha`
- reconstructs the networks with the quadtree approach
- tests `reorganiseReseau` for multiple `gamma` values
- prints the results in the terminal

So:
- inputs: multiple files from `data/`
- output: console display
- no additional `.res` or `.html` file is produced by this program

## Difference Between `ChaineMain` and `ReconstitueReseau`

`ChaineMain` works at the chain-input level: it reads a `.cha` file, checks its structure, displays it, and writes it back.

`ReconstitueReseau` goes further: it starts from the same `.cha` input, but then reconstructs an actual network with nodes, links, and commodities, and produces both a network visualization and a `.res` file.

In short:
- `ChaineMain` visualizes the input data
- `ReconstitueReseau` visualizes the reconstruction result
