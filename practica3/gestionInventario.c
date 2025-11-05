#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==============================
// Variables globales
// ==============================
char **nombresProductos = NULL;
int *cantidades = NULL;
double *precios = NULL;
int numProductos = 0;

// ==============================
// Prototipos de funciones
// ==============================
int agregarProducto(char *nombre, int cantidad, double precio);
void mostrarProductos();
double calcularValorTotalInventario();
void liberarInventario();
void cargarProductosIniciales();


// ==============================
// Función principal 
// ==============================
int main() {
    int opcion;
    char nombre[100];
    int cantidad;
    double precio;
    
    cargarProductosIniciales();

    do {
        printf("\n=== GESTION DE INVENTARIO ===\n");
        printf("1. Agregar producto\n");
        printf("2. Mostrar productos\n");
        printf("3. Calcular valor total del inventario\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar(); // limpiar buffer

        switch (opcion) {
            case 1:
            	 printf("\n=== Agregar Producto ===\n");
                printf("Nombre: ");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';
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
                printf("Valor total del inventario: $%.2f\n", calcularValorTotalInventario());
                break;

            case 4:
                liberarInventario();
                printf("Saliendo del sistema...\n");
                break;

            default:
                printf("Opción inválida. Intente de nuevo.\n");
                break;
        }
    } while (opcion != 4);
    
    liberarInventario();
    return 0;
}

// ==============================
// Implementación de funciones
// ==============================

// Agregar producto
int agregarProducto(char *nombre, int cantidad, double precio) {
    numProductos++;

    nombresProductos = (char **)realloc(nombresProductos, numProductos * sizeof(char *));
    cantidades = (int *)realloc(cantidades, numProductos * sizeof(int));
    precios = (double *)realloc(precios, numProductos * sizeof(double));

    if (nombresProductos == NULL || cantidades == NULL || precios == NULL) {
        printf("Error al asignar memoria.\n");
        exit(1);
    }

    *(nombresProductos + numProductos - 1) = (char *)malloc((strlen(nombre) + 1) * sizeof(char));
    strcpy(*(nombresProductos + numProductos - 1), nombre);

    *(cantidades + numProductos - 1) = cantidad;
    *(precios + numProductos - 1) = precio;

    return 1;
}

// Mostrar productos
void mostrarProductos() {
    if (numProductos == 0) {
        printf("No hay productos en el inventario.\n");
        return;
    }

    printf("\n=== Productos %d ===\n",numProductos);
    int i;
    for (i = 0; i < numProductos; i++) {
    	  double valor = * (cantidades + i) * * (precios + i);
        printf("Pos %d: %s - Cant: %d - Precio: $%.2f - Valor: $%.2f\n",
               i, *(nombresProductos + i),
               *(cantidades + i),
               *(precios + i),
               valor);
    }
}

// Calcular valor total del inventario
double calcularValorTotalInventario() {
	int i;
    double total = 0;
    for (i = 0; i < numProductos; i++) {
        total += (*(cantidades + i)) * (*(precios + i));
    }
    return total;
}

// Liberar memoria
void liberarInventario() {
	int i;
    for (i = 0; i < numProductos; i++) {
        free(*(nombresProductos + i));
    }
    free(nombresProductos);
    free(cantidades);
    free(precios);
    nombresProductos = NULL;
    cantidades = NULL;
    precios = NULL;
    numProductos = 0;
}

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

