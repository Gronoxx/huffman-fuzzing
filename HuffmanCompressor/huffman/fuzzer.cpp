#include "huffman.h"
#include <fuzzer/FuzzedDataProvider.h>   // Adicione este include

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    FuzzedDataProvider provider(data, size);
    
    // Gerar flags e dados de forma mais controlada
    bool compress = provider.ConsumeBool();
    std::string flag = compress ? "-c" : "-d";
    
    // Gerar entrada a partir dos dados do fuzzer
    std::string input = provider.ConsumeRemainingBytesAsString();

    processarDadosHuffman(flag, input);
    return 0;
}