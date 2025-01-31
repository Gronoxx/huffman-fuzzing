#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

// Função que você está testando
void lerArquivo(const std::string& filename, std::ofstream& log_file) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Não foi possível abrir o arquivo." << std::endl;
        log_file << "[ERROR] Não foi possível abrir o arquivo: " << filename << std::endl;
    }
}

// Função do fuzzer
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Criar um arquivo temporário para o fuzzer testar
    std::ofstream temp_file("temp_input.txt", std::ios::binary);
    if (temp_file.is_open()) {
        temp_file.write(reinterpret_cast<const char*>(Data), Size);
        temp_file.close();
    }

    // Criar ou abrir o arquivo de log
    std::ofstream log_file("fuzzer_log.txt", std::ios::app);
    if (!log_file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de log." << std::endl;
        return 1;
    }

    // Adicionar um timestamp no log
    std::time_t now = std::time(nullptr);
    log_file << "[INFO] Teste iniciado em " << std::ctime(&now) << std::endl;

    // Chamar a função que você deseja testar (no caso, lerArquivo)
    log_file << "[INFO] Testando o arquivo temp_input.txt..." << std::endl;
    lerArquivo("temp_input.txt", log_file);  // Agora passamos log_file como argumento

    // Finalizar o teste
    log_file << "[INFO] Teste concluído." << std::endl;
    log_file << "--------------------------------------------" << std::endl;

    log_file.close();

    return 0;  // Retorna 0 para o fuzzer
}
