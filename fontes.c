//Simulador de Circuitos no Tempo
//Grupo:
//			Antonio Lobato
//			Eduardo Frimer
//			Ulisses Figueiredo
//
//Circuitos Eletricos II
//Professor: Antônio Carlos Moreirão de Queiroz

#include "spice.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float valorFonteSin (float nivelContinuo, float amplitude, float frequencia, float atraso, float atenuacao, float angulo, float numCiclos, float tempo)
{
	float cicloAtual;
	float valor;

	cicloAtual = frequencia*(tempo-atraso);
	if ((cicloAtual > numCiclos)||(tempo < atraso))
		valor = nivelContinuo+amplitude*sinf(M_PI*angulo/180);
	else
		valor = nivelContinuo+amplitude*expf((-1)*(tempo-atraso)*atenuacao)*sinf((2*M_PI*frequencia*(tempo-atraso)+M_PI*angulo/180));
	return valor;
}

float valorFontePulse (float amplitude1, float amplitude2, float atraso, float tempoSubida, float tempoDescida, float tempoLigada, float periodo, float numCiclos, float tempo, float passoSimulacao)
{
	float cicloAtual;

	if (tempoSubida==0)
		tempoSubida=passoSimulacao;
	if (tempoDescida==0)
		tempoDescida=passoSimulacao;
	cicloAtual = (tempo-atraso)/periodo;
	if ((cicloAtual > numCiclos)||(tempo < atraso))
		return (amplitude1);
	if ((fmodf((tempo-atraso),periodo)) < tempoSubida)
		return (amplitude1+((amplitude2-amplitude1)*(fmodf((tempo-atraso),periodo)/tempoSubida)));
	if ((fmodf((tempo-atraso),periodo)) < (tempoSubida+tempoLigada))
		return (amplitude2);
	if ((fmodf((tempo-atraso),periodo)) < (tempoSubida+tempoLigada+tempoDescida))
		return (amplitude2+((amplitude1-amplitude2)*(fmodf((tempo-atraso),periodo)-tempoSubida-tempoLigada)/tempoDescida));
	else
		return (amplitude1);
}
