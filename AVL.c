#include <stdlib.h>
#include <stdio.h>
#include "funcoes.h"

// Criação da AVL
AVL* cria() {
    AVL* arvore = malloc(sizeof(AVL));

    if (arvore == NULL) {
        return NULL;
    }

    arvore->raiz = NULL;
    return arvore;
}

// Verifica se a AVL está vazia
int vazia(AVL* arvore) {
    return (arvore == NULL || arvore->raiz == NULL);
}

// Percorre a árvore
void percorrer(No* no) {
    if (no != NULL) {
        printf("%i ", no->valor);
        percorrer(no->esquerda);
        percorrer(no->direita);
    }
}

// Altura
int altura(No* no) {
    if (no == NULL) {
        return -1;
    }

    return no->altura;
}

// Atualiza a altura de um nó
void atualizaAltura(No* no) {
    if (no != NULL) {
        int esquerda = altura(no->esquerda);
        int direita = altura(no->direita);

        no->altura = (esquerda > direita ? esquerda : direita) + 1;
    }
}

// Fator de balanceamento
int fb(No* no) {
    if (no == NULL) {
        return 0;
    }

    return altura(no->esquerda) - altura(no->direita);
}

// Rotação simples à esquerda
No* rse(No* no, AVL* a) {
    if (no == NULL || no->direita == NULL) {
        return no;
    }

    No* pai = no->pai;
    No* direita = no->direita;

    no->direita = direita->esquerda;

    if (no->direita != NULL) {
        no->direita->pai = no;
    }

    direita->esquerda = no;
    no->pai = direita;
    direita->pai = pai;

    if (pai == NULL) {
        a->raiz = direita;
    } else if (pai->esquerda == no) {
        pai->esquerda = direita;
    } else {
        pai->direita = direita;
    }

    atualizaAltura(no);
    atualizaAltura(direita);

    return direita;
}

// Rotação simples à direita
No* rsd(No* no, AVL* a) {
    if (no == NULL || no->esquerda == NULL) {
        return no;
    }

    No* pai = no->pai;
    No* esquerda = no->esquerda;

    no->esquerda = esquerda->direita;

    if (no->esquerda != NULL) {
        no->esquerda->pai = no;
    }

    esquerda->direita = no;
    no->pai = esquerda;
    esquerda->pai = pai;

    if (pai == NULL) {
        a->raiz = esquerda;
    } else if (pai->esquerda == no) {
        pai->esquerda = esquerda;
    } else {
        pai->direita = esquerda;
    }

    atualizaAltura(no);
    atualizaAltura(esquerda);

    return esquerda;
}

// Rotação dupla à esquerda
No* rde(No* no, AVL* a) {
    if (no == NULL || no->direita == NULL) {
        return no;
    }

    rsd(no->direita, a);
    return rse(no, a);
}

// Rotação dupla à direita
No* rdd(No* no, AVL* a) {
    if (no == NULL || no->esquerda == NULL) {
        return no;
    }

    rse(no->esquerda, a);
    return rsd(no, a);
}

// Rebalanceamento até a raiz
int rebalancearDe(AVL* a, No* no) {
    int rotacoes = 0;

    while (no != NULL) {
        atualizaAltura(no);

        int fator = fb(no);
        No* novaRaiz = no;

        if (fator > 1) {
            if (fb(no->esquerda) < 0) {
                rse(no->esquerda, a);
                rotacoes++;
            }

            novaRaiz = rsd(no, a);
            rotacoes++;

        } else if (fator < -1) {
            if (fb(no->direita) > 0) {
                rsd(no->direita, a);
                rotacoes++;
            }

            novaRaiz = rse(no, a);
            rotacoes++;
        }

        no = novaRaiz->pai;
    }

    return rotacoes;
}

// Checa o balanceamento após inserção
int checaNovo(No* no, AVL* a) {
    if (no == NULL || a == NULL) {
        return 0;
    }

    return rebalancearDe(a, no->pai);
}

// Processo interno de adicionar nó
No* adicionarNo(No* no, int valor) {
    if (no == NULL) {
        return NULL;
    }

    No* atual = no;
    No* pai = NULL;

    while (atual != NULL) {

        // Não permite números repetidos
        if (valor == atual->valor) {
            return NULL;
        }

        pai = atual;

        if (valor > atual->valor) {
            atual = atual->direita;
        } else {
            atual = atual->esquerda;
        }
    }

    No* novo = malloc(sizeof(No));

    if (novo == NULL) {
        return NULL;
    }

    novo->valor = valor;
    novo->pai = pai;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->altura = 0;

    if (valor > pai->valor) {
        pai->direita = novo;
    } else {
        pai->esquerda = novo;
    }

    return novo;
}

// Processo de adicionar
No* adicionar(AVL* arvore, int valor) {
    if (arvore == NULL) {
        return NULL;
    }

    No* no;

    if (vazia(arvore)) {
        no = malloc(sizeof(No));

        if (no == NULL) {
            return NULL;
        }

        no->valor = valor;
        no->pai = NULL;
        no->direita = NULL;
        no->esquerda = NULL;
        no->altura = 0;

        arvore->raiz = no;

        return no;
    }

    no = adicionarNo(arvore->raiz, valor);

    // Valor repetido ou falha de alocação
    if (no == NULL) {
        return NULL;
    }

    checaNovo(no, arvore);

    return no;
}

// Procura o maior nó da subárvore esquerda
No* procuraFundaEsq(No* no) {
    if (no == NULL) {
        return NULL;
    }

    while (no->direita != NULL) {
        no = no->direita;
    }

    return no;
}

// Remove um nó
void removerNo(No* no, AVL* arvore) {
    if (no == NULL || arvore == NULL) {
        return;
    }

    // Caso tenha dois filhos
    if (no->esquerda != NULL && no->direita != NULL) {

        // Encontra o antecessor
        No* noNovo = procuraFundaEsq(no->esquerda);

        // Copia o valor do antecessor
        no->valor = noNovo->valor;

        // O nó removido passa a ser o antecessor
        no = noNovo;
    }

    // A partir daqui, possui no máximo um filho
    No* pai = no->pai;
    No* filho;

    if (no->esquerda != NULL) {
        filho = no->esquerda;
    } else {
        filho = no->direita;
    }

    // Atualiza o pai do filho
    if (filho != NULL) {
        filho->pai = pai;
    }

    // Atualiza a ligação com o pai
    if (pai == NULL) {
        arvore->raiz = filho;
    } else if (pai->esquerda == no) {
        pai->esquerda = filho;
    } else if (pai->direita == no) {
        pai->direita = filho;
    }

    free(no);

    // Rebalanceia a partir do pai até a raiz
    rebalancearDe(arvore, pai);
}

// Procura um nó para removê-lo
void procuraPraRemover(No* no, int valor, AVL* arvore) {
    if (arvore == NULL) {
        return;
    }

    while (no != NULL) {

        if (valor == no->valor) {
            removerNo(no, arvore);
            return;
        }

        if (valor > no->valor) {
            no = no->direita;
        } else {
            no = no->esquerda;
        }
    }

    printf("Esse no nao existe.\n");
}