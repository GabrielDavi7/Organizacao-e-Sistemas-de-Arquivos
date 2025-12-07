##############################################################################
# Arquivo principal: main.cpp
##############################################################################


# definição de variáveis auxiliares do makefile
MAIN = main
OBJECTS	= $(MAIN).o
FLAGS = -Wall -Wextra -std=c++17 -pedantic-errors 
MATH = -lm
DEBUG = -g
CC = g++

# ajustando alguns parâmetros/comandos ao sistema operacional
# ajuste às necessidades do seu projeto
ifeq ($(OS), Windows_NT)
DEL := del
OUTPUTMAIN := $(MAIN).exe
CLS := cls
else
DEL := rm
OUTPUTMAIN := $(MAIN).out
CLS := clear
endif

# regra de compilação do projeto completo
all: main.exe

# regra para geração do arquivo executavel
main.exe: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(MAIN).exe $(FLAGS)

# regra de compilação do objeto main.o
main.o: $(MAIN).cpp
	$(CC) -c $(MAIN).cpp -o $(MAIN).o 

# regra para limpeza do projeto (excluir arquivos .o e .exe). 
# (OBS.: SO windows)
clean:
	$(DEL) $(OBJECTS)
	$(DEL) $(OUTPUTMAIN)

# regra para compilar todo o projeto (se necessário) e, posteriormente, executar
# (OBS.: SO windows)
run: all
	$(CLS)
	./$(OUTPUTMAIN)