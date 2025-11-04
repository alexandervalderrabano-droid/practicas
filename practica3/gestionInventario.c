#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===============================
// Variables globales
// ===============================
char **nombresProductos = NULL; // Puntero doble para nombres
int *cantidades = NULL; // Puntero simple para cantidades
double *precios = NULL; // Puntero simple para precios
int numProductos = 0; // Total de productos en inventario

// ===============================
// Función para agregar productos
// ===============================
int agregarProducto(char *nombre, int cantidad, double precio) {
    // Reasignar memoria para el nuevo producto
    char **tmpNombres = realloc(nombresProductos, (numProductos + 1) * sizeof(char *));
    int *tmpCantidades = realloc(cantidades, (numProductos + 1) * sizeof(int));
    double *tmpPrecios = realloc(precios, (numProductos + 1) * sizeof(double));

    // Verificar si la reasignación fue exitosa
    if (tmpNombres == NULL || tmpCantidades == NULL || tmpPrecios == NULL) {
        printf("Error: no se pudo asignar memoria.\n");
        return 0;
    }

    // Si todo bien, actualizamos los punteros globales
    nombresProductos = tmpNombres;
    cantidades = tmpCantidades;
    precios = tmpPrecios;

    // Asignar memoria para el nombre y copiarlo
    nombresProductos[numProductos] = malloc(strlen(nombre) + 1);
    strcpy(nombresProductos[numProductos], nombre);

    // Guardar cantidad y precio
    cantidades[numProductos] = cantidad;
    precios[numProductos] = precio;

    numProductos++;
    return 1;
}

// ===============================
// Función para mostrar inventario
// ===============================
void mostrarProductos() {
	int i;
    printf("=== Productos %d ===\n", numProductos);
    for (i = 0; i < numProductos; i++) {
        double valor = cantidades[i] * precios[i];
        printf("Pos %d: %s - Cant: %d - Precio: $%.2f - Valor: $%.2f\n",
               i, nombresProductos[i], cantidades[i], precios[i], valor);
    }
}

// ===============================
// Calcular valor total del inventario
// ===============================
double calcularValorTotalInventario() {
	int i;
    double total = 0;
    for (i = 0; i < numProductos; i++) {
        total += cantidades[i] * precios[i];
    }
    return total;
}

// ===============================
// Liberar memoria dinámica
// ===============================
void liberarInventario() {
	int i;
    for (i = 0; i < numProductos; i++) {
        free(nombresProductos[i]); // Liberar cada nombre
    }
    free(nombresProductos);
    free(cantidades);
    free(precios);
    nombresProductos = NULL;
    cantidades = NULL;
    precios = NULL;
    numProductos = 0;
}

// ===============================
// Cargar productos por defecto
// ===============================
void cargarProductosIniciales() {
    agregarProducto("Laptop HP", 5, 1200.00);
    agregarProducto("Mouse Inalambrico", 15, 25.50);
    agregarProducto("Teclado Mecanico", 8, 75.00);
    agregarProducto("Camisa Casual", 20, 35.00);
    agregarProducto("Jeans", 12, 45.00);
    agregarProducto("Zapatos Deportivos", 10, 60.00);
    agregarProducto("Arroz", 50, 2.50);
    agregarProducto("Frijol", 40, 3.00);
    agregarProducto("Aceite", 30, 4.50);
}

// ===============================
// Menú principal
// ===============================
int main() {
    int opcion;
    char nombre[50];
    int cantidad;
    double precio;

    cargarProductosIniciales(); // Carga inicial de productos

    do {
        printf("\n=== SISTEMA DE INVENTARIO ===\n");
        printf("1. Agregar producto\n");
        printf("2. Mostrar inventario\n");
        printf("3. Calcular valor total del inventario\n");
        printf("4. Salir\n");
        printf("Seleccione opcion: ");
        scanf("%d", &opcion);
        getchar(); // limpiar buffer

        switch (opcion) {
            case 1:
                printf("\n=== Agregar Producto ===\n");
                printf("Nombre: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0'; // quitar salto de línea
                printf("Cantidad: ");
                scanf("%d", &cantidad);
                printf("Precio: ");
                scanf("%lf", &precio);
                agregarProducto(nombre, cantidad, precio);
                break;

            case 2:
                mostrarProductos();
                break;

            case 3:
                printf("Valor total inventario: $%.2f\n", calcularValorTotalInventario());
                break;

            case 4:
                printf("Saliendo del sistema...\n");
                break;

            default:
                printf("Opcion invalida. Intente de nuevo.\n");
                break;
        }
    } while (opcion != 4);

    liberarInventario();
    return 0;
}

