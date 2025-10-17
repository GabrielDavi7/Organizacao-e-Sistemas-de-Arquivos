#include "SistemaGerenciador.h"

int main() {
    SistemaGerenciador sistema("alunos.csv", "alunos.dat", "indices.dat");
    sistema.iniciar();

    return 0;
}