#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bTree.h"

struct no
{
    registro *registros; // Conteúdo dos nós
    no **filhos;         // Aponta para o filho do nó
    no *pai;             // Aponta para o pai do nó
    int folha;           // Indica se o nó é uma folha ou não
    int chave;           // Quantiade de chaves dentro do nó
};

struct bt
{
    int ordem;
    no *raiz;
};

struct registro
{
    int matricula;
    char nome[50];
    char dataNascimento[11];
    char email[50];
};


void preencheArvore(bt *arv, char *nomeArquivo)
{
    FILE *arquivo;
    struct registro *reg = malloc(sizeof(struct registro));
    int matricula;
    char nome[50], dataNascimento[11], email[50];

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de registros.\n");
        return;
    }

    while (fscanf(arquivo, "%d;%[^;];%[^;];%[^\n]\n", &matricula, nome, dataNascimento, email) == 4)
    {
        reg->matricula = matricula;
        strcpy(reg->nome, nome);
        strcpy(reg->dataNascimento, dataNascimento);
        strcpy(reg->email, email);

        // Salvar o registro na B-tree em memoria
        insereElemento(arv, matricula, reg);
    }

    free(reg);
    fclose(arquivo);
    printf("Registros lidos com sucesso e salvos na B-tree em memoria.\n");
}


no *alocaNo(int ordem)
{
    no *novo = (no *)malloc(sizeof(no));

    if (novo == NULL)
    {
        return NULL;
    }

    novo->registros = (registro *)malloc(sizeof(registro) * (ordem - 1)); // Aloca a quantidade de chaves possíveis dentro da ordem (m-1)

    novo->filhos = (no **)malloc(sizeof(no *) * (ordem));

    if (novo->filhos == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < ordem; i++)
    {
        novo->filhos[i] = NULL;
    }

    // Inicializa o nó
    novo->chave = 0;
    novo->pai = NULL;
    novo->folha = 1;

    return novo;
}

bt *criaArvore(int ordem)
{
    bt *arvore = (bt *)malloc(sizeof(bt));

    if (arvore == NULL)
    {
        return NULL;
    }

    arvore->ordem = ordem;
    arvore->raiz = alocaNo(ordem);

    if (arvore->raiz == NULL)
    {
        return NULL;
    }

    return arvore;
}

no *buscaElemento(no *atual, int valor)
{
    if (atual == NULL)
    {
        return NULL;
    }

    int aux = 0;
    int qtd = atual->chave;

    while ((atual->registros[aux].matricula < valor) && (aux < qtd))
    {
        aux++;
    }

    if ((valor == atual->registros[aux].matricula) && (aux < qtd))
    {
        return atual;
    }

    if (atual->folha == 1)
    {
        return NULL;
    }

    return buscaElemento(atual->filhos[aux], valor);
}

int insereElemento(bt *arvore, int valor, struct registro *reg)
{
    no *aux = arvore->raiz;

    int qtd = aux->chave, tam = arvore->ordem - 1;

    if ((aux->filhos[0] == NULL) && (tam >= qtd))
    {
        while (tam >= qtd)
        {
            if (aux->registros[qtd].matricula > valor)
            {
                aux->registros[qtd] = *reg;
                aux->chave++;
                return 1;
            }
            qtd++;
        }
    }
    else
    {
        while (aux->folha == 0)
        {
            int cont = 0;
            // Percorre o vetor de chaves
            while ((valor > aux->registros[cont].matricula) && (aux->chave > cont))
            {
                cont++;
            }
            aux = aux->filhos[cont];
        }
    }

    // Verifica se o nó está cheio
    if (aux->chave == tam)
    {
        aux = split(arvore, aux, valor);
    }

    qtd = aux->chave;

    while ((aux->registros[qtd - 1].matricula > valor) && (qtd > 0))
    {
        aux->registros[qtd] = aux->registros[qtd - 1];
        qtd--;
    }

    aux->registros[qtd].matricula = valor;

    aux->chave++;

    return 1;
}

