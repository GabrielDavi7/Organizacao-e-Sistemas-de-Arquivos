#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>

// Defina os tamanhos máximos para os campos de string
const int Max_Nome_Tamanho = 50;
const int Max_Curso_Tamanho = 30;

// Calcule o tamanho total do registro em disco
const int Total_Tamanho_Registro = sizeof(char) +       // lapide
                                   Max_Nome_Tamanho +   // nome
                                   sizeof(int) +        // matricula
                                   Max_Curso_Tamanho;   // curso

class Registro {
private:
    char lapide;
    std::string nome;
    int matricula;
    std::string curso;

public:
    // Construtor
    Registro();

    // Métodos de serialização
    std::string packFixed();
    void unpackFixed(const std::string& buffer);

    // Métodos de acesso (Getters e Setters)
    char getLapide() const;
    std::string getNome() const;
    int getMatricula() const;
    std::string getCurso() const;

    void setLapide(char l);
    void setNome(const std::string& n);
    void setMatricula(int m);
    void setCurso(const std::string& c);
};

#endif
