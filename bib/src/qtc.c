#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#include "codage.h"
#include "decodage.h"
#include "segmentation.h"
#include "filtrage.h"
#include "image.h"
#include "Quadtree.h"


/**
 * Affiche l'utilisation du programme à l'utilisateur.
 * 
 * @param executable Nom du programme (argv[0]) (codec)
 */
void printUsage(const char* executable) {
    printf("Usage: %s [-c|-u] -i <input_file> [-o <output_file>] [-a <alpha>] [-g] [-h] [-v]\n", executable);
    printf("Options:\n");
    printf("  -c            Encodeur\n");
    printf("  -u            Decodeur\n");
    printf("  -i <file>     Fichier d'entrée (PGM ou QTC)\n");
    printf("  -o <file>     Fichier de sortie (QTC ou PGM)\n");
    printf("  -a <alpha>    Valeur alpha pour l'encodage avec perte (par defaut: 1.5)\n");
    printf("  -g            Editer la grille de segmentation\n");
    printf("  -h            Affiche cette aide\n");
    printf("  -v            Mode bavard\n");
}


/**
 * @brief Génère une grille de segmentation à partir d'un QuadTree et l'écrit dans un fichier PGM.
 * 
 * Cette fonction crée une grille de segmentation basée sur la structure du QuadTree,
 * puis l'enregistre dans un fichier image au format PGM. La grille représente les segments
 * définis par le QuadTree, avec chaque segment marqué par une intensité différente.
 * 
 * @param tree Pointeur vers le QuadTree utilisé pour générer la grille.
 * @param outputFile Nom de base pour le fichier de sortie. Le fichier de grille aura "_g.pgm" ajouté à ce nom.
 * @param width Largeur de la grille (et hauteur, car la grille est carrée).
 * @param bavard Mode bavard (si différent de 0, affiche des messages détaillés).
 */
static void handleGrid(QuadTree* tree, const char* outputFile, int width, int bavard) {
    printf("\n\nGénération de la grille de segmentation pour %s\n", outputFile);

    char gridOutput[256];
    snprintf(gridOutput, sizeof(gridOutput), "%s_g.pgm", outputFile);

    uint8_t* grid = calloc(width * width, sizeof(uint8_t));
    if (!grid) {
        perror("Erreur : Allocation mémoire pour la grille");
        return;
    }

    memset(grid, 255, width * width); // Initialiser la grille en blanc
    generateSegmentationGrid(tree, grid, width, width, 0, 0, 0, width);

    FILE* gridFile = fopen(gridOutput, "wb");
    if (gridFile) {
        fprintf(gridFile, "P5\n%d %d\n255\n", width, width);
        fwrite(grid, sizeof(uint8_t), width * width, gridFile);
        fclose(gridFile);
        if (bavard) printf("Grille de segmentation écrite dans %s\n", gridOutput);
    } else {
        perror("Erreur : Impossible de créer le fichier de la grille");
    }

    free(grid);
     printf("Grille de segmentation générée avec succès\n");
}


/**
 * Gère le processus d'encodage d'un fichier QTC
 * 
 * @param inputFile Nom du fichier à encoder.
 * @param outputFile Nom du fichier de sortie où écrire les données encodées.
 * @param alpha facteur utilisé dans le cas d'un codage avec avec perte 
 * @param generateGrid utilisé pour savoir si l'option -g de génération de la grille de segemntation est activée.s
 * @param bavard Si différent de 0, affiche des informations détaillées pendant l'exécution.
 */
