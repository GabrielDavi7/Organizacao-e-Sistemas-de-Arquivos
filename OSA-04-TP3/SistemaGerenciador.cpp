// SistemaGerenciador.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>
#include "SistemaGerenciador.h"
#include <cstring>

SistemaGerenciador::SistemaGerenciador(const std::string& csv, const std::string& dados, const std::string& indice)
    : arquivoCSV(csv), arquivoDados(dados), arquivoIndice(indice) {}

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

void SistemaGerenciador::gerarArquivoIndice(){
    std::ifstream dataFile(arquivoDados, std::ios::binary);
    if(!dataFile.is_open()){
        std::cerr << "Erro ao abrir o arquivo de dados." <<std::endl;
        return;
    }
    std::vector<Indice> indices;
    Aluno aluno;
    long Pos = 0;

    while (lerRegistro(dataFile, aluno)) {
        Indice idx;
        idx.matricula = aluno.matricula;
        idx.byte_offset = Pos;
        indices.push_back(idx);
        Pos = dataFile.tellg();
    }

    std::sort(indices.begin(), indices.end(), [](const Indice& a, const Indice& b) {
        return a.matricula < b.matricula;
    });

    std::ofstream indexFile(arquivoIndice, std::ios::binary);
    if (!indexFile.is_open()) {
        std::cerr << "Erro ao criar o arquivo de indice." << std::endl;
        return;
    }

    for (const auto& idx : indices) {
        indexFile.write(reinterpret_cast<const char*>(&idx), sizeof(Indice));
    }

    std::cout << "O arquivo de indice foi gerado com sucesso"<< std::endl;
    dataFile.close();
    indexFile.close();
}


void SistemaGerenciador::buscarRegistroPorMatricula(){
    std::ifstream indexFile(arquivoIndice, std::ios::binary);
    if(!indexFile.is_open()){
        std::cerr << "Erro ao abrir o arquivo de dados." <<std::endl;
        return;}

    std::vector<Indice> indices;
    Indice idx;
    while (indexFile.read(reinterpret_cast<char*>(&idx), sizeof(Indice))) {
        indices.push_back(idx);
    }
    indexFile.close();

    if (indices.empty()) {
        std::cout << "O arquivo de indices esta vazio." << std::endl;
        return;
    }

    int matriculaBusca;
    std::cout << "Digite o numero da matricula: ";
    std::cin >> matriculaBusca;

    // Realiza uma pesquisa binária no vetor de índices
    auto it = std::lower_bound(indices.begin(), indices.end(), matriculaBusca, 
        [](const Indice& a, int mat) {
        return a.matricula < mat;
    });

    if (it != indices.end() && it->matricula == matriculaBusca) {
        std::ifstream dataFile(arquivoDados, std::ios::binary);
        if (!dataFile.is_open()) {
            std::cerr << "Erro ao abrir o arquivo de dados." << std::endl;
            return;
        }

        Aluno aluno;
        // Usa o byte_offset para ler o registro diretamente
        if (lerRegistro(dataFile, aluno, it->byte_offset)) {
            std::cout << "\n=== Aluno Encontrado ===" << std::endl;
            std::cout << "Matricula: " << aluno.matricula << std::endl;
            std::cout << "Nome: " << aluno.nome << std::endl;
            std::cout << "Curso: " << aluno.curso << std::endl;
            std::cout << "==============================" << std::endl;
        }
        dataFile.close();
    } else {
        std::cout <<"Matricula nao encontrada."<<std::endl;
    }
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