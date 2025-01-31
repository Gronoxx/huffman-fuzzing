#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cctype>

// Definição da classe No
class No {
public:
    No(char letra, bool leaf);
    No* esq;
    No* dir;
    int freq;
    bool leaf;
    char letra;
};

// Implementação do construtor da classe No
No::No(char letra, bool leaf) {
    if (leaf == true) {
        this->letra = letra;
        this->freq = 0;
    }
    this->leaf = leaf;
    this->esq = nullptr;
    this->dir = nullptr;
}

// Definição da classe Arvore
class Arvore {
private:
    No* raiz;

public:
    Arvore(No* raiz);
    No* Get_raiz();
    void inOrder(No* node);
    void posOrder(No* node);
    void preOrder(No* node);
    void Apagar(No* node);
    ~Arvore();
};

// Implementação dos métodos da classe Arvore
Arvore::Arvore(No* raiz) {
    this->raiz = raiz;
}

No* Arvore::Get_raiz() {
    return raiz;
}

void Arvore::posOrder(No* node) {
    if (node == nullptr) {
        return;
    }
    posOrder(node->esq);
    posOrder(node->dir);
    if (node->leaf == true) {
        std::cout << "Letra: " << node->letra << ", Frequência: " << node->freq << std::endl;
    } else {
        std::cout << "Frequência: " << node->freq << std::endl;
    }
}

void Arvore::preOrder(No* node) {
    if (node == nullptr) {
        return;
    }
    if (node->leaf == true) {
        std::cout << "Letra: " << node->letra << ", Frequência: " << node->freq << std::endl;
    } else {
        std::cout << "Frequência: " << node->freq << std::endl;
    }
    preOrder(node->esq);
    preOrder(node->dir);
}

void Arvore::inOrder(No* node) {
    if (node == nullptr) {
        return;
    }
    inOrder(node->esq);
    if (node->leaf == true) {
        std::cout << "Letra: " << node->letra << ", Frequência: " << node->freq << std::endl;
    } else {
        std::cout << "Frequência: " << node->freq << std::endl;
    }
    inOrder(node->dir);
}

void Arvore::Apagar(No* node) {
    if (node != nullptr) {
        Apagar(node->esq);
        Apagar(node->dir);
        node = nullptr;
        delete node;
    }
}

Arvore::~Arvore() {
    Apagar(this->raiz);
}

// Definição da classe No_mapa
class No_mapa {
public:
    std::string chave;
    std::string valor;
    No_mapa* esquerda;
    No_mapa* direita;
    int height;

    No_mapa(std::string k, const std::string& v);
};

// Implementação do construtor da classe No_mapa
No_mapa::No_mapa(std::string k, const std::string& v)
    : chave(k), valor(v), esquerda(nullptr), direita(nullptr), height(1) {}

// Definição da classe ArvoreMapaBinaria
class ArvoreMapaBinaria {
public:
    ArvoreMapaBinaria();
    ~ArvoreMapaBinaria();

    void inserir(std::string chave, const std::string& valor);
    bool contem(std::string chave);
    void limpar();
    No_mapa* Get_raiz();
    std::string Encontrar_Valor(std::string chave);

private:
    No_mapa* raiz;

    No_mapa* inserirNo(No_mapa* no, std::string chave, const std::string& valor);
    No_mapa* encontrarNo(No_mapa* no, std::string chave);
    No_mapa* encontrarMinimo(No_mapa* no);
    void limparArvore(No_mapa* no);
};

