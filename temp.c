#include <stdio.h>
#include <stdlib.h>

#define MAX_TAM 6  // Tamanho máximo da memória disponível para os registros

typedef struct {
    int chave;
    int flag;  // 0: ativo, 1: congelado
} Registro;


void CriaReservatorio (){

    FILE* P;

    if ((P = fopen ("reservatorio.dat", "w+b")) == NULL){
                printf ("não foi possível crirar o reservatorio");
                exit(1);
            }

}

//incrementa o valor das partições no return
int CriaParticao (int particao_num){

    char convercao [20];

    sprintf(convercao, "particao_%d.dat", particao_num);
    
    //printf("%s\n", convercao);

    FILE* P;

    if ((P = fopen (convercao, "w+b")) == NULL){
                printf ("não foi possível crirar a particao %d", particao_num);
                exit(1);
            }

    //printf ("arquivo criado com sucesso\n");

    return particao_num +=1;
}

FILE* AbrirParticao (int particao_num){

    char convercao [10];

    sprintf(convercao, "particao_%d.dat", (particao_num-1));

    FILE* P;
    if ((P = fopen (convercao, "r+b")) == NULL){
                printf ("não foi abrir a particao %d", particao_num -1);
                exit(1);
            }

    //printf ("arquivo aberto com sucesso\n");

    return P; 
}

FILE* AbrirReservatorio (){

    FILE* P;
    if ((P = fopen ("Reservatorio.dat", "r+b")) == NULL){
                printf ("não foi possível abrir o reservatorio");
                exit(1);
            }

    //printf ("arquivo aberto com sucesso\n");

    return P;    
}

void InsereReservatorio (Registro* registro, int passadas){

    FILE* R = AbrirReservatorio();

    //coloca o cursor no inicio do arquivo
    fseek (R, 0, SEEK_SET);

    for (int i = 0; i <passadas; i++){

        fread (&registro->chave, sizeof(char), sizeof(int), R);      
        fread (&registro->flag, sizeof(char), sizeof(int), R);
    }

    fwrite (&registro->chave, sizeof(int), 1, R);
    fwrite (&registro->flag, sizeof(int), 1, R);

    fclose (R);
}

void EscreverParticao (Registro* registro, int particao_num){

    FILE* P = AbrirParticao(particao_num);
    
    //coloca o cursor no final do arquivo
    fseek (P, 0, SEEK_END);

    fwrite (&registro->chave, sizeof(int), 1, P);
    
    fwrite (&registro->flag, sizeof(int), 1, P);

    fclose (P);
}

//retorna 1 registro (esta com malloc aberto)
//retorna -1 se não tiver encontrado registro
Registro* RetiraReservatorio (FILE* R){

    //FILE* R = AbrirReservatorio();

    Registro* registro = (Registro*) malloc(sizeof(Registro));

    //coloca o cursor no inicio do arquivo
    //fseek (R, 0, SEEK_SET);


    //caso esteja vazio
    if ((fread (&registro->chave, sizeof(int), 1, R)) < 0){
        registro->chave = -1;
        registro->flag = 1;
        //fclose (R);

        return registro;
    }

    fread (&registro->flag, sizeof(int), 1, R);

    fseek (R, -sizeof(int), SEEK_CUR);

    //colocando 0 na flag antiga para indicar que saiu do reservatorio
    fwrite (0, sizeof(int), 1, R);

    //fclose (R);

    return registro;
}

void LerTodasParicoes (int particao_num){

    Registro* temp = (Registro*) malloc(sizeof(Registro));

    for (int i = 2; i < particao_num; i++){

        FILE* P = AbrirParticao (i);
        
        printf ("============================\n");       
        printf("Lendo particao %d\n", i);
        printf ("============================\n");       

        while ((fread (&temp->chave, sizeof(int), 1, P)) > 0){

            fread (&temp->flag, sizeof(int), 1, P);

            printf ("Chave: %d\t", temp->chave);
            printf ("Flag: %d\n", temp->flag);
        }
        printf ("============================\n");

        fclose (P);
    }

    free (temp);
}

Registro* LerReservatorio (int contador){

    FILE* P = AbrirReservatorio();

    Registro* temp = (Registro*) malloc(sizeof(Registro));    

    if ((fread (&temp->chave, sizeof(int), 1, P)) > 0){

        temp->chave = -1;
        temp->flag = 1;

        fclose (P);

    free (temp);

    return temp;
    }

    fread (&temp->flag, sizeof(int), 1, P);
       
    fclose (P);

    free (temp);

    return temp;
}

