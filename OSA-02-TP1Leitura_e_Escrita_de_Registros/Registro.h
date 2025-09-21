#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>


// Aqui faremos a definicao do tamanho maximo do nome em 50 caractere 
const int Max_Nome_Tamanho = 50;
const int Total_Tamanho_Registro = sizeof(int) + Max_Nome_Tamanho;

class Registro{
    public:

    int idade;
    std::string nome;
    std::string packFixed(); // metodo de serialização converte os atributos da classe para uma string de bytes de tamanho fixo
    void unpackFixed(const std::string& buffer);

};
#endif