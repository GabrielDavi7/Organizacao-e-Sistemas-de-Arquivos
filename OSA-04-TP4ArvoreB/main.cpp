#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <cstring> 

using namespace std;

// ====================================================================
// CONFIGURAÇÕES GLOBAIS
// ====================================================================
const string ARQUIVO_ARVORE = "btree_index.dat"; 
const string ARQUIVO_DADOS = "dados.dat"; 
const int ORDER_B = 2;                     

//Arrays com tamanho +1 para suportar a inserção temporária antes do split
const int MAX_KEYS = 2 * ORDER_B + 1;          
const int MAX_CHILDREN = 2 * ORDER_B + 2;  

const int NULL_PTR = -1;                   

// ====================================================================
// ESTRUTURAS DE DADOS
// ====================================================================

struct RecordIndex {
    int key;         
    long dataOffset; 
    
    RecordIndex() : key(0), dataOffset(-1) {}
    RecordIndex(int k, long off) : key(k), dataOffset(off) {}
};

struct DiskPage {
    int pageRRN;              
    int numKeys;              
    bool isLeaf;              
    
    RecordIndex keys[MAX_KEYS];    
    int children[MAX_CHILDREN];    

    DiskPage() {
        pageRRN = NULL_PTR;
        numKeys = 0;
        isLeaf = true;
        // Inicializa tudo zerado
        for(int i=0; i<MAX_KEYS; i++) keys[i] = RecordIndex();
        for(int i=0; i<MAX_CHILDREN; i++) children[i] = NULL_PTR;
    }
};

struct TreeHeader {
    int rootRRN;     
    int nextFreeRRN; 
    
    TreeHeader() : rootRRN(NULL_PTR), nextFreeRRN(0) {}
};

// ====================================================================
// GERENCIADOR DE DISCO
// ====================================================================
class DiskManager {
private:
    fstream file;
    string fileName;

public:
    TreeHeader header;

    DiskManager(string fname) : fileName(fname) {
        file.open(fileName, ios::in | ios::out | ios::binary);
        if (!file.is_open()) {
            file.open(fileName, ios::out | ios::binary);
            file.close();
            file.open(fileName, ios::in | ios::out | ios::binary);
            header = TreeHeader();
            writeHeader();
        } else {
            readHeader();
        }
    }

    ~DiskManager() {
        if (file.is_open()) file.close();
    }

    void readHeader() {
        file.seekg(0, ios::beg);
        file.read((char*)&header, sizeof(TreeHeader));
    }

    void writeHeader() {
        file.seekp(0, ios::beg);
        file.write((char*)&header, sizeof(TreeHeader));
        file.flush();
    }

    DiskPage readPage(int rrn) {
        DiskPage page;
        if (rrn == NULL_PTR) return page;
        long offset = sizeof(TreeHeader) + (long)rrn * sizeof(DiskPage);
        file.clear(); 
        file.seekg(offset, ios::beg);
        file.read((char*)&page, sizeof(DiskPage));
        return page;
    }

    void writePage(int rrn, const DiskPage& page) {
        long offset = sizeof(TreeHeader) + (long)rrn * sizeof(DiskPage);
        file.clear(); 
        file.seekp(offset, ios::beg);
        file.write((char*)&page, sizeof(DiskPage));
        file.flush(); 
    }

    DiskPage newPage(bool isLeaf) {
        DiskPage page;
        page.pageRRN = header.nextFreeRRN++;
        page.isLeaf = isLeaf;
        writeHeader(); 
        return page;
    }
};

// ====================================================================
// ÁRVORE B
// ====================================================================
class BTree {
private:
    DiskManager& dm;

    struct InsertResult {
        bool overflow;
        RecordIndex promotedItem;
        int newRightRRN;
    };

    InsertResult insertRecursive(int currentRRN, const RecordIndex& item) {
        DiskPage page = dm.readPage(currentRRN);

        int i = 0;
        while (i < page.numKeys && item.key > page.keys[i].key) {
            i++;
        }

        if (i < page.numKeys && page.keys[i].key == item.key) {
            return {false, RecordIndex(), NULL_PTR};
        }

        if (page.isLeaf) {
            for (int k = page.numKeys; k > i; k--) {
                page.keys[k] = page.keys[k-1];
            }
            page.keys[i] = item;
            page.numKeys++;

            dm.writePage(currentRRN, page);

            if (page.numKeys > 2 * ORDER_B) {
                return splitNode(page);
            }
            return {false, RecordIndex(), NULL_PTR};
        } 
        else {
            InsertResult res = insertRecursive(page.children[i], item);

            if (res.overflow) {
                // Abre espaço seguro para chave promovida
                for (int k = page.numKeys; k > i; k--) {
                    page.keys[k] = page.keys[k-1];
                }
                page.keys[i] = res.promotedItem;

                // Abre espaço seguro para filho novo
                for (int k = page.numKeys + 1; k > i + 1; k--) {
                    page.children[k] = page.children[k-1];
                }
                page.children[i+1] = res.newRightRRN;
                
                page.numKeys++;
                dm.writePage(currentRRN, page);

                if (page.numKeys > 2 * ORDER_B) {
                    return splitNode(page);
                }
            }
            return {false, RecordIndex(), NULL_PTR};
        }
    }

