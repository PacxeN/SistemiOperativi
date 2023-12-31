#include "header.h"

#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>

int main(){

    int k, status, queue;
    pid_t pid;
    int i;
    char m[30];
    // assegnazione coda di comunicazione 
    queue = msgget(IPC_PRIVATE, IPC_CREAT|0664);

    // generazione produttore e consumatore
    pid=fork();
    if(pid==0){
        //processo figlio (produttore)
        printf("Sono il produttore. Il mio pid %d\n", getpid());
        for(i=0; i<10;i++){
            sprintf(m,"stringa %d", i);
            usleep(100);
            Produttore(queue,m);
        }
        exit(0);
    }else{
        // genera il secondo figlio (consumatore)
        pid=fork();
        if(pid==0){
            printf("Sono il figlio consumatore. Il mio pid %d\n",getpid());
            sleep(1);
            for(i=0; i<10;i++){
                Consumatore(queue);
            }
            exit(0);
        }
    }

    // attesa di terminazione 
    for(k=0; k<2; k++){
        pid = wait(&status);
        if(pid==-1){
            perror("Errore");
        }else{
            printf("Figlio n.ro %d e' morto con status = %d\n",pid,status>>8);
        }
    }

    //deallocazione code
    msgctl(queue,IPC_RMID,0);
    return 1;

}