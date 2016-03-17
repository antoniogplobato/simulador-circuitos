//Simulador de Circuitos no Tempo
//Grupo:
//			Antonio Lobato
//			Eduardo Frimer
//			Ulisses Figueiredo
//
//Circuitos Eletricos II
//Professor: Antônio Carlos Moreirão de Queiroz

#ifndef SPICE
#define SPICE "spice.h"

#define TAMANHO_MAXIMO_NOME				10
#define MAXIMO_ELEMENTO					100 //maximo de elementos
#define TAMANHO_MAXIMO_NOME_ARQUIVO		30
#define TAMANHO_MAXIMO_LINHA_ARQUIVO	1000
#define TAMANHO_MAXIMO_PARAMETROS		30
#define TAMANHO_MAXIMO_TIPO_FONTE		10
#define TAMANHO_MAXIMO_TIPO_TRANSISTOR	5
#define MAXIMO_AUXILIAR					20
#define MAXIMO_AUXILIAR_LINHA_SAIDA		30
#define MAXIMO_PARAMETROS 				12

//Constantes de tempo para simulacao
//#define TEMPO_DE_SIMULACAO				1 					//segundos
//#define PASSO_SIMULACAO					0.1 		  		//segundos

//Constantes para o metodo de Newton Raphson
#define NUMERO_TENTATIVAS		100
#define NUMERO_CHUTES			30
#define ERRO_MAXIMO_NR			0.0001
#define TENSAO_MAXIMA_DIODO		1

#define TOLG 			1e-9
#define	BASE_NUMERO 	20
#define FAIXA_NUMERO	5
#define M_PI			3.14159265358979323846

#define _CRT_SECURE_NO_WARNINGS //coisa pra funcionar no windows

//Macros de erro
#define OK													0
#define ERRO_FUNCAO_LER_NETLIST								1
#define ERRO_FUNCAO_CRIAR_MATRIZ							2
#define ERRO_FUNCAO_CRIAR_VETOR								3
#define ERRO_TIPO_INVALIDO									4
#define ERRO_ABRINDO_ARQUIVO_ESCRITA						5
#define ERRO_RESOLVER_SISTEMA								6
#define ERRO_FUNCAO_ESCREVER_PRIMEIRA_LINHA_ARQUIVO_SAIDA	7
#define ERRO_FUNCAO_ESCREVER_LINHA_SOLUCAO_ARQUIVO_SAIDA	8
#define CIRCUITO_NAO_CONVERGIU								9
#define TIPO_FONTE_INVALIDA									10
#define ERRO_MONTAR_SISTEMA									11
#define TIPO_TRANSISTOR_INVALIDO							12

typedef struct
{
	char nome[TAMANHO_MAXIMO_NOME + 1];
	char tipo;
	int qtdParametros;
	int noA, noB, noC, noD, correnteX, correnteY; //nos e correntes do elemento 
	float valor;
	float condicaoInicial; //somente para capacitores e indutores
	char parametros[TAMANHO_MAXIMO_PARAMETROS + 1]; //somente fontes
	char tipoFonte[TAMANHO_MAXIMO_TIPO_FONTE + 1]; //somente fontes
	float valorDC; //somente fontes
	float nivelContinuo, amplitude, frequencia, atrasoSin, atenuacao, angulo, numeroCiclosSin; //somente fontes
	float amplitude1, amplitude2, atrasoPulse, tempoSubida, tempoDescida, tempoLigada, periodo, numeroCiclosPulse; //somente fontes
	float Is, Vt; //somente diodo
	char tipoTransistor[TAMANHO_MAXIMO_TIPO_TRANSISTOR + 1]; //somente transistor
	float alfa, alfar, Isbe, Vtbe, Isbc, Vtbc, VA; //somente transistor
}elemento;

elemento *criarVetorElemento(int);

elemento *lerNetlist (char[], int *, int *, int *, int *, float *, float *, float *);

float **criarMatriz(int);

float *criarVetor(int);

float** criarMatrizModificada(int);

int converterMatrizEmVetor(float **,float *, int);

int montarSistema(elemento *, float **, float *, int, float *, float *, float, float);

int imprimirMatriz(float **, int, int, int);

int imprimirVetor(float *, int, int);

int zerarMatriz(float **, int, int);

int zerarVetor(float *, int);

int converterParaResolver(float **, float *, float **, int);

float chutarNumero(int, int);

int resolversistema(float **, int);

int incluirR(float **, int , int, float);

int incluirG(float **, int, int, int, int, float);

int incluirI(float *, int, int, float);

int incluirV(float **, float *, int, int, int, float);

int incluirE(float **, int, int, int, int, int, float);

int incluirF(float **, int, int, int, int, int, float);

int incluirH(float **, int, int, int, int, int, int, float);

int incluirO(float **, int, int, int, int, int);

int incluirC(float **, float *, float *, int , int , int , float , float , float, float);

int incluirL(float **, float *, float *, int , int , int , float , float , float, float);

int incluirD(float **, float *, float *, int, int, float, float);

int incluirQNPN(float **, float *, float *, int, int, int, float, float, float, float, float, float, float);

int incluirQPNP(float **, float *, float *, int, int, int, float, float, float, float, float, float, float);

float valorFonteSin (float, float, float, float, float, float, float, float);

float valorFontePulse (float, float, float, float, float, float, float, float, float, float);

int escreverPrimeiraLinhaArquivoSaida(elemento *, char[], int , int);

int escreverLinhaSolucaoArquivoSaida(float *, int, float, char[]);

#endif
