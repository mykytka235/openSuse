#include<stdio.h>
#include<signal.h>
#include<sys/time.h>
#include <math.h>
#include<time.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<vector>

#define N 3

using namespace std;

struct process{
    pid_t pid;
    int num;
    string file1Path, file2Path;
    int times;
};
vector<process> processes;
int stat;
int i = 0;

process addProcess(int num)
{
    process curentProc;
    curentProc.num = num;
    curentProc.pid = fork();
    srand((unsigned) time(0));

    curentProc.times = 1 + rand() % 15;

    curentProc.file1Path = "/home/nekit/SPZ/Lr4/ForFiles/out" + to_string(num) + ".txt";
    curentProc.file2Path = "/home/nekit/SPZ/Lr4/ForFiles/in" + to_string(num) + ".txt";

    if (!curentProc.pid) {
        kill(getpid(),SIGSTOP);
        execl("/home/nekit/SPZ/Lr4/build-a-Desktop_Qt_5_15_1_GCC_64bit-Debug/a", curentProc.file1Path.c_str(), curentProc.file2Path.c_str(), NULL);
        exit(0);
    }
    if(curentProc.pid){
        sleep(1);
    }

    return curentProc;
}

void catch_signal(int signal)
{
    cout << "caught signal \n";
    if(processes.empty() == true)
        kill(getpid(), SIGTERM);
    if(processes[i].times == 0){
        kill(processes[i].pid, SIGCONT);
        waitpid(processes[i].pid, &stat, WCONTINUED | WSTOPPED);

        cout << "execute " <<processes[i].num
             << " pid: " <<processes[i].pid
             << " times: " <<processes[i].times
             << endl;
        vector<process>::iterator it;
        it = processes.begin() + i;
        processes.erase(it);

        if(processes.empty() == true){
            //kill(getpid(), SIGTERM);
            exit(0);
        }
        i = rand() % processes.size();
    }
    else
        processes[i].times--;
}

void start_timer()
{
    struct itimerval it;
    struct sigaction sa;

    if (!kill(processes[i].pid, SIGCONT))
    {
        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = catch_signal;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sigaction(SIGALRM, &sa, NULL);

        it.it_value.tv_sec = 0;
        it.it_value.tv_usec = 250000;
        it.it_interval.tv_sec = 0;
        it.it_interval.tv_usec = 250000;
        setitimer(ITIMER_REAL, &it, NULL);

        srand((unsigned) time(0));
        int counter = 0;

        while (1){
            counter++;
            if(counter % 4 == 0)
                i = rand() % processes.size();
            pause();
        }
    }
}

int main()
{
    for(i = 0; i < N; i++)
        processes.push_back(addProcess(i));

    cout << "Parent pid:" << getpid() << endl;

    for(i = 0; i < N; i++)
        cout << i << ". child pid: " << processes[i].pid << " times: " << processes[i].times << endl;

    srand((unsigned) time(0));

    i = rand() % processes.size();

    start_timer();
    sleep(1);

    return 0;
}
