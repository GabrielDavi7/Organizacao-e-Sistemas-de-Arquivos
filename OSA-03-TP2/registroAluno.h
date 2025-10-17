#ifndef REGISTRO_ALUNO_H
#define REGISTRO_ALUNO_H

#include "Registro.h"
#include <string>

const int TAMANHO_NOME_FIXO = 50;
const int TAMANHO_CURSO_FIXO = 30;

class RegistroAluno : public Registro {
public:
    int matricula;
    std::string nome;
    std::string curso;

    RegistroAluno(int m = 0, const std::string& n = "", const std::string& c = "");

    void pack(Buffer& buffer, Formato formato) const override;
    void unpack(Buffer& buffer, Formato formato) override;
    std::string getChave() const override;
    int getTamanhoFixo() const override;
};

#endif