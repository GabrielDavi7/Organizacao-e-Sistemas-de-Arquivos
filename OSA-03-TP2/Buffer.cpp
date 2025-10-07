#include "Buffer.h"

void Buffer::EscreverRegistroFixo(Registro& registro, std::ostream& saida){
    std::string Buffer_serial = registro.packFixed();
    saida.write(Buffer_serial.c_str(),Total_Tamanho_Registro);
}

Registro Buffer::LerRegistroFixo(std::istream& entrada){
    char Buffer_Leitura[Total_Tamanho_Registro];
    entrada.read(Buffer_Leitura, Total_Tamanho_Registro);

    std::string string_buffer(Buffer_Leitura, entrada.gcount()); 

    Registro registro_lido;
    if(entrada.gcount() == Total_Tamanho_Registro) {
        registro_lido.unpackFixed(string_buffer);
    }

    return registro_lido;
}
