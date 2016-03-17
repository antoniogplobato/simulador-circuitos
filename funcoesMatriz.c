//Simulador de Circuitos no Tempo
//Grupo:
//			Antonio Lobato
//			Eduardo Frimer
//			Ulisses Figueiredo
//
//Circuitos Eletricos II
//Professor: Antônio Carlos Moreirão de Queiroz

#include "spice.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

float** criarMatriz(int tamanhoMatriz)
{
    int contador1, contador2;
    float **matriz;
    matriz = (float**)(malloc(tamanhoMatriz*sizeof(float*)));
    if (!matriz)
        return NULL;
    for (contador1=0; contador1 < tamanhoMatriz; contador1++)
    {
        matriz[contador1]=(float*)(malloc(tamanhoMatriz*sizeof(float)));
        if (!matriz)
            return NULL;
        for (contador2=0; contador2 < tamanhoMatriz; contador2++)
            matriz[contador1][contador2]=0;
    }
    return matriz;
}

float* criarVetor(int tamanhoVetor)
{
    int contador;
    float *vetor;
    vetor = (float*)(malloc(tamanhoVetor*sizeof(float)));
    if (!vetor)
        return NULL;
    for (contador=0;contador<tamanhoVetor;contador++)
        vetor[contador]=0;
    return vetor;
}

int converterMatrizEmVetor(float **matriz, float *vetor, int dimensao)
{
    int contador1,contador2;
    for (contador1=0; contador1<dimensao; contador1++)
    {       
        for (contador2=0; contador2<dimensao; contador2++)
        {
            vetor[(contador1*dimensao)+contador2]=matriz[contador1][contador2];
        }
    }
    return 0;
}

int imprimirMatriz(float **matriz, int numeroLinhas, int numeroColunas, int valorInicial)
{ 
    int contador1, contador2;
    for(contador1 = valorInicial; contador1 < numeroLinhas; contador1++)
    {
        for(contador2 = valorInicial; contador2 < numeroColunas; contador2 ++)
        {
            if(matriz[contador1][contador2] > 0)
            printf("+%.1f ", matriz[contador1][contador2]);
            else
                if(matriz[contador1][contador2] < 0)
                    printf("%.1f ", matriz[contador1][contador2]);
                else
                    printf(" ... ");  
        }
        printf("\n\r");
        }
    return 0;
}

int imprimirVetor(float *vetor, int tamanhoVetor, int valorInicial)
{
    int contador;
    for(contador = valorInicial; contador < tamanhoVetor; contador++)
    {
        if(vetor[contador] > 0)
            printf("+%.1f\n\r", vetor[contador]);
        else
            if(vetor[contador] < 0)
                printf("%.1f\n\r", vetor[contador]);
            else
                printf(" ...\n\r");
    }
    return 0;
}

float** criarMatrizModificada(int tamanhoMatriz)
{
    int contador1, contador2;
    float **matriz;
    matriz = (float**)(malloc((tamanhoMatriz)*sizeof(float*)));
    if (!matriz)
            return NULL;
    for (contador1=0; contador1 < tamanhoMatriz; contador1++)
    {
            matriz[contador1]=(float*)(malloc((tamanhoMatriz + 1)*sizeof(float)));
            if (!matriz)
                    return NULL;
            for (contador2=0; contador2 < tamanhoMatriz + 1; contador2++)
                    matriz[contador1][contador2]=0;
    }
    return matriz;
}

int zerarMatriz(float **matriz, int numeroLinhas, int numeroColunas)
{
    int contador1, contador2;
    for(contador1 = 0; contador1 < numeroLinhas; contador1++)
    {
        for(contador2 = 0; contador2 < numeroColunas; contador2++)
        {
            matriz[contador1][contador2] = 0;
        }
    }
    return 0;
}

int zerarVetor(float *vetor, int tamanhoVetor)
{
    int contador;
    for(contador = 0; contador < tamanhoVetor; contador++)
    {
        vetor[contador] = 0;
    }
    return 0;
}

int converterParaResolver(float **matrizA, float *vetorB, float **matrizY, int tamanhoA)
{
    int contador1, contador2;
    for (contador1=0;contador1<tamanhoA;contador1++)
    {
        for (contador2=0;contador2<tamanhoA+1;contador2++)
        {
            if (contador2<tamanhoA)
                matrizY[contador1][contador2]=matrizA[contador1][contador2];
            else
                matrizY[contador1][contador2]=vetorB[contador1];
        }
    }
    return 0;
}

float chutarNumero(int chutar, int numeroAtual)
{
    float numero;
    int aux;
  
    srand (time(NULL));
    aux=rand()%(FAIXA_NUMERO*1000);
    numero=(float) aux;
    numero=numero/1000;
    if (chutar)
    {
        numero=(BASE_NUMERO-FAIXA_NUMERO)+2*numero;
        return(numero);
    }
    return((numeroAtual-FAIXA_NUMERO)+2*numero);
}
