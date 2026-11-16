#include "shell.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_BUILTINS (sizeof tabla / sizeof tabla[0])

static int cmd_cd(Shell *sh, char **args);
static int cmd_pwd(Shell *sh, char **args);
static int cmd_help(Shell *sh, char **args);
static int cmd_exit(Shell *sh, char **args);

static const Builtin tabla[] = {
    {"cd", cmd_cd, "Cambia de directorio de trabajo."},
    {"pwd", cmd_pwd, "Muestra el directorio de trabajo actual."},
    {"help", cmd_help, "Muestra la lista de comandos internos."},
    {"exit", cmd_exit, "Salir de Shellby."},
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

static int cmd_cd(Shell *sh, char **args)
{
    (void)sh;
    (void)args;
    //TODO: implementar
    return EXIT_SUCCESS;
}
static int cmd_pwd(Shell *sh, char **args)
{
    (void)sh;
    (void)args;
    //TODO: implementar
    return EXIT_SUCCESS;
}

static int cmd_help(Shell *sh, char **args)
{
    (void)sh;
    (void)args;
    //manejar argumentos invalidos

    if (NUM_BUILTINS == 0)
    {
        fprintf(stderr, "%s: no hay comandos internos registrados\n", SHELL_NAME);
        return EXIT_SUCCESS;
    }

    if (printf("Comandos internos de la shell:\n") < 0)
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

static int cmd_exit(Shell *sh, char **args)
{
    (void)args;
    //manejar argumentos invalidos
    sh->shell_running = false;
    return EXIT_SUCCESS;
}
