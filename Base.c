#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Funcionario
{
    int id;
    char nome [50];
    char cpf [14];
}Funcionario;


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

//Recebe um funcionario e o insere no arquivo binário
void EscreverArquivo (Funcionario* funcionario){

    FILE* P = AbrirArquivo ();

    //coloca o cursor no final do arquivo
    fseek (P, 0, SEEK_END);

    fwrite (&funcionario->id, sizeof(int), 1, P);
    fwrite (&funcionario->nome, sizeof(char), sizeof(funcionario->nome), P);

    fwrite (&funcionario->cpf, sizeof(char), sizeof(funcionario->cpf), P);

    fclose (P);
}

//Printa todos os dados contidos no arquivo
void LerArquivoTodo (){

    FILE* P = AbrirArquivo ();

    //coloca o cursor no inicio do arquivo
    fseek (P, 0, SEEK_SET);
    Funcionario* func = (Funcionario*) malloc(sizeof(Funcionario));
       
    //caso o fread retorna o quanto conseguiu ler, logo valores nulos ou inferiores significam o fim do arquivo
    while ((fread (&func->id, sizeof(int), 1, P)) > 0){
        
        fread (func->nome, sizeof(char), sizeof(func->nome), P);
        fread (func->cpf, sizeof(char), sizeof(func->cpf), P);

        printf ("ID: %d\n", func->id);
        printf ("Nome: %s\n", func->nome);
        printf ("CPF: %s\n", func->cpf);
    }

    free (func);
    
    fclose (P);
}

//Extrai o n-ésimo funcionario
Funcionario LerFuncionario (FILE* P, int n){


}



int main (){

    CriarArquivo();

    Funcionario* abc = (Funcionario*) malloc(sizeof(Funcionario));

    
    char nome [3] = "ab";
    char cpf [14] = "000.000.000-00";

       
    abc->id = 7;

    strcpy(abc->nome, nome);
    strcpy(abc->cpf, cpf);
    

    EscreverArquivo (abc);

    LerArquivoTodo();

    free (abc);

    

    return 0;
}
