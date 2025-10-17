// SistemaGerenciador.h
#ifndef SISTEMA_GERENCIADOR_H
#define SISTEMA_GERENCIADOR_H

#include <string>
#include "Aluno.h"
#include "Indice.h"

class SistemaGerenciador {
public:
    SistemaGerenciador(const std::string& csv, const std::string& dados, const std::string& indice);
    void iniciar();

private:
    void gerarArquivoDados();
    void gerarArquivoIndice();
    void buscarRegistroPorMatricula();

    void escreverRegistro(std::ofstream& out, const Aluno& aluno);
    bool lerRegistro(std::ifstream& in, Aluno& aluno, long offset = -1);

private:
    std::string arquivoCSV;
    std::string arquivoDados;
    std::string arquivoIndice;
};

#endif