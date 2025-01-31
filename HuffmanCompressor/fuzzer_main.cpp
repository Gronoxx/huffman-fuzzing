
#include "../../../Estruturas-de-Dados/Projeto 3/TP/include/arvoreh.hpp"
#include "../../../Estruturas-de-Dados/Projeto 3/TP/include/arvoremapabinaria.hpp"
#include "../../../Estruturas-de-Dados/Projeto 3/TP/include/mapano.hpp"
#include "../../../Estruturas-de-Dados/Projeto 3/TP/include/MinPriorityQueue.hpp"
#include "../../../Estruturas-de-Dados/Projeto 3/TP/include/no.hpp"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <cctype>
#include <bitset>

ArvoreMapaBinaria binmap;

int stringTodecimal(std::string a){
    int resultado=0;
    int size =a.size();
    for(int i=0;i<size;i++){
        resultado+=(int(a[i])-48)*pow(10,size-i-1);
    }
    return resultado;
}

void Cod_Huffman(No* raiz, const std::string& code) {
    if (raiz == nullptr) {
        return;
    }
    if(raiz->leaf){
    std::string aux="";
    aux+=raiz->letra;
    binmap.inserir(aux,code);
    }

    Cod_Huffman(raiz->esq, code + "0");
    Cod_Huffman(raiz->dir, code + "1");
}


No * Huffman(MinPriorityQueue Q,bool cod){
    while(1<Q.Get_size()){
            No* aux1=Q.removeMin();
            No* aux2=Q.removeMin();
            int z_freq = aux1->freq +aux2->freq;
            No* z = new No('\0', false);
            z->freq = z_freq;
            z->esq=aux1;
            z->dir=aux2;
            Q.insert(z);
    }
    No* raiz = Q.removeMin();
    if(cod==true)
    Cod_Huffman(raiz,"");
    return raiz;
}

std::string lerArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << arquivo.rdbuf(); // Lê todo o conteúdo do arquivo e armazena no stringstream

    arquivo.close();

    return buffer.str(); // Retorna o conteúdo do stringstream como uma string
}

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

        MinPriorityQueue pq, pq2;
        int count = 0;
        bool last_jump = false;

        if (arquivoescrita.is_open()) {
            arquivoescrita << n << std::endl;
            for (int i = 0; i < 128; i++) {
                if (aux[i] != 0) {
                    No* p = new No(i, true);
                    p->freq = aux[i];
                    No* aux1 = new No(i, true);
                    aux1->freq = aux[i];
                    pq.insert(p);

                    // Escrita
                    if (0 <= int(aux1->letra) && int(aux1->letra) <= 32) {
                        if (count != 0 && !last_jump)
                            arquivoescrita << std::endl;
                        if (0 <= int(aux1->letra) && int(aux1->letra) <= 9)
                            arquivoescrita << "0" << int(aux1->letra) << " " << aux1->freq << std::endl;
                        else
                            arquivoescrita << int(aux1->letra) << " " << aux1->freq << std::endl;
                        last_jump = true;
                    } else if (isdigit(aux1->letra)) {
                        if (count == 0)
                            arquivoescrita << aux1->letra << aux1->freq;
                        else
                            arquivoescrita << std::endl << aux1->letra << aux1->freq;
                    } else {
                        arquivoescrita << aux1->letra << aux1->freq;
                    }
                    count++;
                }
            }

            Arvore arv(Huffman(pq, true));
            if (arquivoescrita.is_open()) {
                arquivoescrita << std::endl << std::endl;
                std::string aux_str = "";
                std::string saida = "";
                for (int i = 0; i < n; i++) {
                    aux_str += leitura[i];
                    saida += binmap.Encontrar_Valor(aux_str);
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
                    Arvore arv(Huffman(pq, false));
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
    }else {
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