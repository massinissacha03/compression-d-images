#ifndef __DCODAGE__
#define __DCODAGE__

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#include "Quadtree.h"


/**
 * Lit un fichier contenant un QuadTree compressé (format QTC) le copie pour le renvoyer
 * 
 * @param filename Pointeur vers le fichier à lire.
 * @param taille Pointeur pour stocker la taille des données lues.
 * @return Un tableau d'octets contenant les données lues.
 */
uint8_t* readQTCFile(FILE* filename, int* taille) ; 


/**
 * Génère des données compressées à partir d'un QuadTree récursivement
 * 
 * @param tree Pointeur vers le QuadTree à encoder.
 * @param data Tableau où les données compressées seront écrites.
 * @param width Largeur de l'image d'origine.
 * @param height Hauteur de l'image d'origine.
 * @param nodeIndex Index du nœud actuel dans le QuadTree.
 * @param startX Coordonnée X de départ.
 * @param startY Coordonnée Y de départ.
 * @param size Taille de la zone actuelle.
 */
void createDataFromTree(QuadTree* tree, uint8_t* data, int width, int height, int nodeIndex, int startX, int startY, int size); 


/**
 * Reconstruit un QuadTree à partir des données compressées.
 * 
 * @param data Tableau contenant les données compressées.
 * @param tree Pointeur vers le QuadTree à remplir.
 */
void fillQuadTreeFromQTC(const uint8_t* data, QuadTree* tree); 


#endif
