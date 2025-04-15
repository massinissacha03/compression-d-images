#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "decodage.h"


/**
 * @brief Lit un fichier QTC et retourne les données binaires associées.
 * 
 * Cette fonction lit un fichier QTC . 
 * Elle extrait la taille de la structure à partir du premier octet, puis 
 * lit les données binaires restantes.
 * 
 * @param file Pointeur vers le fichier ouvert en mode lecture.
 * @param taille Pointeur où la taille lue sera stockée.
 * @return Un pointeur vers les données binaires lues (tableau d'octets), 
 * ou NULL en cas d'erreur.
 * 
 *  */
uint8_t* readQTCFile(FILE* file, int* taille) {
    if (!file) {
        fprintf(stderr, "Erreur : Fichier non valide\n");
        return NULL;
    }

    char line[256];

    // Lire et ignorer les trois premières lignes du fichier (en-tête QTC)
    for (int i = 0; i < 3; i++) {
        if (!fgets(line, sizeof(line), file)) {
            fprintf(stderr, "Erreur : Format de fichier QTC incorrect ou ligne manquante\n");
            return NULL;
        }
    }

    //  les 8 premiers bits (1 octet) pour la taille
    uint8_t sizeByte;
    if (fread(&sizeByte, sizeof(uint8_t), 1, file) != 1) {
        fprintf(stderr, "Erreur : Impossible de lire la taille dans le fichier QTC\n");
        return NULL;
    }

    
    *taille = sizeByte; 

    // Calculer la taille des données binaires restantes
    long currentPos = ftell(file); //position actuelle dans le fichier
    fseek(file, 0, SEEK_END);      //fin du fichier 
    long fileSize = ftell(file);  //taille du fichier 
    long binaryDataSize = fileSize - currentPos; // taille des donées binaire sans l'entete

    fseek(file, currentPos, SEEK_SET); // reprendre depuis la position actuelle

    if (binaryDataSize <= 0) {
        fprintf(stderr, "Erreur : Données binaires manquantes\n");
        return NULL;
    }

    uint8_t* data = (uint8_t*)malloc(binaryDataSize);
    if (!data) {
        perror("Erreur : Allocation mémoire échouée");
        return NULL;
    }

    if (fread(data, sizeof(uint8_t), binaryDataSize, file) != (size_t)binaryDataSize) {
        fprintf(stderr, "Erreur : Lecture des données binaires échouée\n");
        free(data);
        return NULL;
    }

    return data; 
}


/**
 * @brief Lit un nombre spécifié de bits depuis un tableau de données binaires.
 * 
 * Cette fonction lit `n` bits consécutifs depuis un tableau binaire, à partir 
 * d'un index donné. L'index est mis à jour après la lecture.
 * 
 * @param data Tableau contenant les données binaires.
 * @param bitIndex Pointeur vers l'index des bits à lire (mis à jour après lecture).
 * @param n Nombre de bits à lire.
 * @return La valeur des bits lus sous forme d'entier.
 */
static uint8_t readBits(const uint8_t* data, size_t* bitIndex, int n) {
    uint8_t value = 0;
    size_t byteOffset = *bitIndex / 8;
    int bitOffset = 7 - (*bitIndex % 8);



    //lecture des bits   
    for (int i = 0; i < n; i++) {
        // ajoute le bit actuel à la valeur
        value = (value << 1) | ((data[byteOffset] >> bitOffset) & 1);

        if (--bitOffset < 0) {
            bitOffset = 7; 
            byteOffset++;
        }
        (*bitIndex)++;
    }
    return value;
}


/**
 * @brief Remplit un QuadTree à partir de données compressées en format QTC.
 * 
 * Cette fonction reconstruit les les noeuds d'un QuadTree en utilisant les données binaires 
 * fournies. Elle suit les règles spécifiques de compression pour déterminer les valeurs 
 * des neoud.
 * 
 * @param data Tableau contenant les données binaires QTC.
 * @param tree Pointeur vers le QuadTree à remplir.
 * 
 */
