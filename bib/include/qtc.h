#ifndef QTC_H
#define QTC_H


/**
 * Affiche l'utilisation du programme à l'utilisateur.
 * 
 * @param executable Nom du programme (argv[0]) (codec)
 */
void printUsage(const char* executable) ;



/**
 * Gère le processus d'encodage d'un fichier QTC
 * 
 * @param inputFile Nom du fichier à encoder.
 * @param outputFile Nom du fichier de sortie où écrire les données encodées.
 * @param alpha facteur utilisé dans le cas d'un codage avec avec perte 
 * @param generateGrid utilisé pour savoir si l'option -g de génération de la grille de segemntation est activée.s
 * @param bavard Si différent de 0, affiche des informations détaillées pendant l'exécution.
 */
void handleEncoding(const char* inputFile, const char* outputFile, double alpha, int generateGrid, int bavard) ;


/**
 * Gère le processus de décodage d'un fichier QTC en PGM.
 * 
 * @param inputFile Nom du fichier.pgm à décoder.
 * @param outputFile Nom du fichier de sortie où écrire les données décodées.
 * @param generateGrid utilisé pour savoir si l'option -g de génération de la grille de segemntation est activée.
 * @param bavard Si différent de 0, affiche des informations détaillées pendant l'exécution.
 */
void handleDecoding(const char* inputFile, const char* outputFile, int generateGrid, int bavard) ;


#endif 

