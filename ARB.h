#include <stdio.h>
#include <stdlib.h>

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
int localizaChave(ArvoreB*, int);
NoB* localizaNoB(ArvoreB*, int);
void adicionaChaveNoB(NoB*, NoB*, int);
int transbordo(ArvoreB*,NoB*);
NoB* divideNoB(ArvoreB*, NoB*);
void adicionaChaveRecursivo(ArvoreB*, NoB*, NoB*, int);
void adicionaChave(ArvoreB*, int);
void removeChave(ArvoreB*, int);
void removeChaveFolha(NoB*, int);
int pegaAntecessor(NoB*,int);


ArvoreB* criaArvoreB(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNoB(a);

    return a;
}

NoB* criaNoB(ArvoreB* arvore) {
    int max = arvore->ordem * 2;
    NoB* no = malloc(sizeof(NoB));

    no->pai = NULL;
        
    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(NoB) * (max + 2));
    no->total = 0;
    no->folha = 1;

    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;

    return no;
}

void percorreArvoreB(NoB* no) {
    if (no != NULL) {
        for (int i = 0; i < no->total; i++){
            percorreArvoreB(no->filhos[i]); //visita o filho a esquerda
            
            printf("%d ",no->chaves[i]);
            printf("(%d) ",no->folha);
        }
        
        percorreArvoreB(no->filhos[no->total]); //visita ultimo filho (direita)
    }
}

int pesquisaBinaria(NoB* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;		
    
    while (inicio <= fim) {	
        meio = (inicio + fim) / 2;
        
        if (no->chaves[meio] == chave) {	
		    return meio; //encontrou	
        } else if (no->chaves[meio] > chave) {
                fim	= meio - 1;	
        } else {
            inicio = meio + 1;
        }
    }
    return inicio; //não encontrou	
}

int localizaChave(ArvoreB* arvore, int chave) {	
    NoB *no = arvore->raiz;
    
    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);

        if (i < no->total && no->chaves[i] == chave) {
            return 1; //encontrou
        } else {
            no = no->filhos[i];
        }
    }

    return 0; //não encontrou	
}

NoB* localizaNoB(ArvoreB* arvore, int chave) {	
    NoB *no = arvore->raiz;
    
    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);

        if (no->filhos[i] == NULL)
            return no; //encontrou nó
        else
            no = no->filhos[i];
    }

    return NULL; //não encontrou nenhum nó
}

void adicionaChaveNoB(NoB* no, NoB* novo, int chave) {
    int i = pesquisaBinaria(no, chave);
    
    for (int j = no->total - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }
    
    no->chaves[i] = chave;
    no->filhos[i + 1] = novo;

    no->total++;
}

int transbordo(ArvoreB* arvore, NoB* no) {
    return no->total > arvore->ordem * 2;
}

NoB* divideNoB(ArvoreB* arvore, NoB* no) {
    int meio = no->total / 2;
    NoB* novo = criaNoB(arvore);
    novo->pai = no->pai;
    

    for (int i = meio + 1; i < no->total; i++) {
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        if (novo->filhos[novo->total] != NULL) {
            novo->filhos[novo->total]->pai = novo;
            novo->folha=0;
        }
        
        novo->total++;
    }

    novo->filhos[novo->total] = no->filhos[no->total];
    if (novo->filhos[novo->total] != NULL){ 
        novo->filhos[novo->total]->pai = novo; 
        novo->folha=0;
    }   
    no->total = meio;
    return novo;
}

void adicionaChaveRecursivo(ArvoreB* arvore, NoB* no, NoB* novo, int chave) {
    adicionaChaveNoB(no, novo, chave);
    
    if (transbordo(arvore, no)) {
        int promovido = no->chaves[arvore->ordem]; 
        NoB* novo = divideNoB(arvore, no);

        if (no->pai == NULL) { //raiz da arvore
            NoB* pai = criaNoB(arvore);            
            pai->filhos[0] = no;
            adicionaChaveNoB(pai, novo, promovido);
            pai->folha = 0;
            
            no->pai = pai;
            novo->pai = pai;
            arvore->raiz = pai;
        } else
            adicionaChaveRecursivo(arvore, no->pai, novo, promovido);
    }
}

void adicionaChave(ArvoreB* arvore, int chave) {
    NoB* no = localizaNoB(arvore, chave);

    adicionaChaveRecursivo(arvore, no, NULL, chave);
}
 
void removeChave(ArvoreB* arvore, int chave){
    NoB* no = localizaNoB(arvore, chave);
    
    if(no!=NULL){
        int index = pesquisaBinaria(no,chave);
    
        if(no->folha){                                          
            if(no == arvore->raiz){                         // Caso 1, folha sem underflow
                removeChaveFolha(no,index);
            }else if(no->total-1 >= arvore->ordem){
                removeChaveFolha(no,index);                
            }else{                                          // Caso 3 e 4, folha com underflow
                
            }
        }else{                                              //Caso 2, não folha
            int antes = pegaAntecessor(no,index); 
            removeChave(arvore, antes);
            no->chaves[index] = antes;
            
        }

    }


}

void removeChaveFolha(NoB* no, int index){
    for (int i = index + 1; i < no->total; ++i) {
        no->chaves[i - 1] = no->chaves[i];
    }
    no->total--;
}

int pegaAntecessor(NoB* no, int index){
    NoB* atual = no->filhos[index];
    while(!atual->folha){
        atual = atual->filhos[atual->total];
    }
    return atual->chaves[atual->total-1];
}
