#include <stdio.h>

int main() {
    char dest[20];
    __builtin_strcpy(dest, "Hello, World!");
    printf("%s\n", dest);
    return 0;
}