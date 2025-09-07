#include <iostream>
#include <vector>
#include "leitura.h"
#include "pessoa.h"
using namespace std;

int main(){
    vector<string> nomes = lerArquivo("Arquivo_Nomes.txt");  //criacao do vetor de nomes
    vector<Pessoa> pessoas = lerTodosCSVs("csv");


    exibirNomes(nomes);
    salvarArquivo(nomes, "nomesSaida.txt");

    exibirNomesIdade(pessoas);
    salvarArquivoCsv(pessoas, "nomesIdadesSaida.csv");

    return 0;
}