no *split(bt *arvore, no *noDesbal, int valor)
{
    int i = 0;
    no *pai = noDesbal->pai;
    no *novo = alocaNo(arvore->ordem);

    if (novo == NULL)
    {
        return NULL;
    }

    novo->folha = noDesbal->folha;
    novo->pai = noDesbal->pai;

    int chave = noDesbal->registros[(arvore->ordem - 1) / 2].matricula;
    int aux = ((arvore->ordem - 1) / 2) + 1;

    // Copia os registros do nó desbalanceado
    while (arvore->ordem - 1 > aux)
    {
        noDesbal->chave--;
        novo->chave++;
        novo->registros[i] = noDesbal->registros[aux];

        if (novo->folha == 0)
        {
            novo->filhos[i] = noDesbal->filhos[aux];
            noDesbal->filhos[aux]->pai = novo;
        }

        noDesbal->filhos[aux] = NULL;

        aux++;
        i++;
    }

    // Copia os filhos do nó desbalanceado
    if (novo->folha == 0)
    {
        novo->filhos[i] = noDesbal->filhos[aux];
        noDesbal->filhos[aux]->pai = novo;
    }

    novo->filhos[aux] = NULL;

    if (pai != NULL)
    {
        i = pai->chave;

        if (arvore->ordem - 1 == i)
        {
            pai = split(arvore, pai, chave);
            i = pai->chave;
        }

        while ((pai->registros[i - 1].matricula > chave) && (i > 0))
        {
            pai->registros[i] = pai->registros[i - 1];
            pai->filhos[i + 1] = pai->filhos[i];
            i--;
        }

        pai->registros[i].matricula = chave;
        pai->filhos[i + 1] = novo;
        pai->chave++;

        noDesbal->chave--;

        novo->pai = pai;
    }
    else
    {
        no *raiz = alocaNo(arvore->ordem);

        if (raiz == NULL)
        {
            return NULL;
        }

        raiz->registros[0].matricula = chave;
        raiz->pai = NULL;
        raiz->folha = 0;
        raiz->chave++;

        noDesbal->chave--;

        raiz->filhos[0] = noDesbal;
        raiz->filhos[1] = novo;

        noDesbal->pai = raiz;

        novo->pai = raiz;

        arvore->raiz = raiz;
    }

    if (valor > chave)
    {
        return novo;
    }
    else
    {
        return noDesbal;
    }
}

int removeElemento(bt *arvore, int valor)
{
    no *remove = buscaElemento(arvore->raiz, valor);
    if (remove == NULL)
    {
        return -1;
    }

    no *aux;
    int i = 0;

    if (remove->folha == 0)
    {
        while ((valor > remove->registros[i].matricula) && (remove->chave > i))
        {
            i++;
        }

        aux = remove->filhos[i];

        while (aux->filhos[aux->chave - 1] != NULL)
        {
            aux = aux->filhos[aux->chave];
        }

        remove->registros[i].matricula = aux->registros[aux->chave - 1].matricula;
        remove = aux;

        valor = remove->registros[remove->chave - 1].matricula;
    }

    while ((valor > remove->registros[i].matricula) && (remove->chave > i))
    {
        i++;
    }

    while (remove->chave > i)
    {
        remove->registros[i].matricula = remove->registros[i + 1].matricula;
        i++;
    }

    remove->chave--;

    i = 0;
    int meio = (arvore->ordem - 1) / 2;

    if (meio > remove->chave)
    {
        no *pai = remove->pai;
        while ((valor > pai->registros[i].matricula) && (pai->chave > i))
        {
            i++;
        }

        if ((pai->filhos[i - 1]->chave > meio) && (i > 0))
        {
            rotacao(remove, pai->filhos[i - 1], i);
        }
        else if ((pai->filhos[i + 1]->chave > meio) && (pai->chave > i))
        {
            rotacao(remove, pai->filhos[i + 1], i);
        }
        else
        {
            merge(remove, i);
        }
    }

    return 1;
}

