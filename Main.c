#include "ARB.h"
#include "time.h"

int main() {
    ArvoreB* arvore = criaArvoreB(1);
    
    adicionaChave(arvore, 12);
    adicionaChave(arvore, 3);
    adicionaChave(arvore, 5);
    adicionaChave(arvore, 7);
    adicionaChave(arvore, 15);
    adicionaChave(arvore, 99);
    adicionaChave(arvore, 1);
    adicionaChave(arvore, 20);

    percorreArvoreB(arvore->raiz);

    /*
    Teste de Mesa
    
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
