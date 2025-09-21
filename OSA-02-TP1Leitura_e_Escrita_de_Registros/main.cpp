#include "Registro.h"
#include "Buffer.h"
#include "Arquivo.h"
#include <iostream>
#include <string>

void ImprimirRegistro(const std::string prefixo, const Registro& registro){
    std::cout << prefixo << "Nome: " << registro.nome << ", idade: " << registro.idade << std::endl;
}

int main()
{
   
    //teste das classes registro
    std::cout<< "-----Teste de registro-----" << std::endl;
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
        std::cout<<"Dados iguais codigo funciona"<<std::endl;
    }
    else{
        std::cerr<<"Erro: Dados nao sao iguais"<<std::endl;
        return 1;
}

    return 0;
}

