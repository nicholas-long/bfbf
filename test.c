#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "bloom.h"

#define DATA_SIZE 16
#define DATA_SIZE_CHAR 32

int addhashes();
int helpmenu();
int queryhashes();

int helpmenu(){
    printf("BFBF Help\n");
    printf("  -a   Add items from stdin\n");
    printf("  -q   Query items from stdin. Return Y/N\n");
    printf("  -h   This help\n");
    return 2;
}

int main(int argc, char** argv){
  if (argc < 2){
    printf("Usage %s [ -a | -q | -h ]\n", argv[0]);
    return 1;
  }
  switch (argv[1][1]){
    case 'h': return helpmenu();
    case 'a': return addhashes();
    case 'q': return queryhashes();
    default: return 1;
  }
}

int addhashes(){
  BloomFilter bf;
  /*  elements = 10;
      false positive rate = 5% */
  bloom_filter_init(&bf, 10, 0.05);
  bloom_filter_add_string(&bf, "test");
  if (bloom_filter_check_string(&bf, "test") == BLOOM_FAILURE) {
      printf("'test' is not in the Bloom Filter\n");
  } else {
      printf("'test' is in the Bloom Filter\n");
  }
  if (bloom_filter_check_string(&bf, "blah") == BLOOM_FAILURE) {
      printf("'blah' is not in the Bloom Filter!\n");
  } else {
      printf("'blah' is in th Bloom Filter\n");
  }
  bloom_filter_stats(&bf);
  bloom_filter_destroy(&bf);
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
  return hashes;
}

int queryhashes(){
  char hexdata[32];
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  while ((nread = getline(&line, &len, stdin)) != -1) {
    size_t length = getlinehex(hexdata, line);
    uint64_t* hashes = hashfunction_simple_bytes(9, hexdata);
    for (int n = 0; n < 9; n++){
      printf("%p\n", (void*)hashes[n]);
    }
    nread = length; //
  }
  return 0;
}
