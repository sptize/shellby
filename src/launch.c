/*
 * launch.c - Ciclo de lectura, evaluacion, impresion del shell.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "shell.h"
#include <unistd.h>
#define MIN_BUFFER_SIZE 64
#define TOKEN_DELIMITADOR " \t\r\n\a" // TODO: eliminar /a

// Adiciona una cadena de caracteres a la linea de entrada, redimensionando el buffer si es necesario
static int agregar_entrada(char **linea, size_t *buffer, size_t *tamano, const char *src, size_t n)
{
  if (*tamano + n + 1 > *buffer)
  {
    char *temp_linea = realloc(*linea, *tamano + n + 1);
    if (!temp_linea)
      return -1;
    *linea = temp_linea;
    *buffer = *tamano + n + 1;
  }
  memcpy(*linea + *tamano, src, n);
  *tamano += n;
  (*linea)[*tamano] = '\0';
  return 0;
}
// 0 = linea lista, 1 = fin de archivo (EOF-salir), -1 = error, 2 = continuar
// Lee la entrada de stdin y la almacena en linea, redimensionando el buffer si es necesario
static int leer_entrada(char **linea, size_t *buffer)
{
  char *trozo = NULL;
  size_t trozo_cap = 0;
  size_t trozo_tamano = 0;

  while (1)
  {
    errno = 0;
    ssize_t bytes_leidos = getline(&trozo, &trozo_cap, stdin);
    int err = errno;
    
    if (bytes_leidos > 0)
    {
      if (agregar_entrada(linea, buffer, &trozo_tamano, trozo, bytes_leidos) == -1)
      {
        fprintf(stderr, "%s: sin memoria\n", SHELL_NAME);
        free(trozo);
        return -1;
      }
      if (trozo[bytes_leidos - 1] == '\n' || !isatty(STDIN_FILENO))
      {
        free(trozo);
        return 0;
      }
      // Ctrl+D con texto pendiente: no salir
      clearerr(stdin);
      continue;
    }

    if (feof(stdin))
    {
      if (trozo_tamano > 0)
      {
        clearerr(stdin);
        continue;
      }
      free(trozo);
      // Ctrl+D sin texto pendiente: salir
      if (putchar('\n') == EOF || fflush(stdout) == EOF)
      {
        fprintf(stderr, "%s: no se pudo completar la escritura\n", SHELL_NAME);
      }
      return 1;
    }
    
    free(trozo);
    if (err == EINTR)
    {
      clearerr(stdin);
      return 2;
    }

    return -1;
  }
}

static int segundo_plano(char *linea)
{
  size_t n = strlen(linea);

  // quitar espacios y salto de linea del final
  while (n > 0 && (linea[n - 1] == ' ' || linea[n - 1] == '\t' || linea[n - 1] == '\n' || linea[n - 1] == '\r'))
  {
    n--;
  }
  linea[n] = '\0';

  if (n > 0 && linea[n - 1] == '&')
  {
    linea[n - 1] = '\0'; // borra '&'
    return 1;
  }
  return 0;
}
static char **tokenizar(char *linea, int *argc)
{
  int buffer = MIN_BUFFER_SIZE;
  int index = 0;
  char **tokens = malloc(buffer * sizeof(char *));
  char *token, *guardarptr;

  if (!tokens)
  {
    fprintf(stderr, "%s: error de volcado\n", SHELL_NAME);
    exit(EXIT_FAILURE);
  }

  token = strtok_r(linea, TOKEN_DELIMITADOR, &guardarptr);
  while (token != NULL)
  {
    tokens[index++] = token;

    if (index >= buffer)
    {
      buffer += MIN_BUFFER_SIZE;
      tokens = realloc(tokens, buffer * sizeof(char *));
      if (!tokens)
      {
        fprintf(stderr, "%s: sin memoria\n", SHELL_NAME);
        exit(EXIT_FAILURE);
      }
    }
    token = strtok_r(NULL, TOKEN_DELIMITADOR, &guardarptr);
  }

  tokens[index] = NULL;
  *argc = index;
  return tokens;
}

// TODO: evaluar si valor de retorno void
static void eval_args(Shell *sh, int argc, char **args, int bg)
{

  if (argc == 0)
  {
    // Se introdujo una linea en blanco
    return;
  }

  const Builtin *b = buscar_builtin(args[0]);
  if (b)
  {
    b->ejecutar(sh, argc, args);
    return;
  }
  comando_externo(args, bg);
}

void shell_repl(Shell *sh)
{
  char *linea = NULL;
  char **args = NULL;
  size_t buffer = 0;
  int argc = 0;

  while (sh->shell_running)
  {
    print_prompt();
    // 1. Leemos los datos de stdin
    int resultado = leer_entrada(&linea, &buffer);
    if (resultado == 1)
    {
      break; // Salir del bucle si se llegó al final del archivo
    }
    else if (resultado == -1)
    {
      fprintf(stderr, "%s: error al leer la entrada\n", SHELL_NAME);
      break;
    }
    else if (resultado == 2)
    {
      continue; // Ignorar y volver a pedir entrada
    }
    int bg = segundo_plano(linea); // enviar o no a segundo plano el comando
    // 2. Limpiamos y dividimos los datos recibidos para generar los argumentos del comando
    args = tokenizar(linea, &argc);
    // 3. Evaluamos el tipo de comando (interno o externo) para delegar su ejecucion a la funcion correspondiente
    eval_args(sh, argc, args, bg); // TODO: esta es la funcion que devuelve el ultimo estado
    free(args);
    args = NULL;
  }
  free(linea);
}
