#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "bloom.h"

#define DATA_SIZE 16
#define DATA_SIZE_CHAR 32
#define START_TEXT_PARTITION_OFFSET 0
#define ESTIMATED_ELEMENTS 1000000u
#define CHARSET "0123456789abcdef"

int addhashes();
int helpmenu();
int queryhashes();
int unhex(char digit);
int init();
BloomFilter* get_filter_for(const char * text);
uint64_t* hashfunction_simple_bytes(int num_hashes, const char *bytes);
size_t getlinehex(char* hexdata, char* line);

int helpmenu(){
    printf("BFBF Help\n");
    printf("Must run with -i to initialize before use\n");
    printf("  -a   Add items from stdin\n");
    printf("  -q   Query items from stdin. Return Y/N\n");
    printf("  -i   Initialize first time\n");
    printf("  -h   This help\n");
    return 2;
}

int main(int argc, char** argv){
  if (argc < 2){
    printf("Usage %s [ -a | -q | -i | -h ]\n", argv[0]);
    return 1;
  }
  switch (argv[1][1]){
    case 'h': return helpmenu();
    case 'a': return addhashes();
    case 'q': return queryhashes();
    case 'i': return init();
    default: return 1;
  }
}

size_t getlinehex(char* hexdata, char* line){
  size_t length = strlen(line) - 1;
  // printf("Length %u\n", (uint32_t)length);
  for (size_t i = 0; i < length; i += 2){
    // printf("Chars %c %c\n", line[i], line[i + 1]);
    hexdata[i / 2] = (unhex(line[i]) << 4) + unhex(line[i + 1]);
    // printf("%x\n", (unsigned char)hexdata[i/2]);
  }
  return length / 2;
}

// This hash function returns a set of hashes from the bytes passed in 'bytes'.
// Therefore, it should really only be used for things that are hashes.
uint64_t* hashfunction_simple_bytes(int num_hashes, const char *bytes){
  uint64_t* hashes = malloc(sizeof(uint64_t) * num_hashes);
  int n = 0;
  for (int i = (int)(DATA_SIZE - sizeof(uint64_t)); i >= 0; i--){
    // convert the bytes at this postion directly into a hash
    hashes[n++] = *(uint64_t*)(bytes + i); 
  }
  // for (int i = 0; i < n; i++){
    // printf("%p\n", (void*)hashes[i]);
  // }
  return hashes;
}

BloomFilter* get_filter_for(const char * text){
  BloomFilter* filt = (BloomFilter*)malloc(sizeof(BloomFilter));
  char filename[100];
  sprintf(filename, "data/%c%c", text[0], text[1]);
  // printf("File name %s\n", filename);
  bloom_filter_import_on_disk_alt(filt, filename, &hashfunction_simple_bytes);
  return filt;
}

int init(){
  const char * charset = CHARSET;
  for (int i = 0; i < 16; i++){
    for (int j = 0; j < 16; j++){
      char filename[100];
      sprintf(filename, "data/%c%c", charset[i], charset[j]);
      BloomFilter* filt = (BloomFilter*)malloc(sizeof(BloomFilter));
      bloom_filter_init_on_disk_alt(filt, ESTIMATED_ELEMENTS, 0.05f, filename, &hashfunction_simple_bytes);
    }
  }
  return 0;
}

int addhashes(){
  char hexdata[32];
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  while ((nread = getline(&line, &len, stdin)) != -1) {
    getlinehex(hexdata, line);
    BloomFilter* bf = get_filter_for(line);
    bloom_filter_add_string(bf, hexdata);
    // saving is handled by this library
    free(bf);
  }
  // bloom_filter_add_string(&bf, "test");
  // if (bloom_filter_check_string(&bf, "test") == BLOOM_FAILURE) {
      // printf("'test' is not in the Bloom Filter\n");
  // } else {
      // printf("'test' is in the Bloom Filter\n");
  // }
  // bloom_filter_stats(&bf);
  // bloom_filter_destroy(&bf);
}

int queryhashes(){
  char hexdata[32];
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  while ((nread = getline(&line, &len, stdin)) != -1) {
    getlinehex(hexdata, line);
    BloomFilter* bf = get_filter_for(line);
    if (bloom_filter_check_string(bf, hexdata) == BLOOM_FAILURE){
      puts("N\n");
    } else {
      puts("Y\n");
    }
    free(bf);
    // uint64_t* hashes = hashfunction_simple_bytes(9, hexdata);
    // for (int n = 0; n < 9; n++){
      // printf("%p\n", (void*)hashes[n]);
    // }
    // nread = length; //
  }
  return 0;
}

int unhex(char digit){
  switch(digit){
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return digit - '0';
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
      return (digit & ~32) - 'A' + 10;
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      return digit - 'A' + 10;
  }
}
