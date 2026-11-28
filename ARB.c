#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

/*
  Contador de esforco da arvore B.
  Sao contabilizados: comparacoes de chaves, descidas entre nos,
  deslocamentos de chaves/ponteiros e alteracoes estruturais de
  divisao, emprestimo, fusao e reducao da raiz.
 */
static int esforcoB = 0;

static void contaB() {
    if (esforcoB < 2147483647)
        esforcoB++;
}

static void somaB(int quantidade) {
    if (quantidade > 0 && esforcoB <= 2147483647 - quantidade)
        esforcoB += quantidade;
}

static int maximoChaves(const ArvoreB *arvore) {
    return 2 * arvore->ordem;
}

NoB *criaNoB(ArvoreB *arvore) {
    if (arvore == NULL || arvore->ordem < 1)
        return NULL;

    int max = maximoChaves(arvore);
    NoB *no = malloc(sizeof(NoB));
    if (no == NULL)
        return NULL;

    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = calloc(max + 2, sizeof(NoB *));

    if (no->chaves == NULL || no->filhos == NULL) {
        free(no->chaves);
        free(no->filhos);
        free(no);
        return NULL;
    }

    no->pai = NULL;
    no->total = 0;
    no->folha = 1;
    return no;
}

ArvoreB *criaArvoreB(int ordem) {
    if (ordem < 1)
        return NULL;

    ArvoreB *arvore = malloc(sizeof(ArvoreB));
    if (arvore == NULL)
        return NULL;

    arvore->ordem = ordem;
    arvore->raiz = criaNoB(arvore);

    if (arvore->raiz == NULL) {
        free(arvore);
        return NULL;
    }

    return arvore;
}

void percorreArvoreB(NoB *no) {
    if (no == NULL)
        return;

    for (int i = 0; i < no->total; i++) {
        percorreArvoreB(no->filhos[i]);
        printf("%d (%d) ", no->chaves[i], no->folha);
    }

    percorreArvoreB(no->filhos[no->total]);
}

int pesquisaBinaria(NoB *no, int chave) {
    if (no == NULL)
        return 0;

    int inicio = 0;
    int fim = no->total - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        contaB();
        if (no->chaves[meio] == chave)
            return meio;

        contaB();
        if (no->chaves[meio] > chave)
            fim = meio - 1;
        else
            inicio = meio + 1;
    }

    return inicio;
}

NoB *localizaNoREAL(NoB *no, int chave) {
    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);

        contaB();
        if (i < no->total && no->chaves[i] == chave)
            return no;

        if (no->folha)
            return NULL;

        no = no->filhos[i];
        contaB();
    }

    return NULL;
}

NoB *localizaNoB(ArvoreB *arvore, int chave) {
    if (arvore == NULL)
        return NULL;

    NoB *no = arvore->raiz;

    while (no != NULL && !no->folha) {
        int i = pesquisaBinaria(no, chave);
        no = no->filhos[i];
        contaB();
    }

    return no;
}

/* ----------------------------- INSERCAO ----------------------------- */

static int divideEPropaga(ArvoreB *arvore, NoB *no) {
    int max = maximoChaves(arvore);

    while (no != NULL && no->total > max) {
        int meio = no->total / 2;
        int promovida = no->chaves[meio];
        contaB();

        NoB *novo = criaNoB(arvore);
        if (novo == NULL) {
            fprintf(stderr, "Erro: nao foi possivel alocar um novo no.\n");
            return 0;
        }

        novo->folha = no->folha;
        novo->total = no->total - meio - 1;
        somaB(2);

        for (int i = 0; i < novo->total; i++) {
            novo->chaves[i] = no->chaves[meio + 1 + i];
            contaB();
        }

        if (!no->folha) {
            for (int i = 0; i <= novo->total; i++) {
                novo->filhos[i] = no->filhos[meio + 1 + i];
                contaB();

                if (novo->filhos[i] != NULL) {
                    novo->filhos[i]->pai = novo;
                    contaB();
                }

                no->filhos[meio + 1 + i] = NULL;
                contaB();
            }
        }

        no->total = meio;
        novo->pai = no->pai;
        somaB(2);

        if (no->pai == NULL) {
            NoB *novaRaiz = criaNoB(arvore);
            if (novaRaiz == NULL) {
                fprintf(stderr, "Erro: nao foi possivel alocar a nova raiz.\n");
                free(novo->chaves);
                free(novo->filhos);
                free(novo);
                return 0;
            }

            novaRaiz->folha = 0;
            novaRaiz->total = 1;
            novaRaiz->chaves[0] = promovida;
            novaRaiz->filhos[0] = no;
            novaRaiz->filhos[1] = novo;
            no->pai = novaRaiz;
            novo->pai = novaRaiz;
            arvore->raiz = novaRaiz;
            somaB(7);
            return 1;
        }

        NoB *pai = no->pai;
        int pos = 0;

        while (pos <= pai->total && pai->filhos[pos] != no) {
            pos++;
            contaB();
        }

        if (pos > pai->total) {
            fprintf(stderr, "Erro interno: filho nao encontrado no pai.\n");
            free(novo->chaves);
            free(novo->filhos);
            free(novo);
            return 0;
        }

        for (int i = pai->total - 1; i >= pos; i--) {
            pai->chaves[i + 1] = pai->chaves[i];
            contaB();
        }

        for (int i = pai->total; i > pos; i--) {
            pai->filhos[i + 1] = pai->filhos[i];
            contaB();
        }

        pai->chaves[pos] = promovida;
        pai->filhos[pos + 1] = novo;
        pai->total++;
        pai->folha = 0;
        novo->pai = pai;
        somaB(5);

        no = pai;
    }

    return 1;
}

