#include <stdlib.h>
#include <stdio.h>
#include "funcoes.h"

AVL* cria() {
    AVL *Avl;
    Avl = malloc(sizeof(AVL));
    Avl->raiz = NULL;
    return Avl;
}

int vazia(AVL* AVL) {
    return (AVL->raiz == NULL);
}

/* No* adiciona(AVL* AVL, int valor) {
    No *no = malloc(sizeof(No));
    no->pai = NULL;
    no->direita = NULL;
    no->esquerda = NULL;
    no->valor = valor;
    no->altura = 0;

    int dir = 0;
    
    if (vazia(AVL)) {
        AVL->raiz = no;
        return no;
    }

    No *aux = malloc(sizeof(No));
    aux = AVL->raiz;
    while(aux != NULL){
        no->pai = aux;
        if(no->valor > aux->valor){
            aux = aux->direita;
            dir = 1;
        }else{
            aux = aux->esquerda;
            dir = 0;
        }
    }

    if(dir){
        no->pai->direita = no;
    }else{
        no->pai->esquerda = no;
    }

    aux = no;
    
    while(aux->pai != NULL){
       int fatorant = fb(aux);
       int fatordep = fb(aux->pai);
       printf("%i\n", valor);
       if(fatordep > 1){
            if(fatorant > 0){
                rsd(aux->pai);
            }else if(fatorant < 0){
                rdd(aux->pai);
            }
       }else if(fatordep < -1){
            if(fatorant < 0){
                rse(aux->pai);
            }else if(fatorant > 0){
                rde(aux->pai);
            }
       }

       if(aux->pai != NULL){
            aux = aux->pai;
       }
       

    }
 
    return no;
}*/

void percorrer(No* no) {
    if (no != NULL) {
        //printf("percorrer: %i ", no->valor);
        printf("%i ", no->valor);
        percorrer(no->esquerda);
        percorrer(no->direita);
    }
}

//Balanceamento
int altura(No* no){
    int esquerda = 0,direita = 0;
    if (no->esquerda != NULL) {
        esquerda = altura(no->esquerda) + 1;
    }
    if (no->direita != NULL) {
        direita = altura(no->direita) + 1;
    }
    return esquerda > direita ? esquerda : direita;
}

int fb(No* no) {
    int esquerda = 0,direita = 0;
    if (no->esquerda != NULL) {
        esquerda = altura(no->esquerda) + 1;
    }
    if (no->direita != NULL) {
        direita = altura(no->direita) + 1;
    }
    return esquerda - direita;
}

//Rotações
//Rotação Simples Esquerda
/**No* rse(No* no) {
    No* pai = no->pai;
    No* direita = no->direita;
    no->direita = direita->esquerda;
    no->pai = direita;
    direita->esquerda = no;
    direita->pai = pai;
    return direita;
}
//Rotação Simples Direita
No* rsd(No* no) {
    No* pai = no->pai;
    No* esquerda = no->esquerda;
    no->esquerda = esquerda->direita;
    no->pai = esquerda;
    esquerda->direita = no;
    esquerda->pai = pai;
    return esquerda;
}   
//Rotação Dupla a Esquerda
No* rde(No* no) {
    no->direita = rsd(no->direita);
    return rse(no);
}
//Rotação Dupla a Direita
No* rdd(No* no) {
    no->esquerda = rse(no->esquerda);
    return rsd(no);
}**/

//Minha versão das rotações
No* rse(No* no, AVL* a) {
    No* pai = no->pai;
    No* direita = no->direita;
    no->direita = direita->esquerda;
    no->pai = direita;
    direita->esquerda = no;
    direita->pai = pai;
    if(direita->pai== NULL){
        a->raiz = direita;
    }
    return direita;
}

No* rsd(No* no, AVL* a) {
    No* pai = no->pai;
    No* esquerda = no->esquerda;
    no->esquerda = esquerda->direita;
    no->pai = esquerda;
    esquerda->direita = no;
    esquerda->pai = pai;
    if(esquerda->pai== NULL){
        a->raiz = esquerda;
    }
    return esquerda;
}

No* rde(No* no, AVL* a) {
    no->direita = rsd(no->direita, a);
    return rse(no,a);
}  

No* rdd(No* no, AVL* a) {
    no->esquerda = rse(no->esquerda, a);
    return rsd(no, a);
}

//Checa o Balanceamento, aplicando as rotações necessárias
int checaNovo(No* no, AVL* a){
    int g, p;
    No* temp;
    if (no->pai == NULL){
        return 0;
    }
    g = fb(no->pai);
    if(g>1){
        int fbFilho = fb(no->pai->esquerda);
        if(fbFilho>0){
            rsd(no->pai, a);
        } else{
            rdd(no->pai, a);
        }
    } else if (g<-1){
        int fbFilho = fb(no->pai->direita);
        if(fbFilho<0){
            rse(no->pai, a);
        } else{
            rde(no->pai, a);
        }
    }else{
        checaNovo(no->pai, a);
    }
}

