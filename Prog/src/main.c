
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <qtc.h>





/**
 * @brief Point d'entrée principal du programme de compression/décompression QTC.
 * 
 * Ce programme permet d'encoder ou de décoder des fichiers à l'aide du format QTC.
 * Les options disponibles incluent la spécification des fichiers d'entrée/sortie, 
 * le mode bavard, la génération de grille et la gestion d'un paramètre alpha.
 * 
 * Options :
 * - `-c` : Encode un fichier au format QTC.
 * - `-u` : Décode un fichier QTC.
 * - `-i <fichier>` : Spécifie le fichier d'entrée.
 * - `-o <fichier>` : Spécifie le fichier de sortie (optionnel).
 * - `-a <valeur>` : Spécifie la valeur d'alpha (optionnel pour l'encodage).
 * - `-g` : Génère une grille de segmentation.
 * - `-v` : Active le mode bavard.
 * - `-h` : Affiche l'aide.
 * 
 * @param argc Nombre d'arguments passés en ligne de commande.
 * @param argv Tableau des arguments sous forme de chaînes.
 * @return 0 en cas de succès, une valeur non nulle en cas d'erreur.
 */


int main(int argc, char* argv[]) {
    int isEncode = 0, isDecode = 0, generateGrid = 0, bavard = 0;
    const char *inputFile = NULL, *outputFile = NULL;
    double alpha = -1; // Alpha par défaut désactivé

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0)  isEncode = 1;
        
        else if (strcmp(argv[i], "-u") == 0) isDecode = 1;

        else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) inputFile = argv[++i];
        
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) outputFile = argv[++i];
        
        else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) alpha = atof(argv[++i]);
        
        else if (strcmp(argv[i], "-g") == 0) generateGrid = 1;
        
        else if (strcmp(argv[i], "-v") == 0)  bavard = 1;
        
        else if (strcmp(argv[i], "-h") == 0) {
            printUsage(argv[0]);
            return EXIT_SUCCESS;
        } else {
            fprintf(stderr, "Option inconnue : %s\n", argv[i]);
            printUsage(argv[0]);
            return EXIT_FAILURE;
        }
    }
    if (!isEncode && !isDecode) {
        fprintf(stderr, "Erreur : Vous devez choisir -c (encodeur) ou -u (decodeur).\n");
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }
    if (!inputFile) {
        fprintf(stderr, "Erreur : Vous devez specifier un fichier d'entree avec -i.\n");
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }
    if (!outputFile) {
        outputFile = isEncode ? "out.qtc" : "out.pgm";
    }
    if (isEncode) 
        handleEncoding(inputFile, outputFile, alpha, generateGrid, bavard);
    else if (isDecode) 
        handleDecoding(inputFile, outputFile, generateGrid, bavard);
    
    return EXIT_SUCCESS;
}
