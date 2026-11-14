/*
* main.c - Punto de entrada del shell
*/

#include <stdlib.h>
#include "shell.h"


int main(void)
{   
  print_banner();
  shell_repl();
  return EXIT_SUCCESS;
}