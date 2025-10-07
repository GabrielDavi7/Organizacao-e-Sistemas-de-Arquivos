#include "Registro.h"
#include <cstring>


// Implementação do Construtor
Registro::Registro() : lapide(' '), matricula(0) {}

// Implementação dos Getters (para ler os dados)
char Registro::getLapide() const {
    return this->lapide;
}

std::string Registro::getNome() const {
    return this->nome;
}

int Registro::getMatricula() const {
    return this->matricula;
}

std::string Registro::getCurso() const {
    return this->curso;
}

// Implementação dos Setters (para alterar os dados)
void Registro::setLapide(char l) {
    this->lapide = l;
}

void Registro::setNome(const std::string& n) {
    this->nome = n;
}

void Registro::setMatricula(int m) {
    this->matricula = m;
}

void Registro::setCurso(const std::string& c) {
    this->curso = c;
}



std::string Registro::packFixed() {
    char buffer[Total_Tamanho_Registro];
    std::memset(buffer, '\0', Total_Tamanho_Registro);

    char* ptr = buffer;

    std::memcpy(ptr, &lapide, sizeof(char));
    ptr += sizeof(char);

    std::memcpy(ptr, nome.c_str(), nome.length());
    ptr += Max_Nome_Tamanho;

    std::memcpy(ptr, &matricula, sizeof(int));
    ptr += sizeof(int);

    std::memcpy(ptr, curso.c_str(), curso.length());

    return std::string(buffer, Total_Tamanho_Registro);
}

void Registro::unpackFixed(const std::string& buffer) {
    const char* ptr = buffer.c_str();

    std::memcpy(&lapide, ptr, sizeof(char));
    ptr += sizeof(char);

    char tempNome[Max_Nome_Tamanho + 1];
    std::memcpy(tempNome, ptr, Max_Nome_Tamanho);
    tempNome[Max_Nome_Tamanho] = '\0';
    this->nome = std::string(tempNome);
    ptr += Max_Nome_Tamanho;

    std::memcpy(&matricula, ptr, sizeof(int));
    ptr += sizeof(int);

    char tempCurso[Max_Curso_Tamanho + 1];
    std::memcpy(tempCurso, ptr, Max_Curso_Tamanho);
    tempCurso[Max_Curso_Tamanho] = '\0';
    this->curso = std::string(tempCurso);
}
