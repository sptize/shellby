/*
 * main.c - Punto de entrada del shell
 */

#include <stdlib.h>
#include "shell.h"

int main(void)
{
  Shell sh;
  shell_init(&sh);

  print_banner();
  shell_repl(&sh);
  // TODO: aqui va el manejo del ultimo codigo de estado
  // TODO: aqui va la funcion que limpia los jobs antes de cerrar la shell
  return EXIT_SUCCESS;
}