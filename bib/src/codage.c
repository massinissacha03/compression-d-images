#include "codage.h"


/**
 * @brief Encode un QuadTree dans un fichier binaire en utilisant un format compressé.
 * 
 * Cette fonction parcourt les noeud d'un QuadTree et écrit leurs informations 
 * dans un fichier en format compressé, en respectant les règles suivantes :
 * - Les feuilles qui sont des quatrièmes enfants ne sont pas codées.
 * - Les enfants d'un noeud uniforme ne sont pas codés.
 * - le m des noeuds qui sont quatrièmes enfants n'est pas codé.
 * - Chaque nœud encode ses valeurs `m` et `epsilon` .
 * - Si `epsilon == 0`, le champ `uniform` est également encodé.
 * 
 * @param file Pointeur vers le fichier où écrire les données compressées.
 * @param tree Pointeur vers le QuadTree à encoder.
 * @param bits_de_qtc Pointeur vers une variable pour compter le nombre total de bits écrits.
 * 
 * 
 */
void encoderQuadTree(FILE* file, QuadTree* tree , size_t * bits_de_qtc ) {  
    uint8_t buffer = 0; 
    int bitPos = 0;   

    for (int nodeIndex = 0; nodeIndex < tree->totalNodes; nodeIndex++) {
        QuadTreeNode* node = &tree->nodes[nodeIndex];
        if (nodeIndex != 0) { // sauter les enfant d'un noeud uniforme 
            QuadTreeNode* parentNode = getParentNode(tree, nodeIndex);
            if (parentNode != NULL && parentNode->uniform == 1) {
                continue; 
            }
        }
        if (isLeaf(tree, nodeIndex) && isFourthChild(nodeIndex)) {
            continue; // ne pas coder les feuilles qui sont des quatrièmes enfants
        }
        if (isLeaf(tree, nodeIndex) ) { // si c'est une feuille coder m 
            *bits_de_qtc += 8 ; //m codé sur 8 bits
            for (int i = 7; i >= 0; i--) {
                buffer = (buffer << 1) | ((node->m >> i) & 1);
                bitPos++;
                if (bitPos == 8) {
                    fwrite(&buffer, sizeof(uint8_t), 1, file);
                    buffer = 0;
                    bitPos = 0;
                }
            }
            continue; // passer au prochain nœud
        }
        // coder m
        if (!isFourthChild(nodeIndex)) {
            *bits_de_qtc += 8 ; 

            for (int i = 7; i >= 0; i--) {
                buffer = (buffer << 1) | ((node->m >> i) & 1);
                bitPos++;
                if (bitPos == 8) {
                    fwrite(&buffer, sizeof(uint8_t), 1, file);
                    buffer = 0;
                    bitPos = 0;
                }
            }
        }
        //   coder epsilon
        *bits_de_qtc += 2 ; //epsilon codé sur 2 bits 
        for (int i = 1; i >= 0; i--) {
            buffer = (buffer << 1) | ((node->epsilon >> i) & 1);
            bitPos++;
            if (bitPos == 8) {
                fwrite(&buffer, sizeof(uint8_t), 1, file);
                buffer = 0;
                bitPos = 0;
            }
        }
        if (node->epsilon == 0) {                    //je vais donc coder uniform
            *bits_de_qtc += 1 ; 

            buffer = (buffer << 1) | (node->uniform & 1);
            bitPos++;
            if (bitPos == 8) {
                fwrite(&buffer, sizeof(uint8_t), 1, file);
                buffer = 0;
                bitPos = 0;
            }
        }
    }

    // Écrire les bits restants dans le tampon (si incomplet)    
    if (bitPos > 0) {
        *bits_de_qtc += (8 - bitPos); 

        buffer <<= (8 - bitPos); // Compléter avec des zéros pour former un octet

        fwrite(&buffer, sizeof(uint8_t), 1, file);
    }
}