int adicionaChave(ArvoreB *arvore, int chave) {
    esforcoB = 0;

    if (arvore == NULL || arvore->raiz == NULL)
        return 0;

    /* Impede chaves repetidas. */
    if (localizaNoREAL(arvore->raiz, chave) != NULL)
        return esforcoB;

    NoB *folha = localizaNoB(arvore, chave);
    if (folha == NULL)
        return esforcoB;

    int pos = pesquisaBinaria(folha, chave);

    for (int i = folha->total - 1; i >= pos; i--) {
        folha->chaves[i + 1] = folha->chaves[i];
        contaB();
    }

    folha->chaves[pos] = chave;
    folha->total++;
    somaB(2);

    divideEPropaga(arvore, folha);
    return esforcoB;
}

/* ------------------------------ REMOCAO ------------------------------ */

static void removeChaveFolha(NoB *no, int index) {
    for (int i = index; i < no->total - 1; i++) {
        no->chaves[i] = no->chaves[i + 1];
        contaB();
    }
    no->total--;
    contaB();
}

static int pegaAntecessor(NoB *no) {
    while (!no->folha) {
        no = no->filhos[no->total];
        contaB();
    }

    return no->chaves[no->total - 1];
}

static int pegaSucessor(NoB *no) {
    while (!no->folha) {
        no = no->filhos[0];
        contaB();
    }

    return no->chaves[0];
}

static void emprestaEsquerda(NoB *pai, int indice) {
    NoB *filho = pai->filhos[indice];
    NoB *irmao = pai->filhos[indice - 1];

    for (int i = filho->total - 1; i >= 0; i--) {
        filho->chaves[i + 1] = filho->chaves[i];
        contaB();
    }

    if (!filho->folha) {
        for (int i = filho->total; i >= 0; i--) {
            filho->filhos[i + 1] = filho->filhos[i];
            contaB();
        }

        filho->filhos[0] = irmao->filhos[irmao->total];
        contaB();
        if (filho->filhos[0] != NULL) {
            filho->filhos[0]->pai = filho;
            contaB();
        }

        irmao->filhos[irmao->total] = NULL;
        contaB();
    }

    filho->chaves[0] = pai->chaves[indice - 1];
    pai->chaves[indice - 1] = irmao->chaves[irmao->total - 1];
    somaB(4);

    filho->total++;
    irmao->total--;
    somaB(2);
}

static void emprestaDireita(NoB *pai, int indice) {
    NoB *filho = pai->filhos[indice];
    NoB *irmao = pai->filhos[indice + 1];

    filho->chaves[filho->total] = pai->chaves[indice];
    contaB();

    if (!filho->folha) {
        filho->filhos[filho->total + 1] = irmao->filhos[0];
        contaB();
        if (filho->filhos[filho->total + 1] != NULL) {
            filho->filhos[filho->total + 1]->pai = filho;
            contaB();
        }
    }

    pai->chaves[indice] = irmao->chaves[0];
    contaB();

    for (int i = 0; i < irmao->total - 1; i++) {
        irmao->chaves[i] = irmao->chaves[i + 1];
        contaB();
    }

    if (!irmao->folha) {
        for (int i = 0; i < irmao->total; i++) {
            irmao->filhos[i] = irmao->filhos[i + 1];
            contaB();
        }
        irmao->filhos[irmao->total] = NULL;
        contaB();
    }

    filho->total++;
    irmao->total--;
    somaB(2);
}

