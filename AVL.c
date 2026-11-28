#include <stdlib.h>
#include <stdio.h>
#include "funcoes.h"

/*
 * Contador de esforco da AVL.
 * Sao contabilizados: comparacoes de chaves, visitas durante buscas,
 * atualizacoes de altura/fator de balanceamento e alteracoes estruturais
 * de ponteiros realizadas nas rotacoes e remocoes.
 */
static int esforcoAVL = 0;

static void contaAVL(void) {
    if (esforcoAVL < 2147483647)
        esforcoAVL++;
}

AVL *cria(void) {
    AVL *arvore = malloc(sizeof(AVL));
    if (arvore == NULL)
        return NULL;

    arvore->raiz = NULL;
    return arvore;
}

int vazia(AVL *arvore) {
    return (arvore == NULL || arvore->raiz == NULL);
}

void percorrer(No *no) {
    if (no != NULL) {
        printf("%i ", no->valor);
        percorrer(no->esquerda);
        percorrer(no->direita);
    }
}

int altura(No *no) {
    if (no == NULL)
        return -1;
    return no->altura;
}

void atualizaAltura(No *no) {
    if (no != NULL) {
        int esquerda = altura(no->esquerda);
        int direita = altura(no->direita);
        int novaAltura = (esquerda > direita ? esquerda : direita) + 1;

        contaAVL();
        no->altura = novaAltura;
    }
}

int fb(No *no) {
    if (no == NULL)
        return 0;

    contaAVL();
    return altura(no->esquerda) - altura(no->direita);
}

No *rse(No *no, AVL *a) {
    if (no == NULL || no->direita == NULL)
        return no;

    No *pai = no->pai;
    No *direita = no->direita;

    no->direita = direita->esquerda;
    contaAVL();

    if (no->direita != NULL) {
        no->direita->pai = no;
        contaAVL();
    }

    direita->esquerda = no;
    no->pai = direita;
    direita->pai = pai;
    esforcoAVL += 3;

    if (pai == NULL) {
        a->raiz = direita;
        contaAVL();
    } else if (pai->esquerda == no) {
        pai->esquerda = direita;
        contaAVL();
    } else {
        pai->direita = direita;
        contaAVL();
    }

    atualizaAltura(no);
    atualizaAltura(direita);
    return direita;
}

No *rsd(No *no, AVL *a) {
    if (no == NULL || no->esquerda == NULL)
        return no;

    No *pai = no->pai;
    No *esquerda = no->esquerda;

    no->esquerda = esquerda->direita;
    contaAVL();

    if (no->esquerda != NULL) {
        no->esquerda->pai = no;
        contaAVL();
    }

    esquerda->direita = no;
    no->pai = esquerda;
    esquerda->pai = pai;
    esforcoAVL += 3;

    if (pai == NULL) {
        a->raiz = esquerda;
        contaAVL();
    } else if (pai->esquerda == no) {
        pai->esquerda = esquerda;
        contaAVL();
    } else {
        pai->direita = esquerda;
        contaAVL();
    }

    atualizaAltura(no);
    atualizaAltura(esquerda);
    return esquerda;
}

No *rde(No *no, AVL *a) {
    if (no == NULL || no->direita == NULL)
        return no;

    rse(no->direita, a);
    return rsd(no, a);
}

No *rdd(No *no, AVL *a) {
    if (no == NULL || no->esquerda == NULL)
        return no;

    rse(no->esquerda, a);
    return rsd(no, a);
}

int rebalancearDe(AVL *a, No *no) {
    int rotacoes = 0;

    while (no != NULL) {
        contaAVL();
        atualizaAltura(no);

        int fator = fb(no);
        No *novaRaiz = no;

        contaAVL();
        if (fator > 1) {
            contaAVL();
            if (fb(no->esquerda) < 0) {
                rse(no->esquerda, a);
                rotacoes++;
            }

            novaRaiz = rsd(no, a);
            rotacoes++;
        } else {
            contaAVL();
            if (fator < -1) {
                contaAVL();
                if (fb(no->direita) > 0) {
                    rsd(no->direita, a);
                    rotacoes++;
                }

                novaRaiz = rse(no, a);
                rotacoes++;
            }
        }

        no = novaRaiz->pai;
        contaAVL();
    }

    return rotacoes;
}

