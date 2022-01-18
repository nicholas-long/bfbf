#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bloom.h"

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

int queryhashes(){
  return 0;
}
