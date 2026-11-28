#include <stdlib.h>
#include <stdio.h>
#include "funcoes.h"

/*
  Contador de esforco da rubro-negra.
  Sao contabilizados: comparacoes de chaves, visitas na busca,
  verificacoes de cor, alteracoes de cor e alteracoes de ponteiros
  realizadas nas rotacoes, substituicoes e remocoes.
 */
static int esforcoARN = 0;

static void contaARN() {
    if (esforcoARN < 2147483647)
        esforcoARN++;
}

static void somaARN(int quantidade) {
    if (quantidade > 0 && esforcoARN <= 2147483647 - quantidade)
        esforcoARN += quantidade;
}

int vaziaARN(ArvoreARN *arvore) {
    return (arvore == NULL || arvore->raiz == NULL);
}

NoARN *criarNoARN(ArvoreARN *arvore, NoARN *pai, int valor) {
    if (arvore == NULL)
        return NULL;

    NoARN *no = malloc(sizeof(NoARN));
    if (no == NULL)
        return NULL;

    no->pai = pai;
    no->valor = valor;
    no->direita = arvore->nulo;
    no->esquerda = arvore->nulo;
    no->cor = Vermelho;
    return no;
}

ArvoreARN *criaARN(void) {
    ArvoreARN *arvore = malloc(sizeof(ArvoreARN));
    if (arvore == NULL)
        return NULL;

    arvore->raiz = NULL;
    arvore->nulo = NULL;

    arvore->nulo = criarNoARN(arvore, NULL, 0);
    if (arvore->nulo == NULL) {
        free(arvore);
        return NULL;
    }

    arvore->nulo->cor = Preto;
    arvore->nulo->esquerda = arvore->nulo;
    arvore->nulo->direita = arvore->nulo;
    arvore->nulo->pai = arvore->nulo;
    return arvore;
}

NoARN *localizarARN(ArvoreARN *arvore, int valor) {
    if (arvore == NULL)
        return NULL;

    NoARN *no = arvore->raiz;
    while (no != NULL && no != arvore->nulo) {
        contaARN();
        if (no->valor == valor)
            return no;

        contaARN();
        no = (valor < no->valor) ? no->esquerda : no->direita;
        contaARN();
    }

    return NULL;
}

