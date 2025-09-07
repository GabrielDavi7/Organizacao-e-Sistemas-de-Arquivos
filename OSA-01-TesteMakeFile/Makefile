#make        / Compila e linka
#make run    / Executa o programa
#make clean  / Limpa os arquivos gerados

CXX = g++

# Nome do executável
EXEC = osa_1

# Arquivos fonte (com extensão .cpp)
SRCS = main.cpp leitura.cpp pessoa.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC)

%.o: %.cpp
	$(CXX) -c $< -o $@

run: all
	./$(EXEC)

clean:
	rm -f $(EXEC) $(OBJS)