/*
* main.c - Bucle principal: prompt, lectura de entrada, análisis y ejecución de comandos internos o programa externo.
*/
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

#define MAX_ARGS 256

static int print_prompt(void)
{
    char cwd[PATH_MAX]; //utilizamos la constante del sistema "PATH_MAX" para definir el tamaño máximo del buffer que contendrá el directorio actual

    if (!getcwd(cwd, sizeof(cwd))) {
        int error = errno;
        fprintf(stderr, "%s: no se pudo obtener el directorio actual:   %s\n",
                SHELL_NAME, strerror(error));
        if (snprintf(cwd, sizeof(cwd), "?") < 0) {
            fprintf(stderr, "%s: no se pudo preparar el prompt\n", SHELL_NAME);
            return -1;
        }
    }
// TODO: eliminar salto de linea una vez que se implemente la lectura de comandos en el bucle principal
    if (printf("%s:%s> ", SHELL_NAME, cwd) < 0) {
        fprintf(stderr, "%s: no se pudo escribir el prompt\n", SHELL_NAME);
        return -1;
    }
    if (fflush(stdout) == EOF) {
        fprintf(stderr, "%s: no se pudo completar la escritura del prompt\n",
                SHELL_NAME);
        return -1;
    }
    return 0;
}

static int print_banner(void)
{
    if (fputs(
       
        "\n"
        "\n"
        "               0000000000000000dv\n"
        "              1660666666WW00000000000000000000d0dv\n"
        "               000000000006000001NNNNW6060611000000d00000d0dd00dd0v\n"
        "       W66006600000000006006060061  6600060606v  00006NRRW66R1d0d0d0d0d0dddddd0dd0iv\n"
        "       00000000000000000000000000   60660006W    600061   N6R    000000060601Wdd0dddd\n"
        "      r000000000000000000000000WN  W6000000W v    0000i v  0    60000000000000000000W\n"
        "      0066060001Rm100000000000000000000006  16Nv  R006 66  m1  v0000000000000000000\n"
        "     v0106RNR0R    0006W  R00006RR00000000000000000000000r0R6  1000000000000000000000600m\n"
        "     RNR000616r    WRi      00W   600000000006WW00000000000000000000000000000000000000000\n"
        "     00000000W               v     NNR1060Wr     m0000000001R 10000000000000000000000000i\n"
        "    600000000N   W06        W                      IRR60006    000Rv  00000R  W0000000006\n"
        "    0000000000  00000d   1000N   006660666                     dW       R0W   v000000000v\n"
        "   i00000000000000000000000000  00000060060     6600006606    v               600000000d\n"
        "   0000000000000RNIINW100000000000000000000000000000000000   6006      60m    0000000d06\n"
        "   dWWW600000001  r60 006   imrWIIWNR66000000000000000000000000000  60000W   0000000000m\n"
        "         000000W0    6001  001  6001  06  00  vR0006RW600000000000000000000000000000000\n"
        "       00000000 m66   006  66   000   N 0000   00000W  000000 v6INR0010000000000000000d\n"
        "       06600666661di000I   WR   006  660N000  100000v  000000  v6   00   00  v000000006\n"
        "       6060666066666666666666666W1i      0R1  16W 06   00R006  v   0000   rr000000000m\n"
        "               v0W11666666666666666666666666606W006    0  0Wv  0W   000R  000000\n"
        "                              v66666106666000600006600000000001imm0000I   10dd000\n"
        "                                             r66661606600000000000000000000061id0\n"
        "                                                            d1666100000000000000d\n"
        "                                                                          m166666\n"
        "\n"
      
        
       ,
        stdout) == EOF) {
        fprintf(stderr, "%s: no se pudo escribir el banner\n", SHELL_NAME);
        return -1;
    }
    if (fflush(stdout) == EOF) {
        fprintf(stderr, "%s: no se pudo completar la escritura del banner\n",
                SHELL_NAME);
        return -1;
    }
    return 0;
}

static int tokeniza(char *linea, char **argv, int max_args)
{
    int argc = 0;
    char *p = linea;

    while (*p) {
        while (*p == ' ' || *p == '\t')
            p++;
        if (!*p)
            break;
        if (argc >= max_args - 1)
            return -1;

        char *word = p;
        char *w = p;
        char quote = 0;
        argv[argc++] = word;

        while (*p) {
            if (quote) {
                if (*p == quote) {
                    quote = 0;
                    p++;
                } else {
                    *w++ = *p++;
                }
            } else if (*p == '\'' || *p == '"') {
                quote = *p++;
            } else if (*p == ' ' || *p == '\t') {
                p++;
                break;
            } else {
                *w++ = *p++;
            }
        }
        if (quote)
            return -1;

        *w = '\0';
    }
    argv[argc] = NULL;
    return argc;
}

static void process_line(char *linea)
{
    char *argv[MAX_ARGS];
    size_t len = strlen(linea);

    /* quitar espacios y saltos de linea finales */
    while (len > 0 && (linea[len - 1] == '\n' || linea[len - 1] == '\r' || linea[len - 1] == ' ' || linea[len - 1] == '\t'))
      linea[--len] = '\0';


    char *cmdline = strdup(linea);
    if (!cmdline) {
        fprintf(stderr, "%s: no hay memoria para procesar la línea\n", SHELL_NAME);
        return;
    }

    int argc = tokeniza(linea, argv, MAX_ARGS);
    if (argc < 0) {
        fprintf(stderr, "%s: error al analizar la línea de comandos\n", SHELL_NAME);
        free(cmdline);
        return;
    }
    if (argc == 0) {
        free(cmdline);
        return;
    }
    run_external(argv, cmdline);
    free(cmdline);
}

int main(void)
{
    if (print_banner() != 0)
        return EXIT_FAILURE;

    char *linea = NULL;
    size_t capacidad = 0;
    int resultado = EXIT_SUCCESS;

    for (;;) {
        if (print_prompt() != 0) {
            resultado = EXIT_FAILURE;
            break;
        }

        if (getline(&linea, &capacidad, stdin) == -1) {
            if (ferror(stdin)) {
                int error = errno;
                fprintf(stderr, "%s: error al leer la entrada: %s\n",
                        SHELL_NAME, strerror(error));
                resultado = EXIT_FAILURE;
            } else if (putchar('\n') == EOF || fflush(stdout) == EOF) {
                fprintf(stderr, "%s: no se pudo completar la escritura\n", SHELL_NAME);
                resultado = EXIT_FAILURE;
            }
            break;
        }

        process_line(linea);
    }

    free(linea);
    return resultado;
}