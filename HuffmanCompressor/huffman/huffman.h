#ifndef HUFFMAN_H
#define HUFFMAN_H

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

class No {
public:
    No(char letra, bool leaf);
    No* esq;
    No* dir;
    int freq;
    bool leaf;
    char letra;
};

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
void processarDadosHuffman(const std::string& flag, const std::string& input);
void processarCompressao(const std::string& input);
void processarDescompressao(const std::string& input);

std::string lerArquivo(const std::string& nomeArquivo);
int stringTodecimal(const std::string& str);
int main_fuzzer(int argc, char* argv[]);

#endif // HUFFMAN_H
