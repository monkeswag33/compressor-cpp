# compressor-v2
A compressor/decompressor using Huffman coding  

## Build
```bash
# Run in source directory
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=1 # Set BUILD_TESTING to 0 to not build tests
make -j4 # -j<number of threads>
```

## Test
```bash
make test
```

## How to Use
The compressor and decompressor have the following arguments:
```
Compressor:
Argument 1: File or directory to compress
Argument 2: OPTIONAL - Output filename; defaults to "<file or directory name>.cmp"

Decompressor:
Argument 1: File or directory to decompress
Argument 2: OPTIONAL - Output filename; defaults to file or directory name
```
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

### Compressing a File
Compress a file like this:
```bash
./compressor file_to_compress.txt # Creates a file called file_to_compress.cmp
./decompressor file_to_compress.cmp # Recreates the file_to_compress.txt
```
