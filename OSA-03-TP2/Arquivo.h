#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>      // Para std::runtime_error
#include <type_traits>    // Para std::is_base_of

#include "Registro.h" 
#include "Buffer.h"

template <class T>
class Arquivo {
private:
    std::string nomeArquivo;
    Formato formato;

public:
    Arquivo(const std::string& nome, Formato fmt);

    void adicionarRegistro(const T& reg);
    std::vector<T> lerRegistros();
};



template <class T>
Arquivo<T>::Arquivo(const std::string& nome, Formato fmt)
    : nomeArquivo(nome), formato(fmt) {
    static_assert(std::is_base_of<Registro, T>::value, "T must be a derivative of Registro");
}

template <class T>
void Arquivo<T>::adicionarRegistro(const T& reg) {
    std::ofstream arquivo(nomeArquivo, std::ios::binary | std::ios::app);
    if (!arquivo) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo para escrita: " + nomeArquivo);
    }

    Buffer buffer;
    reg.pack(buffer, formato);

    if (formato == Formato::COMPRIMENTO) {
        short tamanhoTotal = static_cast<short>(buffer.size());
        arquivo.write(reinterpret_cast<const char*>(&tamanhoTotal), sizeof(short));
    } else if (formato == Formato::DELIMITADO) {
        buffer.packDelimitado("",'\n');
    }
    
    buffer.write(arquivo);
    arquivo.close();
}

template <class T>
std::vector<T> Arquivo<T>::lerRegistros() {
    std::vector<T> registros;
    std::ifstream arquivo(nomeArquivo, std::ios::binary);
    if (!arquivo) {
        throw std::runtime_error("Nao foi possivel abrir o arquivo para leitura: " + nomeArquivo);
    }

    Buffer buffer;
    T reg_vazia; 

    if (formato == Formato::FIXO) {
        while (buffer.read(arquivo, reg_vazia.getTamanhoFixo())) {
            T registro;
            registro.unpack(buffer, formato);
            registros.push_back(registro);
        }
    } else if (formato == Formato::DELIMITADO) {
        while (buffer.readDelimitado(arquivo, '\n')) {
            if (buffer.size() <= 1) continue;
            T registro;
            registro.unpack(buffer, formato);
            registros.push_back(registro);
        }
    } 
    else if (formato == Formato::COMPRIMENTO) {
        short tamanhoRegistro;
        while (arquivo.read(reinterpret_cast<char*>(&tamanhoRegistro), sizeof(short))) {
            if (buffer.read(arquivo, tamanhoRegistro)) {
                T registro;
                registro.unpack(buffer, formato);
                registros.push_back(registro);
            }
        }
    }

    arquivo.close();
    return registros;
}

#endif