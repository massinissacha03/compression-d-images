#ifndef FILTRAGE_H
#define FILTRAGE_H
 
#include "Quadtree.h"


/**
 * Calcule la variance moyenne et maximale des blocs dans un QuadTree.
 * 
 * @param tree Pointeur vers un QuadTree.
 * @param medvar Pointeur où la variance moyenne sera stockée.
 * @param maxvar Pointeur où la variance maximale sera stockée.
 */
void avgAndMaxVars(QuadTree* tree, double* medvar, double* maxvar);


/**
 * Applique un filtrage sur un QuadTree en fonction de sa variance moyenne et maximale
 * 
 * @param tree Pointeur vers le QuadTree à filtrer.
 * @param nodeIndex Index du nœud actuel dans le QuadTree.
 * @param sigma Seuil de variance pour le filtrage.
 * @param alpha Facteur pour ajuster le filtrage.
 * @return 1 si le filtrage a été appliqué avec succès, 0 sinon.
 */
int filtrage (QuadTree * tree , int nodeIndex , double sigma , double alpha );


#endif 
