# A Compressor in C++
A compressor/decompressor using Huffman coding  

## How It Works
The compressor follows the following steps:
- Read the input file(s)
- Generate frequencies of each character
- Generate a [huffman tree](https://en.wikipedia.org/wiki/Huffman_coding) from the frequencies
- Write huffman tree to output file
- Read input file(s) again
- Get corresponding bits from huffman tree
- Write bits to output file

The decompressor works in a similar way:
- Read input file (compressed)
- Read [huffman tree](https://en.wikipedia.org/wiki/Huffman_coding) from input file
- Read bits from input file
- Get corresponding character from huffman tree
- Write character to output file

## Build
Building is extremely easy with Zig.
```bash
$ zig build -Doptimize=ReleaseSafe
```

## How to Use
### Compressing a Directory
Say you have a directory structure like this:
```bash
├── directory_to_compress
│   ├── file1.txt
│   ├── file2.txt
│   ├── file3.txt
```
Compress the directory like this:
```bash
./compressor directory_to_compress/ # Creates a file called directory_to_compress.cmp
./decompressor directory_to_compress.cmp # Recreates the directory_to_compress directory
```
