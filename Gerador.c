#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0

#define NUM_CLIENTES 20

typedef struct Clientes
{
    int CodCliente;
    char Nome [50];
    char DataNascimento [20];
    //Flag = 1 significa que esta congelado, igual a 0 é livre
    int Flag;
}Clientes;


void CriarArquivo (){

    FILE* P;

    if ((P = fopen ("test.dat", "w+b")) == NULL){
                printf ("não foi possível crirar o arquivo");
                exit(1);
            }

    printf ("arquivo criado com sucesso\n");
    fclose(P);

    return;
}

FILE* AbrirArquivo (){

    FILE* P;
    if ((P = fopen ("test.dat", "r+b")) == NULL){
                printf ("não foi possível crirar o arquivo");
                exit(1);
            }

    printf ("arquivo aberto com sucesso\n");

    return P;    
}

void EscreverArquivo (Clientes* cliente){
    FILE* P = AbrirArquivo ();

    //coloca o cursor no final do arquivo
    fseek (P, 0, SEEK_END);

    fwrite (&cliente->CodCliente, sizeof(int), 1, P);
    fwrite (&cliente->Nome, sizeof(char), sizeof(cliente->Nome), P);

    fwrite (&cliente->DataNascimento, sizeof(char), sizeof(cliente->DataNascimento), P);

    fwrite (&cliente->Flag, sizeof(int), 1, P);

    fclose (P);
}

// Função para gerar uma string aleatória de nome
void GerarNomeAleatorio(char *nome, int tamanho) {
    const char letras[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < tamanho - 1; i++) {
        nome[i] = letras[rand() % (sizeof(letras) - 1)];
    }
    nome[tamanho - 1] = '\0';  // Adiciona o terminador nulo
}

// Função para gerar uma data de nascimento aleatória no formato dd/mm/yyyy
void GerarDataAleatoria(char *data) {
    int dia = rand() % 28 + 1;  // Dia entre 1 e 28
    int mes = rand() % 12 + 1;  // Mês entre 1 e 12
    int ano = rand() % 30 + 1970;  // Ano entre 1970 e 1999
    sprintf(data, "%02d/%02d/%d", dia, mes, ano);  // Formata a data
}

int main (){

    CriarArquivo();

     for (int i = 0; i < NUM_CLIENTES; i++) {
        Clientes *cliente = (Clientes *)malloc(sizeof(Clientes));
        cliente->CodCliente = NUM_CLIENTES - i;
        cliente->Flag = FALSE;  // Flag definida como FALSE

        GerarNomeAleatorio(cliente->Nome, 10);  // Gera um nome aleatório com 9 letras
        GerarDataAleatoria(cliente->DataNascimento);  // Gera uma data de nascimento aleatória

        EscreverArquivo(cliente);  // Escreve o cliente no arquivo

        free(cliente);  // Libera a memória alocada
    }

    return 0;
}