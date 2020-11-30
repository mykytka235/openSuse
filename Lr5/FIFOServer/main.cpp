#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define MAXLINE 4096
#define N 3

int SidesSum(int arr[N][N])
{
    int sum = 0;
    for(int j = 0; j < N; j++)
    {
        sum += arr[0][j];
        sum += arr[N-1][j];
    }
    for(int i = 1; i < N-1; i++)
    {
        sum = sum + arr[i][0] + arr[i][N-1];
    }

    return sum;
}
void client(int readfd, int writefd)
{
    size_t len;
    char buff[MAXLINE];/* одержання повного імені файлу */
    fgets(buff, MAXLINE, stdin);
    len = strlen(buff);/* fgets() гарантує завершаючий нульовий байт */
    if(buff[len-1] == '\n')
        len--;

    write(writefd, buff, len);/* зчитування з каналу, виведення в stdout*/
}
void server(int readfd, int writefd)
{
    int fd;
    ssize_t n;
    char buff[MAXLINE+1];/* одержання повного імені з каналу IPC */
    if ((n = read(readfd, buff, MAXLINE)) == 0)
        printf("end-of-file while reading pathname");
    buff[n] = '\0';                    /* повне ім'я завершується 0 */
    if ( (fd = open(buff, O_RDONLY)) < 0)
    {/* повідомити клієнта про помилку */
        snprintf(buff + n, sizeof(buff) -n, ": can't open, %s\n",strerror(errno));
        n = strlen(buff);
        write(writefd, buff, n);
    }
    else
    {
        // task
        int array[N][N], sum = 0;
        FILE* fp = fopen(buff, "r");

        for (int i = 0; i < N; ++i)
          for (int j = 0; j < N; ++j)
            fscanf(fp, "%d", &array[i][j]);

        sum = SidesSum(array);
        printf("Sum: %d\n", sum);

        fclose(fp);

        fgets(buff, MAXLINE, stdin);
        fp = fopen(buff, "w");

        fprintf(fp ,"Sum = %d", sum);
        fclose(fp);

        write(writefd, buff, n);
        close(fd);

    }
}

int main(int argc, char **argv)
{
    int readfd, writefd;
    pid_t childpid;
    /* створення двох FIFO, якщо існують –ОК */
    if ((mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST))
        printf("can't create %s", FIFO1);

    if ((mkfifo(FIFO2, FILE_MODE) < 0) && (errno != EEXIST)) {
        unlink(FIFO1);
        printf("can't create %s", FIFO2);
    }
    if ((childpid = fork()) == 0)
    {
        /* нащадок */
        readfd = open(FIFO1, O_RDONLY, 0);
        writefd = open(FIFO2, O_WRONLY, 0);
        server(readfd, writefd);
        exit(0);
    }
    /* батьківський процес */
    writefd = open(FIFO1, O_WRONLY, 0);
    readfd = open(FIFO2, O_RDONLY, 0);
    client(readfd, writefd);
    waitpid(childpid, NULL, 0);   /* очікуванняназавершеннядочірньогопроцесу*/

    close(readfd);
    close(writefd);

    unlink(FIFO1);
    unlink(FIFO2);

    exit(0);
}
