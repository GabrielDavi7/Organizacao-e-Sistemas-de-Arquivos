#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <fstream> // Necessário para gerar o arquivo .dot

using namespace std;

// ====================================================================
// Estrutura do Nó (Página)
// ====================================================================
template <typename Key>
class BTreeNode {
    public:
        // Vetor de chaves. Pode ter temporariamente 2b+1 chaves durante inserção.
        vector<Key> keys;
        // Vetor de filhos. Pode ter temporariamente 2b+2 filhos.
        vector<BTreeNode*> children;
        bool leaf; // Verdadeiro se for folha

        BTreeNode(bool is_leaf) : leaf(is_leaf) {}
        ~BTreeNode() {
            for (auto child : children) {
                delete child;
            }
        }
        // Retorna número de chaves
        int n() const { return keys.size(); }
};

// ====================================================================
// Classe BTree
// ====================================================================
template <typename Key>
class BTree {
private:
    BTreeNode<Key>* root;
    int b; // Ordem mínima

    // Estrutura auxiliar para comunicar o resultado de uma inserção recursiva
    struct InsertResult {
        bool overflow;       // Indica se ocorreu estouro (precisa de split)
        Key promotedKey;     // A chave mediana promovida
        BTreeNode<Key>* newRightNode; // O novo nó criado à direita
    };

    // ----------------------------------------------------------------
    // Lógica de Inserção (Crescimento para Cima)
    // ----------------------------------------------------------------
    
    // Função recursiva que insere e retorna se houve split para o pai tratar
    InsertResult insertRecursive(BTreeNode<Key>* node, const Key& k) {
        // Encontra a posição para descer ou inserir
        // Procura primeira chave >= k
        int i = 0;
        while (i < node->n() && k > node->keys[i]) {
            i++;
        }
        
        // Verifica duplicata (opcional, aqui ignoramos e não inserimos duplicadas simples)
        if (i < node->n() && node->keys[i] == k) {
            return {false, Key(), nullptr};
        }

        if (node->leaf) {
            // Chegamos na folha: Inserir a chave diretamente
            // Permite temporariamente que a página tenha 2b+1 chaves
            node->keys.insert(node->keys.begin() + i, k);
            
            // Verifica se precisa de split (Overflow > 2b)
            if (node->n() > 2 * b) {
                return splitNode(node);
            }
            return {false, Key(), nullptr};
        } 
        else {
            // Nó interno: desce para o filho apropriado
            InsertResult res = insertRecursive(node->children[i], k);
            
            // Na volta da recursão: verifica se o filho sofreu split
            if (res.overflow) {
                // Insere a chave promovida no nó atual (pai)
                node->keys.insert(node->keys.begin() + i, res.promotedKey);
                // Conecta o novo nó filho
                node->children.insert(node->children.begin() + i + 1, res.newRightNode);
                
                // Agora verifica se O PAI estourou (propagação para cima)
                if (node->n() > 2 * b) {
                    return splitNode(node);
                }
            }
            return {false, Key(), nullptr};
        }
    }

    // Função que divide um nó com 2b+1 chaves em dois
    // Retorna a chave mediana e o novo nó direito
    InsertResult splitNode(BTreeNode<Key>* node) {
        // Ponto de divisão: mediana.
        // Com 2b+1 chaves (índices 0 a 2b), a mediana está no índice b.
        // Ex (b=2, 2b=4): [k0, k1, k2(mediana), k3, k4]
        int medianIdx = b; 
        Key medianKey = node->keys[medianIdx];

        // Cria novo nó direito
        BTreeNode<Key>* newRight = new BTreeNode<Key>(node->leaf);

        // Move chaves maiores que a mediana para o novo nó
        // Chaves de medianIdx+1 até o fim
        newRight->keys.assign(node->keys.begin() + medianIdx + 1, node->keys.end());
        
        // Se não for folha, move os filhos correspondentes
        // Filhos de medianIdx+1 até o fim
        if (!node->leaf) {
            newRight->children.assign(node->children.begin() + medianIdx + 1, node->children.end());
            // Remove filhos movidos do nó original
            node->children.resize(medianIdx + 1);
        }

        // Ajusta o nó original (remove mediana e chaves movidas)
        node->keys.resize(medianIdx);

        return {true, medianKey, newRight};
    }

