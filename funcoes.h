#include <stdio.h>
#include <stdlib.h>

//---------------------------  Arvore AVL ------------------------------//
typedef struct no {
    int valor;
    int altura;
    struct no* pai;
    struct no* esquerda;
    struct no* direita;
} No;

typedef struct avl {
    No* raiz;
} AVL;

AVL* cria();
int vazia(AVL* arvore);

void percorrer(No* no);

int altura(No* no);
void atualizaAltura(No* no);
int fb(No* no);

No* rse(No* no, AVL* a);
No* rsd(No* no, AVL* a);
No* rde(No* no, AVL* a);
No* rdd(No* no, AVL* a);

int checaNovo(No* no, AVL* a);

No* adicionarNo(No* no, int valor);
No* adicionar(AVL* arvore, int valor);

No* procuraFundaEsq(No* no);

void removerNo(No* no, AVL* arvore);
void procuraPraRemover(No* no, int valor, AVL* arvore);

//---------------------------  Arvore Rubro Negra ------------------------------//
enum coloracao { Vermelho, Preto };
typedef enum coloracao Cor;

typedef struct noARN {
    struct noARN* pai;
    struct noARN* esquerda;
    struct noARN* direita;
    Cor cor; //cor do nó (Vermelho ou Preto)
    int valor;
} NoARN;

typedef struct arvore {
    struct noARN* raiz;
    struct noARN* nulo;
} ArvoreARN;

ArvoreARN* criaARN();
int vaziaARN(ArvoreARN* AVL);
NoARN* criarNoARN(ArvoreARN*, NoARN*, int);
NoARN* localizarARN(ArvoreARN* arvore, int valor);
void adicionarARN(ArvoreARN* arvore, int valor);
void percorrerARN(ArvoreARN* arvore,NoARN* no, void (*callback)(int));
void percorrerInOrderARN(ArvoreARN* arvore,NoARN* no, void (*callback)(int));
void percorrerPreOrderARN(ArvoreARN* arvore,NoARN* no, void (*callback)(int));
void visitarARN(int valor);
void balancearARN(ArvoreARN* arvore, NoARN* no);
void rotacionarEsquerdaARN(ArvoreARN* arvore, NoARN* no);
void rotacionarDireitaARN(ArvoreARN* arvore, NoARN* no);
void removerARN(ArvoreARN* arvore, int valor);
void balancearRemocaoARN(ArvoreARN* arvore, NoARN* no);
NoARN* menorNoARN(ArvoreARN* arvore, NoARN* no);
void substituirARN(ArvoreARN* arvore, NoARN* antigo, NoARN* novo);

//---------------------------  Arvore B ------------------------------//
typedef struct noB {
    int total;
    int folha;
    int* chaves;
    struct noB** filhos;
    struct noB* pai; 
} NoB;

typedef struct arvoreB {
  NoB* raiz;
  int ordem;
} ArvoreB;

ArvoreB* criaArvoreB(int);
NoB* criaNoB(ArvoreB*);
void percorreArvoreB(NoB*);
int pesquisaBinaria(NoB*, int);
NoB* localizaNoREAL(ArvoreB*, int);
NoB* localizaNoB(ArvoreB*, int);
void adicionaChaveNoB(NoB*, NoB*, int);
int transbordo(ArvoreB*,NoB*);
NoB* divideNoB(ArvoreB*, NoB*);
void adicionaChaveRecursivo(ArvoreB*, NoB*, NoB*, int);
void adicionaChave(ArvoreB*, int);
void removeChave(ArvoreB*, int);
void removeChaveFolha(NoB*, int);
int pegaAntecessor(NoB*,int);
int pegaSucessor(NoB*,int);
void merge(NoB* no, int index);


//--------------------------- FUNÇÕES DIVERSAS ------------------------------//
/*
void liberarArvoreB(NoB *no){
    if (no != NULL)
    {
        // Libera recursivamente todos os filhos
        if (no->filhos != NULL)
        {
            for (int i = 0; i <= no->total; i++)
            { // Modificado para incluir todos os filhos
                if (no->filhos[i] != NULL)
                {
                    liberarArvoreB(no->filhos[i]);
                    no->filhos[i] = NULL; // Evita dupla liberação
                }
            }
            free(no->filhos);
            no->filhos = NULL;
        }

        // Libera array de chaves
        if (no->chaves != NULL)
        {
            free(no->chaves);
            no->chaves = NULL;
        }

        free(no);
    }
}*/