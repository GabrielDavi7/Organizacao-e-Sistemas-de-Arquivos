#ifndef BUFFER_H
#define BUFFER_H
#include"registro.h"
#include <fstream>


class Buffer{
public:

void EscreverRegistroFixo(Registro& registro, std::ofstream& saida);
Registro LerRegistroFixo(std::ifstream& entrada);

};

#endif