void handleEncoding(const char* inputFile, const char* outputFile, double alpha, int generateGrid, int bavard) {
    printf("\n\nEncodage en cours : fichier %s\n\n", inputFile);
    int size, maxval;
    size_t dataSizePGM;
    uint8_t* data = readPGMFile(inputFile, &size, &maxval, &dataSizePGM);
    if (!data) {
        fprintf(stderr, "Erreur : Impossible de lire le fichier PGM\n");
        exit(EXIT_FAILURE);
    }
    if (bavard) printf("Lecture réussie du fichier PGM : taille %dx%d, maxval %d\n", size, size, maxval);
    int depth = calculateDepth(size);
    QuadTree* tree = createQuadTree(depth);
    if (!tree) {
        free(data);
        fprintf(stderr, "Erreur : Impossible de créer le QuadTree\n");
        exit(EXIT_FAILURE);
    }

    if (bavard) printf("QuadTree initialisé avec profondeur %d\n", depth);

    fillQuadTree(tree, data, size, size, depth, 0, 0, 0, size);
    if (bavard) printf("QuadTree rempli avec les données de l'image\n");

    if (alpha > 0) {
        double medvar, maxvar;
        avgAndMaxVars(tree, &medvar, &maxvar);
        if (bavard) printf("Filtrage appliqué avec alpha = %.2f\n", alpha);
        filtrage(tree, 0, medvar / maxvar, alpha);
    }

    FILE* output = fopen(outputFile, "wb+");
    if (!output) {
        perror("Erreur : Impossible de créer le fichier de sortie");
        freeQuadTree(tree);
        free(data);
        exit(EXIT_FAILURE);
    }

    // Écrire un en-tête temporaire avec une longueur fixe pour le taux de compression
    time_t t = time(NULL);
    char placeholderHeader[256];
    snprintf(placeholderHeader, sizeof(placeholderHeader), 
             "Q1\n# %s# compression rate %6s%%\n", ctime(&t), "00.00");
    fwrite(placeholderHeader, sizeof(char), strlen(placeholderHeader), output);

    uint8_t taille = (uint8_t)depth;
    fwrite(&taille, sizeof(uint8_t), 1, output);

    // Encodage et calcul de la taille en bits
    size_t dataSizeQTC = 0;
    encoderQuadTree(output, tree, &dataSizeQTC);

    double TO = (double)dataSizeQTC * 100 / (dataSizePGM * 8);

    // Réécrire le taux de compression dans l'en-tête
    fseek(output, 0, SEEK_SET); // Retour au début
    char updatedHeader[256];
    snprintf(updatedHeader, sizeof(updatedHeader), 
             "Q1\n# %s# compression rate %6.2f%%\n", ctime(&t), TO);
    fwrite(updatedHeader, sizeof(char), strlen(updatedHeader), output);

    fclose(output);

    if (bavard) printf("QuadTree encodé avec un taux de compression de %.2f%%\n", TO);

    if (generateGrid) {
        handleGrid(tree, outputFile, size, bavard);
    }

    freeQuadTree(tree);
    free(data);

    printf("\nEncodage terminé\n");
}


/**
 * Gère le processus de décodage d'un fichier QTC en PGM.
 * 
 * @param inputFile Nom du fichier.pgm à décoder.
 * @param outputFile Nom du fichier de sortie où écrire les données décodées.
 * @param generateGrid utilisé pour savoir si l'option -g de génération de la grille de segemntation est activée.
 * @param bavard Si différent de 0, affiche des informations détaillées pendant l'exécution.
 */
void handleDecoding(const char* inputFile, const char* outputFile, int generateGrid, int bavard) {
    printf("\n\nDécodage en cours : fichier %s\n\n", inputFile);
    FILE* input = fopen(inputFile, "rb");
    if (!input) {
        perror("Erreur : Impossible d'ouvrir le fichier d'entrée");
        exit(EXIT_FAILURE);
    }
    int taille;
    uint8_t* data = readQTCFile(input, &taille);
    if (!data) {
        fclose(input);
        exit(EXIT_FAILURE);
    }
    if(bavard) printf("lecture du fichier réussie du fichier .qtc donné... \n") ; 

    // Créer et remplir le QuadTree à partir des données QTC
    QuadTree* tree = createQuadTree(taille);
    if (!tree) {
        free(data);
        fclose(input);
        fprintf(stderr, "Erreur : Impossible de créer le QuadTree.\n");
        exit(EXIT_FAILURE);
    }
    if(bavard) printf("création d'un abre quadtree vide de taille %d\n" , taille) ; 
    fillQuadTreeFromQTC(data, tree);

    if(bavard) printf("remplissage de l'arbre quatree \n") ; 


    // Calculer la largeur de l'image (2^taille)
    int width = 1 << taille;

    // Allouer la mémoire pour l'image
    uint8_t* image = malloc(width * width);
    if (!image) {
        free(data);
        freeQuadTree(tree);
        fclose(input);
        fprintf(stderr, "Erreur : Allocation mémoire pour l'image échouée.\n");
        exit(EXIT_FAILURE);
    }

    // Reconstruire les données de l'image à partir du QuadTree
    createDataFromTree(tree, image, width, width, 0, 0, 0, width);

    // Écrire les données de l'image dans le fichier de sortie
    if (writePGMFile(outputFile, image, width, width, 255) != 0) {
        fprintf(stderr, "Erreur : Échec de l'écriture du fichier PGM\n");
        free(image);
        free(data);
        freeQuadTree(tree);
        fclose(input);
        exit(EXIT_FAILURE);
    }

    if (bavard) printf("Image décodée avec succès dans %s\n", outputFile);

    // Générer la grille de segmentation si demandé
    if (generateGrid) {
        handleGrid(tree, outputFile, width, bavard);
    }

    // Libérer la mémoire et fermer les fichiers
    free(image);
    free(data);
    freeQuadTree(tree);
    fclose(input);

    printf("\nDécodage terminé.\n");
}
