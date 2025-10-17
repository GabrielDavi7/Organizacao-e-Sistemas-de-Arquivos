#include "RegistroAluno.h"
#include <stdexcept>

RegistroAluno::RegistroAluno(int m, const std::string& n, const std::string& c)
    : matricula(m), nome(n), curso(c) {}

std::string RegistroAluno::getChave() const {
    return std::to_string(matricula);
}

int RegistroAluno::getTamanhoFixo() const {
    return sizeof(int) + TAMANHO_NOME_FIXO + TAMANHO_CURSO_FIXO;
}

void RegistroAluno::pack(Buffer& buffer, Formato formato) const {
    buffer.clear();
    buffer.pack(matricula);
    
    switch (formato) {
        case Formato::FIXO:
            buffer.packFixo(nome, TAMANHO_NOME_FIXO);
            buffer.packFixo(curso, TAMANHO_CURSO_FIXO);
            break;
        case Formato::DELIMITADO:
            buffer.packDelimitado(nome, '|');
            buffer.packDelimitado(curso, '|');
            break;
        case Formato::COMPRIMENTO:
            buffer.packComprimento(nome);
            buffer.packComprimento(curso);
            break;
        default:
            throw std::invalid_argument("Formato de empacotamento desconhecido.");
    }
}

void RegistroAluno::unpack(Buffer& buffer, Formato formato) {
    matricula = buffer.unpackInt();
    
    switch (formato) {
        case Formato::FIXO:
            nome = buffer.unpackFixo(TAMANHO_NOME_FIXO);
            curso = buffer.unpackFixo(TAMANHO_CURSO_FIXO);
            break;
        case Formato::DELIMITADO:
            nome = buffer.unpackDelimitado('|');
            curso = buffer.unpackDelimitado('|');
            break;
        case Formato::COMPRIMENTO:
            nome = buffer.unpackComprimento();
            curso = buffer.unpackComprimento();
            break;
        default:
            throw std::invalid_argument("Formato de desempacotamento desconhecido.");
    }
}