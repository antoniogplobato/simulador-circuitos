CC = gcc 			# Compilador C 
LD = gcc			# Link-editor

CFLAGS = -Wall  		# Flags de compilacao
LFLAGS = -Wall			# Flags de link-edicao

FUNCOESMATRIZOBJS = funcoesMatriz.o
MONTARSISTEMAOBJS = montarSistema.o
LERNETLISTOBJS = lerNetlist.o
ESCREVERARQUIVOSAIDAOBJS = escreverArquivoSaida.o
ESTAMPASOBJS = estampas.o
RESOLVERMATRIZOBJS = resolverMatriz.o
FONTEOBJS = fontes.o
SPICEOBJS = spice.o lerNetlist.o estampas.o montarSistema.o funcoesMatriz.o escreverArquivoSaida.o resolverMatriz.o fontes.o

EXECS = spice

# Regra Implicita
%.o:%.c spice.h
	$(CC) $(CFLAGS) -c $<

# Objetivo ou rotulo: dependencias
# Comandos

all: $(EXECS)

spice: $(SPICEOBJS) spice.h
	$(LD) $(LFLAGS) -o $@ $(SPICEOBJS) -lm

clean:
	rm -f *.o $(EXECS)

