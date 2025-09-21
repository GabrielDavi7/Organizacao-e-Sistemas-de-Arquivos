#include "arquivo.h"
#include "buffer.h"   
#include <fstream>    
#include <sstream>    
#include <iostream>


Arquivo::Arquivo(const std::string& nome) : nomeDoArquivo(nome) {}


std::vector<Registro> Arquivo::lerRegistrosCSV(){           
    std::vector<Registro> vetorRegistros;
    std::ifstream Arquivo(this->nomeDoArquivo);

    if(!Arquivo){
        std::cerr<<"Erro ao abrir o arquivo CSV: "<<nomeDoArquivo<<std::endl;
        return vetorRegistros;}

    std::string linha;
    std::getline(Arquivo,linha);
    
    while(std::getline(Arquivo,linha)){
        std::stringstream  ss(linha); //para ler como se fosse uma linha digitada no console
        std::string nomeLinha;
        std::string idadeLinha;

        if(std::getline(ss,nomeLinha,',') && std::getline(ss,idadeLinha)){
            Registro registro;
            registro.nome = nomeLinha;
            try{ // converter string para int
                registro.idade = std::stoi(idadeLinha); //converte
                vetorRegistros.push_back(registro); // adiciona ao vetor
            }
            catch(const std::invalid_argument& e){
                std::cerr << "Erro ao converter idade";
                }
            }
    }
    Arquivo.close();
    return vetorRegistros;


}

 std::vector<Registro>Arquivo::lerRegistroFixo(){
    std::vector<Registro> vetorRegistros;
    std::ifstream Arquivo(this->nomeDoArquivo); // abrindo arquivo
    
    if(!Arquivo.is_open()){
        std::cerr<<"Erro ao abrir o arquivo: "<<nomeDoArquivo<<std::endl;
        return vetorRegistros;}

    Buffer  leituraBuffer;

    while (Arquivo.peek()!=EOF){ // ler ate chegar no fim
        Registro registro = leituraBuffer.LerRegistroFixo(Arquivo);

        if(Arquivo.gcount() == Total_Tamanho_Registro){
            vetorRegistros.push_back(registro);
        }
    }
    
    Arquivo.close();
    return vetorRegistros;
 }

void Arquivo::adicionarRegistroFixo(Registro& registro){
    std::ofstream Arquivo(this->nomeDoArquivo, std::ios::binary | std::ios::app);
    if(!Arquivo){
        std::cout<<"Erro ao abrir o arquivo: "<<nomeDoArquivo<<std::endl;
        return;
    }

    Buffer escritaBuffer;
    escritaBuffer.EscreverRegistroFixo(registro, Arquivo);
    Arquivo.close();
}