int checaNovo(No *no, AVL *a) {
    if (no == NULL || a == NULL)
        return 0;
    return rebalancearDe(a, no->pai);
}

No *adicionarNo(No *no, int valor) {
    if (no == NULL)
        return NULL;

    No *atual = no;
    No *pai = NULL;

    while (atual != NULL) {
        contaAVL();
        if (valor == atual->valor)
            return NULL;

        pai = atual;
        contaAVL();
        if (valor > atual->valor)
            atual = atual->direita;
        else
            atual = atual->esquerda;
        contaAVL();
    }

    No *novo = malloc(sizeof(No));
    if (novo == NULL)
        return NULL;

    novo->valor = valor;
    novo->pai = pai;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->altura = 0;
    esforcoAVL += 5;

    if (valor > pai->valor) {
        pai->direita = novo;
        contaAVL();
    } else {
        pai->esquerda = novo;
        contaAVL();
    }

    return novo;
}

int adicionar(AVL *arvore, int valor) {
    esforcoAVL = 0;

    if (arvore == NULL)
        return 0;

    No *no;

    if (vazia(arvore)) {
        no = malloc(sizeof(No));
        if (no == NULL)
            return 0;

        no->valor = valor;
        no->pai = NULL;
        no->direita = NULL;
        no->esquerda = NULL;
        no->altura = 0;
        arvore->raiz = no;
        esforcoAVL += 6;
        return esforcoAVL;
    }

    no = adicionarNo(arvore->raiz, valor);
    if (no == NULL)
        return esforcoAVL;

    checaNovo(no, arvore);
    return esforcoAVL;
}

No *procuraFundaEsq(No *no) {
    if (no == NULL)
        return NULL;

    while (no->direita != NULL) {
        no = no->direita;
        contaAVL();
    }

    return no;
}

void removerNo(No *no, AVL *arvore) {
    if (no == NULL || arvore == NULL)
        return;

    if (no->esquerda != NULL && no->direita != NULL) {
        No *noNovo = procuraFundaEsq(no->esquerda);
        no->valor = noNovo->valor;
        contaAVL();
        no = noNovo;
    }

    No *pai = no->pai;
    No *filho;

    if (no->esquerda != NULL)
        filho = no->esquerda;
    else
        filho = no->direita;

    if (filho != NULL) {
        filho->pai = pai;
        contaAVL();
    }

    if (pai == NULL) {
        arvore->raiz = filho;
        contaAVL();
    } else if (pai->esquerda == no) {
        pai->esquerda = filho;
        contaAVL();
    } else if (pai->direita == no) {
        pai->direita = filho;
        contaAVL();
    }

    free(no);
    contaAVL();

    rebalancearDe(arvore, pai);
}

int procuraPraRemover(No *no, int valor, AVL *arvore) {
    esforcoAVL = 0;

    if (arvore == NULL)
        return 0;

    while (no != NULL) {
        contaAVL();
        if (valor == no->valor) {
            removerNo(no, arvore);
            return esforcoAVL;
        }

        contaAVL();
        if (valor > no->valor)
            no = no->direita;
        else
            no = no->esquerda;
        contaAVL();
    }

    return esforcoAVL;
}

static void destroiNoAVL(No *no) {
    if (no == NULL)
        return;
    destroiNoAVL(no->esquerda);
    destroiNoAVL(no->direita);
    free(no);
}

void destroiAVL(AVL *arvore) {
    if (arvore == NULL)
        return;
    destroiNoAVL(arvore->raiz);
    free(arvore);
}