int adicionarARN(ArvoreARN *arvore, int valor) {
    esforcoARN = 0;

    if (arvore == NULL || arvore->nulo == NULL)
        return 0;

    if (vaziaARN(arvore)) {
        arvore->raiz = criarNoARN(arvore, arvore->nulo, valor);
        if (arvore->raiz == NULL)
            return 0;

        arvore->raiz->cor = Preto;
        somaARN(4);
        return esforcoARN;
    }

    NoARN *no = arvore->raiz;

    while (no != arvore->nulo) {
        contaARN();
        if (valor == no->valor)
            return esforcoARN; /* Nao insere repetidos. */

        contaARN();
        if (valor >= no->valor) {
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
        contaARN();
    }

    NoARN *novo = criarNoARN(arvore, no, valor);
    if (novo == NULL)
        return esforcoARN;

    if (valor >= no->valor) {
        no->direita = novo;
        contaARN();
    } else {
        no->esquerda = novo;
        contaARN();
    }

    somaARN(4); /* Inicializacao do novo no. */
    balancearARN(arvore, novo);
    return esforcoARN;
}

void percorrerARN(ArvoreARN *arvore, NoARN *no, void (*callback)(int)) {
    percorrerPreOrderARN(arvore, no, callback);
}

void percorrerInOrderARN(ArvoreARN *arvore, NoARN *no, void (*callback)(int)) {
    if (arvore != NULL && no != NULL && no != arvore->nulo) {
        percorrerInOrderARN(arvore, no->esquerda, callback);
        callback(no->valor);
        percorrerInOrderARN(arvore, no->direita, callback);
    }
}

void percorrerPreOrderARN(ArvoreARN *arvore, NoARN *no, void (*callback)(int)) {
    if (arvore != NULL && no != NULL && no != arvore->nulo) {
        callback(no->valor);
        percorrerPreOrderARN(arvore, no->esquerda, callback);
        percorrerPreOrderARN(arvore, no->direita, callback);
    }
}

void visitarARN(int valor) {
    printf("%d ", valor);
}

void balancearARN(ArvoreARN *arvore, NoARN *no) {
    while (no->pai->cor == Vermelho) {
        contaARN();

        if (no->pai == no->pai->pai->esquerda) {
            NoARN *tio = no->pai->pai->direita;
            contaARN();

            if (tio->cor == Vermelho) {
                tio->cor = Preto;
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                somaARN(3);

                if (no->pai->pai != arvore->raiz) {
                    no = no->pai->pai;
                    contaARN();
                }
            } else {
                contaARN();
                if (no == no->pai->direita) {
                    no = no->pai;
                    contaARN();
                    rotacionarEsquerdaARN(arvore, no);
                } else {
                    no->pai->cor = Preto;
                    no->pai->pai->cor = Vermelho;
                    somaARN(2);
                    rotacionarDireitaARN(arvore, no->pai->pai);
                }
            }
        } else {
            contaARN();
            if (no->pai == no->pai->pai->direita) {
                NoARN *tio = no->pai->pai->esquerda;
                contaARN();

                if (tio->cor == Vermelho) {
                    tio->cor = Preto;
                    no->pai->cor = Preto;
                    no->pai->pai->cor = Vermelho;
                    somaARN(3);

                    if (no->pai->pai != arvore->raiz) {
                        no = no->pai->pai;
                        contaARN();
                    }
                } else {
                    contaARN();
                    if (no == no->pai->esquerda) {
                        no = no->pai;
                        contaARN();
                        rotacionarDireitaARN(arvore, no);
                    } else {
                        no->pai->cor = Preto;
                        no->pai->pai->cor = Vermelho;
                        somaARN(2);
                        rotacionarEsquerdaARN(arvore, no->pai->pai);
                    }
                }
            }
        }
    }

    arvore->raiz->cor = Preto;
    contaARN();
}

void rotacionarEsquerdaARN(ArvoreARN *arvore, NoARN *no) {
    NoARN *direita = no->direita;
    no->direita = direita->esquerda;
    contaARN();

    if (direita->esquerda != arvore->nulo) {
        direita->esquerda->pai = no;
        contaARN();
    }

    direita->pai = no->pai;
    contaARN();

    if (no->pai == arvore->nulo) {
        arvore->raiz = direita;
        contaARN();
    } else if (no == no->pai->esquerda) {
        no->pai->esquerda = direita;
        contaARN();
    } else {
        no->pai->direita = direita;
        contaARN();
    }

    direita->esquerda = no;
    no->pai = direita;
    somaARN(2);
}

void rotacionarDireitaARN(ArvoreARN *arvore, NoARN *no) {
    NoARN *esquerda = no->esquerda;
    no->esquerda = esquerda->direita;
    contaARN();

    if (esquerda->direita != arvore->nulo) {
        esquerda->direita->pai = no;
        contaARN();
    }

    esquerda->pai = no->pai;
    contaARN();

    if (no->pai == arvore->nulo) {
        arvore->raiz = esquerda;
        contaARN();
    } else if (no == no->pai->esquerda) {
        no->pai->esquerda = esquerda;
        contaARN();
    } else {
        no->pai->direita = esquerda;
        contaARN();
    }

    esquerda->direita = no;
    no->pai = esquerda;
    somaARN(2);
}

NoARN *menorNoARN(ArvoreARN *arvore, NoARN *no) {
    while (no->esquerda != arvore->nulo) {
        no = no->esquerda;
        contaARN();
    }
    return no;
}

void substituirARN(ArvoreARN *arvore, NoARN *antigo, NoARN *novo) {
    if (antigo->pai == arvore->nulo) {
        arvore->raiz = novo;
        contaARN();
    } else if (antigo == antigo->pai->esquerda) {
        antigo->pai->esquerda = novo;
        contaARN();
    } else {
        antigo->pai->direita = novo;
        contaARN();
    }

    novo->pai = antigo->pai;
    contaARN();
}

int removerARN(ArvoreARN *arvore, int valor) {
    esforcoARN = 0;

    if (arvore == NULL || arvore->nulo == NULL)
        return 0;

    NoARN *no = arvore->raiz;
    while (no != NULL && no != arvore->nulo && no->valor != valor) {
        contaARN();
        if (valor < no->valor)
            no = no->esquerda;
        else
            no = no->direita;
        contaARN();
    }

    if (no == NULL || no == arvore->nulo)
        return esforcoARN;

    NoARN *sucessor = no;
    NoARN *filho;
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
            contaARN();
        } else {
            substituirARN(arvore, sucessor, sucessor->direita);
            sucessor->direita = no->direita;
            sucessor->direita->pai = sucessor;
            somaARN(2);
        }

        substituirARN(arvore, no, sucessor);
        sucessor->esquerda = no->esquerda;
        sucessor->esquerda->pai = sucessor;
        sucessor->cor = no->cor;
        somaARN(3);
    }

    free(no);
    contaARN();

    contaARN();
    if (corOriginal == Preto)
        balancearRemocaoARN(arvore, filho);

    if (arvore->raiz == arvore->nulo) {
        arvore->raiz = NULL;
        contaARN();
    }

    if (arvore->raiz != NULL) {
        arvore->raiz->cor = Preto;
        arvore->raiz->pai = arvore->nulo;
        somaARN(2);
    }

    return esforcoARN;
}

