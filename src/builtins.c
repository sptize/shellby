/*
 * builtins.c - Comandos internos del shell.
 */

#include "shell.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#define NUM_BUILTINS (sizeof tabla / sizeof tabla[0])

static int cmd_cd(Shell *sh, int argc, char **argv);
static int cmd_pwd(Shell *sh, int argc, char **argv);
static int cmd_help(Shell *sh, int argc, char **argv);
static int cmd_exit(Shell *sh, int argc, char **argv);
static bool sobran_args(const char *cmd, int argc);

static const Builtin tabla[] = {
    {"cd", cmd_cd, "Cambia de directorio de trabajo"},
    {"pwd", cmd_pwd, "Muestra el directorio de trabajo actual"},
    {"help", cmd_help, "Muestra la lista de comandos internos"},
    {"exit", cmd_exit, "Salir de Shellby"},
};

const Builtin *buscar_builtin(const char *nombre)
{
    for (size_t i = 0; i < NUM_BUILTINS; i++)
    {
        if (strcmp(nombre, tabla[i].nombre) == 0)
        {
            return &tabla[i];
        }
    }
    return NULL;
}

static int cmd_cd(Shell *sh, int argc, char **argv)
{
    (void)sh;

    if (argc > 2)
    {
        fprintf(stderr, "cd: demasiados argumentos\n");
        return EXIT_FAILURE;
    }
    const char *dir;

    if (argc == 1)
    {
        dir = getenv("HOME");
        if (!dir || !*dir)
        {
            fprintf(stderr, "cd: la variable HOME no está definida\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        dir = argv[1];
    }
    if (chdir(dir) != 0)
    {
        fprintf(stderr, "cd: %s: %s\n", dir, strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
static int cmd_pwd(Shell *sh, int argc, char **argv)
{
    (void)sh;
    (void)argv;
    char cwd[PATH_MAX];

    if (sobran_args("pwd", argc))
        return EXIT_FAILURE;

    if (!getcwd(cwd, sizeof cwd))
    {
        fprintf(stderr, "pwd: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    if (fprintf(stdout, "%s\n", cwd) < 0)
    {
        perror("pwd");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

static int cmd_help(Shell *sh, int argc, char **argv)
{
    (void)sh;
    (void)argv;

    if (sobran_args("help", argc))
        return EXIT_FAILURE;

    if (printf("Comandos internos del shell:\n") < 0)
    {
        perror("help");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < NUM_BUILTINS; i++)
    {
        const char *desc = tabla[i].ayuda ? tabla[i].ayuda : "(sin descripción)";
        if (printf("  %-8s %s\n", tabla[i].nombre, desc) < 0)
        {
            perror("help");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

static int cmd_exit(Shell *sh, int argc, char **argv)
{
    (void)argv;
    if (sobran_args("exit", argc))
        return EXIT_FAILURE;
    sh->shell_running = false;
    return EXIT_SUCCESS;
}

static bool sobran_args(const char *cmd, int argc)
{
    if (argc > 1)
    {
        fprintf(stderr, "%s: no acepta argumentos\n", cmd);
        return true;
    }
    return false;
}
