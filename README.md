# Fuzzing a Huffman compressor

Cybersecurity coursework at DCC/UFMG. The assignment: take a program you wrote before, in this case
my Huffman file compressor from a data structures course, and attack it with coverage-guided fuzzing.

## Layout

- `TP/`: the target, the original Huffman compressor in C++ (tree, min-priority queue, AVL-balanced
  map used to translate characters to codes).
- `HuffmanCompressor/huffman/`: the fuzzing setup in OSS-Fuzz project layout, with `Dockerfile`,
  `build.sh`, `project.yaml`, and the harness.

## The harness

`fuzzer.cpp` uses libFuzzer's `FuzzedDataProvider` to split each input into a decision and a payload:
one consumed bool picks compression or decompression, and the remaining bytes become the file
content. That way a single corpus exercises both paths, including feeding the decompressor bytes that
no compressor would ever produce.

Built with AddressSanitizer, so out-of-bounds reads and leaks surface as crashes instead of passing
silently.

```bash
cd HuffmanCompressor/huffman
docker build -t huffman-fuzz .
```
