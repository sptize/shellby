# shellby

**Estructua de carpetas**
shellby/
  docs/
  src/
  tests/
  Makefile
  README.md

**Observaciones**
Cuando se recibe la entrada por consola con la senial Ctrl + D, el tty entrega lo escrito hasta el momento y esos caracteres ya salieron de su buffer, lo cual permite la feature de poder seguir escribiendo un comando incluso despues de Ctrl+D pero limita la posibilidad de que Backspace borre lo mostrado en pantalla. 
