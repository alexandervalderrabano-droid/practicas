#include <stdio.h>

// ========================================
// swap: Intercambia los valores de dos enteros
// Par�metros: punteros a dos enteros (a, b)
// Retorno: ninguno
// ========================================
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// ========================================
// longitudCadena: Calcula la longitud de una cadena
// Par�metros: puntero a char (cadena)
// Retorno: entero con la longitud (sin usar strlen)
// ========================================
int longitudCadena(char *cadena) {
    int len = 0;
    while (*cadena != '\0') {
        len++;
        cadena++;
    }
    return len;
}

// ========================================
// invertirArreglo: Invierte el orden de un arreglo de enteros
// Par�metros: puntero a int (arr) y tama�o del arreglo (size)
// Retorno: ninguno
// ========================================
void invertirArreglo(int *arr, int size) {
    int *inicio = arr;
    int *fin = arr + size - 1;
    while (inicio < fin) {
        int temp = *inicio;
        *inicio = *fin;
        *fin = temp;
        inicio++;
        fin--;
    }
}

// ========================================
// cuentaPares: Cuenta cu�ntos n�meros pares hay en un arreglo
// Par�metros: puntero a int (arr) y tama�o del arreglo (size)
// Retorno: entero con la cantidad de n�meros pares
// ========================================
int cuentaPares(int *arr, int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (*(arr + i) % 2 == 0) {
            count++;
        }
    }
    return count;
}