void balancearRemocaoARN(ArvoreARN *arvore, NoARN *no) {
    while (no != arvore->raiz && no->cor == Preto) {
        contaARN();

        if (no == no->pai->esquerda) {
            NoARN *irmao = no->pai->direita;
            contaARN();

            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                somaARN(2);
                rotacionarEsquerdaARN(arvore, no->pai);
                irmao = no->pai->direita;
                contaARN();
            }

            contaARN();
            if (irmao->esquerda->cor == Preto &&
                irmao->direita->cor == Preto) {
                if (irmao != arvore->nulo) {
                    irmao->cor = Vermelho;
                    contaARN();
                }
                no = no->pai;
                contaARN();
            } else {
                contaARN();
                if (irmao->direita->cor == Preto) {
                    irmao->esquerda->cor = Preto;
                    irmao->cor = Vermelho;
                    somaARN(2);
                    rotacionarDireitaARN(arvore, irmao);
                    irmao = no->pai->direita;
                    contaARN();
                }

                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->direita->cor = Preto;
                somaARN(3);
                rotacionarEsquerdaARN(arvore, no->pai);
                no = arvore->raiz;
                contaARN();
            }
        } else {
            NoARN *irmao = no->pai->esquerda;
            contaARN();

            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                somaARN(2);
                rotacionarDireitaARN(arvore, no->pai);
                irmao = no->pai->esquerda;
                contaARN();
            }

            contaARN();
            if (irmao->direita->cor == Preto &&
                irmao->esquerda->cor == Preto) {
                if (irmao != arvore->nulo) {
                    irmao->cor = Vermelho;
                    contaARN();
                }
                no = no->pai;
                contaARN();
            } else {
                contaARN();
                if (irmao->esquerda->cor == Preto) {
                    irmao->direita->cor = Preto;
                    irmao->cor = Vermelho;
                    somaARN(2);
                    rotacionarEsquerdaARN(arvore, irmao);
                    irmao = no->pai->esquerda;
                    contaARN();
                }

                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->esquerda->cor = Preto;
                somaARN(3);
                rotacionarDireitaARN(arvore, no->pai);
                no = arvore->raiz;
                contaARN();
            }
        }
    }

    no->cor = Preto;
    contaARN();
}

static void destroiNosARN(ArvoreARN *arvore, NoARN *no) {
    if (no == NULL || no == arvore->nulo)
        return;

    destroiNosARN(arvore, no->esquerda);
    destroiNosARN(arvore, no->direita);
    free(no);
}

void destroiArvoreARN(ArvoreARN *arvore) {
    if (arvore == NULL)
        return;

    if (arvore->raiz != NULL)
        destroiNosARN(arvore, arvore->raiz);

    free(arvore->nulo);
    free(arvore);
}
