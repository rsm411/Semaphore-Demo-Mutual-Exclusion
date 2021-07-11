//FOR COMPILING USE gcc code5a.c -o code5a -lpthread
//FOR EXECUTING USE ./code5a
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<errno.h>
#include<unistd.h>
#include <sys/wait.h>
#include<fcntl.h>
#include<stdlib.h>
int main()
{
    int i,j,shmid;
    int *p;
    sem_t *sem;
    pid_t pid;
    shmid = shmget(IPC_PRIVATE,sizeof(int),0644| IPC_CREAT);
    p = (int *) shmat (shmid, NULL, 0);  
   sem = sem_open ("sem", O_CREAT | O_EXCL, 0644,1);
    printf("SEMAPHORES INITIALISED\n");
    for (i = 0; i < 2; i++)
    {
        pid=fork();
        if (pid < 0) 
        {
            sem_unlink ("pSem");   
            sem_close(sem);  
            printf ("Fork error.\n");
        }
        else if (pid == 0)
            break;                 
    }
    if (pid != 0)
    {
      *p = 1000;
      printf("THIS IS THE PARENT PROCESS:\n");
      printf ("Initial Value of Shared variable p allocated by parent process : %d\n",*p);
        while (pid = waitpid (-1, NULL, 0))
        {
            if (errno == ECHILD)
                break;
        }
        printf ("BACK TO PARENT PROCESS\nAFTER EXECUTION OF BOTH CHILD PROCESSES VALUE OF SHARED VARIABLE p IS : %d\n",*p);
        shmdt (p);
        shmctl (shmid, IPC_RMID, 0);
        sem_unlink ("sem");   
        sem_close(sem);  
        exit (0);
    }
    else
    {
        sem_wait (sem);        
            if(i==0)
            {  
               printf("INSIDE CHILD 1:\n");
                for(j=0;j<5;j++)
                {
                *p += 200;
                printf ("Value of p after %d iterations =%d\n", j+1, *p);
                
                }
                printf("VALUE OF p AFTER CHILD PROCESS 1: %d\n",*p);
            }
            else if(i==1)
            {
                printf("INSIDE CHILD 2:\n");
                for(j=0;j<5;j++)
                {
                *p += 100;
                printf ("Value of p after %d iterations =%d\n", j+1, *p);
                }
                printf("VALUE OF p AFTER CHILD PROCESS 2: %d\n",*p);
            }
            sem_post (sem);
                exit (0);    
       }
}
