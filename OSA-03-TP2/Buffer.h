#ifndef BUFFER_H
#define BUFFER_H
#include <fstream>
#include "Registro.h"
#include <istream>
#include <ostream>

class Buffer{
public:

void EscreverRegistroFixo(Registro& registro, std::ostream& saida);
Registro LerRegistroFixo(std::istream& entrada);

};

#endif