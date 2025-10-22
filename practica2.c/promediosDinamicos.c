#include <stdio.h>
#include <stdlib.h>

void main(){
	int i;
    printf("=== Calculadora de Promedio Dinamica ===\n\n");
    int opcion = 0;
    int n = 0; // disponible fuera de case 1
    float *calificaciones = NULL; // puntero visible en todo main
    int memoria_reservada = 0; // bandera: 1 si hay memoria por liberar

    while(opcion != 2){
        printf("1) Ingresar calificaciones\n");
        printf("2) Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%i",&opcion);

        switch(opcion){
            case 1:
                {
                    float *ptr; // apuntador auxiliar para recorrer
                    float suma = 0, promedio = 0;

                    printf("\n¿Cuantas calificaciones desea ingresar?: ");
                    scanf("%d", &n);

                    // si ya había memoria reservada, liberarla antes de reservar nueva
                    if(calificaciones != NULL){
                        free(calificaciones);
                        calificaciones = NULL;
                        memoria_reservada = 0;
                    }

                    calificaciones = (float*) malloc(n * sizeof(float));

                    if(calificaciones == NULL){
                        printf("Error al asignar memoria.\n");
                        break;
                    }

                    ptr = calificaciones; // apuntador al inicio del bloque reservado

                    for(i = 0; i < n; i++){
                        printf("Ingrese la calificacion #%d: ", i + 1);
                        scanf("%f", ptr); // leemos directamente en la dirección apuntada
                        suma += *ptr; // sumamos el valor apuntado
                        ptr++; // avanzamos al siguiente float
                    }

                    promedio = suma / n;
                    printf("\nEl promedio es: %.2f\n\n", promedio);

                    memoria_reservada = 1; // ahora hay memoria que podrá liberarse en case 2
                }
                break;

            case 2:
                //se libera la memoria reservada (si existe)
                if(memoria_reservada && calificaciones != NULL){
                    free(calificaciones);
                    calificaciones = NULL;
                    memoria_reservada = 0;
                    printf("\nMemoria liberada correctamente.\n");
                }

                //mensaje de despedida
                printf("\nMemoria liberada!!! Hasta luego.\n");
                break;

            default:
                //opcion del usuario invalida
                printf("\n=== Opcion invalida === \nPor favor seleccione una opcion del menu:\n");
        }
    }
}

