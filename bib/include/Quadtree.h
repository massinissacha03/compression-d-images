#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>


/**
 * @brief Représente un nœud dans un QuadTree.
 * 
 * Cette structure contient les informations nécessaires pour un nœud d'un QuadTree,
 * incluant la moyenne d'intensité, l'uniformité, une erreur de compression et la variance.
 */
typedef struct {
    uint8_t m;           
    uint8_t uniform;     // Indique si le bloc est uniforme (1 : oui, 0 : non)
    uint8_t epsilon;     // Erreur (ou seuil de compression)
    double var; // variance d'un noeud 
} QuadTreeNode;

/**
 * @brief Représente un QuadTree.
 * 
 * Cette structure contient un tableau de nœuds, la profondeur de l'arbre et 
 * le nombre total de nœuds.
 */
typedef struct {
    QuadTreeNode* nodes; 
    int totalNodes;      
    int depth;           

} QuadTree;


/**
 * Vérifie si un noeud est le quatrième enfant de son parent.
 * 
 * @param index Index du nœud dans le tableau.
 * @return 1 si le nœud est le quatrième enfant, 0 sinon.
 */
int isFourthChild(int index);


/**
 * Vérifie si un noeud une feuille
 * 
 * @param tree Pointeur vers le QuadTree.
 * @param nodeIndex Index du neoud à vérifier.
 * @return 1 si le nœud est une feuille, 0 sinon.
 */
int isLeaf(QuadTree* tree, int nodeIndex);


/**
 * Calcule la profondeur du QuadTree à partir de la taille de l'image.
 * 
 * @param size Taille de l'image.
 * @return La profondeur calculée.
 */
int calculateDepth(int size);


/**
 * Récupère le parent d'un noeud donné.
 * 
 * @param tree Pointeur vers le QuadTree.
 * @param nodeIndex Index du noeud dont le parent est recherché.
 * @return Pointeur vers le nœud parent, ou NULL sinon.
 */
QuadTreeNode* getParentNode(QuadTree* tree, int nodeIndex);


/**
 * Crée un QuadTree vide avec une profondeur.
 * 
 * @param depth Profondeur du QuadTree.
 * @return Pointeur vers le QuadTree nouvellement créé.
 */
QuadTree* createQuadTree(int depth);


/**
 * Libère la mémoire associée à un QuadTree.
 * 
 * @param tree Pointeur vers le QuadTree à libérer.
 */
void freeQuadTree(QuadTree* tree);


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
void fillQuadTree(QuadTree* tree, uint8_t* data, int width, int height, int depth, int nodeIndex, int startX, int startY, int size);


/**
 * Affiche les informations du QuadTree.
 * 
 * @param tree Pointeur vers le QuadTree à afficher.
 */
void printQuadTree(QuadTree* tree);


#endif 
