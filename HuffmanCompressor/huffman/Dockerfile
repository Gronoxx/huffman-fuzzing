FROM gcr.io/oss-fuzz-base/base-builder

RUN apt-get update && apt-get install -y cmake make git \
    && rm -rf /var/lib/apt/lists/*

# Mude o WORKDIR para /src (diretório padrão do OSS-Fuzz)
WORKDIR /src

# Copie o projeto para /src (não para /src/huffman)
COPY . /src/

# Garanta permissões
RUN chmod +x /src/build.sh