#include <stdio.h>

// ==============================
// Definición de la estructura
// ==============================
typedef struct {
    char nombre[50];
    int matricula;
    double calificaciones[4];
    double promedio;
} Estudiante;

// ==============================
// Declaración de funciones
// ==============================
void capturarEstudiante(Estudiante *est);
void calcularPromedio(Estudiante *est);
void mostrarEstudiante(Estudiante est);

// ==============================
// Programa principal
// ==============================
int main() {
	int i;
    Estudiante grupo[3];

    printf("=== SISTEMA DE GESTION ACADEMICA ===\n");

    // Capturar datos de 3 estudiantes
    for (i = 0; i < 3; i++) {
        printf("\nEstudiante %d:\n", i + 1);
        capturarEstudiante(&grupo[i]);
    }

    // Mostrar todos los estudiantes
    printf("\n=== LISTA COMPLETA DE ESTUDIANTES ===\n");
    for (i = 0; i < 3; i++) {
        mostrarEstudiante(grupo[i]);
    }

    // Encontrar el mejor promedio
    int mejorIndex = 0;
    for (i = 0; i < 3; i++) {
        if (grupo[i].promedio > grupo[mejorIndex].promedio) {
            mejorIndex = i;
        }
    }

    printf("\n=== ESTUDIANTE CON MEJOR PROMEDIO ===\n");
    printf("Nombre: %s\n", grupo[mejorIndex].nombre);
    printf("Promedio: %.2f\n", grupo[mejorIndex].promedio);

    return 0;
}

// ==============================
// Función 1: Capturar datos del estudiante
// ==============================
void capturarEstudiante(Estudiante *est) {
	int i;
    printf("Ingrese nombre: ");
    scanf(" %[^\n]", est->nombre);

    printf("Ingrese matricula: ");
    scanf("%d", &est->matricula);

    for (i = 0; i < 3; i++) {
        printf("Ingrese calificacion %d: ", i + 1);
        scanf("%lf", &est->calificaciones[i]);
    }

    calcularPromedio(est);
}

// ==============================
// Función 2: Calcular promedio
// ==============================
void calcularPromedio(Estudiante *est) {
	int i;
    double suma = 0;
    for (i = 0; i < 3; i++) {
        suma += est->calificaciones[i];
    }
    est->promedio = suma / 3;
}

// ==============================
// Función 3: Mostrar información del estudiante
// ==============================
void mostrarEstudiante(Estudiante est) {
	int i;
    printf("\n--- Informacion del Estudiante ---\n");
    printf("Nombre: %s\n", est.nombre);
    printf("Matricula: %d\n", est.matricula);
    for (i = 0; i < 3; i++) {
        printf("Calificacion %d: %.2f\n", i + 1, est.calificaciones[i]);
    }
    printf("Promedio: %.2f\n", est.promedio);
}