    // ----------------------------------------------------------------
    // Lógica de Remoção
    // ----------------------------------------------------------------
    void removeRecursive(BTreeNode<Key>* node, const Key& k) {
        int idx = 0;
        while (idx < node->n() && node->keys[idx] < k) {
            idx++;
        }

        // Caso: A chave está neste nó
        if (idx < node->n() && node->keys[idx] == k) {
            if (node->leaf) {
                // Caso 1: P é folha. Simplesmente remove.
                node->keys.erase(node->keys.begin() + idx);
            } else {
                // Caso 2: P é página interna.
                // Substitui pela maior chave da subárvore esquerda (predecessor)
                // (Conforme slide 44/46)
                BTreeNode<Key>* predecessorNode = node->children[idx];
                while (!predecessorNode->leaf) {
                    predecessorNode = predecessorNode->children.back();
                }
                Key predKey = predecessorNode->keys.back();
                
                // Substitui k por predKey
                node->keys[idx] = predKey;
                
                // Recursivamente remove predKey da subárvore esquerda
                removeRecursive(node->children[idx], predKey);

                // CORREÇÃO: Verificar se o filho sofreu underflow após a remoção
                // Isto garante que o backtracking propague as correções para cima
                if (node->children[idx]->n() < b) {
                    fixUnderflow(node, idx);
                }
            }
        } 
        else {
            // A chave não está neste nó. Desce para o filho.
            if (node->leaf) {
                cout << "Chave " << k << " nao encontrada." << endl;
                return;
            }

            // O filho onde a chave deve estar é children[idx]
            removeRecursive(node->children[idx], k);

            // APÓS O RETORNO: Verifica Underflow (menos de b chaves)
            // Slide 43/44: Caso 1.1 e 1.2
            if (node->children[idx]->n() < b) {
                fixUnderflow(node, idx);
            }
        }
    }

    // Repara o nó filho (childIdx) que tem menos de b chaves
    void fixUnderflow(BTreeNode<Key>* parent, int idx) {
        BTreeNode<Key>* problemNode = parent->children[idx];
        
        // Tenta redistribuir com irmão esquerdo (Caso 1.1)
        if (idx > 0) {
            BTreeNode<Key>* leftSibling = parent->children[idx - 1];
            // Verifica se total >= 2b. Como problemNode tem b-1, basta irmao ter > b.
            if (leftSibling->n() > b) {
                // REDISTRIBUIÇÃO (Rotação à direita)
                // Desce chave do pai para problemNode
                problemNode->keys.insert(problemNode->keys.begin(), parent->keys[idx - 1]);
                if (!problemNode->leaf) {
                    problemNode->children.insert(problemNode->children.begin(), leftSibling->children.back());
                    leftSibling->children.pop_back(); // Remove ponteiro movido
                }
                // Sobe maior chave do irmão para o pai
                parent->keys[idx - 1] = leftSibling->keys.back();
                leftSibling->keys.pop_back();
                return;
            }
        }

        // Tenta redistribuir com irmão direito (Caso 1.1)
        if (idx < parent->n()) {
            BTreeNode<Key>* rightSibling = parent->children[idx + 1];
            if (rightSibling->n() > b) {
                // REDISTRIBUIÇÃO (Rotação à esquerda)
                // Desce chave do pai para problemNode
                problemNode->keys.push_back(parent->keys[idx]);
                if (!problemNode->leaf) {
                    problemNode->children.push_back(rightSibling->children.front());
                    rightSibling->children.erase(rightSibling->children.begin());
                }
                // Sobe menor chave do irmão para o pai
                parent->keys[idx] = rightSibling->keys.front();
                rightSibling->keys.erase(rightSibling->keys.begin());
                return;
            }
        }

        // Caso 1.2: Concatenação (Merge)
        // Se não deu pra redistribuir, concatena com um irmão.
        // Soma das chaves < 2b.
        if (idx > 0) {
            // Merge com irmão esquerdo
            mergeNodes(parent, idx - 1); // Funde children[idx-1] e children[idx]
        } else {
            // Merge com irmão direito
            mergeNodes(parent, idx); // Funde children[idx] e children[idx+1]
        }
    }

