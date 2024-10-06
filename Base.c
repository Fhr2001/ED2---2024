#include <stdio.h>
#include <stdlib.h>

#define MAX_TAM 6  // Tamanho máximo da memória disponível para os registros

typedef struct {
    int chave;
    int flag;  // 0: ativo, 1: congelado
} Registro;

// Função para gerar partições
void gerar_particoes(FILE *entrada) {
    Registro memoria[MAX_TAM];
    int num_registros = 0;
    int fim_arquivo = 0;
    int particao_num = 1;  // Contador de partições
    int ultimo_escrito = -1;  // Último registro escrito na partição atual

    // Carrega registros iniciais até encher a memória ou até o fim do arquivo
    while (num_registros < MAX_TAM && !fim_arquivo) {
        if (fscanf(entrada, "%d", &memoria[num_registros].chave) != 1) {
            fim_arquivo = 1;  // Sinaliza fim do arquivo
        } else {
            memoria[num_registros].flag = 0;  // Marca todos como ativos
            num_registros++;
        }
    }

    // Loop principal para gerar múltiplas partições
    while (num_registros > 0) {
        // Cria um novo arquivo de saída para a partição
        char nome_saida[20];
        sprintf(nome_saida, "particao_%d.txt", particao_num);
        FILE *saida = fopen(nome_saida, "w");

        if (!saida) {
            printf("Erro ao criar arquivo de saída.\n");
            return;
        }

        printf("Gerando partição %d\n", particao_num);

        // Processa a partição até que todos os registros fiquem congelados
        while (1) {
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
                fprintf(saida, "%d\n", memoria[menor_indice].chave);
                ultimo_escrito = memoria[menor_indice].chave;
            } else {
                // Se o registro for menor que o último escrito, ele deve ser congelado
                memoria[menor_indice].flag = 1;
            }

            // Lê o próximo registro do arquivo
            if (fscanf(entrada, "%d", &memoria[menor_indice].chave) != 1) {
                memoria[menor_indice].flag = 1;  // Congela o registro se não houver mais entradas
            }
        }

        fclose(saida);
        particao_num++;

        // Descongela todos os registros para a próxima partição
        for (int i = 0; i < num_registros; i++) {
            memoria[i].flag = 0;  // Todos os registros são reativados
        }

        // Verifica se ainda há registros disponíveis para processar
        num_registros = 0;
        ultimo_escrito = -1;  // Reinicia o valor do último escrito
        while (num_registros < MAX_TAM && !fim_arquivo) {
            if (fscanf(entrada, "%d", &memoria[num_registros].chave) != 1) {
                fim_arquivo = 1;  // Sinaliza fim do arquivo
            } else {
                memoria[num_registros].flag = 0;  // Marca todos como ativos
                num_registros++;
            }
        }
    }
}

int main() {
    FILE *entrada = fopen("entrada2.txt", "r");

    if (!entrada) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    gerar_particoes(entrada);

    fclose(entrada);

    return 0;
}
