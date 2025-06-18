#include <stdio.h>

extern void my_strncpy(char *dst, char *src, size_t len) __attribute__((cdecl));

#define STRLEN 20

size_t my_strlen(const char* str)
{
    size_t len;

    asm(
        ".intel_syntax noprefix\n"
        "mov ecx, -1\n"
        "xor eax, eax\n"
        "mov edi, %1\n"
        "cld\n"
        "repne scasb\n"
        "sub edi, %1\n"
        "dec edi\n"
        "mov %0, edi\n"
        ".att_syntax\n"
        : "=r"(len)               // Выходной параметр
        : "r"(str)                // Входной параметр
        : "ecx", "eax", "edi"     // Используемые регистры
    );

    return len;
}

int main(void)
{
    setbuf(stdout, NULL);
    char str[STRLEN] = "123456";

    size_t len = my_strlen(str);

    printf("Str = %s, len = %zu\n", str, len);

    // Тест 1: dst и src не перекрываются
    char buff1[STRLEN];
    my_strncpy(buff1, str, len);
    printf("Buff1 = %s\n", buff1);

    // Тест 2: dst > src (перекрытие)
    char *buff2 = &str[3];
    my_strncpy(buff2, str, len);
    printf("Buff2 = %s\n", buff2);

    // Тест 3: dst == src
    char *buff3 = str;
    my_strncpy(buff3, str, len);
    printf("Buff3 = %s\n", buff3);

    return 0;
}