    // Funde parent->children[idx] e parent->children[idx+1]
    // A chave parent->keys[idx] desce para a junção.
    void mergeNodes(BTreeNode<Key>* parent, int idx) {
        BTreeNode<Key>* leftChild = parent->children[idx];
        BTreeNode<Key>* rightChild = parent->children[idx + 1];

        // 1. Baixa a chave separadora do pai para o final do filho esquerdo
        leftChild->keys.push_back(parent->keys[idx]);

        // 2. Move todas as chaves do filho direito para o esquerdo
        leftChild->keys.insert(leftChild->keys.end(), rightChild->keys.begin(), rightChild->keys.end());

        // 3. Move todos os filhos do filho direito para o esquerdo (se não for folha)
        if (!leftChild->leaf) {
            leftChild->children.insert(leftChild->children.end(), rightChild->children.begin(), rightChild->children.end());
        }

        // 4. Remove a chave do pai e o ponteiro para o filho direito (que agora está vazio/fundido)
        parent->keys.erase(parent->keys.begin() + idx);
        parent->children.erase(parent->children.begin() + idx + 1);

        // Libera memória do nó direito obsoleto (manualmente, pois desvinculamos da árvore)
        // Nota: O destrutor deletaria os filhos recursivamente, mas movemos os filhos para leftChild,
        // então precisamos limpar o vetor children de rightChild antes de deletar para não deletar subárvores válidas.
        rightChild->children.clear();
        delete rightChild;
    }

    // Função auxiliar de impressão atualizada com visualização de arvore deitada e páginas formatadas
    void printRecursive(BTreeNode<Key>* node, string prefix, bool isLast) const {
        if (!node) return;
        
        cout << prefix;
        // Desenha a conexão: `|-- ` para intermediários, `-- ` (ou `L--`) para o último
        cout << (isLast ? "`-- " : "|-- ");

        // Imprime as chaves no formato [ k1 | k2 | ... ]
        // Alterado para usar vírgula como solicitado, embora | seja visualmente melhor para caixas
        cout << "[ ";
        for (int i = 0; i < node->n(); ++i) {
            cout << node->keys[i] << (i < node->n() - 1 ? ", " : "");
        }
        cout << " ]" << endl;

        if (!node->leaf) {
            // Prepara o prefixo para os filhos deste nível
            // Se este nó for o último, não desenha a barra vertical para os níveis abaixo
            string newPrefix = prefix + (isLast ? "    " : "|   ");
            
            for (size_t i = 0; i < node->children.size(); ++i) {
                // O último filho recebe tratamento especial na visualização
                bool lastChild = (i == node->children.size() - 1);
                printRecursive(node->children[i], newPrefix, lastChild);
            }
        }
    }

    // ----------------------------------------------------------------
    // Exportação para Graphviz (DOT)
    // ----------------------------------------------------------------
    int exportToDotRecursive(BTreeNode<Key>* node, int& idCounter, ofstream& out) const {
        int myId = idCounter++;
        
        // Define o nó com formato de registro (caixinhas)
        out << "    struct" << myId << " [label=\"";
        for (int i = 0; i < node->n(); ++i) {
            if (i > 0) out << "|";
            out << "<f" << i << "> " << node->keys[i];
        }
        out << "\"];" << endl;

        if (!node->leaf) {
            for (size_t i = 0; i < node->children.size(); ++i) {
                int childId = exportToDotRecursive(node->children[i], idCounter, out);
                // Cria aresta do pai para o filho
                out << "    struct" << myId << " -> struct" << childId << ";" << endl;
            }
        }
        return myId;
    }

public:
    BTree(int order) : root(nullptr), b(order) {
        if (b < 1) throw invalid_argument("Ordem b deve ser >= 1");
    }

    ~BTree() {
        if (root) delete root;
    }

