// PARAMETERIZED BUILD
#ifndef DATA_SIZE_CHAR
#define DATA_SIZE_CHAR 32
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "bloom/src/bloom.h"

#define DATA_SIZE (DATA_SIZE_CHAR / 2)
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
    printf("Expected hex input size: %d\n", DATA_SIZE_CHAR);
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
  for (size_t i = 0; i < length; i += 2){
    hexdata[i / 2] = (unhex(line[i]) << 4) + unhex(line[i + 1]);
  }
  return length / 2;
}

// This hash function returns a set of hashes from the bytes passed in 'bytes'.
// Therefore, it should really only be used for things that are hashes.
uint64_t* hashfunction_simple_bytes(int num_hashes, const char *bytes){
  uint64_t* hashes = calloc(num_hashes, sizeof(uint64_t));
  int step = (int)(DATA_SIZE - sizeof(uint64_t));
  int n = 0;
  for (int i = (int)(DATA_SIZE - sizeof(uint64_t)); i >= 0 && n <= num_hashes; i -= step){
    // convert the bytes at this postion directly into a hash
    hashes[n++] = *(uint64_t*)(bytes + i); 
  }
  return hashes;
}

BloomFilter* get_filter_for(const char * text){
  const char * charset = CHARSET;
  BloomFilter* filt = (BloomFilter*)malloc(sizeof(BloomFilter));
  memset(filt, 0, sizeof(BloomFilter));
  char filename[100];
  sprintf(filename, "data/%c%c", charset[unhex(text[START_TEXT_PARTITION_OFFSET])], charset[unhex(text[START_TEXT_PARTITION_OFFSET + 1])]);
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
      memset(filt, 0, sizeof(BloomFilter));
      bloom_filter_init_on_disk_alt(filt, ESTIMATED_ELEMENTS, 0.05f, filename, &hashfunction_simple_bytes);
    }
  }
  return 0;
}

int addhashes(){
  char hexdata[DATA_SIZE+1];
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  while ((nread = getline(&line, &len, stdin)) != -1) {
    printf("Added hash %s\n", line);
    fflush(stdout);
    getlinehex(hexdata, line);
    BloomFilter* bf = get_filter_for(line);
    bloom_filter_add_string(bf, hexdata); // saving is handled by this library
    // TODO: could keep it open if it's the same partition next time
    bloom_filter_destroy(bf);
    free(bf);
  }
  return 0;
}

int queryhashes(){
  char hexdata[DATA_SIZE+1];
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  while ((nread = getline(&line, &len, stdin)) != -1) {
    if (line[DATA_SIZE_CHAR] == '\x0a')  line[DATA_SIZE_CHAR] = 0;
    getlinehex(hexdata, line);
    BloomFilter* bf = get_filter_for(line);
    if (bloom_filter_check_string(bf, hexdata) == BLOOM_FAILURE){
      printf("%s:N\n", line);
    } else {
      printf("%s:Y\n", line);
    }
    fflush(stdout);
    // TODO: could keep it open if it's the same partition next time
    bloom_filter_destroy(bf);
    free(bf);
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
    default:
      return -1;
  }
}
