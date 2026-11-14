/*
* main.c - Punto de entrada del shell
*/
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"


int main(void)
{ 
  
  print_banner();
  shell_repl();
  return EXIT_SUCCESS;
}