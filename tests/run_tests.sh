#!/bin/sh
# Pruebas de caja negra para shellby.
# Se ejecuta desde la raíz del proyecto: make test  (o: sh tests/run_tests.sh)

SHELLBY=./shellby
NOMBRE=shellby            # debe coincidir con SHELL_NAME en el código
DIR=$(pwd -P)             # directorio donde se ejecuta la shell
fallos=0

pasa()  { echo "PASS: $1"; }
falla() { echo "FAIL: $1"; fallos=$((fallos + 1)); }

# Ejecuta la shell con la entrada recibida; guarda stdout y stderr en $salida.
ejecutar() {
    salida=$(printf "$1" | timeout 5 "$SHELLBY" 2>&1)
}

# Cuenta cuántas veces aparece "nombre:directorio" en $salida.
contar_prompts() {
    printf '%s' "$salida" | grep -F -o "$NOMBRE:$DIR" | wc -l
}

# --- Prueba 1: el prompt muestra el nombre de la shell y el directorio actual
ejecutar ''
if [ "$(contar_prompts)" -ge 1 ]; then
    pasa "el prompt muestra nombre y directorio"
else
    falla "el prompt no muestra '$NOMBRE:$DIR'"
fi

# --- Prueba 2: una línea vacía vuelve a mostrar el prompt
# Entrada: dos líneas vacías y fin de archivo.
# Prompts esperados: 1 inicial + 1 tras cada línea vacía = 3.
ejecutar '\n\n'
n=$(contar_prompts)
if [ "$n" -eq 3 ]; then
    pasa "una línea vacía vuelve a mostrar el prompt"
else
    falla "se esperaban 3 prompts y hubo $n"
fi

# --- Resumen
echo
if [ "$fallos" -eq 0 ]; then
    echo "Todas las pruebas pasaron."
else
    echo "$fallos prueba(s) fallaron."
    exit 1
fi
