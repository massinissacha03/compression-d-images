#include <stdio.h>

#include "image.h"


/**
 * Lit un fichier image au format PGM.
 * 
 * @param filename Nom du fichier PGM à lire.
 * @param size Pointeur pour stocker la taille de l'image.
 * @param maxval Pointeur pour stocker le niveau de gris de la PGM
 * @param dataSizePGM Pointeur pour stocker la taille des données en octets.
 * @return Un tableau d'octets contenant les données de l'image.
 */
uint8_t* readPGMFile(const char* filename, int* size, int* maxval, size_t* dataSizePGM) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    // Lire la première ligne P5
    char line[256];
    if (fgets(line, sizeof(line), file) == NULL || line[0] != 'P' || line[1] != '5') {
        fprintf(stderr, "Erreur : Format PGM non valide\n");
        fclose(file);
        return NULL;
    }

    // ignorer  les commentaires
    do {
        if (fgets(line, sizeof(line), file) == NULL) {
            fprintf(stderr, "Erreur : En-tête du fichier incorrect\n");
            fclose(file);
            return NULL;
        }
    } while (line[0] == '#');

    // Lire les dimensions et éventuellement le niveau de gris
    int width = 0, height = 0;
    if (sscanf(line, "%d %d %d", &width, &height, maxval) == 3) {
        // Cas où width, height et maxval sont sur la même ligne

    } else if (sscanf(line, "%d %d", &width, &height) == 2) {
        // Cas où seulement width et height sont sur la ligne
        // Lire le niveau de gris sur la ligne suivante
        if (fgets(line, sizeof(line), file) == NULL || sscanf(line, "%d", maxval) != 1) {
            fprintf(stderr, "Erreur : Niveau de gris introuvable\n");
            fclose(file);
            return NULL;
        }
    } else {
        fprintf(stderr, "Erreur : Dimensions introuvables\n");
        fclose(file);
        return NULL;
    }

    // Vérification de la validité des dimensions et du niveau de gris
    if (width != height) {
        fprintf(stderr, "Erreur : L'image doit être carrée\n");
        fclose(file);
        return NULL;
    }
    *size = width;

    if (*maxval <= 0 || *maxval > 255) {
        fprintf(stderr, "Erreur : Niveau maximal invalide (%d)\n", *maxval);
        fclose(file);
        return NULL;
    }

    // Lire les données brutes
    *dataSizePGM = width * height;
    uint8_t* data = (uint8_t*)malloc(*dataSizePGM);
    if (!data) {
        perror("Erreur lors de l'allocation mémoire");
        fclose(file);
        return NULL;
    }

    if (fread(data, sizeof(uint8_t), *dataSizePGM, file) != *dataSizePGM) {
        fprintf(stderr, "Erreur lors de la lecture des données brutes\n");
        free(data);
        fclose(file);
        return NULL;
    }

    fclose(file);
    return data;
}

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
int writePGMFile(const char* filename, const uint8_t* data, int width, int height, int maxval) {
    if (!filename || !data) {
        fprintf(stderr, "Erreur : Paramètres invalides pour writePGMFile\n");
        return -1;
    }

    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        return -1;
    }

    // Écrire l'en-tête PGM
    fprintf(file, "P5\n");
    fprintf(file, "%d %d\n", width, height); // Largeur et hauteur
    fprintf(file, "%d\n", maxval);           // Niveau de gris maximal

    // Écrire les données d'image
    size_t dataSize = width * height;
    if (fwrite(data, sizeof(uint8_t), dataSize, file) != dataSize) {
        fprintf(stderr, "Erreur lors de l'écriture des données d'image dans %s\n", filename);
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}