// Funções auxiliares para rotações e balanceamento
int height(No_mapa* N) {
    if (N == nullptr)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

No_mapa* rightRotate(No_mapa* y) {
    No_mapa* x = y->esquerda;
    No_mapa* T2 = x->direita;

    // Realiza a rotação
    x->direita = y;
    y->esquerda = T2;

    // Atualiza as alturas
    y->height = max(height(y->esquerda), height(y->direita)) + 1;
    x->height = max(height(x->esquerda), height(x->direita)) + 1;

    // Retorna a nova raiz
    return x;
}

No_mapa* leftRotate(No_mapa* x) {
    No_mapa* y = x->direita;
    No_mapa* T2 = y->esquerda;

    // Realiza a rotação
    y->esquerda = x;
    x->direita = T2;

    // Atualiza as alturas
    x->height = max(height(x->esquerda), height(x->direita)) + 1;
    y->height = max(height(y->esquerda), height(y->direita)) + 1;

    // Retorna a nova raiz
    return y;
}

int Get_Balance(No_mapa* N) {
    if (N == nullptr)
        return 0;
    return height(N->esquerda) - height(N->direita);
}

// Implementação dos métodos da classe ArvoreMapaBinaria
ArvoreMapaBinaria::ArvoreMapaBinaria() : raiz(nullptr) {}

ArvoreMapaBinaria::~ArvoreMapaBinaria() {
    limpar();
}

void ArvoreMapaBinaria::inserir(std::string chave, const std::string& valor) {
    raiz = inserirNo(raiz, chave, valor);
}

bool ArvoreMapaBinaria::contem(std::string chave) {
    return encontrarNo(raiz, chave) != nullptr;
}

void ArvoreMapaBinaria::limpar() {
    limparArvore(raiz);
    raiz = nullptr;
}

No_mapa* ArvoreMapaBinaria::inserirNo(No_mapa* no, std::string chave, const std::string& valor) {
    if (no == nullptr) {
        return new No_mapa(chave, valor);
    }

    if (chave < no->chave) {
        no->esquerda = inserirNo(no->esquerda, chave, valor);
    } else if (chave > no->chave) {
        no->direita = inserirNo(no->direita, chave, valor);
    } else {
        // Chave já existe, não faz nada
        return no;
    }

    // Atualiza a altura do nó atual
    no->height = 1 + max(height(no->esquerda), height(no->direita));

    // Verifica o fator de balanceamento
    int balanceFactor = Get_Balance(no);

    // Realiza rotações se necessário
    if (balanceFactor > 1 && chave < no->esquerda->chave) {
        return rightRotate(no);
    }

    if (balanceFactor < -1 && chave > no->direita->chave) {
        return leftRotate(no);
    }

    if (balanceFactor > 1 && chave > no->esquerda->chave) {
        no->esquerda = leftRotate(no->esquerda);
        return rightRotate(no);
    }

    if (balanceFactor < -1 && chave < no->direita->chave) {
        no->direita = rightRotate(no->direita);
        return leftRotate(no);
    }

    return no;
}

No_mapa* ArvoreMapaBinaria::encontrarNo(No_mapa* no, std::string chave) {
    if (no == nullptr || no->chave == chave) {
        return no;
    }

    if (chave < no->chave) {
        return encontrarNo(no->esquerda, chave);
    } else {
        return encontrarNo(no->direita, chave);
    }
}

No_mapa* ArvoreMapaBinaria::encontrarMinimo(No_mapa* no) {
    if (no == nullptr || no->esquerda == nullptr) {
        return no;
    }

    return encontrarMinimo(no->esquerda);
}

void ArvoreMapaBinaria::limparArvore(No_mapa* no) {
    if (no == nullptr) {
        return;
    }

    limparArvore(no->esquerda);
    limparArvore(no->direita);
    delete no;
}

No_mapa* ArvoreMapaBinaria::Get_raiz() {
    return this->raiz;
}

std::string ArvoreMapaBinaria::Encontrar_Valor(std::string chave) {
    No_mapa* no = raiz;
    while (no != nullptr) {
        if (chave < no->chave) {
            no = no->esquerda;
        } else if (chave > no->chave) {
            no = no->direita;
        } else {
            // Chave encontrada
            return no->valor;
        }
    }

    std::cout << "Valor não encontrado, chave procurada: " << chave << std::endl;
    // Chave não encontrada, retornar uma string vazia ou lançar uma exceção, conforme necessário
    return "";
}

// Definição da classe MinPriorityQueue
class MinPriorityQueue {
private:
    No** heap;
    int capacity;
    int size;
    int aux;

    int parent(int i);
    int leftChild(int i);
    int rightChild(int i);
    void resize(int newCapacity);
    void heapifyUp(int i);
    void heapifyDown(int i);

public:
    MinPriorityQueue();
    ~MinPriorityQueue();
    bool empty();
    void insert(No* no);
    No* getMin();
    No* removeMin();
    int Get_size();
};

// Implementação dos métodos da classe MinPriorityQueue
int MinPriorityQueue::parent(int i) {
    return (i - 1) / 2;
}

int MinPriorityQueue::leftChild(int i) {
    return 2 * i + 1;
}

int MinPriorityQueue::rightChild(int i) {
    return 2 * i + 2;
}

void MinPriorityQueue::resize(int newCapacity) {
    No** newHeap = new No*[newCapacity];
    for (int i = 0; i < size; i++) {
        newHeap[i] = heap[i];
    }
    delete[] heap; // Libera o array antigo
    heap = newHeap;
    capacity = newCapacity;
}

void MinPriorityQueue::heapifyUp(int i) {
    while (i > 0 && heap[parent(i)]->freq > heap[i]->freq) {
        std::swap(heap[i], heap[parent(i)]);
        i = parent(i);
    }
}

void MinPriorityQueue::heapifyDown(int i) {
    int smallest = i;
    int left = leftChild(i);
    int right = rightChild(i);

    if (left < size && heap[left]->freq < heap[smallest]->freq)
        smallest = left;

    if (right < size && heap[right]->freq < heap[smallest]->freq)
        smallest = right;

    if (smallest != i) {
        std::swap(heap[i], heap[smallest]);
        heapifyDown(smallest);
    }
}

MinPriorityQueue::MinPriorityQueue() : heap(nullptr), capacity(0), size(0), aux(0) {}

MinPriorityQueue::~MinPriorityQueue() {
    delete[] heap; // Libera o array de ponteiros
}

bool MinPriorityQueue::empty() {
    return size == 0;
}

void MinPriorityQueue::insert(No* no) {
    if (size == capacity) {
        int newCapacity = (capacity == 0) ? 1 : 2 * capacity;
        resize(newCapacity);
    }
    heap[size] = no;
    size++;
    heapifyUp(size - 1);
}

No* MinPriorityQueue::getMin() {
    if (empty()) {
        std::cerr << "Priority queue is empty!" << std::endl;
        return nullptr; // ou lançar uma exceção
    }
    return heap[0];
}

No* MinPriorityQueue::removeMin() {
    if (empty()) {
        std::cerr << "Priority queue is empty!" << std::endl;
        return nullptr; // ou lançar uma exceção
    }
    No* p = heap[0];
    heap[0] = heap[size - 1];
    size--;
    heapifyDown(0);
    return p;
}

int MinPriorityQueue::Get_size() {
    return size;
}

// Função para ler o conteúdo de um arquivo
std::string lerArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << arquivo.rdbuf();
    return buffer.str();
}

