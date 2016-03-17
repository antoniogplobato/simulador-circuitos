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

int incluirR(float **sistemaA, int noA, int noB, float valor)
{
	sistemaA[noA][noA]+=(1/valor);
	sistemaA[noB][noB]+=(1/valor);
	sistemaA[noA][noB]-=(1/valor);
	sistemaA[noB][noA]-=(1/valor);
	return 0;
}
int incluirG(float **sistemaA, int noA, int noB, int noC, int noD, float valor)
{
	sistemaA[noA][noC]+=valor;
	sistemaA[noB][noD]+=valor;
	sistemaA[noA][noD]-=valor;
	sistemaA[noB][noC]-=valor;
	return 0;
}
int incluirI(float *sistemaB, int noA, int noB, float valor)
{
	sistemaB[noA]-=valor;
	sistemaB[noB]+=valor;
	return 0;
}
int incluirV(float **sistemaA, float *sistemaB, int noA, int noB, int correnteX, float valor)
{
	sistemaA[noA][correnteX]+=1;
	sistemaA[noB][correnteX]-=1;
	sistemaA[correnteX][noA]-=1;
	sistemaA[correnteX][noB]+=1;
	sistemaB[correnteX]-=valor;
	return 0;
}
int incluirE(float **sistemaA, int noA, int noB, int noC, int noD, int correnteX, float valor)
{
	sistemaA[noA][correnteX]+=1;
	sistemaA[noB][correnteX]-=1;
	sistemaA[correnteX][noA]-=1;
	sistemaA[correnteX][noB]+=1;
	sistemaA[correnteX][noC]+=valor;
	sistemaA[correnteX][noD]-=valor;
	return 0;
}
int incluirF(float **sistemaA, int noA, int noB, int noC, int noD, int correnteX, float valor)
{
	sistemaA[noA][correnteX]+=valor;
	sistemaA[noB][correnteX]-=valor;
	sistemaA[noC][correnteX]+=1;
	sistemaA[noD][correnteX]-=1;
	sistemaA[correnteX][noC]-=1;
	sistemaA[correnteX][noD]+=1;
	return 0;
}
int incluirH(float **sistemaA, int noA, int noB, int noC, int noD, int correnteX, int correnteY, float valor)
{
	sistemaA[noA][correnteX]+=1;
	sistemaA[noB][correnteX]-=1;
	sistemaA[noC][correnteY]+=1;
	sistemaA[noD][correnteY]-=1;
	sistemaA[correnteX][noA]-=1;
	sistemaA[correnteX][noB]+=1;
	sistemaA[correnteY][noC]-=1;
	sistemaA[correnteY][noD]+=1;
	sistemaA[correnteX][correnteY]+=valor;
	return 0;
}
int incluirO(float **sistemaA, int noA, int noB, int noC, int noD, int correnteX)
{
	sistemaA[noA][correnteX]+=1;
	sistemaA[noB][correnteX]-=1;
	sistemaA[correnteX][noC]+=1;
	sistemaA[correnteX][noD]-=1;
	return 0;
}

int incluirC(float **sistemaA, float *sistemaB, float *solucaoAnterior, int noA, int noB, int correnteX, float valor, float condicaoInicial, float tempoAtual, float passoSimulacao)
{
	float tempoBackward;

	tempoBackward = passoSimulacao*0.000000001;
	//Backward
	if(tempoAtual == 0)
	{
		sistemaA[noA][correnteX]+=1;
		sistemaA[noB][correnteX]-=1;
		sistemaA[correnteX][noA]-=1;
		sistemaA[correnteX][noB]+=1;
		sistemaA[correnteX][correnteX]+=tempoBackward/valor;
		sistemaB[correnteX]-=condicaoInicial;
	}
	else //Trapezio
	{
		sistemaA[noA][correnteX]+=1;
		sistemaA[noB][correnteX]-=1;
		sistemaA[correnteX][noA]-=1;
		sistemaA[correnteX][noB]+=1;
		sistemaA[correnteX][correnteX]+=passoSimulacao/(2*valor);
		sistemaB[correnteX]-=(solucaoAnterior[noA]-solucaoAnterior[noB]) + (passoSimulacao/(2*valor))*solucaoAnterior[correnteX];
	}
	return 0;
}

