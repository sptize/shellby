/*
* exec.c - Ejecucion de programas externos con fork + exec
* 
*/
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "shell.h"



void comando_externo(char **args){
    fflush(NULL); //flush de todos los buffers abiertos

    pid_t pid;
    int wstatus;
    pid_t ret;

    pid = fork(); //fork devuelve dos veces una por el padre y otra por el hijo
    
    if(pid < 0){
        fprintf(stderr, "%s: %s\n", SHELL_NAME, strerror(errno));      
        return;
    }
    if (pid == 0){
        //proceso hijo
        signal(SIGINT, SIG_DFL);
        if(execvp(args[0], args) == -1){
            //llega aqui solo si falla la ejecucion         
            if(errno == ENOENT){
               fprintf(stderr, "%s: comando no encontrado\n", args[0]);
                _exit(127); //salida con error
            }
            fprintf(stderr, "%s: %s\n", args[0], strerror(errno));
            _exit(126);           
        }
    }
    do{
      ret = waitpid(pid, &wstatus, 0);
    } while (ret == -1 && errno == EINTR);

    if (ret == -1){
      perror("waitpid");
    }
    else if (WIFEXITED(wstatus)){}
    else if (WIFSIGNALED(wstatus)){
      fprintf(stderr, "terminado por la señal %d\n", WTERMSIG(wstatus));
    }
   
    

}