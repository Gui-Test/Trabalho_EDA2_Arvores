#include <stdlib.h>
#include <stdio.h>
#include "funcoes.h"

int vaziaARN(ArvoreARN* ARB) {
    return (ARB->raiz == NULL);
}

NoARN* criarNoARN(ArvoreARN* arvore, NoARN* pai, int valor) {
    NoARN* no = (NoARN *)malloc(sizeof(NoARN));

    no->pai = pai;    
    no->valor = valor;
    no->direita = arvore->nulo;
    no->esquerda = arvore->nulo;
    no->cor = Vermelho;

    return no;
}

ArvoreARN* criaARN() {
    ArvoreARN* arvore = (ArvoreARN *) malloc(sizeof(ArvoreARN));
    arvore->nulo = NULL;
    arvore->raiz = NULL;

    arvore->nulo = criarNoARN(arvore, NULL, 0);
    arvore->nulo->cor = Preto;

    return arvore;
}

void adicionarARN(ArvoreARN* arvore, int valor) {
    if (vaziaARN(arvore)) {
        arvore->raiz = criarNoARN(arvore, arvore->nulo, valor);
        arvore->raiz->cor = Preto;
    } else {

    NoARN* no = arvore->raiz;

    while (no != arvore->nulo) {
        if (valor > no->valor) {
            if (no->direita != arvore->nulo) {
                no = no->direita;
            } else {
                break;
            }
        } else {
            if (no->esquerda != arvore->nulo) {
                no = no->esquerda;
            } else {
                break;
            }
        }
    }

    //printf("Adicionando %d\n", valor);
    NoARN* novo = criarNoARN(arvore,no,valor);

    if (valor > no->valor) {
            no->direita = novo;
    } else {
            no->esquerda = novo;
    }
        
    balancearARN(arvore, novo);
    }
}

NoARN* localizarARN(NoARN* no, int valor) {
    while (no != NULL) {
        if (no->valor == valor) {
            return no;
        }
        
        no = valor < no->valor ? no->esquerda : no->direita;
    }

    return NULL;
}

void percorrerInOrderARN(ArvoreARN* arvore,NoARN* no, void (*callback)(int)) {
    if (no != arvore->nulo) {
        percorrerInOrderARN(arvore,no->esquerda,callback);
        callback(no->valor);
        percorrerInOrderARN(arvore,no->direita,callback);
    }
}

void percorrerPreOrderARN(ArvoreARN* arvore, NoARN* no, void (*callback)(int)) {
    if (no != arvore->nulo) {
        callback(no->valor);
        percorrerPreOrderARN(arvore, no->esquerda,callback);
        percorrerPreOrderARN(arvore, no->direita,callback);
    }
}

void visitarARN(int valor){
    printf("%d ", valor);
}

void balancearARN(ArvoreARN* arvore, NoARN* no) {
    while (no->pai->cor == Vermelho) { //Garante que todos os níveis foram balanceados
        if (no->pai == no->pai->pai->esquerda) {
            NoARN *tio = no->pai->pai->direita;
            if (tio->cor == Vermelho) {
                tio->cor = Preto; //Resolve caso 2
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                no = no->pai->pai; //Vai para o nível anterior
            } else {
                if (no == no->pai->direita) {
                    no = no->pai; //Vai para o nível anterior
                    rotacionarEsquerdaARN(arvore, no); //Resolve caso 3
                } else {
                    no->pai->cor = Preto; //Resolve caso 4
                    no->pai->pai->cor = Vermelho;
                    rotacionarDireitaARN(arvore, no->pai->pai);
                }
            }
        } else {
            //Repete o mesmo código do bloco if, invertendo o lado dos direita e esquerda
            if (no->pai == no->pai->pai->direita){
                NoARN *tio = no->pai->pai->esquerda;
                if (tio->cor == Vermelho) {
                    tio->cor = Preto; //Resolve caso 2

                    no->pai->cor = Preto;
                    no->pai->pai->cor = Vermelho;
                    no = no->pai->pai; //Vai para o nível anterior
                } if (no == no->pai->esquerda) {
                    no = no->pai; //Vai para o nível anterior
                    rotacionarDireitaARN(arvore, no); //Resolve caso 3
                } else {
                    no->pai->cor = Preto; //Resolve caso 4
                    no->pai->pai->cor = Vermelho;
                    rotacionarEsquerdaARN(arvore, no->pai->pai);
                }
            }
            }
        }
        arvore->raiz->cor = Preto; //Resolve caso 1
}

void rotacionarEsquerdaARN(ArvoreARN* arvore, NoARN* no) {
    NoARN* direita = no->direita;
    no->direita = direita->esquerda;
    
    if (direita->esquerda != arvore->nulo)
        direita->esquerda->pai = no; //Se houver filho à esquerda em direita, ele será pai do nó

    direita->pai = no->pai; //Ajusta no pai do nó à direita

    if (no->pai == arvore->nulo)
        arvore->raiz = direita; //Se nó for raiz, o nó direita será a nova raiz da árvore
    else if (no == no->pai->esquerda)
        no->pai->esquerda = direita; //Corrige relação pai-filho do novo pai (esquerda)
    else
        no->pai->direita = direita; //Corrige relação pai-filho do novo pai (direita)

    direita->esquerda = no; //Corrige relação pai-filho entre o nó pivô e o nó à direita
    no->pai = direita;
}

