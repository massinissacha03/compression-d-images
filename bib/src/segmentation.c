#include "segmentation.h"


/**
 * @brief Génère une grille de segmentation à partir d'un QuadTree.
 * 
 * Cette fonction remplit une grille en associant les segments définis par le QuadTree 
 * aux pixels correspondants dans l'image.
 * 
 * @param tree Pointeur vers le QuadTree contenant les données de segmentation.
 * @param grid Tableau représentant la grille de segmentation
 * @param width Largeur de la grille.
 * @param height Hauteur de la grille.
 * @param nodeIndex Index du noeud actuel dans le QuadTree.
 * @param x Coordonnée X de départ pour la zone actuelle.
 * @param y Coordonnée Y de départ pour la zone actuelle.
 * @param size Taille de la zone actuelle du carré 
 */
void generateSegmentationGrid(QuadTree* tree, uint8_t* grid, int width, int height, int nodeIndex, int x, int y, int size) {
    if (!tree || !grid || nodeIndex < 0 || size <= 0) return;

    QuadTreeNode* node = &tree->nodes[nodeIndex];

    if (node->uniform == 0) {
        for (int i = 0; i < size; i++) {
            if (x + i < width && y < height) grid[y * width + (x + i)] = 120;            
            if (x + i < width && y + size - 1 < height) grid[(y + size - 1) * width + (x + i)] = 120;

            // Bordures verticales
            if (x < width && y + i < height) grid[(y + i) * width + x] = 120;            
            if (x + size - 1 < width && y + i < height) grid[(y + i) * width + (x + size - 1)] = 120;
        }
    }

    if (isLeaf(tree, nodeIndex)) {
        return;
    }

    // Descendre dans les enfants
    int halfSize = size / 2;
    int childIndex = 4 * nodeIndex + 1;

    generateSegmentationGrid(tree, grid, width, height, childIndex, x, y, halfSize);                
    generateSegmentationGrid(tree, grid, width, height, childIndex + 1, x + halfSize, y, halfSize);
    generateSegmentationGrid(tree, grid, width, height, childIndex + 2, x + halfSize, y + halfSize, halfSize);
    generateSegmentationGrid(tree, grid, width, height, childIndex + 3, x, y + halfSize, halfSize);
}
