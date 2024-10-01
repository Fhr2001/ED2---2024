#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0

int particao = 0;

typedef struct Clientes
{
    int CodCliente;
    char Nome [50];
    char DataNascimento [20];
    //Flag = TRUE (1) significa que esta congelado, igual a FALSE (0) é livre
    int Flag;
}Clientes;

FILE* CriaParticao (){

    char convercao [6];

    sprintf(convercao, "%d.dat", particao);
    
    printf("%s\n", convercao);

    FILE* P;

    if ((P = fopen (convercao, "w+b")) == NULL){
                printf ("não foi possível crirar a particao %d", particao);
                exit(1);
            }

    printf ("arquivo criado com sucesso\n");

    particao +=1;

    return P;
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

void EscreverParticao (Clientes* cliente, FILE* P){
    
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

void LerParticaoToda (){

    FILE* P;
    if ((P = fopen ("0.dat", "r+b")) == NULL){
                printf ("não foi possível abrir a partição");
                exit(1);
            }

    printf ("arquivo aberto com sucesso\n");

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


    if (fread (&func->CodCliente, sizeof(int), 1, P) <= 0){
        printf("arquivo esvaziado\n");
        return func;
    }

    fread (func->Nome, sizeof(char), sizeof(func->Nome), P);
    fread (func->DataNascimento, sizeof(char), sizeof(func->DataNascimento), P);
    fread (&func->Flag, sizeof(int), 1, P);
    
    //printf ("CodCliente: %d\n", func->CodCliente);
    //printf ("Nome: %s\n", func->Nome);
    //printf ("DataNascimento: %s\n", func->DataNascimento);
    //printf ("Flag: %d\n", func->Flag);

    return  func;
}

int main (){

    Clientes Vetor [7];

    FILE* Q = AbrirArquivo();

    Clientes* cliente = (Clientes*) malloc(sizeof(Clientes));
            
        for (int i = 0; i < 7; i++){

        cliente = LerClientes(Q);

        Vetor [i].CodCliente = cliente->CodCliente;
        strcpy(Vetor[i].Nome, cliente->Nome);
        strcpy(Vetor[i].DataNascimento, cliente->DataNascimento);
        Vetor [i].Flag = 0;
        }     
    
    while (cliente->CodCliente > 0){
        //tem que dar fclose antes de criar uma nova partição
        FILE* P = CriaParticao();

        //descongelando o Vetor pra nova partição
        for (int i = 0; i < 7; i++){

            Vetor[i].Flag = FALSE;
        }

        //apenas para settar alguem como o menor valor temporariamente
        int menor = 0;
        for (int i = 1; i < 7; i++){

            if (Vetor [i].CodCliente < Vetor [menor].CodCliente){
                menor = i;
            }
        }

        EscreverParticao (&Vetor [menor], P);
        printf ("o menor no vetor foi %d\n", menor);

        //registra o ultimo inserido para comprarações
        Clientes* UltimoInserido = &Vetor [menor];

        //contador para saber se o vetor inteiro esta congelado ou não
        int congelado = 0;
        while (congelado < 7){
            //apenas para settar alguem como o menor valor temporariamente
            menor = 0;

            for (int i = 1; i < 7; i++){

                if (Vetor [i].Flag != TRUE && (Vetor [i].CodCliente < Vetor [menor].CodCliente )){
                    menor = i;
                }
            }

            if (UltimoInserido->CodCliente > Vetor[menor].CodCliente) {
                Vetor[menor].Flag = TRUE;
                congelado++;
                continue;
            }

            UltimoInserido = &Vetor [menor];
            EscreverParticao (&Vetor [menor], P);

            cliente = LerClientes(Q);
            Vetor [menor].CodCliente = cliente->CodCliente;
            strcpy(Vetor[menor].Nome, cliente->Nome);
            strcpy(Vetor[menor].DataNascimento, cliente->DataNascimento);
            Vetor [menor].Flag = 0;

            if (cliente->CodCliente <= 0) {
                break; // Parar se o cliente não for válido
            }
        }
        fclose (P);
    }

    LerParticaoToda();
    printf ("chegou aqui\n");


    
    free (cliente);
    fclose (Q);
    return 0;
}
