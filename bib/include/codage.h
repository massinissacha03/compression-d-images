#ifndef __CODAGE__
#define __CODAGE__

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include "Quadtree.h"


/**
 * @brief Encode un QuadTree dans un fichier 
 * 
 * Cette fonction permet d'encoder un QuadTree en écrivant sa structure dans un fichier 
 * sous une forme compressée et elle permet aussi de calculer 
 * le nombre de bits des données codées utilisé pour calculer le taux de compression.
 * 
 * @param file Pointeur vers le fichier où l'arbre sera écrit. Le fichier doit être 
 * ouvert en écriture binaire avant l'appel de cette fonction.
 * @param tree Pointeur vers le QuadTree à encoder. 
 * @param bits_de_qtc Pointeur vers une variable de type `size_t` où le nombre total 
 * de bits utilisés pour l'encodage sera stocké.
 * 
 */
void encoderQuadTree(FILE* file, QuadTree* tree , size_t * bits_de_qtc);


#endif // __CODAGE__ß