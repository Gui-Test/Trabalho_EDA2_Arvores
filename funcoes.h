#ifndef FUNCOES_H
#define FUNCOES_H

/* --------------------------- Arvore AVL --------------------------- */
typedef struct no {
    int valor;
    int altura;
    struct no *pai;
    struct no *esquerda;
    struct no *direita;
} No;

typedef struct avl {
    No *raiz;
} AVL;

AVL *cria(void);
int vazia(AVL *arvore);
void percorrer(No *no);
int altura(No *no);
void atualizaAltura(No *no);
int fb(No *no);
No *rse(No *no, AVL *a);
No *rsd(No *no, AVL *a);
No *rde(No *no, AVL *a);
No *rdd(No *no, AVL *a);
int checaNovo(No *no, AVL *a);
No *adicionarNo(No *no, int valor);
int adicionar(AVL *arvore, int valor);
No *procuraFundaEsq(No *no);
void removerNo(No *no, AVL *arvore);
int procuraPraRemover(No *no, int valor, AVL *arvore);
void destroiAVL(AVL *arvore);

/* ---------------------- Arvore Rubro-Negra ------------------------ */
enum coloracao { Vermelho, Preto };
typedef enum coloracao Cor;

typedef struct noARN {
    struct noARN *pai;
    struct noARN *esquerda;
    struct noARN *direita;
    Cor cor;
    int valor;
} NoARN;

typedef struct arvore {
    NoARN *raiz;
    NoARN *nulo;
} ArvoreARN;

ArvoreARN *criaARN(void);
int vaziaARN(ArvoreARN *arvore);
NoARN *criarNoARN(ArvoreARN *arvore, NoARN *pai, int valor);
NoARN *localizarARN(ArvoreARN *arvore, int valor);
int adicionarARN(ArvoreARN *arvore, int valor);
void percorrerARN(ArvoreARN *arvore, NoARN *no, void (*callback)(int));
void percorrerInOrderARN(ArvoreARN *arvore, NoARN *no, void (*callback)(int));
void percorrerPreOrderARN(ArvoreARN *arvore, NoARN *no, void (*callback)(int));
void visitarARN(int valor);
void balancearARN(ArvoreARN *arvore, NoARN *no);
void rotacionarEsquerdaARN(ArvoreARN *arvore, NoARN *no);
void rotacionarDireitaARN(ArvoreARN *arvore, NoARN *no);
int removerARN(ArvoreARN *arvore, int valor);
void balancearRemocaoARN(ArvoreARN *arvore, NoARN *no);
NoARN *menorNoARN(ArvoreARN *arvore, NoARN *no);
void substituirARN(ArvoreARN *arvore, NoARN *antigo, NoARN *novo);
void destroiArvoreARN(ArvoreARN *arvore);

/* ---------------------------- Arvore B ---------------------------- */
typedef struct NoB {
    int *chaves;
    struct NoB **filhos;
    struct NoB *pai;
    int total;
    int folha;
} NoB;

typedef struct ArvoreB {
    int ordem;
    NoB *raiz;
} ArvoreB;

ArvoreB *criaArvoreB(int ordem);
NoB *criaNoB(ArvoreB *arvore);
void percorreArvoreB(NoB *no);
int pesquisaBinaria(NoB *no, int chave);
NoB *localizaNoREAL(NoB *no, int chave);
NoB *localizaNoB(ArvoreB *arvore, int chave);
int adicionaChave(ArvoreB *arvore, int chave);
int removeChave(ArvoreB *arvore, NoB *no, int chave);
void destroiArvoreB(ArvoreB *arvore);

#endif
