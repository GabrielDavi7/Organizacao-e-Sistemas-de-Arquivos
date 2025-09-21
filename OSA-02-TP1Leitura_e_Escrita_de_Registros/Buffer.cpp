#include "buffer.h"

void Buffer::EscreverRegistroFixo(Registro& registro, std::ofstream& saida){
    std::string Buffer_serial = registro.packFixed(); //serializa os dados
    saida.write(Buffer_serial.c_str(),Total_Tamanho_Registro); // escreve os dados garantindo que esta em tamanho fixo
}



Registro Buffer::LerRegistroFixo(std::ifstream& entrada){
    char Buffer_Leitura[Total_Tamanho_Registro];// criacao buffer do tamanho total para armazenar os dados
    entrada.read(Buffer_Leitura, Total_Tamanho_Registro); // ele vai ler do arquivo de entrada o tamanho maximo e salvar no buffer leitura

    std::string string_buffer(Buffer_Leitura, Total_Tamanho_Registro); // converto os dados lidos e salvos no buffer leitura para uma string

    Registro registro_lido;
    registro_lido.unpackFixed(string_buffer); //preenchi o registro_lido

    return registro_lido;
}

