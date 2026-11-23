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

NoB* localizaNoREAL(ArvoreB* arvore, int chave) {	
    NoB *no = arvore->raiz;
    
    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);

        if (i < no->total && no->chaves[i] == chave) {
            return no; //encontrou
        } else {
            no = no->filhos[i];
        }
    }

    return NULL; //não encontrou	
}

NoB* localizaNoB(ArvoreB* arvore, int chave) {	// 21/11 - 15:43 QUE ÓDIO DESSA FUNÇÃO TO USANDO ELA DE FORMA ERRADA ATÉ AGORA
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
    NoB* no = localizaNoREAL(arvore, chave);
    
    if(no!=NULL){
        int index = pesquisaBinaria(no,chave);
    
        if(no->folha){                                          
            if(no == arvore->raiz){                         // Caso 1, folha sem underflow
                removeChaveFolha(no,index);
            }else if(no->total-1 >= arvore->ordem){
                removeChaveFolha(no,index);                
            }else{                                          // Caso 3 e 4, folha com underflow
                //Caso 3 empresta dos irmãos
                int teste = pesquisaBinaria(no->pai, chave);
                
                NoB* pai = no->pai;
                NoB* bro_left = pai->filhos[teste-1];
                NoB* bro_right = pai->filhos[teste+1];
                //Empresta Esquerda
                if(bro_left->total>=arvore->ordem){
                    int chavepai = pai->chaves[teste-1];
                    int chaveleft = bro_left->chaves[bro_left->total-1];

                    pai->chaves[teste-1] = chaveleft;
                    no->chaves[index] = chavepai; 
                    bro_left->total--;
                //Empresta Direita
                }else if(bro_right->total>=arvore->ordem){
                    int chavepai = pai->chaves[teste-1];
                    int chaveright = bro_right->chaves[0];

                    pai->chaves[teste-1] = chaveright;
                    no->chaves[index] = chavepai;
                    
                    for(int i = 0;i < bro_right->total; i++){           // Pra arrumar a ordem eu acho
                        bro_right->chaves[i] = bro_right->chaves[i+1];
                        bro_right->filhos[i] = bro_right->filhos[i+1];
                    } 
                    bro_right->filhos[bro_right->total-1] = bro_right->filhos[bro_right->total];
                    bro_right->total--;
                    
                //Caso 4 merge
                }else{
                   

                }             
                

            }
        }else{                                              //Caso 2, não folha
            if(no->filhos[index]->total>=arvore->ordem){
                int antes = pegaAntecessor(no,index); 
                removeChave(arvore, antes);
                no->chaves[index] = antes;
            }else if(no->filhos[index+1]->total>=arvore->ordem){
                int depois = pegaSucessor(no,index); 
                removeChave(arvore, depois);
                no->chaves[index] = depois; 
            }else{
                //Eu não sei ao certo se isso é posssível pensando agora, ah pera, acho que aqui acontece caso tu 
                // tente tirar da raiz quando ela não é folha talvez
            }

            
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

int pegaSucessor(NoB* no, int index){
    NoB* atual = no->filhos[index+1];
    while(!atual->folha){
        atual = atual->filhos[0];
    }
    return atual->chaves[0];
}