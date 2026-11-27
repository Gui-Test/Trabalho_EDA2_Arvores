#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funcoes.h"

// compilação: gcc Main.c funcoes.h AVL.c ARN.c ARB.c -o Main.exe

int main() {
    int tam = 0;
    srand(time(NULL));
    /*
    for(int tam = 1000; tam<=10000;tam+=1000){
        for(int i = 0; i< tam; i++){
           int randchave = rand() % tam+1; 
        }
    }
    */
   
    for(int tam = 1000; tam<=10000;tam+=1000){
        AVL* Avl = cria();
        ArvoreARN* Arn = criaARN();
        ArvoreB* Ab1 = criaArvoreB(1);
        
        for(int i = 0; i< tam; i++){
           int randchave = rand() % (tam*10) ;
           printf("tam:%d\n", tam);
           printf("randchave:%d\n", randchave);
           adicionar(Avl, randchave);
           printf("avl,\n");
           adicionarARN(Arn, randchave);
           printf("arn,\n");
           adicionaChave(Ab1, randchave);
           printf("arb1,\n");

           printf("\n");
        }
        
        
        percorrer(Avl->raiz);
        printf("\n");
        percorrerInOrderARN(Arn,Arn->raiz,visitarARN);
        printf("\n");
        percorreArvoreB(Ab1->raiz);
        printf("\n\n");

        free(Avl);
        free(Arn);
        //liberarArvoreB(Ab1->raiz);
        free(Ab1);

    }
    
    

    return 0;
}

    

    //ArvoreB* AB5 = criaArvoreB(5);
    //ArvoreB* Ab10 = criaArvoreB(10);
    
    
    


    /*
    ArvoreARN* Arn = criaARN();
    adicionarARN(Arn, 9);
    percorrerInOrderARN(Arn,Arn->raiz,visitarARN);
    printf("\n");
    adicionarARN(Arn, 1);
    percorrerInOrderARN(Arn,Arn->raiz,visitarARN);
    printf("\n");
    adicionarARN(Arn, 5);
    percorrerInOrderARN(Arn,Arn->raiz,visitarARN);
    printf("\n");
    adicionarARN(Arn, 6);
    percorrerInOrderARN(Arn,Arn->raiz,visitarARN);
    printf("\n");   

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
    adicionaChave(arvore, 12);
    adicionaChave(arvore, 3);
    adicionaChave(arvore, 5);
    adicionaChave(arvore, 7);
    adicionaChave(arvore, 15);
    adicionaChave(arvore, 99);
    adicionaChave(arvore, 1);
    adicionaChave(arvore, 20);
    adicionaChave(arvore, 40);
    removeChave(arvore,5);
    adicionaChave(arvore, 25);
    removeChave(arvore,15);

    printf("chave (folha?) \n");
    percorreArvoreB(arvore->raiz); 
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
   

