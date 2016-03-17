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

elemento *criarVetorElemento(int tamanhoVetor)
{
        elemento *vetorElemento;
        vetorElemento = (elemento*)(malloc(tamanhoVetor*sizeof(elemento)));
        if (vetorElemento == NULL)
                return NULL;
        return vetorElemento;
}

elemento *lerNetlist (char nomeArquivo [TAMANHO_MAXIMO_NOME_ARQUIVO + 1], int *numeroElementos, int *numeroNos, int *numeroVariaveis, int *NR, float *tempoSimulacao, float *passoSimulacao, float *passoInterno)
{
	elemento* tabelaElementos;
	FILE *arquivoNetlist;
	char linhaArquivo[MAXIMO_ELEMENTO][TAMANHO_MAXIMO_LINHA_ARQUIVO + 1];
	char auxiliar [MAXIMO_PARAMETROS][MAXIMO_AUXILIAR + 1];
	int linhaAtual;
	int numeroLinhas;
	int inicioPalavra;
	int fimPalavra;
	int qtdParametros;
	int auxiliarLetras;

	arquivoNetlist = fopen(nomeArquivo, "r");
	if (arquivoNetlist == NULL)
		return NULL;

	fgets(linhaArquivo[0], TAMANHO_MAXIMO_LINHA_ARQUIVO + 1, arquivoNetlist);
	*numeroNos = atoi(linhaArquivo[0]);
	*numeroVariaveis = *numeroNos;

	linhaAtual = 0;
	while (fgets(linhaArquivo[linhaAtual], TAMANHO_MAXIMO_LINHA_ARQUIVO + 1, arquivoNetlist) != NULL)
	{
		if (linhaArquivo[linhaAtual][strlen (linhaArquivo[linhaAtual]) - 1] == '\n')
			linhaArquivo[linhaAtual][strlen (linhaArquivo[linhaAtual]) - 1] = '\0';
		//printf("%s\n", linhaArquivo[linhaAtual]);
		if(linhaArquivo[linhaAtual][0] == '*')
			printf("Comentario: %s\n", linhaArquivo[linhaAtual]);
		else
			linhaAtual = linhaAtual + 1;
	}

	fclose(arquivoNetlist);

	numeroLinhas = linhaAtual;
	*numeroElementos = numeroLinhas;
	*NR = 0;

	tabelaElementos = criarVetorElemento(*numeroElementos);
	if(tabelaElementos == NULL)
		return NULL;

	for(linhaAtual = 0; linhaAtual < numeroLinhas; linhaAtual++)
	{
		inicioPalavra = 0;
		fimPalavra = 0;
		qtdParametros = 0;

		do
		{
			if (linhaArquivo[linhaAtual][fimPalavra] == ' ')
			{
				for(auxiliarLetras = 0; inicioPalavra < fimPalavra; auxiliarLetras++)
				{
					auxiliar[qtdParametros][auxiliarLetras] = linhaArquivo[linhaAtual][inicioPalavra];
					inicioPalavra = inicioPalavra + 1;
				}
				auxiliar[qtdParametros][auxiliarLetras] = '\0';
				//printf("%s\n", auxiliar[qtdParametros]);
				qtdParametros = qtdParametros + 1;
				inicioPalavra = fimPalavra + 1;
			}
			fimPalavra = fimPalavra + 1;
		}
		while(linhaArquivo[linhaAtual][fimPalavra] != '\0');
		for(auxiliarLetras = 0; inicioPalavra <= fimPalavra; auxiliarLetras++)
		{
			auxiliar[qtdParametros][auxiliarLetras] = linhaArquivo[linhaAtual][inicioPalavra];
			inicioPalavra = inicioPalavra + 1;
		}
		auxiliar[qtdParametros][auxiliarLetras] = '\0';
		//printf("%s\n", auxiliar[qtdParametros]);
		qtdParametros = qtdParametros + 1;

		tabelaElementos[linhaAtual].tipo = auxiliar[0][0];
		strcpy(tabelaElementos[linhaAtual].nome, auxiliar[0]);
		tabelaElementos[linhaAtual].qtdParametros = qtdParametros;

		switch (tabelaElementos[linhaAtual].tipo)
		{
			case 'R':
				tabelaElementos[linhaAtual].noA = atoi(auxiliar[1]);
				tabelaElementos[linhaAtual].noB = atoi(auxiliar[2]);
				tabelaElementos[linhaAtual].valor = atof(auxiliar[3]);
			break;
			case 'L':
				tabelaElementos[linhaAtual].noA = atoi(auxiliar[1]);
				tabelaElementos[linhaAtual].noB = atoi(auxiliar[2]);
				tabelaElementos[linhaAtual].valor = atof(auxiliar[3]);
				if(tabelaElementos[linhaAtual].qtdParametros == 5)
					tabelaElementos[linhaAtual].condicaoInicial = atof(auxiliar[4]);
				else
					tabelaElementos[linhaAtual].condicaoInicial = 0;
				*numeroVariaveis = *numeroVariaveis + 1;
				tabelaElementos[linhaAtual].correnteX = *numeroVariaveis;
			break;
			case 'C':
				tabelaElementos[linhaAtual].noA = atoi(auxiliar[1]);
				tabelaElementos[linhaAtual].noB = atoi(auxiliar[2]);
				tabelaElementos[linhaAtual].valor = atof(auxiliar[3]);
				if(tabelaElementos[linhaAtual].qtdParametros == 5)
					tabelaElementos[linhaAtual].condicaoInicial = atof(auxiliar[4]);
				else
					tabelaElementos[linhaAtual].condicaoInicial = 0;
				*numeroVariaveis = *numeroVariaveis + 1;
				tabelaElementos[linhaAtual].correnteX = *numeroVariaveis;
			break;
			case 'E':
				tabelaElementos[linhaAtual].noA = atoi(auxiliar[1]);
				tabelaElementos[linhaAtual].noB = atoi(auxiliar[2]);
				tabelaElementos[linhaAtual].noC = atoi(auxiliar[3]);
				tabelaElementos[linhaAtual].noD = atoi(auxiliar[4]);
				tabelaElementos[linhaAtual].valor = atof(auxiliar[5]);
				*numeroVariaveis = *numeroVariaveis + 1;
				tabelaElementos[linhaAtual].correnteX = *numeroVariaveis;
			break;
			case 'F':
				tabelaElementos[linhaAtual].noA = atoi(auxiliar[1]);
				tabelaElementos[linhaAtual].noB = atoi(auxiliar[2]);
				tabelaElementos[linhaAtual].noC = atoi(auxiliar[3]);
				tabelaElementos[linhaAtual].noD = atoi(auxiliar[4]);
				tabelaElementos[linhaAtual].valor = atof(auxiliar[5]);
				*numeroVariaveis = *numeroVariaveis + 1;
				tabelaElementos[linhaAtual].correnteX = *numeroVariaveis;
			break;
			case 'G':
				tabelaElementos[linhaAtual].noA = atoi(auxiliar[1]);
				tabelaElementos[linhaAtual].noB = atoi(auxiliar[2]);
				tabelaElementos[linhaAtual].noC = atoi(auxiliar[3]);
				tabelaElementos[linhaAtual].noD = atoi(auxiliar[4]);
				tabelaElementos[linhaAtual].valor = atof(auxiliar[5]);
			break;
			case 'H':
				tabelaElementos[linhaAtual].noA = atoi(auxiliar[1]);
				tabelaElementos[linhaAtual].noB = atoi(auxiliar[2]);
				tabelaElementos[linhaAtual].noC = atoi(auxiliar[3]);
				tabelaElementos[linhaAtual].noD = atoi(auxiliar[4]);
				tabelaElementos[linhaAtual].valor = atof(auxiliar[5]);
				*numeroVariaveis = *numeroVariaveis + 2;
				tabelaElementos[linhaAtual].correnteX = *numeroVariaveis - 1;
				tabelaElementos[linhaAtual].correnteY = *numeroVariaveis;
			break;
			case 'I':
				tabelaElementos[linhaAtual].noA = atoi(auxiliar[1]);
				tabelaElementos[linhaAtual].noB = atoi(auxiliar[2]);
				strcpy(tabelaElementos[linhaAtual].tipoFonte, auxiliar[3]);
				if(strcmp(tabelaElementos[linhaAtual].tipoFonte,"DC") == 0)
					tabelaElementos[linhaAtual].valorDC = atof(auxiliar[4]);
				else
				{
					if(strcmp(tabelaElementos[linhaAtual].tipoFonte, "SIN") == 0)
					{
						tabelaElementos[linhaAtual].nivelContinuo = atof(auxiliar[4]);
						tabelaElementos[linhaAtual].amplitude = atof(auxiliar[5]);
						tabelaElementos[linhaAtual].frequencia = atof(auxiliar[6]);
						tabelaElementos[linhaAtual].atrasoSin = atof(auxiliar[7]);
						tabelaElementos[linhaAtual].atenuacao = atof(auxiliar[8]);
						tabelaElementos[linhaAtual].angulo = atof(auxiliar[9]);
						tabelaElementos[linhaAtual].numeroCiclosSin = atof(auxiliar[10]);
					}
					else
					{
						if(strcmp(tabelaElementos[linhaAtual].tipoFonte, "PULSE") == 0)
						{
							tabelaElementos[linhaAtual].amplitude1 = atof(auxiliar[4]);
							tabelaElementos[linhaAtual].amplitude2 = atof(auxiliar[5]);
							tabelaElementos[linhaAtual].atrasoPulse = atof(auxiliar[6]);
							tabelaElementos[linhaAtual].tempoSubida = atof(auxiliar[7]);
							tabelaElementos[linhaAtual].tempoDescida = atof(auxiliar[8]);
							tabelaElementos[linhaAtual].tempoLigada = atof(auxiliar[9]);
							tabelaElementos[linhaAtual].periodo = atof(auxiliar[10]);
							tabelaElementos[linhaAtual].numeroCiclosPulse = atof(auxiliar[11]);
						}
					}
				}
			break;
			case 'V':
				tabelaElementos[linhaAtual].noA = atoi(auxiliar[1]);
				tabelaElementos[linhaAtual].noB = atoi(auxiliar[2]);
				strcpy(tabelaElementos[linhaAtual].tipoFonte, auxiliar[3]);
				if(strcmp(tabelaElementos[linhaAtual].tipoFonte, "DC") == 0)
					tabelaElementos[linhaAtual].valorDC = atof(auxiliar[4]);
				else
				{
					if(strcmp(tabelaElementos[linhaAtual].tipoFonte,"SIN") == 0)
					{
						tabelaElementos[linhaAtual].nivelContinuo = atof(auxiliar[4]);
						tabelaElementos[linhaAtual].amplitude = atof(auxiliar[5]);
						tabelaElementos[linhaAtual].frequencia = atof(auxiliar[6]);
						tabelaElementos[linhaAtual].atrasoSin = atof(auxiliar[7]);
						tabelaElementos[linhaAtual].atenuacao = atof(auxiliar[8]);
						tabelaElementos[linhaAtual].angulo = atof(auxiliar[9]);
						tabelaElementos[linhaAtual].numeroCiclosSin = atof(auxiliar[10]);
					}
					else
					{
						if(strcmp(tabelaElementos[linhaAtual].tipoFonte, "PULSE") == 0)
						{
							tabelaElementos[linhaAtual].amplitude1 = atof(auxiliar[4]);
							tabelaElementos[linhaAtual].amplitude2 = atof(auxiliar[5]);
							tabelaElementos[linhaAtual].atrasoPulse = atof(auxiliar[6]);
							tabelaElementos[linhaAtual].tempoSubida = atof(auxiliar[7]);
							tabelaElementos[linhaAtual].tempoDescida = atof(auxiliar[8]);
							tabelaElementos[linhaAtual].tempoLigada = atof(auxiliar[9]);
							tabelaElementos[linhaAtual].periodo = atof(auxiliar[10]);
							tabelaElementos[linhaAtual].numeroCiclosPulse = atof(auxiliar[11]);
						}
					}
				}
				*numeroVariaveis = *numeroVariaveis + 1;
				tabelaElementos[linhaAtual].correnteX = *numeroVariaveis;
			break;
			case 'O':
				tabelaElementos[linhaAtual].noA = atoi(auxiliar[1]);
				tabelaElementos[linhaAtual].noB = atoi(auxiliar[2]);
				tabelaElementos[linhaAtual].noC = atoi(auxiliar[3]);
				tabelaElementos[linhaAtual].noD = atoi(auxiliar[4]);
				*numeroVariaveis = *numeroVariaveis + 1;
				tabelaElementos[linhaAtual].correnteX = *numeroVariaveis;
			break;
			case 'D':
				*NR = 1;
				tabelaElementos[linhaAtual].noA = atoi(auxiliar[1]);
				tabelaElementos[linhaAtual].noB = atoi(auxiliar[2]);
				if(tabelaElementos[linhaAtual].qtdParametros == 5)
				{
					tabelaElementos[linhaAtual].Is = atof(auxiliar[3]);
					tabelaElementos[linhaAtual].Vt = atof(auxiliar[4]);
				}
				else
				{
					tabelaElementos[linhaAtual].Is = 0.000000000000037751345;
					tabelaElementos[linhaAtual].Vt = 0.025;
				}
			break;
			case 'Q':
				*NR = 1;
				tabelaElementos[linhaAtual].noA = atoi(auxiliar[1]);
				tabelaElementos[linhaAtual].noB = atoi(auxiliar[2]);
				tabelaElementos[linhaAtual].noC = atoi(auxiliar[3]);
				strcpy(tabelaElementos[linhaAtual].tipoTransistor, auxiliar[4]);
				if(tabelaElementos[linhaAtual].qtdParametros == 12)
				{
					tabelaElementos[linhaAtual].alfa = atof(auxiliar[5]);
					tabelaElementos[linhaAtual].alfar = atof(auxiliar[6]);
					tabelaElementos[linhaAtual].Isbe = atof(auxiliar[7]);
					tabelaElementos[linhaAtual].Vtbe = atof(auxiliar[8]);
					tabelaElementos[linhaAtual].Isbc = atof(auxiliar[9]);
					tabelaElementos[linhaAtual].Vtbc = atof(auxiliar[10]);
					tabelaElementos[linhaAtual].VA = atof(auxiliar[11]);
				}
				else
				{
					tabelaElementos[linhaAtual].alfa = 0.99;
					tabelaElementos[linhaAtual].alfar = 0.5;
					tabelaElementos[linhaAtual].Isbe = 3.7751345e-14;
					tabelaElementos[linhaAtual].Vtbe = 25e-3;
					tabelaElementos[linhaAtual].Isbc = 3.7751345e-14;
					tabelaElementos[linhaAtual].Vtbc = 25e-3;
					tabelaElementos[linhaAtual].VA = 100;
				}
			break;
			case '.':
				*numeroElementos = *numeroElementos - 1;
				*tempoSimulacao = atof(auxiliar[1]);
				*passoSimulacao = atof(auxiliar[2]);
				*passoInterno = atof(auxiliar[4]);
				break;
			default:
				return NULL;
				break;
		} 
	}
	return tabelaElementos;
}
