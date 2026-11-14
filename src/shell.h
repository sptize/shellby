/*
* shell.h - Declaraciones comunes del shell
*/
#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>
#include <sys/types.h>


#define SHELL_NAME "shellby"
#define shell_running 1
/* launch.c */
void shell_repl(void);
/* configuracion.c */
void print_banner(void);
void print_prompt(void);
/* exec.c */
int run_external(char **argv, const char *cmdline);

#endif /* SHELL_H */