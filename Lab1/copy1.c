#include <io.h>       //open(), eof(), read(), write(), close()
#include <conio.h>    //getch()
#include <stdio.h>    //printf()
#include <stdlib.h>   //exit()
#include <fcntl.h>    //O_BINARY, O_RDONLY, O_WRONLY, O_CREAT, O_EXCL, O_TRUNC
#include <systypes.h> //
#include <sysstat.h>  //S_IREAD, S_IWRITE
#include <alloc.h>    //
#include <errno.h>    //EEXIST
#define BUFSIZE 10000 //Макрос, що визначає розміру масиву

void main(int argc, char **argv)
{
    int source, target;
    int i;
    char *buffer;
    int count;

    if (argc != 3)
    {
        printf("n"
               "Usage: COPY1 [d:][\path]source_file [d:][\path]target_filen");
        getch();
        exit(1);
    }
    if ((source = open(argv[1], O_BINARY | O_RDONLY)) == -1)
    {
        printf("nOpen source file error: %d", errno);
        getch();
        exit(2);
    }
    target = open(argv[2], O_BINARY | O_WRONLY | O_CREAT | O_EXCL, S_IREAD | S_IWRITE);

    if (errno == EEXIST)
    {
        printf("nFile already exists. Overwrite? (Y/N)n");
        i = getch();
        if ((i == 'y') || (i == 'Y'))
            target = open(argv[2], O_BINARY | O_WRONLY | O_CREAT | O_TRUNC, S_IREAD | S_IWRITE);
    }

    if (target == -1)
    {
        printf("nOpen target file error: %d", errno);
        getch();
        exit(2);
    }

    count = BUFSIZE;
    if ((buffer = (char *)malloc(count)) == NULL)
    {
        printf("nNot enough memory");
        getch();
        exit(3);
    }
    while (!eof(source))
    {
        if ((count = read(source, buffer, count)) == -1)
        {
            printf("nRead file error: %d", errno);
            getch();
            exit(4);
        }
        if ((count = write(target, buffer, count)) == -1)
        {
            printf("nWrite file error: %d", errno);
            getch();
            exit(5);
        }
    }
    close(source);
    close(target);
    free(buffer);
    printf("File copy...");
    getch();
}