    InsertResult splitNode(DiskPage& page) {
        DiskPage newPage = dm.newPage(page.isLeaf);
        
        int midIndex = ORDER_B; // Ponto de quebra
        RecordIndex promoted = page.keys[midIndex];

        // Copia a metade direita para o novo nó
        int moveCount = 0;
        for (int i = midIndex + 1; i < page.numKeys; i++) {
            newPage.keys[moveCount] = page.keys[i];
            moveCount++;
        }
        newPage.numKeys = moveCount;

        // Copia os filhos correspondentes
        if (!page.isLeaf) {
            int childMoveCount = 0;
            for (int i = midIndex + 1; i <= page.numKeys; i++) {
                newPage.children[childMoveCount] = page.children[i];
                //Limpar o ponteiro antigo para não deixar lixo
                page.children[i] = NULL_PTR; 
                childMoveCount++;
            }
        }

        // Atualiza o nó original
        page.numKeys = midIndex; 
        
        // Salva as alterações
        dm.writePage(page.pageRRN, page);
        dm.writePage(newPage.pageRRN, newPage);

        return {true, promoted, newPage.pageRRN};
    }

    void exportRecursive(int rrn, ofstream& out) {
        DiskPage page = dm.readPage(rrn);
        
        out << "    node" << rrn << " [label=\"";
        for (int i = 0; i < page.numKeys; i++) {
            if (i > 0) out << "|";
            out << "<f" << i << "> " << page.keys[i].key;
        }
        out << "\"];" << endl;

        if (!page.isLeaf) {
            for (int i = 0; i <= page.numKeys; i++) {
                if (page.children[i] != NULL_PTR) {
                    exportRecursive(page.children[i], out);
                    out << "    node" << rrn << " -> node" << page.children[i] << ";" << endl;
                }
            }
        }
    }

    void printRecursive(int rrn, int level) {
        DiskPage page = dm.readPage(rrn);
        cout << "Nivel " << level << " [RRN " << rrn << "]: ";
        for(int i=0; i<page.numKeys; i++) cout << page.keys[i].key << " ";
        cout << endl;

        if(!page.isLeaf) {
            for(int i=0; i<=page.numKeys; i++) {
                if(page.children[i] != NULL_PTR)
                    printRecursive(page.children[i], level + 1);
            }
        }
    }

public:
    BTree(DiskManager& diskMgr) : dm(diskMgr) {}

    void insert(int key, long offset) {
        RecordIndex item(key, offset);

        if (dm.header.rootRRN == NULL_PTR) {
            DiskPage root = dm.newPage(true);
            root.keys[0] = item;
            root.numKeys = 1;
            dm.writePage(root.pageRRN, root);
            dm.header.rootRRN = root.pageRRN;
            dm.writeHeader();
            return;
        }

        InsertResult res = insertRecursive(dm.header.rootRRN, item);
        
        if (res.overflow) {
            DiskPage newRoot = dm.newPage(false);
            newRoot.keys[0] = res.promotedItem;
            newRoot.children[0] = dm.header.rootRRN;
            newRoot.children[1] = res.newRightRRN;
            newRoot.numKeys = 1;
            
            dm.writePage(newRoot.pageRRN, newRoot);
            dm.header.rootRRN = newRoot.pageRRN;
            dm.writeHeader();
        }
    }

    void print() {
        if (dm.header.rootRRN == NULL_PTR) {
            cout << "Arvore Vazia" << endl;
            return;
        }
        printRecursive(dm.header.rootRRN, 0);
    }

    void exportToDot(string filename) {
        ofstream out(filename);
        out << "digraph BTree {" << endl;
        out << "    node [shape=record];" << endl;
        if (dm.header.rootRRN != NULL_PTR) {
            exportRecursive(dm.header.rootRRN, out);
        }
        out << "}" << endl;
        cout << "Arquivo DOT gerado: " << filename << endl;
    }
};

long salvarDadoSimulado(int id, string info) {
    ofstream dataFile(ARQUIVO_DADOS, ios::app | ios::binary);
    dataFile.seekp(0, ios::end);
    long offset = dataFile.tellp();
    dataFile << id << "|" << info << "|";
    dataFile.close();
    return offset;
}

int main() {
    remove(ARQUIVO_ARVORE.c_str());
    remove(ARQUIVO_DADOS.c_str());

    DiskManager diskManager(ARQUIVO_ARVORE);
    BTree t(diskManager);

    struct Item { int id; string nome; };
    vector<Item> dados = {
        {20, "A"}, {40, "B"}, {10, "C"}, {30, "D"}, {15, "E"}, 
        {35, "F"}, {7, "G"}, {26, "H"}, {18, "I"}, {22, "J"}, 
        {5, "K"}, {42, "L"}, {13, "M"}, {46, "N"}, {27, "O"}, 
        {8, "P"}, {32, "Q"}, {38, "R"}, {24, "S"}, {45, "T"}, {25, "U"}
    };

    cout << "=== INSERCAO ===" << endl;
    for (auto& d : dados) {
        long offset = salvarDadoSimulado(d.id, d.nome);
        t.insert(d.id, offset);
    }

    cout << "\n=== ESTRUTURA FINAL ===" << endl;
    t.print();
    t.exportToDot("arvore.dot");
    
    return 0;
}