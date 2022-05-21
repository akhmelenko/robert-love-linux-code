
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #include <string.h>
// void *memchr(const void *s, int c, size_t n);
// #define _GNU_SOURCE
// void *memrchr(const void *s, int c, size_t n); - reverse
// scans the initial n bytes of the memory
// area pointed to by s for the first instance of c.
// Both c and the bytes of the memory area pointed to by s are interpreted as unsigned char
// Return a pointer to the matching byte or NULL if the character does not occur in the given memory area
//
// void *memmem(const void *haystack, size_t haystacklen, const void *needle, size_t needlelen);
// Returns a pointer to the beginning of the substring, or NULL if the substring is not found
//
// #define _GNU_SOURCE
// void *memfrob(void *s, size_t n);
// encrypts the first n bytes of the memory area s by XORing each character with the number 42.
// The effect can be reversed by using memfrob() on the encrypted memory area
// Returns a pointer to the encrypted memory area

void SearchByte()
{
    char buf[] = "kazak";

    printf("\n--search char forward--\n");
    char *res = memchr(buf, 'a', strlen(buf));
    printf("memchr() buf data=%s ptr=%p res char=%s res ptr=%p\n", buf, buf, res, res);

    printf("\n--search char reverse--\n");
    res = memrchr(buf, 'a', strlen(buf));
    printf("memrchr() buf data=%s ptr=%p res char=%s res ptr=%p\n", buf, buf, res, res);
}

void SearchStringInString()
{
    printf("\n--search needle in haystack--\n");
    char *haystack = "Find needle in the haystack";
    char *needle = "needle";
    char *res = memmem(haystack, strlen(haystack), needle, strlen(needle));
    if (res)
        printf("=%s if haystack=%s found at =%s\n", needle, haystack, res);
    else
        printf("needle not found\n");
}

void ShadowingBytes()
{
    printf("\n--shadowing bytes by 42--\n");
    char buf[] = "Clear string";
    printf("initial=%s\n", buf);
    char *res = memfrob(buf, strlen(buf));
    printf("shadowed=%s res=%s\n", buf, res);
    res = memfrob(buf, strlen(buf));
    printf("dowble shadowed=%s res=%s\n", buf, res);
}

int main()
{
    SearchByte();
    SearchStringInString();
    ShadowingBytes();

    exit(EXIT_SUCCESS);
}