    void insert(const Key& k) {
        if (root == nullptr) {
            root = new BTreeNode<Key>(true);
            root->keys.push_back(k);
            return;
        }
        InsertResult res = insertRecursive(root, k);
        // Se a raiz estourou, cria nova raiz (Crescimento em Altura)
        if (res.overflow) {
            BTreeNode<Key>* newRoot = new BTreeNode<Key>(false);
            newRoot->keys.push_back(res.promotedKey);
            newRoot->children.push_back(root);
            newRoot->children.push_back(res.newRightNode);
            root = newRoot;
            cout << "--- Raiz dividida! Nova altura. ---" << endl;
        }
    }

    void remove(const Key& k) {
        if (!root) return;
        removeRecursive(root, k);
        // Se a raiz ficou vazia (após merges), mas tem filhos
        // A raiz antiga desaparece e o único filho vira a nova raiz (Diminuição em Altura)
        if (!root->leaf && root->n() == 0) {
            BTreeNode<Key>* oldRoot = root;
            root = root->children[0]; // O único filho restante
            // Evita deletar os netos ao deletar oldRoot
            oldRoot->children.clear(); 
            delete oldRoot;
            cout << "--- Raiz vazia removida! Altura diminuiu. ---" << endl;
        } 
        else if (root->leaf && root->n() == 0) {
            // Árvore ficou vazia
            delete root;
            root = nullptr;
        }
    }

    void print() const {
        if (!root) {
            cout << "Arvore Vazia" << endl;
            return;
        }
        // Imprime a raiz separadamente para evitar prefixos de linha iniciais desnecessários
        cout << "[ ";
        for (int i = 0; i < root->n(); ++i) {
            cout << root->keys[i] << (i < root->n() - 1 ? ", " : "");
        }
        cout << " ]" << endl;
        if (!root->leaf) {
            for (size_t i = 0; i < root->children.size(); ++i) {
                bool lastChild = (i == root->children.size() - 1);
                printRecursive(root->children[i], "", lastChild);
            }
        }
    }

    // Gera arquivo DOT
    void exportToDot(const string& filename) const {
        ofstream out(filename);
        if (!out) {
            cerr << "Erro ao criar arquivo: " << filename << endl;
            return;
        }
        out << "digraph BTree {" << endl;
        out << "    node [shape=record];" << endl; // Formato de "registro" para visual vertical bonita
        if (root) {
            int idCounter = 0;
            exportToDotRecursive(root, idCounter, out);
        }
        out << "}" << endl;
        cout << "\nArquivo DOT gerado: " << filename << endl;
        cout << "Copie o conteudo e cole em: https://graphvizonline.net/" << endl;
    }
};

// ====================================================================
// Teste Baseado nos Slides
// ====================================================================
// ====================================================================
// Configuração da Ordem
// ====================================================================
// Nos slides, 'b' é a ordem mínima.
// Mínimo de chaves por página (exceto raiz): b
// Máximo de chaves por página: 2b
// Exemplo dos slides usa b=2 (Min 2, Max 4)
const int ORDER_B = 2;
int main() {
    // Cria árvore de ordem 2 (Max chaves = 4, Overflow em 5)
    BTree<int> t(ORDER_B); 

    // Lista de inserção do exercício/slides
    vector<int> insertions = {20, 40, 10, 30, 15, 35, 7, 26, 18, 22, 5, 42, 13, 46, 27, 8, 32, 38, 24, 45, 25};
    
    cout << "=== INICIO DAS INSERCOES ===" << endl;
    for (int k : insertions) {
        cout << "Inserindo " << k << "..." << endl;
        t.insert(k);
    }
    
    cout << "\n=== ARVORE APOS INSERCOES COMPLETAS ===" << endl;
    t.print();
    t.exportToDot("arvore_inicial.dot");

    // Teste de Remoção (Exemplos dos slides)
    // Remove 25 (Folha, sem underflow)
    // Remove 45 (Folha, underflow -> redistribuição)
    // Remove 24 (Interno -> substitui por predecessor, gera underflow -> concatenação)
    
    vector<int> removals = {25, 45, 24};
    cout << "\n=== INICIO DAS REMOCOES ===" << endl;
    
    for (int k : removals) {
        cout << "\nRemovendo " << k << "..." << endl;
        t.remove(k);
        t.print();
    }
    
    t.exportToDot("arvore_final.dot");

    return 0;
}