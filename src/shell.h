/*
* shell.h - Declaraciones comunes del shell
*/
#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>

#define SHELL_NAME "shellby"

/* exec.c */
int run_external(char **argv, const char *cmdline);

#endif /* SHELL_H */