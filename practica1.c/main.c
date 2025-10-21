#include <stdio.h>

// Prototipos de las funciones
void swap(int *a, int *b);
int longitudCadena(char *cadena);
void invertirArreglo(int *arr, int size);
int cuentaPares(int *arr, int size);

int main() {
    // ----- Prueba 1: swap -----
    int a = 5, b = 10;
    printf("Antes del swap: a=%d, b=%d\n", a, b);
    swap(&a, &b);
    printf("Después del swap: a=%d, b=%d\n\n", a, b);

    // ----- Prueba 2: longitudCadena -----
    char texto[] = "Hola Mundo";
    printf("Cadena: %s\n", texto);
    printf("Longitud: %d\n\n", longitudCadena(texto));

    // ----- Prueba 3: invertirArreglo -----
    int arr[] = {1, 2, 3, 4, 5};
    int size = 5;
    printf("Arreglo original: ");
    for (int i = 0; i < size; i++) printf("%d ", arr[i]);
    printf("\n");
    invertirArreglo(arr, size);
    printf("Arreglo invertido: ");
    for (int i = 0; i < size; i++) printf("%d ", arr[i]);
    printf("\n\n");

    // ----- Prueba 4: cuentaPares -----
    int arr2[] = {1, 2, 3, 4, 5, 6};
    int size2 = 6;
    printf("Cantidad de pares en el arreglo: %d\n", cuentaPares(arr2, size2));

    return 0;
}
