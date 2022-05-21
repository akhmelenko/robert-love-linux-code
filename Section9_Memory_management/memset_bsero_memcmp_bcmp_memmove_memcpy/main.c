#define _GNU_SOURCE
#include <stdio.h>
#include <string.h> // memset() ...
#include <stdlib.h> // exit()

// include <string.h>
// void * memset(void *s, int c, size_t size); - returns s;
// void bzero(void *s, size_t n); - erases the data in the n bytes of the memory
// starting at the location pointed to by s. Returns none
//
// int memcmp(const void *s1, const void *s2, size_t n); - compares the first n bytes (each
// interpreted as unsigned char) of the memory areas s1 and s2
// Returns an integer less than, equal to, or
//    greater than zero if the first n bytes of s1 is found,
//    respectively, to be less than, to match, or be greater than the
//    first n bytes of s2
//
// int bcmp(const void *s1, const void *s2, size_t n); - compares the two byte sequences s1 and s2 of
// length n each.  If they are equal, and in particular if n is
// zero, bcmp() returns 0.  Otherwise, it returns a nonzero result
//
// void *memmove(void *dest, const void *src, size_t n);
// copies n bytes from memory area src to memory area dest.  The memory areas may overlap
// Returns a pointer to dest
//
// void bcopy(const void *src, void *dest, size_t n);
// copies n bytes from src to dest.  The result is correct, even when both areas overlap
// Return none
//
// void *memcpy(void *restrict dest, const void *restrict src, size_t n);
// copies n bytes from memory area src to memory area dest.
// !!! The memory areas must not overlap !!!
// Returns a pointer to dest
//
// void *memccpy(void *restrict dest, const void *restrict src, int c, size_t n);
// copies no more than n bytes from memory area src to memory area dest,
// stopping when the character c is found
// !!! If the memory areas overlap, the results are undefined !!!
// Return: a pointer to the next character in dest after c,
// or NULL if c was not found in the first n characters of src
//
// #define _GNU_SOURCE
// void *mempcpy(void *restrict dest, const void *restrict src, size_t n);
// is nearly identical to the memcpy(3) function.
// It copies n bytes from the object beginning at src into the object pointed to by dest
// Return: dest + n

void Set()
{
    printf("\n--set--\n");
    // memset()
    char buf[20];
    buf[19] = 0;
    memset(buf, 'a', 19);
    printf("memset buf=%s\n", buf);

    // bzero()
    bzero(buf, 20);
    printf("bzero buf=%s\n", buf);
}

void Compare()
{
    printf("\n--compare--\n");
    size_t size = 20;
    char buf[size];
    strcpy(buf, "string");
    char buf2[size];
    strcpy(buf2, "strink");
    char buf3[size];
    strcpy(buf3, "strina");
    char buf4[size];
    strcpy(buf4, "string");
    printf("memcmp() compare buf=%s with %s =%d\n", buf, buf2, memcmp(buf, buf2, strlen(buf)));
    printf("memcmp() compare buf=%s with %s =%d\n", buf, buf3, memcmp(buf, buf3, strlen(buf)));
    printf("memcmp() compare buf=%s with %s =%d\n", buf, buf4, memcmp(buf, buf, strlen(buf)));
    printf("bcmp() compare buf=%s with %s =%d\n", buf, buf2, bcmp(buf, buf2, strlen(buf)));
    printf("bcmp() compare buf=%s with %s =%d\n", buf, buf3, bcmp(buf, buf3, strlen(buf)));
    printf("bcmp() compare buf=%s with %s =%d\n", buf, buf4, bcmp(buf, buf, strlen(buf)));
}

void Copy()
{
    printf("\n--copy--\n");
    size_t size = 20;
    char buf[size];
    strcpy(buf, "string");
    char buf2[size];
    // memmove()
    memmove(buf2, buf, strlen(buf));
    printf("memmove() copy buf=%s to %s\n", buf, buf2);
    // bcopy()
    bzero(buf2, size);
    bcopy(buf, buf2, strlen(buf)); // first src, next dest !
    printf("bcopy() copy buf=%s to %s\n", buf, buf2);
    // memcpy() - mast not overlap
    bzero(buf2, size);
    memcpy(buf2, buf, strlen(buf));
    printf("memcpy() copy buf=%s to %s\n", buf, buf2);
    // memccpy() - mast not overlap
    bzero(buf2, size);
    char *ptr = memccpy(buf2, buf, 'i', strlen(buf));
    printf("memccpy() copy buf=%s to %s res ptr=%p dest ptr=%p\n", buf, buf2, ptr, buf2);
    // mempcpy() - points to next byte of last copied (mast not overlap)
    bzero(buf2, size);
    ptr = mempcpy(buf2, buf, strlen(buf));
    printf("mempcpy() copy buf=%s to %s res ptr=%p dest ptr=%p\n", buf, buf2, ptr, buf2);
}

int main()
{
    Set();
    Compare();
    Copy();
    exit(EXIT_SUCCESS);
}