#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0

int particao = 2;

typedef struct Clientes
{
    int CodCliente;
    char Nome [50];
    char DataNascimento [20];
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

void CriaParticao (){

    char convercao [5];

    convercao[0] = particao+'0';
    convercao [1] = '.';
    convercao [2] = 'd';
    convercao [3] = 'a';
    convercao [4] = 't';


    FILE* P;

    if ((P = fopen (convercao, "w+b")) == NULL){
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

//Printa todos os dados contidos no arquivo
void LerArquivoTodo (){

    FILE* P = AbrirArquivo ();

    //coloca o cursor no inicio do arquivo
    fseek (P, 0, SEEK_SET);
    Clientes* func = (Clientes*) malloc(sizeof(Clientes));
       
    //caso o fread retorna o quanto conseguiu ler, logo valores nulos ou inferiores significam o fim do arquivo
    while ((fread (&func->CodCliente, sizeof(int), 1, P)) > 0){
        
        fread (func->Nome, sizeof(char), sizeof(func->Nome), P);
        fread (func->DataNascimento, sizeof(char), sizeof(func->DataNascimento), P);
        fread (&func->Flag, sizeof(int), 1, P);

        printf ("CodCliente: %d\n", func->CodCliente);
        printf ("Nome: %s\n", func->Nome);
        printf ("DataNascimento: %s\n", func->DataNascimento);
    }

    free (func);
    
    fclose (P);
}

//Arquivo fica em aberto (salva o cursor por não fechar)
//Iniciar arquivo com antecedencia 
Clientes* LerClientes (FILE* P){

    Clientes* func = (Clientes*) malloc(sizeof(Clientes));

    fread (&func->CodCliente, sizeof(int), 1, P);
    fread (func->Nome, sizeof(char), sizeof(func->Nome), P);
    fread (func->DataNascimento, sizeof(char), sizeof(func->DataNascimento), P);
    fread (&func->Flag, sizeof(int), 1, P);
    
    printf ("CodCliente: %d\n", func->CodCliente);
    printf ("Nome: %s\n", func->Nome);
    printf ("DataNascimento: %s\n", func->DataNascimento);
    printf ("Flag: %d\n", func->Flag);

    return  func;
}



int main (){

    CriaParticao();

    CriarArquivo();

    Clientes* abc = (Clientes*) malloc(sizeof(Clientes));

    Clientes Vetor [7];
   
    char nome [3] = "ab";
    char data [20] = "00/00/0000";

       
    abc->CodCliente = 7;
    abc->Flag = FALSE;

    strcpy(abc->Nome, nome);
    strcpy(abc->DataNascimento, data);
    

    EscreverArquivo (abc);

    Clientes* def = (Clientes*) malloc(sizeof(Clientes));
  
    char nome2 [3] = "cd";
    char data2 [20] = "11/11/1111";

       
    def->CodCliente = 56;
    def->Flag = TRUE;

    strcpy(def->Nome, nome2);
    strcpy(def->DataNascimento, data2);
    

    EscreverArquivo (def);

    //LerArquivoTodo();


    Clientes* temp1;
    Clientes* temp2;


    //É necessario abrir o arquivo antes para evitar de resetar o cursor do arquivo
    //Lembre de fechar o arquivo quando terminar de usar
    //Dar free no malloc do cliente após o uso
    FILE* P = AbrirArquivo ();

    temp1 = LerClientes(P);
    temp2 = LerClientes(P);

    fclose (P);

    free (temp1);
    free (temp2);

    free (abc);
    free (def);

    

    

    return 0;
}
