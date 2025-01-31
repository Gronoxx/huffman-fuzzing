FROM gcr.io/oss-fuzz-base/base-builder

# Instalação das dependências para compilar o projeto
RUN apt-get update && apt-get install -y \
    build-essential \
    clang \
    cmake \
    libssl-dev \
    libboost-all-dev \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

# Copia o código do fuzzer para o contêiner
COPY . /src

# Compila o fuzzer
RUN make -C /src

# Especifica o comando de execução do fuzzer
CMD ["/src/fuzz_lerArquivo"]
