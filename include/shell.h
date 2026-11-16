/*
 * shell.h - Declaraciones comunes del shell
 */
#ifndef SHELL_H
#define SHELL_H
#define SHELL_NAME "shellby"
#include <stdbool.h>


typedef struct
{
    bool shell_running;
    // int ultimo estado => $?
    // TODO: tabla de jobs con cantidad
} Shell;
void shell_init(Shell *sh);

typedef int (*builtin_fn)(Shell *sh, char **args);
typedef struct
{
    const char *nombre;
    builtin_fn ejecutar;
    const char *ayuda;
} Builtin;

const Builtin *buscar_builtin(const char *nombre);

/* builtins.c */

/* launch.c */
void shell_repl(Shell *sh);
/* configuracion.c */
void print_banner(void);
void print_prompt(void);
/* exec.c */
void comando_externo(char **args, int bg);

#endif /* SHELL_H */