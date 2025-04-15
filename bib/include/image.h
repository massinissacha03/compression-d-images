
#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdlib.h>  


/**
 * Lit un fichier image au format PGM.
 * 
 * @param filename Nom du fichier PGM à lire.
 * @param size Pointeur pour stocker la taille de l'image.
 * @param maxval Pointeur pour stocker le niveau de gris de la PGM
 * @param dataSizePGM Pointeur pour stocker la taille des données en octets.
 * @return Un tableau d'octets contenant les données de l'image.
 */

uint8_t* readPGMFile(const char* filename, int* size, int* maxval , size_t * dataSizePGM );

/**
 * Écrit une image au format PGM dans un fichier.pgm
 * 
 * @param filename Nom du fichier PGM à écrire.
 * @param data Tableau contenant les données des pixels de l'image.
 * @param width Largeur de l'image.
 * @param height Hauteur de l'image.
 * @param maxval Valeur maximale des pixels.
 * @return 0 en cas de succès, une valeur non nulle en cas d'erreur.
 */
int writePGMFile(const char* filename, const uint8_t* data, int width, int height, int maxval) ;

#endif

