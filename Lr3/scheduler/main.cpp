#include<stdio.h>
#include<signal.h>
#include<sys/time.h>
#include<math.h>
#include<time.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>

using namespace std;

struct cherg
{
    int pi;
    int type;
    int num;//number in order in the list
    char fileName[20];
    cherg *next,*prev;
};
int times;

void addqueu(cherg **head, int pid,int type,int num)
{
    cherg* cur=(*head);
    cherg* tmp=(cherg*)malloc(sizeof(cherg));
    if ((*head) != NULL)
    {
        while(cur->next  !=  NULL)//шукаємо  останій елемент списку
            cur = cur->next;

        tmp->pi=pid;
        tmp->type=type;
        tmp->num=num;
        cur->next = tmp;
        tmp->prev=cur;
        tmp->next = NULL;
    }
    else
    {
        (*head) = tmp;
        (*head)->next = NULL;
        (*head)->prev = NULL;
        (*head)->pi=pid;
        (*head)->type=type;
        (*head)->num=num;
    }
}
void addProcess(cherg **head,int num,int type)
{
    if (type==1)
    {
        pid_t pid = fork();
        if (pid == 0)
        {//child//
            kill(getpid(),SIGSTOP);//створення  процесу  спля-чим
            execl("/home/nekit/SPZ/Lr3/ExecFiles/a1","a1", NULL);
            exit(0);//вихідз дочірнього процесу
        }
        if(pid > 0)
        {//процес-батько
            sleep(1);
            addqueu(head,pid,type,num);//батьку    вертається pid, його і решту інфи заносим в //чергу
        }
    }
    if (type==2)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            kill(getpid(),SIGSTOP);//create sleeping process
            execl("/home/nekit/SPZ/Lr3/ExecFiles/a2","a2", NULL);//тут використано file(а не fileName), передається в параметри функції
            exit(0);
        }
        if(pid > 0){//parent
            sleep(1);
            addqueu(head,pid,type,num);
        }
    }
    if (type==3)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            kill(getpid(),SIGSTOP);//create sleeping process
            execl("/home/nekit/SPZ/Lr3/ExecFiles/a3", "a3", NULL);
            exit(0);
        }
        if(pid > 0)
        {//parent
            sleep(1);
            addqueu(head,pid,type,num);
        }
    }
}
void deleteNode(cherg **head, cherg * del)
{
    if(*head == NULL || del == NULL)
        return;

    if (*head == del)
        *head = del->next;

    if(del->next != NULL)
        del->next->prev = del->prev;

    if(del->prev != NULL)
        del->prev->next = del->next;

    free(del);
}
int  select_proc(cherg **head, int *type, int *num)
{
    int pi;//розбір отриманого елементу на складові

    pi=(*head)->pi;
    *type=(*head)->type;
    *num=(*head)->num;

    if((*head)->next == NULL)//only one element
    {
        delete (*head);//delete him
        (*head)=NULL;
    }
    else
    {
        (*head)=(*head)->next;//head element is next element
        delete (*head)->prev;//delete previous head
        (*head)->prev=NULL;
    }
    return pi;
}

int  select_3proc(cherg **head, int *type, int *num)
{
    int pi;//розбір отриманого елементу на складові

        int task_number = ((*head)->num+1);

        if (task_number % 3 == 0)
        {
            cherg *tmp = *head;
            pi=(*head)->pi;
            *type=(*head)->type;
            *num=(*head)->num;

            deleteNode(head, tmp);

            return pi;
        }
        else
            (*head) = (*head)->next;
    return -1;
}

int main()
{
    int start;
    cherg *head=NULL;
    int type, num;//спочатку всі першого типу
    int pi;


    cout << "How many tasks? " << endl << "Input value: ";
    cin >> times;

    srand(time(NULL));//штампуємо  3  процеси  першого  типу  (максимальний  розмір  черги 3)
    //Варіант A
    for(int i=0;i<times;i++)
    {
        //покинекінецьчерги
        addProcess(&head, i, 1);
        addProcess(&head, i, 2);
        addProcess(&head, i, 3);
    }

    cherg* tmp = head;
    while(tmp != NULL)
    {
        pi=select_3proc(&tmp, &type, &num);//отрималипід, файл, тип, номервидаленого//елементузчерги
        if(pi == -1)
            continue;
        kill(pi,SIGCONT);//йомутребапродовжитисвоєви-конання
        waitpid(pi, &start, WCONTINUED| WSTOPPED);//чекаємопокинащадокзавершиться

        cout<<"execute "<< num << " " << pi << " " << type << endl;

        sleep(1);
    }
    delete tmp;

    while(head != NULL)
    {
        pi=select_proc(&head, &type, &num);//отрималипід, файл, тип, номервидаленого//елементузчерги
        kill(pi,SIGCONT);//йомутребапродовжитисвоєви-конання
        waitpid(pi, &start, WCONTINUED| WSTOPPED);//чекаємопокинащадокзавершиться

        cout<<"execute "<< num << " " << pi << " " << type << endl;

        sleep(1);
    }

    return 0;
}