//processo interno de adicionar nó
No* adicionarNo(No* no, int valor) {
    if (valor > no->valor) {
        if (no->direita == NULL) {
            No* novo = malloc(sizeof(No));
            novo->valor = valor;
            novo->pai = no;
            novo->esquerda = NULL;
            novo->direita = NULL;

            no->direita = novo;
				
            return novo;
        } else {
            return adicionarNo(no->direita, valor);
        }
    } else if(valor < no->valor) {
        if (no->esquerda == NULL) {
            No* novo = malloc(sizeof(No));
			novo->valor = valor;
            novo->pai = no;
            novo->esquerda = NULL;
            novo->direita = NULL;
			
            no->esquerda = novo;
			
            return novo;
        } else {
            return adicionarNo(no->esquerda, valor);
        }
    }else{
        return NULL;
    }

}

//Processo de adicionar
No* adicionar(AVL* arvore, int valor) {
    No* no;
    int g =1;
    if (vazia(arvore)) {
        no = malloc(sizeof(No));
        no->valor = valor;
        no->pai = NULL;
        no->direita = NULL;
        no->esquerda = NULL;
        
        arvore->raiz = no;
    } else {
        no = adicionarNo(arvore->raiz, valor);
        if (no!=NULL){
            g = checaNovo(no, arvore);
        }
    }
    return no;
}

//Procura recursiva para achar o nó mais baixo à esquerda
No* procuraFundaEsq(No* no){
    if(!no->esquerda && !no->direita){
        return no;
    }else if(no->esquerda && no->direita){
        procuraFundaEsq(no->direita);
    }else if(!no->esquerda && no->direita){
        procuraFundaEsq(no->direita);
    }else{
        procuraFundaEsq(no->esquerda);
    }
}

//Removedor de nós
void removerNo(No* no, AVL* arvore){
    No* noNovo;
    if(!no->pai){
        if(!no->esquerda && !no->direita){
            vazia(arvore);
        //Raiz solo
        } else if(no->esquerda && no->direita){
            if(!no->esquerda->direita){
                no->esquerda->pai = NULL;
                arvore->raiz = no->esquerda;
                no->esquerda->direita = no->direita;
                no->direita->pai = no->esquerda;
                free(no);
            } else if(!no->direita->esquerda){
                no->direita->pai = NULL;
                arvore->raiz = no->direita;
                no->direita->esquerda = no->esquerda;
                no->esquerda->pai = no->direita;
                free(no);
            } else{
                noNovo = procuraFundaEsq(no->esquerda);
                no->valor = noNovo->valor;
                if(noNovo->pai->valor < noNovo->valor){
                    noNovo->pai->direita = NULL;
                    checaNovo(noNovo->pai,arvore);
                    free(noNovo);
                }else{
                    noNovo->pai->esquerda = NULL;
                    checaNovo(noNovo->pai,arvore);
                    free(noNovo);
                }
            }
        } else if(no->esquerda && !no->direita){
            no->esquerda->pai= NULL;
            arvore->raiz = no->esquerda;
            free(no);
        //Raiz com filho esquerdo
        } else{
            no->direita->pai= NULL;
            arvore->raiz = no->direita;
            free(no);
        //Raiz com filho direito
        }
    }else{
        if(!no->esquerda && !no->direita){
            if(no->pai->valor > no->valor){
                no->pai->esquerda = NULL;
                checaNovo(noNovo->pai,arvore);
                free(no);
            }else{
                no->pai->direita = NULL;
                checaNovo(noNovo->pai,arvore);
                free(no);
            }
            //sem filhos
        } else if (no->esquerda && no->direita){
            noNovo = procuraFundaEsq(no->esquerda);
            no->valor = noNovo->valor;
            if(noNovo->pai->valor < noNovo->valor){
                noNovo->pai->direita = NULL;
                free(noNovo);
            }else{
                noNovo->pai->esquerda = NULL;
                free(noNovo);
            }
            //Dois Filhos
        } else if (no->esquerda && !no->direita){
            if(no->pai->valor > no->valor){
                no->pai->esquerda = no->esquerda;
                no->esquerda->pai = no->pai;
                free(no);
            }else{
                no->pai->direita = no->esquerda;
                no->esquerda->pai = no->pai;
                free(no);
            }
            //Apenas Filho À Esquerda
        } else{
            if(no->pai->valor > no->valor){
                no->pai->esquerda = no->direita;
                no->direita->pai = no->pai;
                free(no);
            }else{
                no->pai->direita = no->direita;
                no->direita->pai = no->pai;
                free(no);
            }
            //Apenas Filho À Direita
        }   
    }
}

//Procura um nó para ver se ele existe, para daí remover
void procuraPraRemover(No* no, int a, AVL* arvore){
    if(a == no->valor){
        removerNo(no, arvore);
    } else{
        No *esq = no->esquerda;
        No *dire = no->direita;
        if(!esq  && !dire){
            printf("Esse no nao existe.");
        }
        if(a > no->valor){
            procuraPraRemover(no->direita, a, arvore);
        } else{
            procuraPraRemover(no->esquerda, a, arvore);
        }
    }
}