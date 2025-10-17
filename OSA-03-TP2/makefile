
# Compilador C++
CC		:= g++

FLAGS 	:= -Wall -Wextra -std=c++17 -pedantic-errors

DEBUG 	:= -g

MATH 	:= -lm

MAIN 	:= main


OBJECTS := $(MAIN).o RegistroAluno.o Buffer.o

ifeq ($(OS), Windows_NT)
	OUTPUTMAIN := $(MAIN).exe
else
	OUTPUTMAIN := $(MAIN).out
endif


#--- Regras do Makefile ---

# Regra padrão: compila todo o projeto
all: $(OUTPUTMAIN)
	@echo Compilacao concluida! Executavel: $(OUTPUTMAIN)

# Regra de linkagem: cria o executável final a partir dos arquivos objeto
$(OUTPUTMAIN): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(OUTPUTMAIN) $(MATH)
	
main.o: Main.cpp Arquivo.h RegistroAluno.h
	$(CC) $(FLAGS) -c Main.cpp -o main.o
	
RegistroAluno.o: RegistroAluno.cpp RegistroAluno.h Registro.h
	$(CC) $(FLAGS) -c RegistroAluno.cpp

# Regra para compilar a classe Buffer
Buffer.o: Buffer.cpp Buffer.h
	$(CC) $(FLAGS) -c Buffer.cpp

clean:
	@echo Limpando arquivos gerados...
	rm -f $(OBJECTS) $(OUTPUTMAIN)
	@echo Limpeza concluida!

# Regra "run": compila e executa o programa
run: all
	./$(OUTPUTMAIN)

# Evita que o 'make' confunda as regras com nomes de arquivos
.PHONY: all clean run