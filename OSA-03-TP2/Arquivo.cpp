#include "Arquivo.h"
#include "Buffer.h"
#include "Registro.h"
#include <fstream>
#include <sstream>
#include <iostream>

Arquivo::Arquivo(const std::string& nome) : nomeDoArquivo(nome) {}

// Adaptado para ler o CSV de Alunos com delimitador ';'
std::vector<Registro> Arquivo::lerRegistrosCSV() {
    std::vector<Registro> vetorRegistros;
    std::ifstream arquivo(this->nomeDoArquivo);

    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo CSV: " << nomeDoArquivo << std::endl;
        return vetorRegistros;
    }

    std::string linha;
    std::getline(arquivo, linha); // Pula a linha do cabeçalho

    while (std::getline(arquivo, linha)) {
        std::stringstream ss(linha);
        std::string campo;
        Registro reg;
        
        reg.setLapide(' '); // Registros do CSV são sempre ativos
        
        std::string tempNome, tempCurso;
        
        std::getline(ss, tempNome, ';');
        reg.setNome(tempNome);

        std::getline(ss, campo, ';'); 
        try {
            reg.setMatricula(std::stoi(campo)); 
        } catch (...) {
            continue;
        }

        std::getline(ss, tempCurso, ';');
        reg.setCurso(tempCurso);

        vetorRegistros.push_back(reg);
    }
    arquivo.close();
    return vetorRegistros;
}

void Arquivo::adicionarRegistroFixo(Registro& registro) {
    std::ofstream arquivo(this->nomeDoArquivo, std::ios::binary | std::ios::app);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeDoArquivo << std::endl;
        return;
    }
    Buffer escritaBuffer;
    escritaBuffer.EscreverRegistroFixo(registro, arquivo);
    arquivo.close();
}

// Lógica de busca sequencial no arquivo binário
Registro Arquivo::buscarPorMatricula(int matricula) {
    std::ifstream arquivo(this->nomeDoArquivo, std::ios::binary);
    Buffer leituraBuffer;

    while (arquivo.peek() != EOF) {
        Registro reg = leituraBuffer.LerRegistroFixo(arquivo);
        if (arquivo.gcount() > 0) {
            if (reg.getLapide() == ' ' && reg.getMatricula() == matricula) {
                arquivo.close();
                return reg;
            }
        }
    }
    arquivo.close();
    return Registro();
}

// Remoção lógica: encontra o registro e marca a lápide com '*'
bool Arquivo::removerPorMatricula(int matricula) {
    std::fstream arquivo(this->nomeDoArquivo, std::ios::in | std::ios::out | std::ios::binary);
    if (!arquivo) return false;

    Buffer leituraBuffer;
    long long pos;

    while (arquivo.peek() != EOF) {
        pos = arquivo.tellg();
        // CORREÇÃO: Chamando a função corretamente
        Registro reg = leituraBuffer.LerRegistroFixo(arquivo);
        
        if (arquivo.gcount() > 0) {
            if (reg.getLapide() == ' ' && reg.getMatricula() == matricula) {
                arquivo.seekp(pos);
                char lapide = '*';
                arquivo.write(&lapide, sizeof(char));
                arquivo.close();
                return true;
            }
        }
    }
    arquivo.close();
    return false;
}

// Atualização: encontra o registro e o sobrescreve com novos dados
bool Arquivo::atualizarPorMatricula(int matricula) {
    std::fstream arquivo(this->nomeDoArquivo, std::ios::in | std::ios::out | std::ios::binary);
    if (!arquivo) return false;

    Buffer buffer;
    long long pos;

    while (arquivo.peek() != EOF) {
        pos = arquivo.tellg();
        // CORREÇÃO: Chamando a função corretamente
        Registro reg = buffer.LerRegistroFixo(arquivo);
        
        if (arquivo.gcount() > 0) {
            if (reg.getLapide() == ' ' && reg.getMatricula() == matricula) {
                std::cout << "Registro encontrado. Digite os novos dados:\n";
                
                std::string novoNome, novoCurso;
                std::cout << "Novo nome: ";
                std::getline(std::cin >> std::ws, novoNome);
                reg.setNome(novoNome);

                std::cout << "Novo curso: ";
                std::getline(std::cin >> std::ws, novoCurso);
                reg.setCurso(novoCurso);

                arquivo.seekp(pos);
                buffer.EscreverRegistroFixo(reg, arquivo);
                arquivo.close();
                return true;
            }
        }
    }
    arquivo.close();
    return false;
}
