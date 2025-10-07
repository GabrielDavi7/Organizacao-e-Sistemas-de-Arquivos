#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <vector>
#include <string>
#include "Registro.h"

class Arquivo {
public:
    std::string nomeDoArquivo;

    Arquivo(const std::string& nome);

    std::vector<Registro> lerRegistrosCSV();
    void adicionarRegistroFixo(Registro& registro);
    std::vector<Registro> lerTodosRegistrosFixos(); // Renomeado para clareza

    Registro buscarPorMatricula(int matricula);
    bool removerPorMatricula(int matricula);
    bool atualizarPorMatricula(int matricula);
};

#endif