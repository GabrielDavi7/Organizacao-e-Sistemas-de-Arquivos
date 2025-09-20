#include "registro.h"
#include <cstring>



std::string Registro::packFixed(){
char buffer[Total_Tamanho_Registro]; // criacao da array de caracteres(bloco na memoria) com o tamanho maximo

std::memset(buffer, '\0', Total_Tamanho_Registro); // *preenche* o espaco da memoria(1 byte) e limpa o lixo

std::memcpy(buffer, &idade, sizeof(int)); //memory copy para copiar os bytes da variavel idade para o inicio da array(buffer)
std::memcpy(buffer + sizeof(int), nome.c_str(), nome.length());//aqui vamos avancar na array ate o momento final da aplicacao da idade e comecar na "quarta casa da array" onde
                                                            // comeca o nome, apos isso pegamos um ponteiro do nome da string pegamos o tamanho para nao estourar o buffer

return std::string(buffer, Total_Tamanho_Registro); // retorna a string que criei com o buffer

}

void Registro::unpackFixed(const std::string& buffer){
    std::memcpy(&idade,buffer.data(),sizeof(int)); // aqui eu copiei para a idade os primeiros 4 bytes da buffer.data()
    char buffer_temporario_nome[Max_Nome_Tamanho + 1]; // buffer temporario para armazenar o nome e garantir com o +1 que ele terminou com \0

    std::memcpy(buffer_temporario_nome, buffer.c_str() + sizeof(int), Max_Nome_Tamanho);// aqui eu copiei para o bf temporario os dados de tamabho "Max_Nome" da posicao do ponteiro + size int
    buffer_temporario_nome[Max_Nome_Tamanho] = '\0'; // garantir que ele termine com \0

    this->nome = std::string(buffer_temporario_nome); // salvando o conteudo temporario na string permanente
} 