// Função para converter uma string em um número decimal
int stringTodecimal(const std::string& str) {
    int result = 0;
    for (char ch : str) {
        if (isdigit(ch)) {
            result = result * 10 + (ch - '0');
        }
    }
    return result;
}

// Função principal do fuzzer
int main_fuzzer(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Uso: programa <flag> <arquivo1> <arquivo2>" << std::endl;
        return 1;
    }

    std::string flag = argv[1];
    std::string nomeArquivo1 = argv[2];
    std::string nomeArquivo2 = argv[3];

    if (flag == "-c") {
        int aux[128] = {};
        std::ofstream arquivoescrita(nomeArquivo2);
        std::string leitura = lerArquivo(nomeArquivo1);
        int n = leitura.size();

        // Verificando as letras presentes
        for (int i = 0; i < n; i++) {
            aux[leitura[i]]++;
        }

        MinPriorityQueue pq;
        int count = 0;
        bool last_jump = false;

        if (arquivoescrita.is_open()) {
            arquivoescrita << n << std::endl;
            for (int i = 0; i < 128; i++) {
                if (aux[i] != 0) {
                    No* p = new No(i, true);
                    p->freq = aux[i];
                    pq.insert(p);

                    // Escrita
                    if (0 <= int(p->letra) && int(p->letra) <= 32) {
                        if (count != 0 && !last_jump)
                            arquivoescrita << std::endl;
                        if (0 <= int(p->letra) && int(p->letra) <= 9)
                            arquivoescrita << "0" << int(p->letra) << " " << p->freq << std::endl;
                        else
                            arquivoescrita << int(p->letra) << " " << p->freq << std::endl;
                        last_jump = true;
                    } else if (isdigit(p->letra)) {
                        if (count == 0)
                            arquivoescrita << p->letra << p->freq;
                        else
                            arquivoescrita << std::endl << p->letra << p->freq;
                    } else {
                        arquivoescrita << p->letra << p->freq;
                    }
                    count++;
                }
            }

            Arvore arv(pq.removeMin());
            if (arquivoescrita.is_open()) {
                arquivoescrita << std::endl << std::endl;
                std::string aux_str = "";
                std::string saida = "";
                for (int i = 0; i < n; i++) {
                    aux_str += leitura[i];
                    saida += arv.Get_raiz()->letra; // Exemplo simplificado
                    aux_str = "";
                }

                // Escrevendo os bytes no arquivo
                int size = saida.size();
                for (int i = 0; i < size; i += 8) {
                    char byte = 0;
                    for (int j = 0; j < 8; j++) {
                        if (i + j < size) {
                            byte <<= 1;
                            byte |= saida[i + j] - '0';
                        } else {
                            byte <<= 1;
                        }
                    }
                    arquivoescrita << byte;
                }
                arquivoescrita.close();
            } else {
                std::cerr << "Erro ao abrir o arquivo para escrita." << std::endl;
                return -1;
            }
        } else {
            std::cerr << "Erro ao abrir o arquivo para escrita." << std::endl;
            return -1;
        }
    } else if (flag == "-d") {
        std::string linha;
        std::ifstream arquivoleitura(nomeArquivo1);
        std::ofstream arquivoescrita(nomeArquivo2);
        MinPriorityQueue pq;
        std::string aux = "";

        if (arquivoescrita.is_open() && arquivoleitura.is_open()) {
            std::getline(arquivoleitura, linha);
            int n = stringTodecimal(linha);

            while (std::getline(arquivoleitura, linha)) {
                if (linha.empty()) {
                    Arvore arv(pq.removeMin());
                    std::getline(arquivoleitura, linha);
                    if (linha.empty()) {
                        std::getline(arquivoleitura, linha);
                    }

                    std::string linha2 = "";
                    while (std::getline(arquivoleitura, linha2)) {
                        linha += '\n';
                        linha += linha2;
                    }

                    std::string bitstring = "";
                    int size = linha.size();
                    for (int i = 0; i < size; i++) {
                        bitstring += std::bitset<8>(linha[i]).to_string();
                    }

                    No* auxilar = arv.Get_raiz();
                    int count1 = 0;
                    for (int i = 0; i < bitstring.size(); i++) {
                        if (auxilar->leaf) {
                            arquivoescrita << auxilar->letra;
                            auxilar = arv.Get_raiz();
                            count1++;
                            if (count1 == n)
                                break;
                        }
                        if (bitstring[i] == '1') {
                            auxilar = auxilar->dir;
                        } else if (bitstring[i] == '0') {
                            auxilar = auxilar->esq;
                        } else {
                            std::cerr << "ERRO: Binário inválido" << std::endl;
                            return -1;
                        }
                    }
                    break;
                }

                int size = linha.size();
                if (isdigit(linha[0])) {
                    if (linha[2] == ' ') {
                        // 0 a 32
                        aux += linha[0];
                        aux += linha[1];
                        int indice = stringTodecimal(aux);
                        char letra = indice;
                        aux = "";
                        for (int i = 3; i < size; i++) {
                            aux += linha[i];
                        }
                        No* p = new No(letra, true);
                        p->freq = stringTodecimal(aux);
                        pq.insert(p);
                        aux = "";
                    } else {
                        // digit
                        for (int i = 0; i < size; i++) {
                            char letra = linha[i];
                            while (isdigit(linha[i + 1])) {
                                aux += linha[i + 1];
                                i++;
                            }
                            No* p = new No(letra, true);
                            p->freq = stringTodecimal(aux);
                            pq.insert(p);
                            aux = "";
                        }
                    }
                } else {
                    for (int i = 0; i < size; i++) {
                        char letra = linha[i];
                        while (isdigit(linha[i + 1])) {
                            aux += linha[i + 1];
                            i++;
                        }
                        No* p = new No(letra, true);
                        p->freq = stringTodecimal(aux);
                        pq.insert(p);
                        aux = "";
                    }
                }
            }

            arquivoleitura.close();
            arquivoescrita.close();
        } else {
            std::cerr << "Erro ao abrir arquivos." << std::endl;
            return -1;
        }
    } else {
        std::cerr << "Flag inválida: " << flag << std::endl;
        return -1; // Invalid flag
    }

    return 0; // Success
}

