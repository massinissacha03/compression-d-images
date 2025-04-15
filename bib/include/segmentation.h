#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "Quadtree.h"


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
void generateSegmentationGrid(QuadTree* tree, uint8_t* grid, int width, int height, int nodeIndex, int x, int y, int size);

#endif