static void fundeNos(NoB *pai, int indice) {
    NoB *esquerdo = pai->filhos[indice];
    NoB *direito = pai->filhos[indice + 1];
    int totalEsquerdo = esquerdo->total;
    int totalDireito = direito->total;

    esquerdo->chaves[totalEsquerdo] = pai->chaves[indice];
    contaB();

    for (int i = 0; i < totalDireito; i++) {
        esquerdo->chaves[totalEsquerdo + 1 + i] = direito->chaves[i];
        contaB();
    }

    if (!esquerdo->folha) {
        for (int i = 0; i <= totalDireito; i++) {
            esquerdo->filhos[totalEsquerdo + 1 + i] = direito->filhos[i];
            contaB();
            if (esquerdo->filhos[totalEsquerdo + 1 + i] != NULL) {
                esquerdo->filhos[totalEsquerdo + 1 + i]->pai = esquerdo;
                contaB();
            }
        }
    }

    esquerdo->total = totalEsquerdo + 1 + totalDireito;
    contaB();

    for (int i = indice; i < pai->total - 1; i++) {
        pai->chaves[i] = pai->chaves[i + 1];
        contaB();
    }

    for (int i = indice + 1; i < pai->total; i++) {
        pai->filhos[i] = pai->filhos[i + 1];
        contaB();
    }

    pai->filhos[pai->total] = NULL;
    pai->total--;
    somaB(2);

    free(direito->chaves);
    free(direito->filhos);
    free(direito);
    contaB();
}

static void corrigeUnderflow(ArvoreB *arvore, NoB *pai, int indice) {
    int minimo = arvore->ordem;
    NoB *irmaoEsquerdo = (indice > 0) ? pai->filhos[indice - 1] : NULL;
    NoB *irmaoDireito = (indice < pai->total) ? pai->filhos[indice + 1] : NULL;

    contaB();
    if (irmaoEsquerdo != NULL && irmaoEsquerdo->total > minimo) {
        emprestaEsquerda(pai, indice);
    } else {
        contaB();
        if (irmaoDireito != NULL && irmaoDireito->total > minimo) {
            emprestaDireita(pai, indice);
        } else if (irmaoEsquerdo != NULL) {
            fundeNos(pai, indice - 1);
        } else if (irmaoDireito != NULL) {
            fundeNos(pai, indice);
        }
    }
}

static int removeRecursivo(ArvoreB *arvore, NoB *no, int chave) {
    int indice = pesquisaBinaria(no, chave);
    int minimo = arvore->ordem;

    contaB();
    if (indice < no->total && no->chaves[indice] == chave) {
        if (no->folha) {
            removeChaveFolha(no, indice);
            return 1;
        }

        NoB *filhoEsquerdo = no->filhos[indice];
        NoB *filhoDireito = no->filhos[indice + 1];

        contaB();
        if (filhoEsquerdo->total > minimo) {
            int antecessor = pegaAntecessor(filhoEsquerdo);
            no->chaves[indice] = antecessor;
            contaB();
            removeRecursivo(arvore, filhoEsquerdo, antecessor);

            contaB();
            if (filhoEsquerdo->total < minimo)
                corrigeUnderflow(arvore, no, indice);
        } else {
            contaB();
            if (filhoDireito->total > minimo) {
                int sucessor = pegaSucessor(filhoDireito);
                no->chaves[indice] = sucessor;
                contaB();
                removeRecursivo(arvore, filhoDireito, sucessor);

                contaB();
                if (filhoDireito->total < minimo)
                    corrigeUnderflow(arvore, no, indice + 1);
            } else {
                int antecessor = pegaAntecessor(filhoEsquerdo);
                no->chaves[indice] = antecessor;
                contaB();
                removeRecursivo(arvore, filhoEsquerdo, antecessor);

                contaB();
                if (filhoEsquerdo->total < minimo)
                    corrigeUnderflow(arvore, no, indice);
            }
        }

        return 1;
    }

    if (no->folha)
        return 0;

    NoB *filho = no->filhos[indice];
    contaB();
    int removida = removeRecursivo(arvore, filho, chave);

    contaB();
    if (removida && filho->total < minimo)
        corrigeUnderflow(arvore, no, indice);

    return removida;
}

int removeChave(ArvoreB *arvore, NoB *no, int chave) {
    esforcoB = 0;

    if (arvore == NULL || arvore->raiz == NULL || no == NULL)
        return 0;

    while (no->pai != NULL) {
        no = no->pai;
        contaB();
    }

    if (no != arvore->raiz)
        return esforcoB;

    int removida = removeRecursivo(arvore, arvore->raiz, chave);
    if (!removida)
        return esforcoB;

    /* Reduz a altura quando a raiz fica sem chaves. */
    if (arvore->raiz->total == 0 && !arvore->raiz->folha) {
        NoB *antigaRaiz = arvore->raiz;
        arvore->raiz = antigaRaiz->filhos[0];
        arvore->raiz->pai = NULL;
        antigaRaiz->filhos[0] = NULL;
        somaB(3);

        free(antigaRaiz->chaves);
        free(antigaRaiz->filhos);
        free(antigaRaiz);
        contaB();
    }

    return esforcoB;
}

static void destroiNoB(NoB *no) {
    if (no == NULL)
        return;

    if (!no->folha) {
        for (int i = 0; i <= no->total; i++)
            destroiNoB(no->filhos[i]);
    }

    free(no->chaves);
    free(no->filhos);
    free(no);
}

void destroiArvoreB(ArvoreB *arvore) {
    if (arvore == NULL)
        return;

    destroiNoB(arvore->raiz);
    free(arvore);
}