// Log file for the fuzzer report
std::ofstream fuzzer_log("fuzzer_report.txt");

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Ensure the input is large enough to generate at least 3 arguments
    if (size < 3) return 0;

    // Split the input data into 3 parts: flag, arquivo1, arquivo2
    size_t flag_size = data[0] % (size - 2) + 1; // Random size for flag
    size_t arquivo1_size = data[1] % (size - flag_size - 1) + 1; // Random size for arquivo1
    size_t arquivo2_size = size - flag_size - arquivo1_size; // Remaining data for arquivo2

    // Extract the flag, arquivo1, and arquivo2 from the input data
    std::string flag(reinterpret_cast<const char*>(data), flag_size);
    std::string arquivo1(reinterpret_cast<const char*>(data + flag_size), arquivo1_size);
    std::string arquivo2(reinterpret_cast<const char*>(data + flag_size + arquivo1_size), arquivo2_size);

    // Ensure the flag is either "-c" or "-d"
    if (flag != "-c" && flag != "-d") {
        flag = (data[0] % 2 == 0) ? "-c" : "-d"; // Randomly choose between "-c" and "-d"
    }

    // Log the inputs
    fuzzer_log << "Testing inputs:" << std::endl;
    fuzzer_log << "Flag: " << flag << std::endl;
    fuzzer_log << "Arquivo1: " << arquivo1 << std::endl;
    fuzzer_log << "Arquivo2: " << arquivo2 << std::endl;

    // Prepare argv for main_fuzzer
    char* argv[] = {
        const_cast<char*>("programa"), // argv[0] (program name)
        const_cast<char*>(flag.c_str()), // argv[1] (flag)
        const_cast<char*>(arquivo1.c_str()), // argv[2] (arquivo1)
        const_cast<char*>(arquivo2.c_str()) // argv[3] (arquivo2)
    };
    int argc = 4;

    // Call the main_fuzzer function
    int result = main_fuzzer(argc, argv);

    // Log the result
    if (result == 0) {
        fuzzer_log << "Result: Valid case" << std::endl;
    } else {
        fuzzer_log << "Result: Error (return code " << result << ")" << std::endl;
    }

    fuzzer_log << "-------------------------" << std::endl;

    return 0;
}