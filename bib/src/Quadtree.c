#include "Quadtree.h"

/**
 * @brief Calcule le nombre total de noeud dans un QuadTree donné sa profondeur.
 * 
 * @param depth Profondeur du QuadTree 
 * @return Le nombre total de noeuds dans le QuadTree.
 */
static int calculateTotalNodes(int depth) {
    return (int)((pow(4, depth + 1) - 1) / 3); // Somme de 4^k pour k=0 à depth
}

/**
 * Vérifie si un noeud est le quatrième enfant de son parent.
 * 
 * @param index Index du nœud dans le tableau.
 * @return 1 si le nœud est le quatrième enfant, 0 sinon.
 */
int isFourthChild(int index) { //1 si le noeud est 4eme sinon 0
    return (index - 1) % 4 == 3;
}

/**
 * Vérifie si un noeud une feuille
 * 
 * @param tree Pointeur vers le QuadTree.
 * @param nodeIndex Index du neoud à vérifier.
 * @return 1 si le nœud est une feuille, 0 sinon.
 */
int isLeaf(QuadTree* tree, int nodeIndex) {
    int childIndex = 4 * nodeIndex + 1; // Index du premier fils
    return (childIndex >= tree->totalNodes); // Aucun fils valide
}


/**
 * Calcule la profondeur du QuadTree à partir de la taille de l'image.
 * 
 * @param size Taille de l'image.
 * @return La profondeur calculée.
 */

int calculateDepth(int size) {
    return (int)log2(size); // Taille = largeur (ou hauteur) de l'image
}


/**
 * Calcule la variance pour un noeud dans le QuadTree.
 * 
 * @param tree Pointeur vers le QuadTree.
 * @param indexNode Index du noeud pour lequel calculer la variance.
 */
static void calculerVariance( QuadTree * tree , int indexNode ) {
    
    if (isLeaf(tree, indexNode)) {
        tree->nodes[indexNode].var = 0 ; 
        return;
    }

    double mu = 0.0; 
    int firstChildIndex = 4 * indexNode + 1;
    for (int i = 0 ; i < 4 ; i++ ){
        mu += (tree->nodes[firstChildIndex+i].var * tree->nodes[firstChildIndex+i].var ) + 
        ((tree->nodes[indexNode].m -  tree->nodes[firstChildIndex+i].m )*(tree->nodes[indexNode].m -  tree->nodes[firstChildIndex+i].m));
    }
    
 tree->nodes[indexNode].var = sqrt(mu)/4 ; 
}


/**
 * Récupère le parent d'un noeud donné.
 * 
 * @param tree Pointeur vers le QuadTree.
 * @param nodeIndex Index du noeud dont le parent est recherché.
 * @return Pointeur vers le nœud parent, ou NULL sinon.
 */
QuadTreeNode* getParentNode(QuadTree* tree, int nodeIndex) {    //cette fonction sert à retourner le parent d'un noeud passé en parametre ( indice )
    if (nodeIndex == 0) {
        return NULL;
    }

    int parentIndex = (nodeIndex - 1) / 4;

    return &tree->nodes[parentIndex];
}


/**
 * Crée un QuadTree vide avec une profondeur.
 * 
 * @param depth Profondeur du QuadTree.
 * @return Pointeur vers le QuadTree nouvellement créé.
 */
QuadTree* createQuadTree(int depth) {
    int totalNodes = calculateTotalNodes(depth);

    QuadTree* tree = (QuadTree*)malloc(sizeof(QuadTree));
    if (!tree) {
        perror("Erreur lors de l'allocation du QuadTree");
        return NULL;
    }

    tree->nodes = (QuadTreeNode*)malloc(totalNodes * sizeof(QuadTreeNode));
    if (!tree->nodes) {
        perror("Erreur lors de l'allocation des nœuds du QuadTree");
        free(tree);
        return NULL;
    }

    tree->totalNodes = totalNodes;
    tree->depth = depth;

    return tree;
}


/**
 * Libère la mémoire associée à un QuadTree.
 * 
 * @param tree Pointeur vers le QuadTree à libérer.
 */

void freeQuadTree(QuadTree* tree) {
    if (tree) {
        free(tree->nodes);
        free(tree);
    }
}


/**
 * @brief Calcule la moyenne des intensités et vérifie l'uniformité d'un bloc d'image.
 * 
 * Cette fonction analyse un bloc rectangulaire d'une image représentée par un tableau de pixels.
 * Elle calcule la moyenne des intensités dans le bloc et détermine si toutes les intensités 
 * du bloc sont identiques.
 * 
 * @param data Tableau contenant les pixels de l'image.
 * @param startX Coordonnée X de départ du bloc.
 * @param startY Coordonnée Y de départ du bloc.
 * @param size Taille du bloc 
 * @param width Largeur de l'image (utilisée pour le calcul des indices).
 * @param m Pointeur où la moyenne des intensités du bloc sera stockée.
 * @param uniform Pointeur où sera stocké 1 si le bloc est uniforme, 0 sinon.
 */
