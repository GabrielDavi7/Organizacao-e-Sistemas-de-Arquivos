#ifndef LEITURA_H
#define LEITURA_H

#include <vector>   
#include <string>  

using namespace std;

vector<string> lerArquivo(const string& nomeArquivo);
void exibirNomes(const vector<string>& vetorNomes);
void salvarArquivo(const vector<string>& vetorNomes, const string& saidaArquivo);


#endif
