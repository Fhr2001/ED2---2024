#include <stdio.h>
#include <limits.h>

#define MAX_PARTICOES 5     // Número de partições
#define MAX_ELEMENTOS 15    // Número máximo de elementos por partição (ajustar conforme necessário)

// Função para encontrar o índice do vencedor entre dois índices
int find_winner(int a, int b, int candidatos[]) {
    return (candidatos[a] < candidatos[b]) ? a : b;  // Retornar o índice do menor elemento
}

// Função para construir a árvore de vencedores com múltiplas partições
void build_winner_tree(int candidatos[], int n_particoes, int winner_tree[]) {
    int start = n_particoes;

    // Copiar os índices dos candidatos para as folhas da árvore
    for (int i = 0; i < n_particoes; i++) {
        winner_tree[start + i] = i;
    }

    // Preencher os nós internos da árvore com os índices dos vencedores
    for (int i = start - 1; i > 0; i--) {
        winner_tree[i] = find_winner(winner_tree[2 * i], winner_tree[2 * i + 1], candidatos);
    }
}

// Função para obter o próximo elemento de uma partição
int obter_proximo(int particoes[][MAX_ELEMENTOS], int indice_particao, int *indice_elemento, int n_elementos[]) {
    if (indice_elemento[indice_particao] < n_elementos[indice_particao]) {
        return particoes[indice_particao][indice_elemento[indice_particao]++];
    } else {
        return INT_MAX; // Retornar um valor muito alto para indicar que a partição está vazia
    }
}

int main() {
    int particoes[MAX_PARTICOES][MAX_ELEMENTOS] = {
        {6, 7, 14, 29, 46, 48, 59, 74, 75, 76},
        {4, 10, 18, 20, 21, 22, 26, 49, 56, 65},
        {5, 8, 11, 15, 16, 19, 25, 50, 55, 57, 66, 77, 78},
        {9, 12, 17, 30, 32, 38, 43, 51, 54, 58, 73, 79},
        {1, 3, 13, 27, 31, 36, 47, 60, 80}
    };
    int n_particoes = 5; // Número de partições
    int n_elementos[MAX_PARTICOES] = {10, 10, 13, 12, 9}; // Número de elementos em cada partição
    int candidatos[MAX_PARTICOES]; // Guarda os candidatos atuais de cada partição
    int indice_elemento[MAX_PARTICOES] = {0}; // Índice do próximo elemento em cada partição
    int winner_tree[2 * MAX_PARTICOES]; // Árvore de vencedores

    // Inicializar os candidatos com o primeiro elemento de cada partição
    for (int i = 0; i < n_particoes; i++) {
        candidatos[i] = obter_proximo(particoes, i, &indice_elemento[i], n_elementos);
    }

    // Construir a árvore de vencedores
    build_winner_tree(candidatos, n_particoes, winner_tree);

    // Processar os vencedores até que todos os elementos sejam consumidos
    printf("Elementos ordenados:\n");
    while (1) {
        int vencedor_indice = winner_tree[1]; // Índice do vencedor atual (raiz da árvore)
        int vencedor_valor = candidatos[vencedor_indice];

        // Se o valor do vencedor for INT_MAX, todos os elementos foram processados
        if (vencedor_valor == INT_MAX) {
            break;
        }

        // Imprimir o vencedor atual
        printf("%d ", vencedor_valor);

        // Atualizar o candidato da partição que forneceu o vencedor
        candidatos[vencedor_indice] = obter_proximo(particoes, vencedor_indice, &indice_elemento[vencedor_indice], n_elementos);

        // Reconstruir a árvore de vencedores após atualizar o candidato
        build_winner_tree(candidatos, n_particoes, winner_tree);
    }

    printf("\n");
    return 0;
}
