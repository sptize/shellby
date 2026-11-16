/*
 * configuracion.c - Elementos auxiliares del shell. Estilo y arranque.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "shell.h"

void print_banner(void){
 errno = 0;
 if(puts(
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
        ) == EOF){
            fprintf(stderr, "%s: error al escribir el banner: %s\n",
                SHELL_NAME, strerror(errno));
        
        } else {
             fflush(stdout);
        }       
}

void print_prompt(void) {
    char *cwd = getcwd(NULL, 0);
    if (cwd != NULL) {
        printf("%s:%s$ ", SHELL_NAME, cwd);
        free(cwd);  
    } else {
        printf("%s:?$ ", SHELL_NAME);
        fprintf(stderr, "%s: no se pudo obtener el directorio actual: %s\n", SHELL_NAME, strerror(errno));
    }
    fflush(stdout);   
}

void shell_init(Shell *sh){
    sh-> shell_running = true;
    //ultimo estado
    //total de jobs inicial = 0
}