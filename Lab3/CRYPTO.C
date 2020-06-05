#include <string.h>

#include <io.h>

#include <conio.h>

#include <stdio.h>

#include <stdlib.h>

#include <fcntl.h>

#include <systypes.h>

#include <sysstat.h>

#include <alloc.h>

#include <errno.h>

#define BUFSIZE 10000

char *buffer;

//структура для збереження заголовку

struct Header

{

    char signat[3]; //сiгнатуру виду файлу;

    int version; //номер версії програми;

    char CopyRight[30]; //рядок Copyright

    char filename[13]; // імя вихідного файлу

    unsigned long CRC; //контрольна сума

} header;

unsigned long Cryptographer(int count, char *pass, unsigned long conf, char *argv);

/***************************************************************************/

void main(int argc, char **argv)

{

    int source, target;

    int i;

    int count;

    unsigned long CRCM = 0;

    clrscr();

    //перевірка кількості аргументів командного рядка

    if (argc != 5)

    {

        printf("nUsage:"

               "tMYENCODEn"

               "t[d:][\path]\oriuginal_filen"

               "t[d:][\path]\target_filen"

               "tpasswordn"

               "tidenteficator( e or d )n");

        getch();

        exit(1);
    }

    //відкриваємо вихідний файл

    source = open(argv[1], O_BINARY | O_RDONLY);

    //обробка помилки відкриття файлу

    if (source == -1)

    {

        printf("nOpen source file error: %d", errno);

        getch();

        exit(2);
    }

    //створюємо новий файл куди зпишим код

    target = open(argv[2],

                  O_BINARY | O_WRONLY | O_CREAT | O_EXCL,

                  S_IREAD | S_IWRITE);

    //перевіряємо на наявність одноіменного файлу

    //та запитуємо користувача про необхідність його заміни новим

    if (errno == EEXIST)

    {

        printf("nFile already exists. Overwrite? (Y/N)n");

        i = getch();

        if ((i == 'y') || (i == 'Y'))

            target = open(argv[2],

                          O_BINARY | O_WRONLY | O_CREAT | O_TRUNC,

                          S_IREAD | S_IWRITE);
    }

    //обробка помилки створеннянового файлу

    if (target == -1)

    {

        printf("nOpen target file error: %d", errno);

        getch();

        exit(2);
    }

    //якщо потрібно декодувати файл,

    //то зчитуємо із закодованого файлу заголовок

    if (strcmp(argv[4], "/d") == 0)

        read(source, &header, sizeof(struct Header));

    //якщо потрібно кодувати файл,

    //то записуємо заголовок до файлу з кодом

    if (strcmp(argv[4], "/e") == 0)

    {

        strcpy(header.signat, "AY");

        header.version = 1;

        strcpy(header.CopyRight, "Uncode");

        header.CRC = 0;

        strcpy(header.filename, argv[1]);

        write(target, &header, sizeof(struct Header));
    }

    //виділяємо память для символьного масиву

    count = BUFSIZE;

    if ((buffer = (char *)malloc(count)) == NULL)

    {

        printf("nNot enough memory");

        getch();

        exit(3);
    }

    while (!eof(source))

    {

        //зчитуємо блок символів з вихідного файлу

        count = read(source, buffer, count)

            if (count == -1)

        {

            printf("nRead file error: %d", errno);

            getch();

            exit(4);
        }

        //кодуємо зчитані символи

        CRCM += Cryptographer(count, argv[3], CRCM, argv[4]);

        //записуємо блок закодованих символів до вхідного файлу

        count = write(target, buffer, count)

            if (count == -1)

        {

            printf("nWrite file error: %d", errno);

            getch();

            exit(5);
        }
    }

    //якщо потрібно кодувати файл,

    //то перезаписуємо заголовок файлу з кодом виправляючи контрольну суму

    if (strcmp(argv[4], "/e") == 0)

    {

        header.CRC = CRCM;

        lseek(target, 0L, 0);

        write(target, &header, sizeof(struct Header));
    }

    //якщо потрібно декодувати файл,

    if (strcmp(argv[4], "/d") == 0)

        //то порівнюємо контрольну суму

        if (header.CRC != CRCM)

            printf("Access denied! Password error! Please try again");

    //закрити файли та звільнити память

    close(source);

    close(target);

    free(buffer);

    getch();
}

/***************************************************************************/

/* Функція приймає:

розмір тексту

пароль

поточну конторльну суму

ключ

Функція повертає:

конторльну суму

*/

unsigned long Cryptographer(int zise, char *password, unsigned long conf, char *key)

{

    int Len = strlen(password);

    int askikod;

    int cur = 0;

    for (int a = 0; a < zise; a++)

    {

        if (cur > Len)

            cur = 0;

        //якщо варто декодувати

        if (strcmp(key, "/d") == 0)

        {

            buffer[a] ^= password[cur];

            askikod = buffer[a];

            conf += askikod;
        }

        //якщо варто закодувати

        if (strcmp(key, "/e") == 0)

        {

            askikod = buffer[a];

            conf += askikod;

            buffer[a] ^= password[cur];
        }

        cur++;
    }

    return (conf);
}

/***************************************************************************/