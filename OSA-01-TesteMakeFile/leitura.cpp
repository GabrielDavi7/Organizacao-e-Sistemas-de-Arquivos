#include "leitura.h"
#include <iostream>
#include <fstream>
#include <iomanip>  // para setw, setfill
using namespace std;

vector<string> lerArquivo(const string& nomeArquivo){
    ifstream arquivo(nomeArquivo);
    vector<string> vetorNomes; 
    string stringNome;

    if(!arquivo.is_open()){
        cerr<< "erro ao abrir arquivo de nomes: " <<nomeArquivo <<endl;
        return vetorNomes;
    }

    while (getline(arquivo, stringNome)){ // pega a linha do arquivo e adiciona na string nome, verifica se a string nao é vazia e adiciona no final do vetor nomes
        if(!stringNome.empty()){
            vetorNomes.push_back(stringNome);
        }
    }

    arquivo.close();
    return vetorNomes;
}

void exibirNomes(const vector<string>& vetorNomes){
    system("clear");
    cout << setfill('-') << setw(30) << "-" << endl;
    cout << "             Nomes" << endl;
    cout << setw(30) << "-" << endl;

    int cont = 1;
    for(const auto& stringNomeExibir : vetorNomes){ // roda todo o vetor nomes pegando a string nome e imprimindo
        cout << cont << ". " << stringNomeExibir <<endl;
        cont ++;
    }
}

void salvarArquivo(const vector<string>& vetorNomes, const string& saidaArquivo){ // recebe o vetor de nomes e o nome do arquivo que ele vai salvar
    ofstream arquivoSaida(saidaArquivo);

    if (!arquivoSaida.is_open()){
        cout << "erro ao abrir o arquivo de saida nome: " << saidaArquivo << endl;
        return;
    }

    for (const auto& stringNome : vetorNomes){ // percorre cada nome e salva ele no arquivo de saida
        arquivoSaida << stringNome << endl;
    }
    arquivoSaida.close();
}