int incluirL(float **sistemaA, float *sistemaB, float *solucaoAnterior, int noA, int noB, int correnteX, float valor, float condicaoInicial, float tempoAtual, float passoSimulacao)
{
	float tempoBackward;

	tempoBackward = passoSimulacao*0.000000001;
	//Backward
	if(tempoAtual == 0)
	{
		sistemaA[noA][correnteX]+=1;
		sistemaA[noB][correnteX]-=1;
		sistemaA[correnteX][noA]-=1;
		sistemaA[correnteX][noB]+=1;
		sistemaA[correnteX][correnteX]+=valor/tempoBackward;
		sistemaB[correnteX]+=(valor/tempoBackward)*condicaoInicial;
	}
	else //Trapezio
	{
		sistemaA[noA][correnteX]+=1;
		sistemaA[noB][correnteX]-=1;
		sistemaA[correnteX][noA]-=1;
		sistemaA[correnteX][noB]+=1;
		sistemaA[correnteX][correnteX]+=(2*valor)/passoSimulacao;
		sistemaB[correnteX]+=(solucaoAnterior[noA]-solucaoAnterior[noB]) + solucaoAnterior[correnteX]*((2*valor)/passoSimulacao);
	}
	return 0;
}

int incluirD(float **sistemaA, float *sistemaB, float *solucaoAtual, int noA, int noB, float Is, float Vt)
{
	float Go;
	float Io;
	float tensaoAtual;

	tensaoAtual = solucaoAtual[noA] - solucaoAtual[noB];
	if(tensaoAtual > TENSAO_MAXIMA_DIODO)
		tensaoAtual = TENSAO_MAXIMA_DIODO;
	Go = (Is/Vt)*expf(tensaoAtual/Vt);
	Io = Is*(expf(tensaoAtual/Vt) - 1) - (Is/Vt)*expf(tensaoAtual/Vt)*tensaoAtual;
	sistemaA[noA][noA]+=Go;
	sistemaA[noB][noB]+=Go;
	sistemaA[noA][noB]-=Go;
	sistemaA[noB][noA]-=Go;
	sistemaB[noA]-=Io;
	sistemaB[noB]+=Io;
	return 0;
}