static void computeBlock(uint8_t* data, int startX, int startY, int size, int width, uint8_t* m, uint8_t* uniform) {
    int sum = 0;
    uint8_t firstValue = data[startY * width + startX];
    *uniform = 1; // Supposer uniforme par défaut

    for (int y = startY; y < startY + size; y++) {
        for (int x = startX; x < startX + size; x++) {
            uint8_t value = data[y * width + x];
            sum += value;

            // Vérifier l'uniformité
            if (value != firstValue) {
                *uniform = 0;
            }
        }
    }

    *m = sum / (size * size); // Moyenne des intensités
}


/**
 * Remplit le QuadTree avec les données d'une image.
 * 
 * @param tree Pointeur vers le QuadTree.
 * @param data Tableau contenant les données de l'image.
 * @param width Largeur de l'image.
 * @param height Hauteur de l'image.
 * @param depth Profondeur maximale du QuadTree.
 * @param nodeIndex Index du noeud actuel.
 * @param startX Coordonnée X de départ.
 * @param startY Coordonnée Y de départ.
 * @param size Taille de la zone à analyser.
 */
void fillQuadTree(QuadTree* tree, uint8_t* data, int width, int height, int depth, int nodeIndex, int startX, int startY, int size ) {
    if (depth == 0) {
        computeBlock(data, startX, startY, size, width, &tree->nodes[nodeIndex].m, &tree->nodes[nodeIndex].uniform);
        tree->nodes[nodeIndex].epsilon = 0; // Les feuilles ont epsilon = 0
        tree->nodes[nodeIndex].var = 0; 
        return;
    }

    // Nœud interne : diviser en 4 sous-blocs
    int halfSize = size / 2;
    int childIndex = 4 * nodeIndex + 1;

    fillQuadTree(tree, data, width, height, depth - 1, childIndex, startX, startY, halfSize);                  // Haut-gauche
    fillQuadTree(tree, data, width, height, depth - 1, childIndex + 1, startX + halfSize, startY, halfSize);  // Haut-droit
    fillQuadTree(tree, data, width, height, depth - 1, childIndex + 2, startX + halfSize, startY + halfSize, halfSize); // Bas-droit
    fillQuadTree(tree, data, width, height, depth - 1, childIndex + 3, startX, startY + halfSize, halfSize);  // Bas-gauche

    // Calculer les moyennes des 4 sous-blocs
    uint8_t m1 = tree->nodes[childIndex].m ;
    uint8_t m2 = tree->nodes[childIndex + 1].m ; 
    uint8_t m3 = tree->nodes[childIndex + 2].m ;
    uint8_t m4 = tree->nodes[childIndex + 3].m ;

    // Calculer epsilon pour le nœud courant
    tree->nodes[nodeIndex].epsilon = (m1 + m2 + m3 + m4) % 4;

    // Calculer m pour le nœud courant (moyenne des sous-blocs)
    tree->nodes[nodeIndex].m = (m1 + m2 + m3 + m4) / 4;

    // Calculer u pour le nœud courant
    tree->nodes[nodeIndex].uniform = (
        tree->nodes[childIndex].uniform &&
        tree->nodes[childIndex + 1].uniform &&
        tree->nodes[childIndex + 2].uniform &&
        tree->nodes[childIndex + 3].uniform &&
        (m1 == m2 && m2 == m3 && m3 == m4) // Tous les m identiques
    );

    calculerVariance(tree , nodeIndex); 
}


/**
 * Affiche les informations du QuadTree.
 * 
 * @param tree Pointeur vers le QuadTree à afficher.
 */
void printQuadTree(QuadTree* tree) {
    printf("=== QuadTree ===\n");
    printf("Profondeur : %d\n", tree->depth);
    printf("Nombre total de nœuds : %d\n", tree->totalNodes);

    int nodeIndex = 0;
    for (int level = 0; level <= tree->depth; level++) {
        int nodesAtLevel = (1 << (2 * level)); // Nombre de nœuds au niveau actuel
        printf("Niveau %d:\n", level);

        for (int i = 0; i < nodesAtLevel && nodeIndex < tree->totalNodes; i++, nodeIndex++) {
            printf("  Node %d -> m: %d, epsilon: %d, uniform: %d , variance %lf  \n" ,
                   nodeIndex, 
                   tree->nodes[nodeIndex].m, 
                   tree->nodes[nodeIndex].epsilon, 
                   tree->nodes[nodeIndex].uniform,
                   tree->nodes[nodeIndex].var 
            );      
        }
    }
}
