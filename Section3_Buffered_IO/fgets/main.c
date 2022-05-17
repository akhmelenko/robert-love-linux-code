#include <stdio.h>
#include <errno.h>
#include <string.h>

//// File access
// mode | Meaning       | Explanation                  | Action if file already exists   | Action if file  does not exist
// "r"	| read	        | Open a file for reading      | read from start                 | failure to open
// "w"	| write	        | Create a file for writing	   | destroy contents	             | create new
// "a"	| append        | Append to a file	           | write to end	                 | create new
// "r+"	| read extended	| Open a file for read/write   | read from start	             | error
// "w+"	| write extended| Create a file for read/write | destroy contents	             | create new
// "a+"	|append extended| Open a file for read/write   | write to end#pragma region      | create new

// !!! "char* fgets(char * buf, int maxNumToRead, FILE * pFile)" read line(with '\n' and without EOF)
// from stdin/file to the buffer (maxNumToRead is including '\0') !!!
// also deprecated because function not check is '\0' exists in out buffer or not => use if only if sure that data not contain '\0'

void ReadFirstLineFromFile(FILE *pFile)
{
    char buf[255];

    if (!fgets(buf, sizeof(buf), pFile))
    {
        printf("fgets() error %s", strerror(errno));
        return;
    }
    // terminate the end of line
    char *ch = strchr(buf, '\n'); // get searching character pointer or NULL
    if (ch)
    { // terminate
        *ch = '\0';
        printf("Found new-line symbol\n");
    }
    printf("%s b[5]=%d\n", buf, (int)buf[5]);
}

int main()
{
    FILE *pFile = fopen("test.txt", "r");
    if (!pFile)
        printf("Error file open!\n");
    ReadFirstLineFromFile(pFile);
    printf("\nEnter the line\n");
    ReadFirstLineFromFile(stdin);
}