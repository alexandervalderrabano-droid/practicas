#include <stdio.h>
#include <stdlib.h>

// Prototipos de funciones
void ingresarCalificaciones(float **calificaciones, int *total);
float calcularPromedio(float *calificaciones, int total);
void mostrarResultado(float promedio);
void liberarMemoria(float *calificaciones);

int main() {
    int opcion;
    float *calificaciones = NULL;
    int total_calificaciones = 0;

    do {
        printf("\n=== Calculadora de Promedio Dinamica ===\n");
        printf("1) Ingresar calificaciones\n");
        printf("2) Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                ingresarCalificaciones(&calificaciones, &total_calificaciones);
                break;
            case 2:
                printf("\n ¡Memoria liberada! Hasta luego.\n");
                break;
            default:
                printf("Opción invalida. Intente nuevamente.\n");
        }

    } while (opcion != 2);

    liberarMemoria(calificaciones);
    return 0;
}

// --- Funciones ---

void ingresarCalificaciones(float **calificaciones, int *total) {
	int i;
    int n;
    printf("\n Cuantas calificaciones desea ingresar? ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Debe ingresar al menos una calificación.\n");
        return;
    }

    // Reservar o expandir memoria con realloc
    float *temp = realloc(*calificaciones, (*total + n) * sizeof(float));
    if (temp == NULL) {
        printf("Error: no se pudo asignar memoria.\n");
        return;
    }
    *calificaciones = temp;

    // Capturar nuevas calificaciones
    for (i = 0; i < n; i++) {
        float nota;
        do {
            printf("Ingrese la calificacion %d: ", *total + i + 1);
            scanf("%f", &nota);
            if (nota < 0 || nota > 10)
                printf("Calificación invalida. Debe estar entre 0 y 10.\n");
        } while (nota < 0 || nota > 10);

        (*calificaciones)[*total + i] = nota;
    }

    *total += n;

    // Calcular y mostrar promedio
    float promedio = calcularPromedio(*calificaciones, *total);
    mostrarResultado(promedio);

    // Preguntar si desea agregar más calificaciones
    char respuesta;
    do {
        printf("\n Desea agregar mas calificaciones? (s/n): ");
        scanf(" %c", &respuesta);
        if (respuesta == 's' || respuesta == 'S') {
            ingresarCalificaciones(calificaciones, total);
            return;
        } else if (respuesta == 'n' || respuesta == 'N') {
            return;
        } else {
            printf("Respuesta inválida. Intente nuevamente.\n");
        }
    } while (1);
}

float calcularPromedio(float *calificaciones, int total) {
	int i;
    float suma = 0;
    for (i = 0; i < total; i++) {
        suma += calificaciones[i];
    }
    return suma / total;
}

void mostrarResultado(float promedio) {
    printf("\nPromedio: %.2f\n", promedio);
    if (promedio >= 7.0)
        printf("Estado: APROBADO\n");
    else
        printf("Estado: REPROBADO\n");
}

void liberarMemoria(float *calificaciones) {
    free(calificaciones);
}
