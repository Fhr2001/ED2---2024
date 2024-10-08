#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int chave;
    int flag;  // 0: ativo, 1: congelado
} Registro;

void CriaReservatorio (){

    FILE* P;

    if ((P = fopen ("entrada2.dat", "w+b")) == NULL){
                printf ("não foi possível crirar a entrada2");
                exit(1);
            }

}

FILE* AbrirReservatorio (){

    FILE* P;
    if ((P = fopen ("entrada2.dat", "r+b")) == NULL){
                printf ("não foi possível abrir a entrada2");
                exit(1);
            }

    //printf ("arquivo aberto com sucesso\n");

    return P;    
}

void EscreverReservatorio (int temp){

    FILE* P = AbrirReservatorio();

    Registro registro;
    registro.chave = temp;
    registro.flag = 0;
    
    //coloca o cursor no final do arquivo
    fseek (P, 0, SEEK_END);

    fwrite (&registro.chave, sizeof(int), 1, P);
    
    fwrite (&registro.flag, sizeof(int), 1, P);

    fclose (P);
}

void LerReservatorio (){

    FILE* P = AbrirReservatorio();

    Registro* temp = (Registro*) malloc(sizeof(Registro));
       
    while ((fread (&temp->chave, sizeof(int), 1, P)) > 0){

        fread (&temp->flag, sizeof(int), 1, P);

        printf ("Chave: %d\t", temp->chave);
        printf ("Flag: %d\n", temp->flag);
    }
    
    fclose (P);

    free (temp);
}

int main (){

    int Vetor [54] = {29, 14, 76, 75, 59, 6, 7, 74,48,46,10,18,56,20,26,4,21,65,22,49,11,16,8,15,5,19,50,55,25,66,57,77,12,30,17,9,54,78,43,38,51,32,58,13,73,79,27,1,3,60,36,47,31,80,};

    CriaReservatorio();

    for (int i = 0; i < 54; i++){

        EscreverReservatorio(Vetor[i]);
    }

    LerReservatorio();

}