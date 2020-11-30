#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <iostream>

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

int main()
{
    pid_t child;
    int to_child_fd[2];
    int to_parent_fd[2];
    pipe(to_child_fd);
    pipe(to_parent_fd);

    if ((child = fork()) == 0) {
        int array[N][N];
        int sum;

        close(to_child_fd[1]);
        close(to_parent_fd[0]);
        //server
        read(to_child_fd[0], &array, sizeof(array));
        sum = SidesSum(array);
        printf("Sum: %d\n", sum);
        //write(to_parent_fd[1], &sum, sizeof(sum));
        close(to_parent_fd[1]);
    } else {
        int sum;
        int array[N][N] = {{1, 2, 3},
                           {4, 5, 6},
                           {7, 8, 9}};
        //client
        close(to_parent_fd[1]); /* parent closes write side of parent pipe */
        close(to_child_fd[0]);  /* parent closes read  side of child  pipe */
        write(to_child_fd[1], &array, sizeof(array));
        close(to_child_fd[1]);
        read(to_parent_fd[0], &sum, sizeof(sum));
    }
    return 0;
}
