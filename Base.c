#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TRUE 1
#define FALSE 0

int particao = 0;

int lidos = 0;

typedef struct Clientes
{
    int CodCliente;
    char Nome [50];
    char DataNascimento [20];
    //Flag = TRUE (1) significa que esta congelado, igual a FALSE (0) é livre
    int Flag;
}Clientes;

void CriaParticao (){

    char convercao [6];

    sprintf(convercao, "%d.dat", particao);
    
    //printf("%s\n", convercao);

    FILE* P;

    if ((P = fopen (convercao, "w+b")) == NULL){
                printf ("não foi possível crirar a particao %d", particao);
                exit(1);
            }

    //printf ("arquivo criado com sucesso\n");

    particao +=1;
}

FILE* AbrirArquivo (){

    FILE* P;
    if ((P = fopen ("test.dat", "r+b")) == NULL){
                printf ("não foi possível crirar o arquivo");
                exit(1);
            }

    //printf ("arquivo aberto com sucesso\n");

    return P;    
}

FILE* AbrirParticao (int particaoID){

    char convercao [10];

    sprintf(convercao, "%d.dat", (particaoID));

    FILE* P;
    if ((P = fopen (convercao, "r+b")) == NULL){
                printf ("não foi possível crirar o arquivo");
                exit(1);
            }

    //printf ("arquivo aberto com sucesso\n");

    return P; 
}

void EscreverParticao (Clientes* cliente, int percorrido, int particaoID){

    FILE* P = AbrirParticao(particaoID);
    
    //coloca o cursor no final do arquivo
    fseek (P, sizeof (Clientes) * percorrido, SEEK_SET);

    fwrite (&cliente->CodCliente, sizeof(int), 1, P);
    fwrite (cliente->Nome, sizeof(char), sizeof(cliente->Nome), P);

    fwrite (cliente->DataNascimento, sizeof(char), sizeof(cliente->DataNascimento), P);

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

    for (int i = 0; i < (particao -1); i++){

        char convercao [6];

        sprintf(convercao, "%d.dat", i);

        FILE* P;
        if ((P = fopen (convercao, "r+b")) == NULL){
            printf ("não foi possível abrir a partição");
            exit(1);
        }

        printf ("\n\nPartocao %d aberta com sucesso\n", i);


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
}


//Arquivo fica em aberto (salva o cursor por não fechar)
//Iniciar arquivo com antecedencia 
Clientes* LerClientes (FILE* P){

    //FILE* P = AbrirArquivo();

    Clientes* func = (Clientes*) malloc(sizeof(Clientes));

    //colocando o cursor exatamente no final do ultimo lido
    //fseek (P, sizeof(Clientes) * lidos, SEEK_SET);

    lidos++;


    if (fread (&func->CodCliente, sizeof(int), 1, P) <= 0){
        printf("arquivo esvaziado\n");

        func->CodCliente = -1;
        func->Flag = TRUE;
        //fclose(P);
        return func;
    }

    fread (func->Nome, sizeof(char), sizeof(func->Nome), P);
    fread (func->DataNascimento, sizeof(char), sizeof(func->DataNascimento), P);
    fread (&func->Flag, sizeof(int), 1, P);
    
    printf ("CodCliente: %d\n", func->CodCliente);
    printf ("Nome: %s\n", func->Nome);
    printf ("DataNascimento: %s\n", func->DataNascimento);
    printf ("Flag: %d\n", func->Flag);

    fclose (P);
    
    //return  func;
}

int main (){

    FILE* Q = AbrirArquivo();

    Clientes* Vetor [7];
       
    for (int i = 0; i < 7; i++){

    Vetor [i] = LerClientes(Q);     
    }     
    
    //não encontre um proximo valor, soma
    int vazio = 0;


    int teste = 0;

    while (vazio < 7){

        //descongelando o Vetor pra nova partição
        for (int i = 0; i < 7; i++){

            //printf("codigos contidos no vetor: %d\n", Vetor[i]->CodCliente);

            //caso o vetor não esteja vazio, descongela
            if (Vetor[i]->CodCliente > 0) Vetor[i]->Flag = FALSE;
        }

        //registra quantos itens foram inseridos na partição atual
        int percorrido = 0;

        percorrido++;

        //registra o ultimo inserido para comprarações
        Clientes* UltimoInserido;

        //contador para saber se o vetor inteiro esta congelado ou não
        int congelado = 0;

        while (congelado < 7 && vazio < 7) {
            int menor = -1;


            //printf("Vetor[%d] -> CodCliente: %d, Flag: %d\n", menor, Vetor[menor]->CodCliente, Vetor[menor]->Flag);
            //printf("Clientes congelados: %d, Vazio: %d\n", congelado, vazio);


            // Encontrar o menor cliente não congelado
            for (int i = 0; i < 7; i++) {
                if ((Vetor[i]->Flag == FALSE) && (Vetor[i]->CodCliente > 0)) {
                    if (menor == -1 || Vetor[i]->CodCliente < Vetor[menor]->CodCliente) {
                        menor = i;

                    }
                }              
            }

            if (Vetor[menor]->CodCliente <= 0) {
                Vetor[menor]->Flag = TRUE;
                vazio++;
            }

            if (menor == -1) {
                // Todos os clientes estão congelados ou vazios
                break;
            }

            if (UltimoInserido->CodCliente >= Vetor[menor]->CodCliente) {
                Vetor[menor]->Flag = TRUE;
                congelado++;
                continue;
            }
            
            if (Vetor[menor]->CodCliente > 0) {
                if (percorrido == 0) {  // Cria partição apenas se for o primeiro cliente.
                    CriaParticao();
                }
                EscreverParticao(Vetor[menor], percorrido, particao - 1);
                UltimoInserido = Vetor[menor];
                Vetor[menor] = LerClientes(Q);
                percorrido++;
            }
        }
    }

    LerParticaoToda();
    
    fclose (Q);

    return 0;
}
