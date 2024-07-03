#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "bTree.h"

int main()
{
    char nomeArquivo[13] = "registros.txt";
    int opcao, matricula, ordem, teste;
    struct timeval inicio, fim;
    bt *arv;
    no *elemento;
    long tempo;

    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Criar indice\n");
        printf("2. Procurar elemento\n");
        printf("3. Remover registro\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            while (1)
            {
                printf("Informe a ordem da B-Tree (deve ser par e entre 4 e 10): ");
                scanf("%d", &ordem);

                if (4 <= ordem && ordem <= 10 && ordem % 2 == 0)
                {
                    break;
                }
                else
                {
                    printf("Ordem invalida!\n");
                }
            }

            arv = criaArvore(ordem); // Cria a arvore

            srand(time(NULL));

            preencheArvore(arv, nomeArquivo);
            printf("Registros inseridos na B-Tree!\n");
            break;
        case 2:
            printf("Informe a matricula a ser pesquisada: ");
            scanf("%d", &matricula);

            gettimeofday(&inicio, NULL);
            elemento = buscaElemento(getRaiz(arv), matricula);
            gettimeofday(&fim, NULL);
            tempo = (fim.tv_sec * 1000000 + fim.tv_usec) - (inicio.tv_sec * 1000000 + inicio.tv_usec);

            if (elemento != NULL)
            {
                printf("Chave encontrada na B-Tree!\n");
                printf("Chave encontrada em %ld microssegundos\n", tempo);
            }
            else
            {
                printf("Chave nao encontrada na B-Tree!\n");
            }

            gettimeofday(&inicio, NULL);
            teste = buscarRegistro(matricula);
            gettimeofday(&fim, NULL);
            tempo = (fim.tv_sec * 1000000 + fim.tv_usec) - (inicio.tv_sec * 1000000 + inicio.tv_usec);

            if (teste == 1)
            {
                printf("Chave encontrada nos registros!\n");
                printf("Chave encontrada em %ld microssegundos\n", tempo);
            }
            else
            {
                printf("Chave nao encontrada nos regsitros!\n");
            }
            break;
        case 3:
            printf("Informe a chave a ser removida: ");
            scanf("%d", &matricula);
            teste = removeElemento(arv, matricula);
            if (teste == 1)
            {
                printf("Chave removida da B-Tree!\n");
            }
            else if (teste == -1)
            {
                printf("Chave nao encontrada na B-Tree!\n");
            }
            break;

        case 4:
            printf("Saindo...\n");
            free(arv);
            return 0;

        default:
            printf("Opcao invalida!\n");
            break;
        }
    }
}