#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void filecpy(FILE *stream_from, FILE *stream_to);
char buf1[BUFSIZ * 10];
char buf2[BUFSIZ * 10];

void main(int argc, char *argv[])
{
    time_t start, end;
    FILE *stream_from, *stream_to;
    if (argc < 3)
    {
        printf("nUsage:"
               " COPY2 [d:][\path]source_file [d:][\path]target_filen");
        exit(1);
    }
    if ((stream_from = fopen(argv[1], "rt")) == NULL)
    {
        printf("nOpen source file error: %d", errno);
        exit(1);
    }
    stream_to = fopen(argv[2], "wt+");
    start = clock();
    filecpy(stream_from, stream_to);
    end = clock();
    printf("Copying time is %5.1f. Buffer size is %d bytesn", ((float)end - start) / CLK_TCK, BUFSIZ);

    if ((stream_from = fopen(argv[1], "rt")) == NULL)
        exit(1);
    stream_to = fopen(argv[2], "wt+");
    setvbuf(stream_from, buf1, _IOFBF, sizeof(buf1));
    setvbuf(stream_to, buf2, _IOFBF, sizeof(buf2));
    start = clock();
    filecpy(stream_from, stream_to);
    end = clock();
    printf("Copying time is %5.1f. Buffer size is %d bytesn", ((float)end - start) / CLK_TCK, BUFSIZ * 10);
    if ((stream_from = fopen(argv[1], "rt")) == NULL)
        exit(1);
    stream_to = fopen(argv[2], "wt+");
    setvbuf(stream_from, NULL, _IONBF, 0);
    setvbuf(stream_to, NULL, _IONBF, 0);
    start = clock();
    filecpy(stream_from, stream_to);
    end = clock();
    printf("Copying time is %5.1f. Buffers is not usedn",
           ((float)end - start) / CLK_TCK);
    exit(0);
}
void filecpy(FILE *stream_from, FILE *stream_to)
{
    char linebuf[256];
    while (!feof(stream_from))
    {
        if (fgets(linebuf, 255, stream_from) == NULL)
            break;
        if (fputs(linebuf, stream_to) == EOF)
            break;
    }
    fclose(stream_from);
    fclose(stream_to);
}
