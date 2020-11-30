#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 4096      /* максимальна довжина рядка */
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
int main(int argc, char**argv)
{
    size_t n;
    char buff[MAXLINE], command[MAXLINE];

    FILE *fp;/* зчитування повного імені */
    fgets(buff, MAXLINE, stdin);
    n = strlen(buff);        /* fgets() гарантує завершуючий нуль */

    if (buff[n-1] == '\n')
        n--;                /* видалення переведення рядка з повернення fgets() */
    snprintf(command, sizeof(command), "cat %s", buff);
    fp = popen(command, "r");/* копіювання з каналу в стандартне виведення */

    int array[N][N];
    int sum = 0;
    int i, j;

    for (i = 0; i < N; ++i)
      for (j = 0; j < N; ++j)
        fscanf(fp, "%d", &array[i][j]);

    sum = SidesSum(array);

    pclose(fp);
    fclose(fp);

    printf("Sum: %d\n", sum);

    exit(0);
}

