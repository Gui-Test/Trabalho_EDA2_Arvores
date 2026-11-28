#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funcoes.h"

#define MAX_N 10000
#define PASSO 1000
#define AMOSTRAS 10

static void embaralhar(int vetor[], int tamanho) {
    for (int i = tamanho - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int aux = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = aux;
    }
}

int main(void) {
    srand((unsigned)time(NULL));

    FILE *csvAdd = fopen("adicao.csv", "w");
    FILE *csvRemove = fopen("remocao.csv", "w");

    if (csvAdd == NULL || csvRemove == NULL) {
        printf("Erro ao criar os arquivos CSV.\n");
        if (csvAdd != NULL) fclose(csvAdd);
        if (csvRemove != NULL) fclose(csvRemove);
        return 1;
    }

    fprintf(csvAdd, "Quantidade_de_nos,AVL,ARN,ARB_1,ARB_5,ARB_10\n");
    fprintf(csvRemove, "Quantidade_de_nos,AVL,ARN,ARB_1,ARB_5,ARB_10\n");

    int universo[MAX_N];
    int extras[MAX_N];
    int ativos[MAX_N + 1];

   
    long long somaAdd[10][5] = {{0}};
    long long somaRemove[10][5] = {{0}};

    
    for (int amostra = 0; amostra < AMOSTRAS; amostra++) {
        printf("\n           AMOSTRA %d              \n", amostra + 1);

        for (int i = 0; i < MAX_N; i++)
            universo[i] = i;

        for (int i = 0; i < MAX_N; i++)
            extras[i] = MAX_N + i;

        embaralhar(universo, MAX_N);
        embaralhar(extras, MAX_N);

        AVL *avl = cria();
        ArvoreARN *arn = criaARN();
        ArvoreB *ab1 = criaArvoreB(1);
        ArvoreB *ab5 = criaArvoreB(5);
        ArvoreB *ab10 = criaArvoreB(10);

        if (avl == NULL || arn == NULL || ab1 == NULL ||
            ab5 == NULL || ab10 == NULL) {
            printf("Erro ao criar uma ou mais arvores.\n");
            destroiAVL(avl);
            destroiArvoreARN(arn);
            destroiArvoreB(ab1);
            destroiArvoreB(ab5);
            destroiArvoreB(ab10);
            fclose(csvAdd);
            fclose(csvRemove);
            return 1;
        }

        int qtdAtivos = 0;

        for (int nivel = 1; nivel <= 10; nivel++) {
            int tam = nivel * PASSO;
            int inicio = (nivel - 1) * PASSO;

            printf("Amostra %d - conjunto com %d nos\n",
                   amostra + 1, tam);

            for (int i = inicio; i < tam; i++) {
                int chave = universo[i];

                (void)adicionar(avl, chave);
                (void)adicionarARN(arn, chave);
                (void)adicionaChave(ab1, chave);
                (void)adicionaChave(ab5, chave);
                (void)adicionaChave(ab10, chave);

                ativos[qtdAtivos++] = chave;
            }

            // Adição 
            int chaveAdd = extras[nivel - 1];

            somaAdd[nivel - 1][0] += adicionar(avl, chaveAdd);
            somaAdd[nivel - 1][1] += adicionarARN(arn, chaveAdd);
            somaAdd[nivel - 1][2] += adicionaChave(ab1, chaveAdd);
            somaAdd[nivel - 1][3] += adicionaChave(ab5, chaveAdd);
            somaAdd[nivel - 1][4] += adicionaChave(ab10, chaveAdd);

            ativos[qtdAtivos++] = chaveAdd;

            // Remoção 
            int pos = rand() % tam;
            int chaveRemove = ativos[pos];

            somaRemove[nivel - 1][0] +=
                procuraPraRemover(avl->raiz, chaveRemove, avl);
            somaRemove[nivel - 1][1] += removerARN(arn, chaveRemove);
            somaRemove[nivel - 1][2] += removeChave(ab1, ab1->raiz, chaveRemove);
            somaRemove[nivel - 1][3] += removeChave(ab5, ab5->raiz, chaveRemove);
            somaRemove[nivel - 1][4] += removeChave(ab10, ab10->raiz, chaveRemove);

            // Atualiza o vetor de chaves presentes nas arvores
            ativos[pos] = ativos[qtdAtivos - 1];
            qtdAtivos--;
        }

        // Libera todos os nos antes de iniciar a proxima amostra
        destroiAVL(avl);
        destroiArvoreARN(arn);
        destroiArvoreB(ab1);
        destroiArvoreB(ab5);
        destroiArvoreB(ab10);
    }

    // Calcula a media das dez operacoes medidas em cada nivel
    for (int nivel = 0; nivel < 10; nivel++) {
        int tam = (nivel + 1) * PASSO;
        double mediaAdd[5];
        double mediaRemove[5];

        for (int estrutura = 0; estrutura < 5; estrutura++) {
            mediaAdd[estrutura] = somaAdd[nivel][estrutura] / (double)AMOSTRAS;
            mediaRemove[estrutura] = somaRemove[nivel][estrutura] / (double)AMOSTRAS;
        }

        fprintf(csvAdd, "%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                tam, mediaAdd[0], mediaAdd[1], mediaAdd[2],
                mediaAdd[3], mediaAdd[4]);

        fprintf(csvRemove, "%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                tam, mediaRemove[0], mediaRemove[1], mediaRemove[2],
                mediaRemove[3], mediaRemove[4]);
    }

    fclose(csvAdd);
    fclose(csvRemove);

    printf("\nExperimento finalizado.\n");
    printf("Arquivos gerados: adicao.csv e remocao.csv\n");
    return 0;
}
