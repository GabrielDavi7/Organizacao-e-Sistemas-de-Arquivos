#include <iostream>
#include <string>
#include <vector>
#include <limits> // Necessário para limpar o buffer de entrada
#include <cstdio> // Para std::remove

#include "Arquivo.h"
#include "Registro.h"

// Arquivo binário padrão
const std::string ARQUIVO_BINARIO = "dados.bin";

// --- Funções Auxiliares de Interface ---

// Função para exibir o menu e ler a opção do usuário
int menu() {
    int op;
    std::cout << "\n--- GERENCIADOR DE REGISTROS DE ALUNOS ---\n";
    std::cout << "1. Importar registros de um arquivo CSV\n";
    std::cout << "2. Inserir novo registro\n";
    std::cout << "3. Buscar um registro por matricula\n";
    std::cout << "4. Remover um registro por matricula\n";
    std::cout << "5. Atualizar um registro por matricula\n";
    std::cout << "0. Sair\n";
    std::cout << "------------------------------------------\n";
    std::cout << "Digite sua opcao: ";
    std::cin >> op;

    // Limpa o buffer de entrada para evitar problemas com leituras futuras (ex: getline)
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    return op;
}

// Função para imprimir os detalhes de um registro na tela
void imprimirRegistro(const Registro& reg) {
    std::cout << "-----------------------------------\n";
    std::cout << "Matricula: " << reg.getMatricula() << "\n";
    std::cout << "Nome:      " << reg.getNome() << "\n";
    std::cout << "Curso:     " << reg.getCurso() << "\n";
    std::cout << "-----------------------------------\n";
}

// --- Funções para cada opção do menu ---

void importarDeCSV() {
    std::string arquivoCSV;
    std::cout << "Digite o nome do arquivo CSV (ex: dados.csv): ";
    std::getline(std::cin, arquivoCSV);

    std::cout << "Importando dados de '" << arquivoCSV << "' para '" << ARQUIVO_BINARIO << "'...\n";
    
    Arquivo csv(arquivoCSV);
    std::vector<Registro> registros = csv.lerRegistrosCSV();

    if (registros.empty()) {
        std::cerr << "Nenhum registro lido do CSV ou arquivo nao encontrado.\n";
        return;
    }

    std::remove(ARQUIVO_BINARIO.c_str()); // Remove o arquivo binário antigo
    Arquivo bin(ARQUIVO_BINARIO);
    for (Registro& reg : registros) {
        bin.adicionarRegistroFixo(reg);
    }
    std::cout << registros.size() << " registros importados com sucesso!\n";
}

void inserirNovoRegistro() {
    Registro novoReg;
    int matricula;
    std::string nome, curso;

    std::cout << "--- Inserindo novo registro ---\n";
    std::cout << "Matricula: ";
    std::cin >> matricula;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer
    
    std::cout << "Nome: ";
    std::getline(std::cin, nome);
    
    std::cout << "Curso: ";
    std::getline(std::cin, curso);

    novoReg.setMatricula(matricula);
    novoReg.setNome(nome);
    novoReg.setCurso(curso);
    novoReg.setLapide(' ');

    Arquivo bin(ARQUIVO_BINARIO);
    bin.adicionarRegistroFixo(novoReg);
    std::cout << "Registro inserido com sucesso!\n";
}

void buscarRegistro() {
    int matricula;
    std::cout << "Digite a matricula a ser buscada: ";
    std::cin >> matricula;

    Arquivo bin(ARQUIVO_BINARIO);
    Registro reg = bin.buscarPorMatricula(matricula);

    if (reg.getMatricula() != 0) { // Verifica se um registro válido foi retornado
        imprimirRegistro(reg);
    } else {
        std::cout << "Registro com matricula " << matricula << " nao encontrado.\n";
    }
}

void removerRegistro() {
    int matricula;
    std::cout << "Digite a matricula a ser removida: ";
    std::cin >> matricula;

    Arquivo bin(ARQUIVO_BINARIO);
    if (bin.removerPorMatricula(matricula)) {
        std::cout << "Registro com matricula " << matricula << " removido (logicamente).\n";
    } else {
        std::cout << "Registro com matricula " << matricula << " nao encontrado.\n";
    }
}

void atualizarRegistro() {
    int matricula;
    std::cout << "Digite a matricula a ser atualizada: ";
    std::cin >> matricula;

    Arquivo bin(ARQUIVO_BINARIO);
    if (bin.atualizarPorMatricula(matricula)) {
        std::cout << "Registro com matricula " << matricula << " atualizado com sucesso.\n";
    } else {
        std::cout << "Registro com matricula " << matricula << " nao encontrado.\n";
    }
}

// --- Função Principal ---

int main() {
    int op;
    do {
        op = menu();
        switch (op) {
            case 1:
                importarDeCSV();
                break;
            case 2:
                inserirNovoRegistro();
                break;
            case 3:
                buscarRegistro();
                break;
            case 4:
                removerRegistro();
                break;
            case 5:
                atualizarRegistro();
                break;
            case 0:
                std::cout << "Saindo do programa...\n";
                break;
            default:
                std::cout << "Opcao invalida. Tente novamente.\n";
                break;
        }
        if (op != 0) {
            std::cout << "\nPressione Enter para continuar...";
            std::cin.get();
        }
    } while (op != 0);

    return 0;
}