// SistemaGerenciador.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>
#include "SistemaGerenciador.h"
#include <cstring>

void SistemaGerenciador::iniciar() {
    int opcao;
    do {
        // Exibe o menu de opções [cite: 115, 117]
        std::cout << "\n============ Sistema de Gerenciamento de Alunos ============" << std::endl;
        std::cout << "1. Gerar Arquivo de Dados a partir do CSV" << std::endl;
        std::cout << "2. Gerar Arquivo de Indice" << std::endl; 
        std::cout << "3. Buscar Aluno por Matricula" << std::endl; 
        std::cout << "4. Sair" << std::endl; 
        std::cout << "Escolha uma opcao (): "; 
        std::cin >> opcao;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (opcao) {
            case 1:
                gerarArquivoDados();
                break;
            case 2:
                gerarArquivoIndice();
                break;
            case 3:
                buscarRegistroPorMatricula();
                break;
            case 4:
                std::cout << "Saindo do sistema." << std::endl;
                break;
            default:
                std::cout << "Opcao invalida." << std::endl;
        }
    } while (opcao != 4);
}


void SistemaGerenciador::gerarArquivoDados(){
    std::ifstream csvFile(arquivoCSV);
    if(!csvFile.is_open()){
        std::cerr << "Erro ao abrir o arquivo csv." <<std::endl;
        return;
    }
    std::ofstream dataFile(arquivoDados, std::ios::binary);
    if(!dataFile.is_open()){
        std::cerr << "Erro ao criar o arquivo de dados." <<std::endl;
        return;
    }

    std::string linha;
    std::getline(csvFile, linha);
    Aluno aluno;

    while(std::getline(csvFile, linha)){
        std::stringstream ss(linha);
        std::string field;

        std::getline(ss, field, ',');
        aluno.matricula = std::stoi(field);

        std::getline(ss, field, ',');
        strncpy(aluno.nome, field.c_str(), sizeof(aluno.nome) - 1);
        aluno.nome[sizeof(aluno.nome) - 1] = '\0';

        std::getline(ss, field, ',');
        strncpy(aluno.curso, field.c_str(), sizeof(aluno.curso) - 1);
        aluno.curso[sizeof(aluno.curso) - 1] = '\0';
        escreverRegistro(dataFile, aluno);
    }

    std::cout<<"O arquivo de dados foi gerado com sucesso"<<std::endl;
    csvFile.close();
    dataFile.close();
}


void SistemaGerenciador::buscarRegistroPorMatricula(){



}


void SistemaGerenciador::gerarArquivoIndice(){
    

    
}


void SistemaGerenciador::escreverRegistro(std::ofstream& out, const Aluno& aluno) {
    out.write(reinterpret_cast<const char*>(&aluno), sizeof(Aluno));
}

bool SistemaGerenciador::lerRegistro(std::ifstream& in, Aluno& aluno, long offset) {
    if (offset >= 0) {
        in.seekg(offset); 
    }
    in.read(reinterpret_cast<char*>(&aluno), sizeof(Aluno));
    return in.gcount() == sizeof(Aluno); 
}