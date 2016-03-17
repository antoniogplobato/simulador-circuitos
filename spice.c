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
#include <math.h>

int main(int argc, char const *argv[])
{
	char nomeArquivo[TAMANHO_MAXIMO_NOME_ARQUIVO + 1];
	char nomeArquivoSaida[TAMANHO_MAXIMO_NOME_ARQUIVO + 1];
	elemento *tabelaElementos;
	int numeroElementos;
	int numeroNos;
	int numeroVariaveis;
	int NR;
	float tempoAtual;
	float tempoSimulacao;
	float passoSimulacao;
	float passoInterno;

	float *solucaoAtual;
	float *solucaoAnterior;
	float **sistemaA;
	float *sistemaB;
	float **sistemaResposta;
	int erro;
	int contador;
	int tentativas;
	int tentativasChute;
	int convergiu;
	int tempoSalvar;
	int iteracoesTempo;

	printf("Digite o nome do arquivo de netlist que deseja abrir:\n");
	scanf("%s", nomeArquivo);
	printf("Digite o nome do arquivo para salvar os dados da simulacao:\n");
	scanf("%s", nomeArquivoSaida);

	tabelaElementos = lerNetlist(nomeArquivo, &numeroElementos, &numeroNos, &numeroVariaveis, &NR, &tempoSimulacao, &passoSimulacao, &passoInterno);
	if(tabelaElementos == NULL)
	{
		printf("Erro funcao lerNetlist.\n");
		exit(ERRO_FUNCAO_LER_NETLIST);
	}
	sistemaA = criarMatriz(numeroVariaveis + 1);
	if(sistemaA == NULL)
	{
		printf("Erro funcao criar matriz.\n");
		exit(ERRO_FUNCAO_CRIAR_MATRIZ);
	}
	sistemaB = criarVetor(numeroVariaveis + 1);
	if(sistemaB == NULL)
	{
		printf("Erro funcao criar vetor.\n");
		exit(ERRO_FUNCAO_CRIAR_VETOR);
	}
	solucaoAtual = criarVetor(numeroVariaveis + 1);
	if(solucaoAtual == NULL)
	{
		printf("Erro funcao criar vetor.\n");
		exit(ERRO_FUNCAO_CRIAR_VETOR);
	}
	for(contador = 1; contador < numeroVariaveis + 1; contador++)
		solucaoAtual[contador] = 0.3; 
	solucaoAnterior = criarVetor(numeroVariaveis + 1);
	if(solucaoAnterior == NULL)
	{
		printf("Erro funcao criar vetor.\n");
		exit(ERRO_FUNCAO_CRIAR_VETOR);
	}
	sistemaResposta = criarMatrizModificada(numeroVariaveis + 1);
	if(sistemaResposta == NULL)
	{
		printf("Erro funcao criar matriz.\n");
		exit(ERRO_FUNCAO_CRIAR_VETOR);
	}
	erro = escreverPrimeiraLinhaArquivoSaida(tabelaElementos, nomeArquivoSaida, numeroNos, numeroElementos);
	if(erro != 0)
	{
		printf("Erro funcao escrever primeira linha arquivo saida.\n");
		return ERRO_FUNCAO_ESCREVER_PRIMEIRA_LINHA_ARQUIVO_SAIDA;
	}
	tempoSalvar = (int) passoInterno;
	if(tempoSalvar < 1)
		tempoSalvar = 1;
	iteracoesTempo = 0;
	for(tempoAtual = 0; tempoAtual <= tempoSimulacao; tempoAtual+=passoSimulacao)
	{
		if(NR == 0)
		{
			zerarMatriz(sistemaA,  numeroVariaveis + 1, numeroVariaveis + 1);
			zerarVetor(sistemaB, numeroVariaveis + 1);
			erro = montarSistema(tabelaElementos, sistemaA, sistemaB, numeroElementos, solucaoAtual, NULL, tempoAtual, passoSimulacao);
			if(erro != 0)
			{
				printf("Erro ao montar sistema\n");
				return ERRO_MONTAR_SISTEMA;
			}
			//imprimirMatriz(sistemaA, numeroVariaveis + 1, numeroVariaveis + 1, 1);
			//printf("\n");
			converterParaResolver(sistemaA, sistemaB, sistemaResposta, numeroVariaveis + 1);
			//imprimirMatriz(sistemaResposta, numeroVariaveis + 1, numeroVariaveis + 2, 1);
			//printf("\n");
			erro = resolversistema(sistemaResposta, numeroVariaveis);
			if(erro != 0)
			{
				printf("Circuito sem solucao.\n");
				return ERRO_RESOLVER_SISTEMA;
			}
			//imprimirMatriz(sistemaResposta, numeroVariaveis + 1, numeroVariaveis + 2, 1);
			//printf("\n");
			for(contador = 1; contador < numeroVariaveis + 1; contador++)
			{
				solucaoAtual[contador] = sistemaResposta[contador][numeroVariaveis + 1];
			}
			if((iteracoesTempo % tempoSalvar) == 0)
			{
				erro = escreverLinhaSolucaoArquivoSaida(solucaoAtual, numeroVariaveis, tempoAtual, nomeArquivoSaida);
				if(erro != 0)
				{
					printf("Erro escrever solucao no arquivo saida.\n");
					return ERRO_FUNCAO_ESCREVER_LINHA_SOLUCAO_ARQUIVO_SAIDA;
				}
			}
			iteracoesTempo = iteracoesTempo + 1;
		}
		else
		{
			tentativasChute = 0;
			denovo:
			for(tentativas = 0; tentativas <= NUMERO_TENTATIVAS; tentativas++)
			{
				convergiu = 1;
				zerarMatriz(sistemaA,  numeroVariaveis + 1, numeroVariaveis + 1);
				zerarVetor(sistemaB, numeroVariaveis + 1);
				erro = montarSistema(tabelaElementos, sistemaA, sistemaB, numeroElementos, solucaoAnterior, solucaoAtual, tempoAtual, passoSimulacao);
				//solucao anterior eh a do tempo anterior, enquanto a atual eh o palpite do NR
				if(erro != 0)
				{
					printf("Erro ao montar sistema\n");
					return ERRO_MONTAR_SISTEMA;
				}
				//imprimirMatriz(sistemaA, numeroVariaveis + 1, numeroVariaveis + 1, 1);
				//printf("\n");
				converterParaResolver(sistemaA, sistemaB, sistemaResposta, numeroVariaveis + 1);
				//imprimirMatriz(sistemaResposta, numeroVariaveis + 1, numeroVariaveis + 2, 1);
				//printf("\n");
				erro = resolversistema(sistemaResposta, numeroVariaveis);
				if(erro != 0)
				{
					printf("Circuito sem solucao.\n");
					return ERRO_RESOLVER_SISTEMA;
				}
				//imprimirMatriz(sistemaResposta, numeroVariaveis + 1, numeroVariaveis + 2, 1);
				//printf("\n");
				for(contador = 1; contador < numeroVariaveis + 1; contador++)
				{
					if(fabs(solucaoAtual[contador] - sistemaResposta[contador][numeroVariaveis + 1]) > ERRO_MAXIMO_NR)
					{
						convergiu = 0;
					}
					solucaoAtual[contador] = sistemaResposta[contador][numeroVariaveis + 1];
				}
				if(convergiu == 1)
					break;
			}
			if(convergiu == 0)
			{
				printf("Circuito nao convergiu para o tempo %f. Tentando de novo: %i\n", tempoAtual, tentativasChute);
				for (contador = 1; contador < numeroVariaveis + 1; contador++)
					solucaoAtual[contador] = 0.3;
				tentativasChute = tentativasChute + 1;
				if (tentativasChute < NUMERO_CHUTES)
					goto denovo;
				return CIRCUITO_NAO_CONVERGIU;
			}
			for(contador = 1; contador < numeroVariaveis + 1; contador++)
			{
				solucaoAnterior[contador] = solucaoAtual[contador]; 
			}
			if((iteracoesTempo % tempoSalvar) == 0)
			{
				erro = escreverLinhaSolucaoArquivoSaida(solucaoAtual, numeroVariaveis, tempoAtual, nomeArquivoSaida);
				if(erro != 0)
				{
					printf("Erro escrever solucao no arquivo saida.\n");
					return ERRO_FUNCAO_ESCREVER_LINHA_SOLUCAO_ARQUIVO_SAIDA;
				}
			}
			iteracoesTempo = iteracoesTempo + 1;
		}
	}
	return 0;
}
