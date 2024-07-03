#ifndef ARVORE_B
#define ARVORE_B


typedef struct no no;

typedef struct bt bt;

typedef struct registro registro;


//Aloca um novo nó
no *alocaNo(int ordem);

//Aloca uma nova B-Tree
bt *criaArvore(int m);

//Busca um elemento na B-Tree
no *buscaElemento(no *atual, int valor);

//Insere um elemento na B-Tree
int insereElemento(bt *arvore, int valor, registro *reg);

//Divide um nó
no *split(bt *arvore, no *noDesbal, int valor);

//Remove um elemento da B-Tree
int removeElemento(bt *arvore, int valor);

//Rotaciona um nó
void rotacao(no *noDesbal, no *irmao, int posPai);

//Combina dois nós
no *merge(no *noDesbal, int posPai);

//Retorna a raiz da B-Tree
no* getRaiz(bt *tree);

//Processa os registros do arquivo
void preencheArvore(bt *arv, char *nomeArquivo);

//Busca um registro no arquivo
int buscarRegistro(int matricula);

void imprimeRegistro(no* raiz, int matricula);

#endif //ARVORE_B