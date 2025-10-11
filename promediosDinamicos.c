#include <stdio.h>
#include <stdlib.h>

void main(){
	int i;
    printf("=== Calculadora de Promedio Dinamica ===\n\n");
    int opcion = 0;

    while(opcion != 2){
        printf("1) Ingresar calificaciones\n");
        printf("2) Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%i",&opcion);

        switch(opcion){
            case 1:
                {
                    int n;
                    float *calificaciones;
                    float suma = 0, promedio = 0;

                    printf("\n Cuantas calificaciones desea ingresar?: ");
                    scanf("%d", &n);

                    calificaciones = (float*) malloc(n * sizeof(float));

                    if(calificaciones == NULL){
                        printf("Error al asignar memoria.\n");
                        break;
                    }

                    for(i = 0; i < n; i++){
                        printf("Ingrese la calificacion #%d: ", i+1);
                        scanf("%f", &calificaciones[i]);
                        suma += calificaciones[i];
                    }

                    promedio = suma / n;
                    printf("\nEl promedio es: %.2f\n\n", promedio);

                    free(calificaciones);
                }
                break;

            case 2:
                //se libera la memoria reservada

                //mensaje de despedida
                printf("\nMemoria liberada!!! Hasta luego.\n");
                break;

            default:
                //opcion del usuario invalida
                printf("\n=== Opcion invalida === \nPor favor seleccione una opcion del menu:\n");
        }
    }
}