void rotacionarDireitaARN(ArvoreARN* arvore, NoARN* no) {
    NoARN* esquerda = no->esquerda;
    no->esquerda = esquerda->direita;
    
    if (esquerda->direita != arvore->nulo)
        esquerda->direita->pai = no; //Se houver filho à direita em esquerda, ele será pai do nó

    esquerda->pai = no->pai; //Ajusta no pai do nó à esquerda

    if (no->pai == arvore->nulo)
        arvore->raiz = esquerda; //Se nó for raiz, o nó esquerda será a nova raiz da árvore
    else if (no == no->pai->esquerda)
        no->pai->esquerda = esquerda; //Corrige relação pai-filho do novo pai (esquerda)
    else
        no->pai->direita = esquerda; //Corrige relação pai-filho do novo pai (direita)
    
    esquerda->direita = no; //Corrige relação pai-filho entre o nó pivô e o nó à esquerda
    no->pai = esquerda;
}

NoARN* menorNoARN(ArvoreARN* arvore, NoARN* no) {
    while (no->esquerda != arvore->nulo) {
        no = no->esquerda;
    }
    return no;
}

void substituirARN(ArvoreARN* arvore, NoARN* antigo, NoARN* novo) {
    if (antigo->pai == arvore->nulo) {
        arvore->raiz = novo;
    } else if (antigo == antigo->pai->esquerda) {
        antigo->pai->esquerda = novo;
    } else {
        antigo->pai->direita = novo;
    }
    novo->pai = antigo->pai;
}

void removerARN(ArvoreARN* arvore, int valor) {
    NoARN* no = arvore->raiz;

    while (no != NULL && no != arvore->nulo && no->valor != valor) {
        if (valor < no->valor) {
            no = no->esquerda;
        } else {
            no = no->direita;
        }
    }

    if (no == NULL || no == arvore->nulo) {
        return;
    }

    NoARN* sucessor = no;
    NoARN* filho;
    Cor corOriginal = sucessor->cor;
    if (no->esquerda == arvore->nulo) {
        filho = no->direita;
        substituirARN(arvore, no, no->direita);
    } else if (no->direita == arvore->nulo) {
        filho = no->esquerda;
        substituirARN(arvore, no, no->esquerda);
    } else {
        sucessor = menorNoARN(arvore, no->direita);
        corOriginal = sucessor->cor;
        filho = sucessor->direita;

        if (sucessor->pai == no) {
            filho->pai = sucessor;
        } else {
            substituirARN(arvore, sucessor, sucessor->direita);
            sucessor->direita = no->direita;
            sucessor->direita->pai = sucessor;
        }

        substituirARN(arvore, no, sucessor);

        sucessor->esquerda = no->esquerda;
        sucessor->esquerda->pai = sucessor;
        sucessor->cor = no->cor;
    }
    free(no);

    if (corOriginal == Preto) {
        balancearRemocaoARN(arvore, filho);
    }

    if (arvore->raiz == arvore->nulo) {
        arvore->raiz = NULL;
    }

    if (arvore->raiz != NULL) {
        arvore->raiz->cor = Preto;
        arvore->raiz->pai = arvore->nulo;
    }
}

void balancearRemocaoARN(ArvoreARN* arvore, NoARN* no) {
    while (no != arvore->raiz && no->cor == Preto) {
        if (no == no->pai->esquerda) {
            NoARN* irmao = no->pai->direita;

            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                rotacionarEsquerdaARN(arvore, no->pai);
                irmao = no->pai->direita;
            }

            if (irmao->esquerda->cor == Preto &&
                irmao->direita->cor == Preto) {

                if (irmao != arvore->nulo) {
                    irmao->cor = Vermelho;
                }

                no = no->pai;
            } else {
                if (irmao->direita->cor == Preto) {
                    irmao->esquerda->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarDireitaARN(arvore, irmao);
                    irmao = no->pai->direita;
                }

                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->direita->cor = Preto;
                rotacionarEsquerdaARN(arvore, no->pai);
                no = arvore->raiz;
            }

        } else {
            NoARN* irmao = no->pai->esquerda;

            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                rotacionarDireitaARN(arvore, no->pai);
                irmao = no->pai->esquerda;
            }

            if (irmao->direita->cor == Preto &&
                irmao->esquerda->cor == Preto) {

                if (irmao != arvore->nulo) {
                    irmao->cor = Vermelho;
                }

                no = no->pai;
            } else {
                if (irmao->esquerda->cor == Preto) {
                    irmao->direita->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarEsquerdaARN(arvore, irmao);
                    irmao = no->pai->esquerda;
                }

                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->esquerda->cor = Preto;
                rotacionarDireitaARN(arvore, no->pai);
                no = arvore->raiz;
            }
        }
    }

    no->cor = Preto;
}