int incluirQNPN(float **sistemaA, float *sistemaB, float *solucaoAtual, int noA, int noB, int noC, float alfa, float alfaR, float Isbe, float Vtbe, float Isbc, float Vtbc, float Va)
{
	float G1, G2, G3, G4, G5;
	float Io, I1, I2;
	float vce, vbc, vbe;

	vbc = solucaoAtual[noB] - solucaoAtual[noA];
	vbe = solucaoAtual[noB] - solucaoAtual[noC];
	vce = solucaoAtual[noA] - solucaoAtual[noC];

	if (vbe > TENSAO_MAXIMA_DIODO)
		vbe=TENSAO_MAXIMA_DIODO;

	if (vbc > TENSAO_MAXIMA_DIODO)
		vbc=TENSAO_MAXIMA_DIODO;
	
    G1=(Isbe/Vtbe)*expf(vbe/Vtbe);
    I1=Isbe*(expf(vbe/Vtbe)-1)-(Isbe/Vtbe)*expf(vbe/Vtbe)*vbe;
    G2=(Isbc/Vtbc)*expf(vbc/Vtbc);
    I2=Isbc*(expf(vbc/Vtbc)-1)-(Isbc/Vtbc)*expf(vbc/Vtbc)*vbc;
    
    if (vce < 0)
    {
            G3=0;
            G4=0;
            G5=0;
            Io=0;
    }
    else
    {
            G3 = (alfa*vce*G1)/Va;
            G4 =-(vce*G2)/Va;
            G5 = (alfa*((Isbe*expf(vbe/Vtbe))) - (Isbc*expf(vbc/Vtbc)))/Va;
            Io =-G3*vbe -G4*vbc;
    }

        //noA=C noB=B noC=E
        sistemaA[noA][noA]+= G5+G2-G4;
        sistemaA[noA][noB]+= G1*alfa-G2+G3+G4;
        sistemaA[noA][noC]+=-G1*alfa-G3-G5;
        sistemaA[noB][noA]+= G2*(alfaR-1);
        sistemaA[noB][noB]+= G1*(1-alfa)+G2*(1-alfaR);
        sistemaA[noB][noC]+= G1*(alfa-1);
        sistemaA[noC][noA]+=-G2*alfaR+G4-G5;
        sistemaA[noC][noB]+=-G1+G2*alfaR-G3-G4;
        sistemaA[noC][noC]+= G1+G3+G5;
        sistemaB[noA]+= I2-alfa*I1-Io;
        sistemaB[noB]+=-I1*(1-alfa)-I2*(1-alfaR);
        sistemaB[noC]+= I1-I2*alfaR+Io;
	return 0;
}
int incluirQPNP(float **sistemaA, float *sistemaB, float *solucaoAtual, int noA, int noB, int noC, float alfa, float alfaR, float Isbe, float Vtbe, float Isbc, float Vtbc, float Va)
{
	float G1, G2, G3, G4, G5;
	float Io, I1, I2;
	float vce, vbc, vbe;

	vbc = solucaoAtual[noB] - solucaoAtual[noA];
	vbe = solucaoAtual[noB] - solucaoAtual[noC];
	vce = solucaoAtual[noA] - solucaoAtual[noC];

	if (vbe > TENSAO_MAXIMA_DIODO)
		vbe=TENSAO_MAXIMA_DIODO;

	if (vbc > TENSAO_MAXIMA_DIODO)
		vbc=TENSAO_MAXIMA_DIODO;
	
    G1=(Isbe/Vtbe)*expf(vbe/Vtbe);
    I1=Isbe*(expf(vbe/Vtbe)-1)-(Isbe/Vtbe)*expf(vbe/Vtbe)*vbe;
    G2=(Isbc/Vtbc)*expf(vbc/Vtbc);
    I2=Isbc*(expf(vbc/Vtbc)-1)-(Isbc/Vtbc)*expf(vbc/Vtbc)*vbc;
    
    if (vce < 0)
    {
            G3=0;
            G4=0;
            G5=0;
            Io=0;
    }
    else
    {
            G3 =-(alfa*vce*G1)/Va;
            G4 = (vce*G2)/Va;
            G5 = (-alfa*(Isbe*expf(vbe/Vtbe)) + Isbc*expf(vbc/Vtbc))/Va;
            Io =-G3*vbe -G4*vbc;
    }
    //noA=C noB=B noC=E
	sistemaA[noA][noA]+= G2+G5-G4;
	sistemaA[noA][noB]+=-G2-G1*alfa+G3+G4;	  	  
	sistemaA[noA][noC]+= G1*alfa-G3-G5;
	sistemaA[noB][noA]+= G2*(-alfaR-1);
    sistemaA[noB][noB]+= G1*(1+alfa)+G2*(1+alfaR);	
	sistemaA[noB][noC]+= G1*(-alfa-1);
	sistemaA[noC][noA]+= G2*alfaR+G4-G5;
	sistemaA[noC][noB]+=-G1-G2*alfaR-G3-G4;
    sistemaA[noC][noC]+= G1+G3+G5;
	sistemaB[noA]+=-I2+I1*alfa-Io;
    sistemaB[noB]+= I1-I2*alfaR+I2-I1*alfa;
    sistemaB[noC]+=-I1+I2*alfaR+Io;
 return 0;
 
}
