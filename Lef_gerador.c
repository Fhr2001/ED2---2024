#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define bool int

#define MAX 10
#define CARGA

#pragma pack(1)
typedef struct Pessoa
{
    int cod;
    char nome[50];
    int idade;
    int prox;
} Pessoa;
#pragma pack()

//Cria um arquivo
void CriarArquivo (){

    FILE* P;

    if ((P = fopen ("Lefkowitz_Idade.dat", "w+b")) == NULL){
                printf ("não foi possível crirar o arquivo");
                exit(1);
            }

    //printf ("arquivo criado com sucesso\n");
    fclose(P);

    return;
}

FILE* AbrirArquivo (){

    FILE* P;
    if ((P = fopen ("Lefkowitz_Idade.dat", "r+b")) == NULL){
                printf ("não foi possível crirar o arquivo");
                exit(1);
            }

    //printf ("arquivo aberto com sucesso\n");

    return P;    
}

void Escreve (Pessoa* pessoa){
    FILE* P = AbrirArquivo ();

    //coloca o cursor no final do arquivo
    fseek (P, 0, SEEK_END);
    fwrite (&pessoa->cod, sizeof(int), 1, P);
    fwrite (pessoa->nome, sizeof(char), sizeof(pessoa->nome), P);

    fwrite (&pessoa->idade, sizeof(int), 1, P);

    fwrite (&pessoa->prox, sizeof(int), 1, P);

    fclose (P);
}

void LerArquivoTodo (){

    FILE* P = AbrirArquivo ();

    //coloca o cursor no inicio do arquivo
    fseek (P, 0, SEEK_SET);
    Pessoa* pessoa = (Pessoa*) malloc(sizeof(Pessoa));
       
    //caso o fread retorna o quanto conseguiu ler, logo valores nulos ou inferiores significam o fim do arquivo
    while ((fread (&pessoa->cod, sizeof(int), 1, P)) > 0){
        
        fread (pessoa->nome, sizeof(char), sizeof(pessoa->nome), P);
        fread (&pessoa->idade, sizeof(int), 1, P);
        fread (&pessoa->prox, sizeof(int), 1, P);

        printf ("codpessoa: %d\n", pessoa->cod);
        printf ("nome: %s\n", pessoa->nome);
        printf ("idade: %d\n", pessoa->idade);
        printf ("prox: %d\n", pessoa->prox);
    }

    free (pessoa);
    
    fclose (P);
}

int main (){

    CriarArquivo ();

    FILE* P = AbrirArquivo ();

    Pessoa* pessoa = (Pessoa*) malloc(sizeof(Pessoa));
        
    char temp [2];
    temp [1] = '\0';

    for (int i = 0; i < MAX; i++){

        pessoa->cod = i;
        pessoa->idade = i % CARGA;

        temp [0] = 'A' + i;

        strcpy(pessoa->nome, temp);

        //printf ("%c\n", temp);

        pessoa->prox = -1;

        Escreve (pessoa);
    }

    LerArquivoTodo ();

    free (pessoa);
    fclose (P);
}