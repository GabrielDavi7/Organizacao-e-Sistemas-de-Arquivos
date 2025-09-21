#include "Registro.h"
#include "Buffer.h"
#include "Arquivo.h"
#include <iostream>
#include <string>

void ImprimirRegistro(const std::string prefixo, const Registro& registro){
    std::cout << prefixo << "Nome: " << registro.nome << ", idade: " << registro.idade << std::endl;
}

void imprimirVetor(const std::vector<Registro>& vetorRegistros){
    int cont = 1;
    for(const auto& registro: vetorRegistros){
        std::cout << "Registro "<< cont++ << " - Nome: " << registro.nome << ", idade: " << registro.idade << std::endl;
    }
}

int testarClasseRegistro(){
    std::cout<<"-----------------------------------------------------------------------------------------------"<<std::endl;
    std::cout<<"                                        Teste registro "<<std::endl;
    std::cout<<"-----------------------------------------------------------------------------------------------"<<std::endl;

    Registro RegistroOriginalTeste;
    RegistroOriginalTeste.nome = "Gabriel Davi";
    RegistroOriginalTeste.idade = 22;
    ImprimirRegistro("Original-> ",RegistroOriginalTeste);

    //teste serializacao
    std::cout<< "-----Teste de serializacao-----"<< std::endl;
    std::string TesteSerial = RegistroOriginalTeste.packFixed();
    std::cout<< " Tamanho do buffer(array) em bytes: " << TesteSerial.length() << std::endl;
    std::cout<< " Tamanho esperado(maximo): "<< Total_Tamanho_Registro << std::endl;
    if(TesteSerial.length()!=Total_Tamanho_Registro){
        std::cerr << "O buffer que foi serializado é maior que o permitido" << std::endl;
        return 1;}

        
    //teste deserializacao
    std::cout<< "-----Teste de deserializacao-----"<<std::endl;
    Registro TesteDeserial;
    TesteDeserial.unpackFixed(TesteSerial);
    ImprimirRegistro("Deserializacao-> ",TesteDeserial);

    if(TesteDeserial.nome==RegistroOriginalTeste.nome && TesteDeserial.idade==RegistroOriginalTeste.idade){
        std::cout<<"Dados iguais codigo funciona"<<std::endl;}

    else{
        std::cerr<<"Erro: Dados nao sao iguais"<<std::endl;
        return 1; }

    std::cout<<"Teste dos registros concluido"<<std::endl;
    return 0;    
}

int testarClasseBuffer(){

    std::cout<<"-----------------------------------------------------------------------------------------------"<<std::endl;
    std::cout<<"                                        Teste buffer "<<std::endl;
    std::cout<<"-----------------------------------------------------------------------------------------------"<<std::endl;
    //testes escrita
    const std::string nomeArquivo = "teste.bin";
    Buffer LeitorEscritor;

    std::cout<<"----- Teste Escrita em arquivo -----"<<std::endl;
    {
    Registro RegistroOriginalTeste;
    RegistroOriginalTeste.nome = "Gabriel Davi";
    RegistroOriginalTeste.idade = 22;
    ImprimirRegistro("Original-> ",RegistroOriginalTeste);

    std::ofstream saidaArquivo(nomeArquivo, std::ios::binary); // abre arquivo para escrita em binario
    if(!saidaArquivo){
        std::cerr<<"Erro ao abrir arquivo para escrita nome: "<<nomeArquivo<<std::endl;
        return 1;}

    LeitorEscritor.EscreverRegistroFixo(RegistroOriginalTeste,saidaArquivo); // chamar a funcao de escrita para o arquivo que virou binario
    std::cout<<" Escrita com sucesso no arquivo: "<< nomeArquivo<<". " << std::endl;
    saidaArquivo.close();
    }// garanti que o arquivo é fechado e destruido


    //testes leitura
    std::cout<<"----- Teste Leitura em arquivo Binario -----"<<std::endl;
    {

    std::ifstream entradaArquivo(nomeArquivo, std::ios::binary);
    if(!entradaArquivo){
        std::cerr<<"Erro ao abrir arquivo para leitura nome:"<<nomeArquivo<<std::endl;
        return 1; }

    std::cout<<"Lendo o arquivo.........."<<std::endl;
    Registro registroLido = LeitorEscritor.LerRegistroFixo(entradaArquivo); // cria um buffer para armazenar os dados lidos do arquivo
    ImprimirRegistro("Foi lido-> ", registroLido);
    entradaArquivo.close();
    }// garanti que o arquivo é fechado e destruido

    std::cout<<"Teste de buffer concluido"<<std::endl;


    return 0;
}

int testarClasseArquivo(){

    return 0;
}


int main(){

/* teste das classes
    std::cout<<"==============================================================================================="<<std::endl;
    std::cout<<"                                        Teste das classes "<<std::endl;
    std::cout<<"==============================================================================================="<<std::endl;

    testarClasseRegistro();
    testarClasseBuffer();
    testarClasseArquivo();
*/

const std::string ArquivoCSV = "CSV/Nomes_Idades_1.csv";
const std::string ArquivoBIN = "arquivo.bin";

Arquivo csv(ArquivoCSV);
std::vector<Registro> registroLido = csv.lerRegistrosCSV();

 if (registroLido.empty()) {
        std::cerr << "Nenhum registro foi lido no csv." << std::endl;
        return 1;
    }

Arquivo bin(ArquivoBIN);
std::remove(ArquivoBIN.c_str());

for(auto& registro: registroLido){
    bin.adicionarRegistroFixo(registro);
}

std::vector<Registro> registrosNoArquivo = bin.lerRegistroFixo();
if (registrosNoArquivo.empty()) {
        std::cerr << "Nenhum registro foi lido no binario." << std::endl;
        return 1;
    }

std::cout << registrosNoArquivo.size() << " registros lidos do arquivo " << ArquivoBIN << ":" << std::endl;
imprimirVetor(registrosNoArquivo);

std::cout << "\nProcesso concluido com sucesso!" << std::endl;

    return 0;
}

