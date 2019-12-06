#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_SIZE 4

char *buf;
size_t bufp;
size_t capacity;

int sb_init(){
    buf = malloc(sizeof(char) * BUF_SIZE);
    if(!buf) return -1;
    bufp = 0;
    capacity = BUF_SIZE;
    return 0;
}

int sb_putc(char c){
    if(bufp == capacity){
        size_t new_cap = capacity * 2;
        void *tmp = realloc(buf, new_cap);
        if(!tmp) return -1;
        buf = tmp;
        capacity = new_cap;
    }
    buf[bufp++] = c;
    return 0;
}

int sb_puts(char *s){
    size_t len = strlen(s);
    if(bufp + len > capacity){
        int expand = ((int)(len/capacity)) + 1;
        size_t new_cap = capacity * (1 + expand);
        void *tmp = realloc(buf, new_cap);
        if(!tmp) return -1;
        buf = tmp;
        capacity = new_cap;
    }
    memcpy(buf+bufp, s, len);
    bufp += len;
    return 0;
}

char *sb_flush(){
    char *ret = buf;
    buf = NULL;
    return ret;
}

int main(){
    sb_init();
    sb_puts("Foo");
    sb_puts("Bar");
    sb_putc('\0');
    printf("%s\n", sb_flush());
}
