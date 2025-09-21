#ifndef ARQUIVO_H
#define ARQUIVO_H
#include <vector>
#include <string>
#include "Registro.h"

class Arquivo{
    public:
    std::string nomeDoArquivo;

    Arquivo(const std::string&nome);
    std::vector<Registro> lerRegistrosCSV();
    void adicionarRegistroFixo(Registro& Registro); //vai adicionar no arquivo binario
    std::vector<Registro> lerRegistroFixo();
};

#endif