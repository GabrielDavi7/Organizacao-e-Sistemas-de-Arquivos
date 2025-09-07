#include "pessoa.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <filesystem> 
#include <algorithm>  
#include <cctype>     
namespace fs = std::filesystem;

vector<Pessoa> lerTodosCSVs(const string& pasta) {
    vector<Pessoa> pessoas;

    for (const auto& entry : fs::directory_iterator(pasta)) {
        if (entry.path().extension() == ".csv") {
            ifstream arquivo(entry.path().string());
            string linha;

            if (!arquivo.is_open()) {
                cerr << "Erro ao abrir " << entry.path() << endl;
                continue;
            }

            bool primeiraLinha = true;
            while (getline(arquivo, linha)) {
                if (primeiraLinha) { // pula cabeçalho
                    primeiraLinha = false;
                    continue;
                }

                stringstream ss(linha);
                string nome, idadeStr;

                if (getline(ss, nome, ',') && getline(ss, idadeStr)) {
                    idadeStr.erase(remove_if(idadeStr.begin(), idadeStr.end(), ::isspace), idadeStr.end());

                    try {
                        int idade = stoi(idadeStr);
                        pessoas.emplace_back(nome, idade);
                    } catch (invalid_argument&) {
                        cerr << "Linha inválida: " << linha << endl;
                        continue;
                    }
                }
            }

            arquivo.close();
        }
    }

    return pessoas;
}

void exibirNomesIdade(const vector<Pessoa>& pessoas) {
    cout << endl;
    
    cout << left << setw(30) << "Nome" 
         << setw(10) << "Idade" << endl;

    for (const auto& p : pessoas) {
        cout << left << setw(30) << p.nome
             << setw(10) << p.idade << endl;  
    }

    cout << endl; 
}

void salvarArquivoCsv(const vector<Pessoa>& pessoas, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo); // abre para escrita (sobrescreve se existir)

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo para escrita: " << nomeArquivo << endl;
        return;
    }

    arquivo << "name,age" << endl;     // Escreve cabeçalho

    for (const auto& p : pessoas) {
        arquivo << p.nome << "," << p.idade << endl;
    }

    arquivo.close();
    cout << "Arquivo CSV salvo em: " << nomeArquivo << endl;
}