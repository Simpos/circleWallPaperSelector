#include <stdlib.h>
#include <stdio.h>

#define HEADER_DA_INIT_CAP 10

typedef struct {
  size_t capacity;
  size_t count;
} Header;

#define da_push(arr, x)\
  do {\
    if((arr) == NULL){\
      Header *h = (Header*)malloc(sizeof(*arr) * HEADER_DA_INIT_CAP + sizeof(Header));\
      h->count = 0;\
      h->capacity = HEADER_DA_INIT_CAP;\
      (arr) = (void*)(h + 1);\
    }\
    Header *h = (Header*)(arr) - 1;\
    if(h->count >= h->capacity){\
      h->capacity *= 2;\
      h = realloc(h,sizeof(*arr) * h->capacity + sizeof(Header));\
      (arr) = (void*)(h + 1);\
    }\
    (arr)[h->count++] = (x);\
  }while(0)\

#define da_len(arr)((Header*)(arr) - 1)->count
#define da_free(arr)free((Header*)(arr) - 1)
#define da_print(arr,...)\
  do {\
    if(!(arr))break;\
    for(size_t i = 0; i < da_len((arr)); ++i){\
      printf(__VA_ARGS__);\
    }\
  }while(0)\

