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
#include "shell.h"


int run_external(char **argv, const char *cmdline)
{
    sigset_t chld, old;
    const char *display_cmd = cmdline ? cmdline : (argv && argv[0] ? argv[0] : "");

    if (!argv || !argv[0]) {
        fprintf(stderr, "%s: no se recibió un comando para ejecutar\n", SHELL_NAME);
        return 2;
    }

    if (sigemptyset(&chld) == -1 || sigaddset(&chld, SIGCHLD) == -1) {
        int error = errno;
        fprintf(stderr, "%s: no se pudo preparar el bloqueo de SIGCHLD: %s\n",
                SHELL_NAME, strerror(error));
        return 1;
    }
    if (sigprocmask(SIG_BLOCK, &chld, &old) == -1) {
        int error = errno;
        fprintf(stderr, "%s: no se pudo bloquear SIGCHLD: %s\n",
                SHELL_NAME, strerror(error));
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        int error = errno;
        if (sigprocmask(SIG_SETMASK, &old, NULL) == -1) {
            int restore_error = errno;
            fprintf(stderr, "%s: no se pudo restaurar la máscara de señales: %s\n",
                    SHELL_NAME, strerror(restore_error));
        }
        fprintf(stderr, "%s: no se pudo iniciar «%s»: %s\n",
                SHELL_NAME, display_cmd, strerror(error));
        return 1;
    }

    if (pid == 0) {
        if (sigprocmask(SIG_SETMASK, &old, NULL) == -1) {
            int error = errno;
            fprintf(stderr, "%s: no se pudo restaurar la máscara de señales: %s\n",
                    SHELL_NAME, strerror(error));
            _exit(126);
        }
        execvp(argv[0], argv);
        int error = errno;
        fprintf(stderr, "%s: no se pudo ejecutar «%s»: %s\n",
                SHELL_NAME, display_cmd, strerror(error));
        _exit(error == ENOENT ? 127 : 126);
    }

    int status;
    pid_t waited;
    do {
        waited = waitpid(pid, &status, 0);
    } while (waited == -1 && errno == EINTR);

    int wait_error = errno;
    if (sigprocmask(SIG_SETMASK, &old, NULL) == -1) {
        int error = errno;
        fprintf(stderr, "%s: no se pudo restaurar la máscara de señales: %s\n",
                SHELL_NAME, strerror(error));
        return 1;
    }
    if (waited == -1) {
        fprintf(stderr, "%s: error esperando a «%s»: %s\n",
                SHELL_NAME, display_cmd, strerror(wait_error));
        return 1;
    }
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status);
    }
    return 1;
}