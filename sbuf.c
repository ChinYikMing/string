#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>

typedef struct strbuf {
  char *val;
  size_t capacity; /* the allocated size */
  size_t size; /* the already used size */
} StrBuf;

int strbuf_init(StrBuf *p, unsigned cap_bits);
int strbuf_putc(StrBuf *p, char c);
int strbuf_putn(StrBuf *p, char *src, size_t n);
int strbuf_puts(StrBuf *p, char *str);
char *strbuf_flush(StrBuf *p);

int main(){
  StrBuf sbuf;
  strbuf_init(&sbuf, 5); 

  strbuf_putc(&sbuf, 'a');
  strbuf_putc(&sbuf, 'p');
  strbuf_putc(&sbuf, 'p');
  strbuf_puts(&sbuf, "le");

  char arr[] = {0x2c, 0x20, 0x77, 0x6f, 0x72, 0x6c, 0x64, 0x21};
  strbuf_putn(&sbuf, arr, 8);

  char *result = strbuf_flush(&sbuf);
  printf("The string buffer now is %s\n", result);

  free(result);
}

int strbuf_init(StrBuf *p, unsigned cap_bits){
  const size_t capacity = 1u << cap_bits;
  p->val = malloc(capacity);
  if(!p->val) return -1;
  p->capacity = capacity;
  p->size = 0;
  return 0;
}

int strbuf_putc(StrBuf *p, char c){
  if(p->size == p->capacity){
    const size_t new_capacity = p->capacity << 1u;
    void *tmp = realloc(p->val, new_capacity);
    if(!tmp) return -1;
    p->val = tmp;
    p->capacity = new_capacity;
  }
  p->val[p->size++] = c;
  return 0;
}

int strbuf_putn(StrBuf *p, char *src, size_t n){
  const size_t capacity = p->capacity;
  const size_t size = p->size;
  const size_t balance = capacity - size;
  const size_t extra_need = (balance < n) ? (n - balance) : 0;

  if(extra_need > 0){
    const size_t total_need = capacity + extra_need;
    size_t new_capacity = capacity;
    do
      new_capacity <<= 1;
    while(new_capacity < total_need);

    void *tmp = realloc(p->val, new_capacity);
    if(!tmp) return -1;
    p->val = tmp;
    p->capacity = new_capacity;
  }

  memcpy(p->val + size, src, n);
  p->size += n;
  return 0;
}

int strbuf_puts(StrBuf *p, char *str){
  strbuf_putn(p, str, strlen(str));
}

char *strbuf_flush(StrBuf *p){
  size_t size = p->size;

  if(0 == p->size || p->val[p->size - 1] != '\0'){
    strbuf_putc(p, '\0');
    size++;
  }

  char *ret = realloc(p->val, size);

  p->val = NULL;
  return ret;
}
