#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "AVL.h"
#include "ARN.h"
#include "ARB.h"




int main() {
    
    ArvoreB* arvore = criaArvoreB(2);
    
    adicionaChave(arvore, 12);
    adicionaChave(arvore, 3);
    adicionaChave(arvore, 5);
    adicionaChave(arvore, 7);
    adicionaChave(arvore, 15);
    adicionaChave(arvore, 99);
    adicionaChave(arvore, 1);
    adicionaChave(arvore, 20);
    adicionaChave(arvore, 40);
    removeChave(arvore,15);

    printf("chave (folha?) \n");
    percorreArvoreB(arvore->raiz); 
    


    /*
    ArvoreARN* arvoreARN = criaARN();
    adicionarARN(arvoreARN, 10);
    adicionarARN(arvoreARN, 11);
    adicionarARN(arvoreARN, 2);
    adicionarARN(arvoreARN, 8);
    adicionarARN(arvoreARN, 9);
    percorrerInOrderARN(arvoreARN, arvoreARN->raiz, visitarARN);
    removerARN(arvoreARN,8);
    printf("\n");
    percorrerInOrderARN(arvoreARN, arvoreARN->raiz, visitarARN);
        */


    /*
    Teste de Mesa 12,3,5,7,15,99,1,20,40
    
    12) [][12][]

    3)  [][3][][12][]
    
    5)  
            [][5][]
           /      \
        [][3][] [][12][]

    7)
            [][5][]
           /      \
        [][3][] [][7][][12][]
    
    15)     
            [][5][][12][]
           /      \      \
        [][3][] [][7][] [][15][]

    99)     
            [][5][][12][]
           /      \      \
        [][3][] [][7][] [][15][][99][]
    
    1)
                [][5][][12][]
                /     \      \
        [][1][][3][] [][7][] [][15][][99][]
    
    20)
                [][5][][12][]
                /     \      \
        [][1][][3][] [][7][] [][15][][20][][99][]

                [][5][][12][][20][]
                /     \      \
        [][1][][3][] [][7][] [][15][][99][]

                        [][12][]
                      /          \
                [][5][]          [][20][]
                /     \          /        \
        [][1][][3][] [][7][] [][15][]     [][99][]

    */ 
   
}