void rotacao(no *noDesbal, no *irmao, int posPai)
{
    no *pai = irmao->pai;

    if (pai->filhos[posPai - 1] == irmao)
    {
        noDesbal->registros[noDesbal->chave].matricula = pai->registros[posPai - 1].matricula;
        noDesbal->chave++;
        pai->registros[posPai - 1].matricula = irmao->registros[irmao->chave - 1].matricula;
        irmao->chave--;
    }
    else
    {
        int aux = irmao->chave - 1;
        noDesbal->registros[noDesbal->chave].matricula = pai->registros[posPai].matricula;
        noDesbal->chave++;
        pai->registros[posPai].matricula = irmao->registros[0].matricula;

        while (aux > 0)
        {
            irmao->registros[aux - 1].matricula = irmao->registros[aux].matricula;
            irmao->filhos[aux - 1] = irmao->filhos[aux];
            aux--;
        }

        irmao->chave--;
    }
}

no *merge(no *noDesbal, int posPai)
{
    no *pai = noDesbal->pai; // Pai do nó desbalanceado
    no *irmao;               // Ponteiro do irmão
    int ret = 0;             // Variável para definir o retorno
    int aux = 0;             // Variável auxiliar para contar

    // Merge acontece com irmão da esquerda
    if (posPai > 0)
    {
        irmao = pai->filhos[posPai - 1]; // Irmão é o filho do pai na posição posPai-1

        // Adiciona a chave do pai no irmão
        irmao->registros[irmao->chave].matricula = pai->registros[posPai - 1].matricula;
        irmao->chave++;

        // Copia as chaves e filhos do nó desbalanceado para o irmão
        while (noDesbal->chave > aux)
        {
            irmao->registros[irmao->chave + aux].matricula = noDesbal->registros[aux].matricula;
            irmao->filhos[irmao->chave + aux] = noDesbal->filhos[aux];
            irmao->chave++;
            aux++;
        }

        aux = posPai;
        // Retorna o irmão (ret = 0)

        // Merge acontece com irmão da direita
    }
    else
    {
        irmao = pai->filhos[posPai + 1]; // Irmão é o filho do pai na posição posPai+1

        // Adiciona a chave do pai no nó desbalanceado
        noDesbal->registros[noDesbal->chave].matricula = pai->registros[posPai].matricula;
        noDesbal->chave++;

        // Copia as chaves e filhos do irmão para o nó desbalanceado
        while (irmao->chave > aux)
        {
            noDesbal->registros[noDesbal->chave + aux].matricula = irmao->registros[aux].matricula;
            noDesbal->filhos[noDesbal->chave + aux] = irmao->filhos[aux];
            noDesbal->chave++;
            aux++;
        }

        aux = posPai + 1;
        ret++; // Retorna o nó desbalanceado
    }

    // Remove a chave do pai e os filhos deslocando as posições
    while (pai->chave > aux)
    {
        pai->registros[aux - 1].matricula = pai->registros[aux].matricula;
        pai->filhos[aux] = pai->filhos[aux + 1];
        pai->chave--;
        aux++;
    }

    // Define o retorno
    if (ret == 0)
    {
        return irmao; // Retorna o irmão
    }
    else
    {
        return noDesbal; // Retorna o nó desbalanceado
    }
}

no *getRaiz(bt *tree)
{
    return tree->raiz;
}

int buscarRegistro(int matricula)
{
    FILE *arquivo;
    registro registro;

    arquivo = fopen("registros.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo de registros.\n");
        return -1;
    }

    while (fscanf(arquivo, "%d;%[^;];%[^;];%[^\n]\n", &registro.matricula, registro.nome, registro.dataNascimento, registro.email) == 4)
    {
        if (registro.matricula == matricula)
        {
            fclose(arquivo);
            return 1;
        }
    }
    fclose(arquivo);
    return -1;
}

void imprimeRegistro(no *raiz, int matricula)
{
    no *resultado = buscaElemento(raiz, matricula);
    printf("Nome: %s\n", resultado->registros->nome);
}