#include "filtrage.h"


/**
 * Calcule la variance moyenne et maximale des blocs dans un QuadTree.
 * 
 * @param tree Pointeur vers un QuadTree.
 * @param medvar Pointeur où la variance moyenne sera stockée.
 * @param maxvar Pointeur où la variance maximale sera stockée.
 */
void avgAndMaxVars(QuadTree* tree, double* medvar, double* maxvar) {
    int nodeIndex = 0;
    double sumvars = 0.0;
    double maxVar = 0.0;
    int totalNode =0 ; 

    for (int level = 0; level <= tree->depth; level++) {
        int nodesAtLevel = (1 << (2 * level));

        for (int i = 0; i < nodesAtLevel && nodeIndex < tree->totalNodes; i++, nodeIndex++) {
            if (tree->nodes[nodeIndex].var > maxVar) maxVar = tree->nodes[nodeIndex].var;
            sumvars += tree->nodes[nodeIndex].var;
            if(!isLeaf(tree , nodeIndex)) totalNode +=1 ; 
        }
    }

    *medvar = sumvars/totalNode;
    *maxvar = maxVar;
}


/**
 * Applique un filtrage sur un QuadTree en fonction de sa variance moyenne et maximale
 * 
 * @param tree Pointeur vers le QuadTree à filtrer.
 * @param nodeIndex Index du nœud actuel dans le QuadTree.
 * @param sigma Seuil de variance pour le filtrage.
 * @param alpha Facteur pour ajuster le filtrage.
 * @return 1 si le filtrage a été appliqué avec succès, 0 sinon.
 */
int filtrage (QuadTree * tree , int nodeIndex , double sigma , double alpha ) {  // must return 0 or 1 
    QuadTreeNode * node = &tree->nodes[nodeIndex]; 

    if( node->uniform == 1) return 1 ; 
    
    if (isLeaf(tree , nodeIndex)) return 1 ; 

    int childIndex = 4 * nodeIndex+1 ;

    int s = 0 ; 
    s += filtrage(tree , childIndex ,  sigma*alpha  , alpha); 
    s += filtrage(tree , childIndex+ 1 ,  sigma*alpha  , alpha); 
    s += filtrage(tree , childIndex +2 ,  sigma*alpha  , alpha); 
    s += filtrage(tree , childIndex + 3,  sigma*alpha  , alpha); 
    
    if ( s < 4) return 0 ; 
    if( node->var > sigma) return 0 ;


    
    node->epsilon = 0 ; 
    node-> uniform = 1 ; 

  
    return 1 ; 
}
