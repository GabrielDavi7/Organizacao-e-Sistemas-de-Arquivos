# Define o compilador
CXX = g++

# Define as flags de compilação
CXXFLAGS = -std=c++11 -Wall

# Define o nome do executável
TARGET = gerenciador

# Define os arquivos fonte
SOURCES = main.cpp SistemaGerenciador.cpp

# Define os arquivos objeto
OBJECTS = $(SOURCES:.cpp=.o)

# Alvo principal: compila o projeto
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Regra para compilar arquivos .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Alvo para executar o programa
run: all
	./$(TARGET)

# Alvo para limpar os arquivos gerados
clean:
	rm -f $(TARGET) $(OBJECTS) alunos.dat indices.dat

.PHONY: all run clean