// Função para gerar partições
void gerar_particoes(FILE *entrada) {

    int contador = 0; //conta quantos Clientes foram guardados no reservatório (6 é o limite)

    Registro memoria[MAX_TAM];
    int num_registros = 0;
    int fim_arquivo = 0;
    int particao_num = 1;  // Contador de partições
    int ultimo_escrito = -1;  // Último registro escrito na partição atual

    Registro* a;

    // Carrega registros iniciais até encher a memória ou até o fim do arquivo
    while (num_registros < MAX_TAM && !fim_arquivo) {
        a = LerReservatorio (contador);
        memoria [num_registros].chave = a->chave;
        memoria [num_registros].flag = a->flag;
        if (memoria[num_registros].chave == -1) {
            fim_arquivo = 1;  // Sinaliza fim do arquivo
        } else {
            num_registros++;
        }
    }

    

    //vetor que carregará os itens do reservatório
    Registro reservatorio [MAX_TAM];

    for (int i = 0; i < MAX_TAM; i++){

        //para evitar que seja utilizado quanto o reservatorio ainda não foi preenchido
        reservatorio[i].flag = 1;
    }

    // Loop principal para gerar múltiplas partições
    while (num_registros > 0) {              
        
        // Cria um novo arquivo de saída para a partição
        particao_num = CriaParticao(particao_num);

        FILE* saida = AbrirParticao(particao_num);

        printf("Gerando partição %d\n", particao_num-1);
        
        //Reservatorio esvaziado
        contador = 0;    
          
        // Processa a partição até que todos os registros fiquem congelados
        while (contador < MAX_TAM) {
            // Encontra o menor registro ativo
            int menor_indice = -1;
            for (int i = 0; i < num_registros; i++) {
                if (memoria[i].flag == 0 && (menor_indice == -1 || memoria[i].chave < memoria[menor_indice].chave)) {
                    menor_indice = i;
                }
            }

            if (menor_indice == -1) {
                break;  // Todos os registros estão congelados
            }

            // Verifica se o registro atual pode ser escrito na partição atual
            if (memoria[menor_indice].chave >= ultimo_escrito) {
                // Escreve o menor registro na partição de saída
                EscreverParticao (&memoria[menor_indice], particao_num);
                ultimo_escrito = memoria[menor_indice].chave;
            } else {
                // Se o registro for menor que o último escrito, ele deve ser congelado
                memoria[menor_indice].flag = 1;
                InsereReservatorio (&memoria[menor_indice], contador);
                contador++;
            }

            // Lê o próximo registro do arquivo
            a = LerReservatorio (contador);
            memoria [num_registros].chave = a->chave;
            memoria [num_registros].flag = a->flag;

            if (memoria[menor_indice].chave == -1) {
                memoria[menor_indice].flag = 1;  // Congela o registro se não houver mais entradas
            }
        }

        //caso o reservatório tenha lotado, deve-se esvaziar o resto da memoria na partição       
        while (1){

            int menor_indice = -1;
            for (int i = 0; i < num_registros; i++) {
                if (memoria[i].flag == 0 && (menor_indice == -1 || memoria[i].chave < memoria[menor_indice].chave)) {
                    menor_indice = i;
                }
            }     

            if (menor_indice != -1){
                EscreverParticao (&memoria[menor_indice], particao_num);
                memoria[menor_indice].flag = 1;
            }
            else break;
        }
        

        fclose(saida);
        particao_num++;

        Registro* temporario;

        //Retirando os itens do reservatório
        FILE * R = AbrirReservatorio();
        for (int i = 0; i < MAX_TAM; i++) {
            temporario = RetiraReservatorio (R);

            memoria[i].chave = temporario->chave;
            memoria[i].flag = temporario->flag;
        }

        fclose (R);

        //Verificando se o vetor todo esta vazio/com lixo
        num_registros = 0;
        for (int i = 0; i < MAX_TAM; i++){

            if (memoria[i].flag == 0){
                num_registros++;
            }            
        }
        
        
        /*
        // Verifica se ainda há registros disponíveis para processar
        num_registros = 0;
        ultimo_escrito = -1;  // Reinicia o valor do último escrito
        while (num_registros < MAX_TAM && !fim_arquivo) {
            if (fscanf(entrada, "%d", &memoria[num_registros]->chave) != 1) {
                fim_arquivo = 1;  // Sinaliza fim do arquivo
            } else {
                memoria[num_registros]->flag = 0;  // Marca todos como ativos
                num_registros++;
            }
        }
        */
    }

    LerTodasParicoes (particao_num);
}

int main() {
    CriaReservatorio();

    FILE* entrada = AbrirReservatorio ();
    
    gerar_particoes(entrada);

    fclose(entrada);

    return 0;
}