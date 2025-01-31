#include "huffman.h"

// Implementação da classe No
No::No(char letra, bool leaf) {
    if (leaf) {
        this->letra = letra;
        this->freq = 0;
    }
    this->leaf = leaf;
    this->esq = nullptr;
    this->dir = nullptr;
}

// Implementação da classe Arvore
Arvore::Arvore(No* raiz) {
    this->raiz = raiz;
}

No* Arvore::Get_raiz() {
    return raiz;
}

void Arvore::posOrder(No* node) {
    if (node == nullptr) return;
    posOrder(node->esq);
    posOrder(node->dir);
    if (node->leaf) {
        std::cout << "Letra: " << node->letra << ", Frequência: " << node->freq << std::endl;
    } else {
        std::cout << "Frequência: " << node->freq << std::endl;
    }
}

void Arvore::Apagar(No* node) {
    if (node != nullptr) {
        Apagar(node->esq);
        Apagar(node->dir);
        delete node;
    }
}

Arvore::~Arvore() {
    Apagar(this->raiz);
}

// Implementação da classe MinPriorityQueue
MinPriorityQueue::MinPriorityQueue() : heap(nullptr), capacity(0), size(0), aux(0) {}

MinPriorityQueue::~MinPriorityQueue() {
    delete[] heap;
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
        return nullptr;
    }
    return heap[0];
}

No* MinPriorityQueue::removeMin() {
    if (empty()) {
        std::cerr << "Priority queue is empty!" << std::endl;
        return nullptr;
    }
    No* p = heap[0];
    heap[0] = heap[size - 1];
    size--;
    heapifyDown(0);
    return p;
}

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

int stringTodecimal(const std::string& str) {
    int result = 0;
    for (char ch : str) {
        if (isdigit(ch)) {
            result = result * 10 + (ch - '0');
        }
    }
    return result;
}

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

    if (left < size && heap[left]->freq < heap[smallest]->freq) {
        smallest = left;
    }
    if (right < size && heap[right]->freq < heap[smallest]->freq) {
        smallest = right;
    }
    if (smallest != i) {
        std::swap(heap[i], heap[smallest]);
        heapifyDown(smallest);
    }
}

// Função principal de processamento (substitui a lógica de arquivo)
void processarDadosHuffman(const std::string& flag, const std::string& input) {
    if (flag == "-c") {
        processarCompressao(input);
    } else if (flag == "-d") {
        processarDescompressao(input);
    }
}

// Lógica de compressão em memória
void processarCompressao(const std::string& input) {
    MinPriorityQueue pq;
    int aux[128] = {};

    // Contar frequências diretamente da entrada
    for (char ch : input) {
        if (static_cast<unsigned char>(ch) < 128) { // Remove a verificação redundante
             aux[static_cast<unsigned char>(ch)]++;
        }
    }

    // Construir a fila de prioridade
    for (int i = 0; i < 128; i++) {
        if (aux[i] > 0) {
            No* p = new No(i, true);
            p->freq = aux[i];
            pq.insert(p);
        }
    }

    // Simular saída de compressão (apenas para teste)
    std::stringstream simulated_output;
    simulated_output << input.size() << "\n";
    while (!pq.empty()) {
        No* min = pq.removeMin();
        simulated_output << min->letra << " " << min->freq << "\n";
        delete min; // Liberar memória (ajuste conforme necessário)
    }
}

// Lógica de descompressão em memória (simplificada para fuzzing)
void processarDescompressao(const std::string& input) {
    // Simular leitura de cabeçalho "comprimido"
    try {
        size_t pos = 0;
        int total_chars = std::stoi(input, &pos);
        
        MinPriorityQueue pq;
        std::string remaining = input.substr(pos);
        
        // Simular reconstrução da árvore (puramente para teste)
        for (size_t i = 0; i < remaining.size(); i += 3) {
            char letra = remaining[i];
            int freq = static_cast<int>(remaining[i+2]); // Valor fictício
            No* p = new No(letra, true);
            p->freq = freq;
            pq.insert(p);
        }
        
        // Liberar memória alocada
        while (!pq.empty()) {
            delete pq.removeMin();
        }
    } catch (...) {
        // Ignorar erros de parsing (serão detectados pelo fuzzer)
    }
}

// Função principal do sistema (sem fuzzing)
int main_fuzzer(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Uso: programa <flag> <arquivo1> <arquivo2>" << std::endl;
        return 1;
    }

    std::string flag = argv[1];
    std::string nomeArquivo1 = argv[2];
    std::string nomeArquivo2 = argv[3];

    if (flag == "-c") {
        std::string leitura = lerArquivo(nomeArquivo1);
        int n = leitura.size();

        MinPriorityQueue pq;
        int aux[128] = {};

        for (int i = 0; i < n; i++) {
            aux[leitura[i]]++;
        }

        std::ofstream arquivoescrita(nomeArquivo2);
        if (arquivoescrita.is_open()) {
            arquivoescrita << n << std::endl;
            for (int i = 0; i < 128; i++) {
                if (aux[i] != 0) {
                    No* p = new No(i, true);
                    p->freq = aux[i];
                    pq.insert(p);
                    arquivoescrita << p->letra << " " << p->freq << std::endl;
                }
            }
            arquivoescrita.close();
        }
    }
    return 0;
}