void fillQuadTreeFromQTC(const uint8_t* data, QuadTree* tree) {
    if (!tree || !data) {
        fprintf(stderr, "Erreur : QuadTree ou données binaires nulles\n");
        return;
    }

    size_t bitIndex = 0; //  position des bits 
    int nodeIndex = 0;  

    while (nodeIndex < tree->totalNodes) {
        QuadTreeNode* node = &tree->nodes[nodeIndex];

        //si le parent est uniforme on attribue m du parent directement à ce noeud
        
        QuadTreeNode* parent = getParentNode(tree, nodeIndex); 

        if (parent && parent->uniform == 1) {

            node->m = parent->m;
            node->epsilon = 0;
            node->uniform = 1;
            nodeIndex++;
            continue; 
        }

        if (isLeaf(tree, nodeIndex)) { //si c'est une feuille on ne peut lire que `m`
            
            if (!isFourthChild(nodeIndex)) { //on sait que le 4ème noeud n'est pas codé 

                node->m = readBits(data, &bitIndex, 8);
            } else {
                QuadTreeNode* firstChild = &tree->nodes[nodeIndex - 3];
                QuadTreeNode* secondChild = &tree->nodes[nodeIndex - 2];
                QuadTreeNode* thirdChild = &tree->nodes[nodeIndex - 1];

                node->m = (4 * parent->m + parent->epsilon) -
                          (firstChild->m + secondChild->m + thirdChild->m);
            }
            node->epsilon = 0;
            node->uniform = 1; // Par défaut pour les feuilles
      
        } else { // cas des noeuds internes 

            if (!isFourthChild(nodeIndex)) { 
                //  m est codé pour trois enfants 
                node->m = readBits(data, &bitIndex, 8);
            } else {
                // calcul du `m4` 
                if (!parent) {
                    fprintf(stderr, "Erreur : Nœud parent non trouvé pour le 4ème fils\n");
                    return;
                }

                QuadTreeNode* firstChild = &tree->nodes[nodeIndex - 3];
                QuadTreeNode* secondChild = &tree->nodes[nodeIndex - 2];
                QuadTreeNode* thirdChild = &tree->nodes[nodeIndex - 1];

                node->m = (4 * parent->m + parent->epsilon) -
                          (firstChild->m + secondChild->m + thirdChild->m);
            }

            // lire `epsilon` 
            node->epsilon = readBits(data, &bitIndex, 2);

            if (node->epsilon == 0) { // si epsilon est de valeur 0 donc uniforme et codé et on dot le lire 
                node->uniform = readBits(data, &bitIndex, 1);
            } else { // si epsilon ne vaut pas 0 , uniform=0 automatiquement

                node->uniform = 0;
            }
        }

        nodeIndex++; 
    }

}


/**
 * @brief Génère les données d'image à partir d'un QuadTree.
 * 
 * Cette fonction remplit un tableau représentant une image en divisant l'image en blocs 
 * selon la structure du QuadTree. Les feuilles du QuadTree contiennent les intensités 
 * à appliquer aux blocs correspondants.
 * 
 * @param tree Pointeur vers le QuadTree contenant les données.
 * @param data Tableau de données représentant l'image à remplir.
 * @param width Largeur de l'image.
 * @param height Hauteur de l'image.
 * @param nodeIndex Index du nœud actuel dans le QuadTree.
 * @param startX Coordonnée X de départ pour le bloc courant.
 * @param startY Coordonnée Y de départ pour le bloc courant.
 * @param size Taille du bloc courant (longueur du côté).
 */
void createDataFromTree(QuadTree* tree, uint8_t* data, int width, int height, int nodeIndex, int startX, int startY, int size) {
    if (isLeaf(tree, nodeIndex)) {
        // Si c'est une feuille, remplir le bloc correspondant dans les données de l'image
        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                data[(startY + y) * width + (startX + x)] = tree->nodes[nodeIndex].m;
            }
        }
        return;
    }

    // Nœud interne : diviser en 4 sous-blocs
    int halfSize = size / 2;
    int childIndex = 4 * nodeIndex + 1;

    // Haut-gauche
    createDataFromTree(tree, data, width, height, childIndex, startX, startY, halfSize);
    // Haut-droit
    createDataFromTree(tree, data, width, height, childIndex + 1, startX + halfSize, startY, halfSize);
    // Bas-droit
    createDataFromTree(tree, data, width, height, childIndex + 2, startX + halfSize, startY + halfSize, halfSize);
    // Bas-gauche
    createDataFromTree(tree, data, width, height, childIndex + 3, startX, startY + halfSize, halfSize);
}

