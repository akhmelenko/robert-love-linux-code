#include <stdio.h>

// !!! "int getc(FILE *pFile)" can read from FILE and stdin !!!

void ReadCharsFromFile()
{
    char ch = '\0';
    printf("ch=%d\n\n", ch);

    FILE *pFile;

    if (pFile = fopen("test.txt", "rw"))
    {
        while ((ch = getc(pFile)) != EOF) // getc() returns an int, then convert int to char
            printf("%c", ch);
    }
    printf("\n");
    fclose(pFile);
}

void ReadCharFromKeyboard()
{
    char ch = '\0';
    ch = getc(stdin); // getc() returns an int, then convert int to char
    printf("character=%c\n", ch);
}

int main()
{
    ReadCharsFromFile();
    ReadCharFromKeyboard();
    return 0;
};
