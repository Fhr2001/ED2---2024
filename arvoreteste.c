#include <stdio.h>
#include <stdlib.h>
#define HV 9999999

int tam = 0;

void troca(int *x,int *y)
{
    int temp = *y;
    *y = *x;
    *x = temp;
}

void arv(int vetor[], int tam, int i)
{
    int maior, noEsq, noDir;
    if(tam == 1)
    {
        printf("so um elemento existe \n");
    }
    else
    {
        maior = i;
        noEsq = 2 * i + 1;
        noDir = 2 * i + 2;
        if(noEsq < tam && vetor[noEsq] > vetor[i])
            maior = noEsq;
        if(noDir < tam && vetor[noDir] > vetor[i])
            maior = noDir;
        if(maior != i)
        {
            troca(&vetor[i], &vetor[maior]);
            arv(vetor,tam,maior);
        }
    }
}

void insere(int vetor[], int novo)
{
    int i;
    if(tam == 0)
    {
        vetor[0] = novo;
        tam = tam+1;
    }
    else
    {
        vetor[tam] = novo;
        tam = tam+1;
        for(i = tam/(2-1);i >= 0;i--)
        {
           arv(vetor,tam,i); 
        }
    }
}

void print(int vetor[],int tam)
{
    int i;
    for(i = 0; i < tam; i++)
    {
        printf("%d",vetor[i]);
        printf("\n");
    }
}

int main()
{
    int vetor[10];

    insere(vetor,10);
    insere(vetor,4);
    insere(vetor,1);
    insere(vetor,8);
    insere(vetor,5);
    insere(vetor,3);

    print(vetor,tam);

    return 0;
}


