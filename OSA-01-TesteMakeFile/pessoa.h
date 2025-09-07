#ifndef PESSOA_H
#define PESSOA_H

#include <string>
#include <vector>
using namespace std;

struct Pessoa {
    string nome;
    int idade;
    
    Pessoa(const string& n = "", int i = 0) : nome(n), idade(i) {}
};

vector<Pessoa> lerPessoasDoCSV(const string& nomeArquivo);
vector<Pessoa> lerTodosCSVs(const string& pasta);
void exibirNomesIdade(const vector<Pessoa>& pessoas);
void salvarArquivoCsv(const vector<Pessoa>& pessoas, const string& nomeArquivo);

#endif