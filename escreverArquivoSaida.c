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

int escreverPrimeiraLinhaArquivoSaida(elemento *tabelaElementos, char nomeArquivoSaida[TAMANHO_MAXIMO_NOME_ARQUIVO + 1], int numeroNos, int numeroElementos)
{
	FILE *arquivoSaida;
	char linhaArquivo[TAMANHO_MAXIMO_LINHA_ARQUIVO + 1];
	char auxiliar[MAXIMO_AUXILIAR_LINHA_SAIDA + 1];
	int contador;

	arquivoSaida = fopen(nomeArquivoSaida, "w");
	if(arquivoSaida == NULL)
		return ERRO_ABRINDO_ARQUIVO_ESCRITA;
	strcpy(linhaArquivo, "t");//tralha colocado para plotar graficos com gnuplot
	for(contador = 1; contador <= numeroNos; contador++)
	{
		sprintf(auxiliar, " %i", contador);
		strcat(linhaArquivo, auxiliar);
	}
	for(contador = 0; contador < numeroElementos; contador++)
	{
		switch (tabelaElementos[contador].tipo)
		{
			case 'L':
				strcpy(auxiliar, " j");
				strcat(linhaArquivo, auxiliar);
				strcpy(auxiliar, tabelaElementos[contador].nome);
				strcat(linhaArquivo, auxiliar);
			break;
			case 'C':
				strcpy(auxiliar, " j");
				strcat(linhaArquivo, auxiliar);
				strcpy(auxiliar, tabelaElementos[contador].nome);
				strcat(linhaArquivo, auxiliar);
			break;
			case 'E':
				strcpy(auxiliar, " j");
				strcat(linhaArquivo, auxiliar);
				strcpy(auxiliar, tabelaElementos[contador].nome);
				strcat(linhaArquivo, auxiliar);
			break;
			case 'F':
				strcpy(auxiliar, " jCurtoControle");
				strcat(linhaArquivo, auxiliar);
				strcpy(auxiliar, tabelaElementos[contador].nome);
				strcat(linhaArquivo, auxiliar);
			break;
			case 'H':
				strcpy(auxiliar, " j");
				strcat(linhaArquivo, auxiliar);
				strcpy(auxiliar, tabelaElementos[contador].nome);
				strcat(linhaArquivo, auxiliar);
				strcpy(auxiliar, " jCurtoControle");
				strcat(linhaArquivo, auxiliar);
				strcpy(auxiliar, tabelaElementos[contador].nome);
				strcat(linhaArquivo, auxiliar);
			break;
			case 'V':
				strcpy(auxiliar, " j");
				strcat(linhaArquivo, auxiliar);
				strcpy(auxiliar, tabelaElementos[contador].nome);
				strcat(linhaArquivo, auxiliar);
			break;
			case 'O':
				strcpy(auxiliar, " j");
				strcat(linhaArquivo, auxiliar);
				strcpy(auxiliar, tabelaElementos[contador].nome);
				strcat(linhaArquivo, auxiliar);
			break;
			default:
				//
			break;
		} 
	}
	strcat(linhaArquivo, "\n");
	fprintf(arquivoSaida, "%s", linhaArquivo);
	fclose(arquivoSaida);
	return OK;
}

int escreverLinhaSolucaoArquivoSaida(float *solucao, int numeroVariaveis, float tempo, char nomeArquivoSaida[TAMANHO_MAXIMO_NOME_ARQUIVO + 1])
{
	char auxiliar[MAXIMO_AUXILIAR_LINHA_SAIDA + 1];
	char linhaArquivo[TAMANHO_MAXIMO_LINHA_ARQUIVO + 1];
	int contador;
	FILE *arquivoSaida;

	arquivoSaida = fopen(nomeArquivoSaida, "a");
	if(arquivoSaida == NULL)
		return ERRO_ABRINDO_ARQUIVO_ESCRITA;
	sprintf(auxiliar, "%g", tempo);
	strcpy(linhaArquivo, auxiliar);
	for(contador = 1; contador <= numeroVariaveis; contador++)
	{
		sprintf(auxiliar, " %g", solucao[contador]);
		strcat(linhaArquivo, auxiliar);
	}
	strcat(linhaArquivo, "\n");
	fprintf(arquivoSaida, "%s", linhaArquivo);
	fclose(arquivoSaida);
	return OK;
}
