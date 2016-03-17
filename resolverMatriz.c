//Simulador de Circuitos no Tempo
//Grupo:
//			Antonio Lobato
//			Eduardo Frimer
//			Ulisses Figueiredo
//
//Circuitos Eletricos II
//Professor: Antônio Carlos Moreirão de Queiroz

//==============================================================================
//
// Codigo retirado do programa mnaamp
//
// Autor: Antonio Carlos M. de Queiroz acmq@coe.ufrj.br
//
//==============================================================================

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "spice.h"

int resolversistema(float **Yn, int neq)
{
  int i,j,l, a;
  double t, p;

  for (i=1; i<=neq; i++) {
    t=0.0;
    a=i;
    for (l=i; l<=neq; l++) {
      if (fabs(Yn[l][i])>fabs(t)) {
  a=l;
  t=Yn[l][i];
      }
    }
    if (i!=a) {
      for (l=1; l<=neq+1; l++) {
  p=Yn[i][l];
  Yn[i][l]=Yn[a][l];
  Yn[a][l]=p;
      }
    }
    if (fabs(t)<TOLG) {
      printf("Sistema singular\n\r");
      return 1;
    }
    for (j=neq+1; j>0; j--) {  /* Ponha j>0 em vez de j>i para melhor visualizacao */
      Yn[i][j] /= t;
      p=Yn[i][j];
      for (l=1; l<=neq; l++) {
  if (l!=i)
    Yn[l][j]-=Yn[l][i]*p;
      }
    }
  }
  return 0;
}
