#ifndef REGISTRO_H
#define REGISTRO_H

#include <string>
#include "Buffer.h" 

enum class Formato {
    FIXO,
    DELIMITADO,
    COMPRIMENTO
};

class Registro {
public:
    virtual ~Registro() = default;
    virtual void pack(Buffer& buffer, Formato formato) const = 0;
    virtual void unpack(Buffer& buffer, Formato formato) = 0;
    virtual std::string getChave() const = 0;
    virtual int getTamanhoFixo() const = 0;
};

#endif