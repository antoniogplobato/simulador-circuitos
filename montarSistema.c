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

int montarSistema(elemento *tabelaElemento, float **sistemaA, float *sistemaB, int numeroElementos, float *solucaoAnterior, float *solucaoAtual, float tempoAtual, float passoSimulacao)
{
    //solucao anterior eh a do tempo anterior, usada para modelar os capacitores e indutores no metodo dos trapezios
    //solucao atual eh o palpite do NR, usada para modelar os elementos nao lineares
    int contador;
    float valorSIN;
    float valorPULSE;

    for(contador =0; contador < numeroElementos; contador++)
    {
        switch(tabelaElemento[contador].tipo)
        {
            case 'R':
                incluirR(sistemaA, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].valor);
                break;
            case 'G':
                incluirG(sistemaA, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].noC, tabelaElemento[contador].noD, tabelaElemento[contador].valor);
                break;
            case 'I':
                if(strcmp(tabelaElemento[contador].tipoFonte, "DC") == 0)
                	incluirI(sistemaB, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].valorDC);
                else 
                	if(strcmp(tabelaElemento[contador].tipoFonte, "SIN") == 0)
                	{
                		valorSIN = valorFonteSin(tabelaElemento[contador].nivelContinuo, tabelaElemento[contador].amplitude, tabelaElemento[contador].frequencia, tabelaElemento[contador].atrasoSin, tabelaElemento[contador].atenuacao, tabelaElemento[contador].angulo, tabelaElemento[contador].numeroCiclosSin, tempoAtual);
                		incluirI(sistemaB, tabelaElemento[contador].noA, tabelaElemento[contador].noB, valorSIN);
                	}
                	else	
                		if(strcmp(tabelaElemento[contador].tipoFonte, "PULSE") == 0)
                		{
                			valorPULSE = valorFontePulse (tabelaElemento[contador].amplitude1, tabelaElemento[contador].amplitude2, tabelaElemento[contador].atrasoPulse, tabelaElemento[contador].tempoSubida, tabelaElemento[contador].tempoDescida, tabelaElemento[contador].tempoLigada, tabelaElemento[contador].periodo, tabelaElemento[contador].numeroCiclosPulse, tempoAtual, passoSimulacao);
                			incluirI(sistemaB, tabelaElemento[contador].noA, tabelaElemento[contador].noB, valorPULSE);
                		}
                		else
                		{	
                			printf("Tipo Fonte invalida\n");
                			return TIPO_FONTE_INVALIDA;
                		}
                break;
            case 'V':
                if(strcmp(tabelaElemento[contador].tipoFonte, "DC") == 0)
                    incluirV(sistemaA, sistemaB, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].correnteX, tabelaElemento[contador].valorDC);
                else
                    if(strcmp(tabelaElemento[contador].tipoFonte, "SIN") == 0)
                	{
                        valorSIN = valorFonteSin(tabelaElemento[contador].nivelContinuo, tabelaElemento[contador].amplitude, tabelaElemento[contador].frequencia, tabelaElemento[contador].atrasoSin, tabelaElemento[contador].atenuacao, tabelaElemento[contador].angulo, tabelaElemento[contador].numeroCiclosSin, tempoAtual);
                        incluirV(sistemaA, sistemaB, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].correnteX, valorSIN);
                    }
                	else	
                		if(strcmp(tabelaElemento[contador].tipoFonte, "PULSE") == 0)
                		{
                			valorPULSE = valorFontePulse (tabelaElemento[contador].amplitude1, tabelaElemento[contador].amplitude2, tabelaElemento[contador].atrasoPulse, tabelaElemento[contador].tempoSubida, tabelaElemento[contador].tempoDescida, tabelaElemento[contador].tempoLigada, tabelaElemento[contador].periodo, tabelaElemento[contador].numeroCiclosPulse, tempoAtual, passoSimulacao);
                			incluirV(sistemaA, sistemaB, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].correnteX, valorPULSE);
                		}
                	else
                		{	
                			printf("Tipo Fonte invalida\n");
                			return TIPO_FONTE_INVALIDA;
                		}
                break;
            case 'E':
                incluirE(sistemaA, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].noC, tabelaElemento[contador].noD, tabelaElemento[contador].correnteX, tabelaElemento[contador].valor);
                break;
            case 'F':
                incluirF(sistemaA, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].noC, tabelaElemento[contador].noD, tabelaElemento[contador].correnteX, tabelaElemento[contador].valor);
                break;
            case 'H':
                incluirH(sistemaA, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].noC, tabelaElemento[contador].noD, tabelaElemento[contador].correnteX, tabelaElemento[contador].correnteY, tabelaElemento[contador].valor);
                break;
            case 'O':
                incluirO(sistemaA, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].noC, tabelaElemento[contador].noD, tabelaElemento[contador].correnteX);
                break;
            case 'C':
                incluirC(sistemaA, sistemaB, solucaoAnterior, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].correnteX, tabelaElemento[contador].valor, tabelaElemento[contador].condicaoInicial, tempoAtual, passoSimulacao);
                break;
            case 'L':
                incluirL(sistemaA, sistemaB, solucaoAnterior, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].correnteX, tabelaElemento[contador].valor, tabelaElemento[contador].condicaoInicial, tempoAtual, passoSimulacao);
                break;
            case 'D':
            	incluirD(sistemaA, sistemaB, solucaoAtual, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].Is, tabelaElemento[contador].Vt);
            	break;
            case 'Q':
                if(strcmp(tabelaElemento[contador].tipoTransistor, "NPN") == 0)
                     incluirQNPN(sistemaA, sistemaB, solucaoAtual, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].noC, tabelaElemento[contador].alfa, tabelaElemento[contador].alfar, tabelaElemento[contador].Isbe, tabelaElemento[contador].Vtbe, tabelaElemento[contador].Isbc, tabelaElemento[contador].Vtbc, tabelaElemento[contador].VA);
                else if(strcmp(tabelaElemento[contador].tipoTransistor, "PNP") == 0)
                     incluirQPNP(sistemaA, sistemaB, solucaoAtual, tabelaElemento[contador].noA, tabelaElemento[contador].noB, tabelaElemento[contador].noC, tabelaElemento[contador].alfa, tabelaElemento[contador].alfar, tabelaElemento[contador].Isbe, tabelaElemento[contador].Vtbe, tabelaElemento[contador].Isbc, tabelaElemento[contador].Vtbc, tabelaElemento[contador].VA);
                else 
                     {	
                	 printf("Tipo Transistor invalido\n");
                     return TIPO_TRANSISTOR_INVALIDO;
                     }
                break;
            default:
                    return ERRO_TIPO_INVALIDO;
                    break;
        }
    }
        return 0;

}

