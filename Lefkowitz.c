#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define bool int

#pragma pack(1)
typedef struct Pessoa
{
    int cod;
    char nome[50];
    int idade;
    int prox;
} Pessoa;
#pragma pack()

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
       
    printf ("\n=================================================================================================\n");

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

    printf ("=================================================================================================\n\n");

    free (pessoa);
    
    fclose (P);
}

void LeIdade (int idade){

    FILE* P = AbrirArquivo ();

    Pessoa* pessoa = (Pessoa*) malloc(sizeof(Pessoa));

    //procurando a idade desejada no arquivo

    bool achou = FALSE;
    while (!achou && ((fread (&pessoa->cod, sizeof(int), 1, P)) > 0)){

        fread (pessoa->nome, sizeof(char), sizeof(pessoa->nome), P);
        fread (&pessoa->idade, sizeof(int), 1, P);
        fread (&pessoa->prox, sizeof(int), 1, P);

        if (idade == pessoa->idade){
            achou = TRUE;
        }
    }

    fseek (P, -sizeof(Pessoa), SEEK_CUR);

    if (achou){

        printf ("\n=================================================================================================\n");

        do{
            fread (&pessoa->cod, sizeof(int), 1, P);
            fread (pessoa->nome, sizeof(char), sizeof(pessoa->nome), P);
            fread (&pessoa->idade, sizeof(int), 1, P);
            fread (&pessoa->prox, sizeof(int), 1, P);

            fseek (P, sizeof(Pessoa) * pessoa->prox, SEEK_SET);

            printf ("Pesssoa %d de nome %s tem idade %d\n", pessoa->cod, pessoa->nome, pessoa->idade);
        } while (pessoa->prox != -1);

        printf ("=================================================================================================\n\n");
    }
        
    else printf ("Idade nao encontrada\n");

    fclose (P);
    free (pessoa);
}


//criar um novo arquivo que organiza
void OrganizaSaltos() {
    FILE* P = AbrirArquivo();

    // Posição inicial no arquivo.
    fseek(P, 0, SEEK_SET);

    Pessoa* organizado = (Pessoa*) malloc(sizeof(Pessoa));
    Pessoa* temp = (Pessoa*) malloc(sizeof(Pessoa));

    int posicao_atual = 0; // Índice do registro atual.

    printf ("\n=================================================================================================\n");

    while (fread(organizado, sizeof(Pessoa), 1, P) > 0) {
        printf("Registro atual: %d, Idade: %d, Prox antes: %d\n", 
               posicao_atual, organizado->idade, organizado->prox);

        // Verifica se o campo 'prox' já foi configurado. Se sim, pula.
        if (organizado->prox != -1) {
            posicao_atual++;
            continue;
        }

        // Calcula o próximo registro com a mesma idade.
        int prox_pos = -1; // Valor padrão se não encontrar.
        int busca_pos = posicao_atual + 1;

        fseek(P, sizeof(Pessoa) * busca_pos, SEEK_SET); // Vai para o próximo registro.

        while (fread(temp, sizeof(Pessoa), 1, P) > 0) {
            if (temp->idade == organizado->idade) {
                prox_pos = busca_pos;
                break;
            }
            busca_pos++;
        }

        // Atualiza o campo 'prox' do registro atual.
        organizado->prox = prox_pos;

        // Posiciona o cursor de volta para reescrever o registro atualizado.
        fseek(P, sizeof(Pessoa) * posicao_atual, SEEK_SET);
        if (fwrite(organizado, sizeof(Pessoa), 1, P) != 1) {
            printf("Erro ao escrever no arquivo.\n");
            exit(1);
        }

        //printf("Atualizado: Registro atual %d -> Prox = %d\n", posicao_atual, prox_pos);

        // Move para o próximo registro.
        posicao_atual++;
        fseek(P, sizeof(Pessoa) * posicao_atual, SEEK_SET);
    }

    printf ("=================================================================================================\n\n");

    
    free(organizado);
    free(temp);
    fclose(P);
}

int main (){
    
    int escolha;

    bool organizado = FALSE;

    int idade;

    while (TRUE){

        printf ("O que dejesa fazer?\n");
        printf ("1- Imprimir o arquivo atual\n");
        printf ("2- Organizar os saltos do arquivo\n");
        printf ("3- Buscar todos os usuarios com a mesma idade\n");
        printf ("0- Encerrar o programa\n");

        scanf ("%d", &escolha);


        switch (escolha)
        {
        case 1:

            LerArquivoTodo ();
            
            break;
        
        case 2:
            
            OrganizaSaltos ();

            organizado = TRUE;

            break;
        
        case 3:

            printf ("Insira a idade que deseja pesquisar: ");
            scanf("%d", &idade);

            //para caso a pessoa tente procurar antes de organizar
            if (!organizado) OrganizaSaltos ();

            LeIdade (idade);

            break;
        
        case 0:
            
            exit(1);

            break;
        
        default:
            printf ("Opcao invalida, reinsira por favor\n");
            break;
        }
    }
}