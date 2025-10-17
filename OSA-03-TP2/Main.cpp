#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cstdio> 

#include "Arquivo.h"
#include "RegistroAluno.h"

std::vector<RegistroAluno> lerCSV(const std::string& nomeArquivo) {
    std::vector<RegistroAluno> registros;
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo CSV: " + nomeArquivo);
    }

    std::string linha, cabecalho;
    std::getline(arquivo, cabecalho); 

    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        std::string nome, curso, strMatricula;
        
        std::getline(ss, nome, ';');
        std::getline(ss, strMatricula, ';');
        std::getline(ss, curso, ';');
        
        try {
            registros.emplace_back(std::stoi(strMatricula), nome, curso);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Aviso: Linha invalida no CSV ignorada: " << linha << std::endl;
        }
    }
    return registros;
}

void imprimirRegistro(const RegistroAluno& reg) {
    std::cout << "Matricula: " << reg.matricula
              << ", Nome: " << reg.nome
              << ", Curso: " << reg.curso << std::endl;
}

int main() {
    try {
        std::vector<RegistroAluno> alunos = lerCSV("dados.csv");
        std::cout << alunos.size() << " registros lidos do CSV.\n" << std::endl;

        const std::string arqFixo = "dados_fixo.bin";
        const std::string arqDelim = "dados_delimitado.bin";
        const std::string arqComp = "dados_comprimento.bin";

        std::remove(arqFixo.c_str());
        std::remove(arqDelim.c_str());
        std::remove(arqComp.c_str());

        std::cout << "--- Processando Formato FIXO ---" << std::endl;
        Arquivo<RegistroAluno> arquivoFixo(arqFixo, Formato::FIXO);
        for (const auto& aluno : alunos) {
            arquivoFixo.adicionarRegistro(aluno);
        }
        std::cout << "Arquivo '" << arqFixo << "' gerado com sucesso." << std::endl;
        
        std::cout << "\n--- Processando Formato DELIMITADO ---" << std::endl;
        Arquivo<RegistroAluno> arquivoDelim(arqDelim, Formato::DELIMITADO);
        for (const auto& aluno : alunos) {
            arquivoDelim.adicionarRegistro(aluno);
        }
        std::cout << "Arquivo '" << arqDelim << "' gerado com sucesso." << std::endl;
        
        std::cout << "\n--- Processando Formato COMPRIMENTO ---" << std::endl;
        Arquivo<RegistroAluno> arquivoComp(arqComp, Formato::COMPRIMENTO);
        for (const auto& aluno : alunos) {
            arquivoComp.adicionarRegistro(aluno);
        }
        std::cout << "Arquivo '" << arqComp << "' gerado com sucesso." << std::endl;
        
        std::cout << "\n--- Lendo para Verificacao ---" << std::endl;
        
        std::cout << "\nLendo de '" << arqFixo << "':" << std::endl;
        std::vector<RegistroAluno> alunosLidosFixo = arquivoFixo.lerRegistros();
        std::cout << alunosLidosFixo.size() << " registros lidos." << std::endl;
        if (!alunosLidosFixo.empty()) {
            std::cout << "Primeiro registro: ";
            imprimirRegistro(alunosLidosFixo.front());
            std::cout << "Ultimo registro: ";
            imprimirRegistro(alunosLidosFixo.back());
        }

        std::cout << "\nLendo de '" << arqDelim << "':" << std::endl;
        std::vector<RegistroAluno> alunosLidosDelim = arquivoDelim.lerRegistros();
        std::cout << alunosLidosDelim.size() << " registros lidos." << std::endl;
         if (!alunosLidosDelim.empty()) {
            std::cout << "Primeiro registro: ";
            imprimirRegistro(alunosLidosDelim.front());
            std::cout << "Ultimo registro: ";
            imprimirRegistro(alunosLidosDelim.back());
        }

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}