#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------------
   ESTRUCTURAS
   ------------------------- */
typedef struct {
    int id;
    char titulo[100];
    char autor[50];
    char isbn[20];
    int anio;
    char categoria[30];
    int disponible; /* 1 = disponible, 0 = prestado */
} Libro;

typedef struct {
    int id;
    char nombre[50];
    char carrera[50];
    int matricula;
    int activo;
} Usuario;

typedef struct {
    int id;
    int id_libro;
    int id_usuario;
    char fecha_prestamo[12]; /* "DD/MM/YYYY" */
    char fecha_devolucion[12];
    int devuelto; /* 0 = activo, 1 = devuelto */
    char fecha_devolucion_real[12];
} Prestamo;

/* -------------------------
   VARIABLES GLOBALES (punteros dobles)
   ------------------------- */
Libro **libros = NULL;
Usuario **usuarios = NULL;
Prestamo **prestamos = NULL;

int totalLibros = 0;
int totalUsuarios = 0;
int totalPrestamos = 0;

/* -------------------------
   UTILIDADES
   ------------------------- */
void limpiarEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int generarId(int *ultimo_id) {
    (*ultimo_id)++;
    return *ultimo_id;
}

void quitarNuevaLinea(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

/* Valida formato DD/MM/YYYY y rangos básicos (sin time.h) */
int validarFecha(const char *fecha) {
    if (!fecha) return 0;
    int d, m, y;
    if (sscanf(fecha, "%2d/%2d/%4d", &d, &m, &y) != 3) return 0;
    if (y < 1) return 0;
    if (m < 1 || m > 12) return 0;
    int diasMes;
    switch (m) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            diasMes = 31; break;
        case 4: case 6: case 9: case 11:
            diasMes = 30; break;
        case 2:
            diasMes = ( (y%400==0) || (y%4==0 && y%100!=0) ) ? 29 : 28;
            break;
        default:
            return 0;
    }
    if (d < 1 || d > diasMes) return 0;
    return 1;
}

/* Suma días (ej. 15) a una fecha DD/MM/YYYY sin usar time.h */
void calcularFechaDevolucion(const char *fecha_in, int dias_sumar, char *fecha_out) {
    int d, m, y;
    if (sscanf(fecha_in, "%2d/%2d/%4d", &d, &m, &y) != 3) {
        strcpy(fecha_out, "00/00/0000");
        return;
    }
    d += dias_sumar;
    /* ajustar mes/año */
    while (1) {
        int diasMes;
        switch (m) {
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                diasMes = 31; break;
            case 4: case 6: case 9: case 11:
                diasMes = 30; break;
            case 2:
                diasMes = ( (y%400==0) || (y%4==0 && y%100!=0) ) ? 29 : 28;
                break;
            default:
                diasMes = 31;
        }
        if (d <= diasMes) break;
        d -= diasMes;
        m++;
        if (m > 12) { m = 1; y++; }
    }
    sprintf(fecha_out, "%02d/%02d/%04d", d, m, y);
}

/* -------------------------
   GESTION MEMORIA
   ------------------------- */
void liberarMemoriaCompleta(Libro ***pl, int *totLib, Usuario ***pu, int *totUsr, Prestamo ***pp, int *totPres) {
    if (pl && *pl) {
    	int i;
        for (i = 0; i < *totLib; ++i) free((*pl)[i]);
        free(*pl);
        *pl = NULL;
        *totLib = 0;
    }
    if (pu && *pu) {
    	int i;
        for (i = 0; i < *totUsr; ++i) free((*pu)[i]);
        free(*pu);
        *pu = NULL;
        *totUsr = 0;
    }
    if (pp && *pp) {
    	int i;
        for (i = 0; i < *totPres; ++i) free((*pp)[i]);
        free(*pp);
        *pp = NULL;
        *totPres = 0;
    }
}

/* -------------------------
   FUNCIONES PARA LIBROS
   ------------------------- */
Libro **redimensionarLibros(Libro **array, int nuevo_tam) {
    Libro **tmp = realloc(array, sizeof(Libro*) * nuevo_tam);
    if (!tmp && nuevo_tam > 0) {
        fprintf(stderr, "Error realloc libros\n");
        return array;
    }
    return tmp;
}

/* Agregar libro (se redimensiona el array) */
Libro **agregarLibro(Libro **libArr, int *totLib) {
    Libro *n = malloc(sizeof(Libro));
    if (!n) { fprintf(stderr,"malloc libro\n"); return libArr; }

    printf("Titulo: ");
    if (!fgets(n->titulo, sizeof(n->titulo), stdin)) n->titulo[0] = '\0';
    quitarNuevaLinea(n->titulo);

    printf("Autor: ");
    if (!fgets(n->autor, sizeof(n->autor), stdin)) n->autor[0] = '\0';
    quitarNuevaLinea(n->autor);

    printf("ISBN: ");
    if (!fgets(n->isbn, sizeof(n->isbn), stdin)) n->isbn[0] = '\0';
    quitarNuevaLinea(n->isbn);

    printf("Anio: ");
    if (scanf("%d", &n->anio) != 1) n->anio = 0;
    limpiarEntrada();

    printf("Categoria: ");
    if (!fgets(n->categoria, sizeof(n->categoria), stdin)) n->categoria[0] = '\0';
    quitarNuevaLinea(n->categoria);

    /* validar isbn duplicado */
    int i;
    for (i = 0; i < *totLib; ++i) {
        if (strcmp(libArr[i]->isbn, n->isbn) == 0) {
            printf("ISBN duplicado. Cancela.\n");
            free(n);
            return libArr;
        }
    }
    n->disponible = 1;

    Libro **tmp = redimensionarLibros(libArr, (*totLib) + 1);
    if (!tmp) { free(n); return libArr; }
    tmp[*totLib] = n;
    *totLib += 1;
    printf("Libro agregado con ID %d\n", n->id);
    return tmp;
}

Libro *buscarLibroPorTitulo(Libro **libArr, int totLib, const char *titulo_busq) {
    if (!titulo_busq) return NULL;
    int i;
    for (i = 0; i < totLib; ++i) {
        if (strstr(libArr[i]->titulo, titulo_busq) != NULL) return libArr[i];
    }
    return NULL;
}

Libro *buscarLibroPorId(Libro **libArr, int totLib, int id) {
	int i;
    for (i = 0; i < totLib; ++i) if (libArr[i]->id == id) return libArr[i];
    return NULL;
}

void mostrarLibros(Libro **libArr, int totLib) {
    if (totLib == 0) { printf("No hay libros.\n"); return; }
    printf("ID | Titulo | Autor | ISBN | Anio | Cat | Disp\n");
    int i;
    for (i = 0; i < totLib; ++i) {
        printf("%d | %s | %s | %s | %d | %s | %d\n",
               libArr[i]->id, libArr[i]->titulo, libArr[i]->autor,
               libArr[i]->isbn, libArr[i]->anio, libArr[i]->categoria, libArr[i]->disponible);
    }
}

/* -------------------------
   FUNCIONES PARA USUARIOS
   ------------------------- */
Usuario **redimensionarUsuarios(Usuario **array, int nuevo_tam) {
    Usuario **tmp = realloc(array, sizeof(Usuario*) * nuevo_tam);
    if (!tmp && nuevo_tam > 0) { fprintf(stderr,"realloc usuarios\n"); return array; }
    return tmp;
}

Usuario **agregarUsuario(Usuario **usrArr, int *totUsr) {
    Usuario *n = malloc(sizeof(Usuario));
    if (!n) { fprintf(stderr,"malloc usuario\n"); return usrArr; }

    printf("Nombre: ");
    if (!fgets(n->nombre, sizeof(n->nombre), stdin)) n->nombre[0] = '\0';
    quitarNuevaLinea(n->nombre);

    printf("Carrera: ");
    if (!fgets(n->carrera, sizeof(n->carrera), stdin)) n->carrera[0] = '\0';
    quitarNuevaLinea(n->carrera);

    printf("Matricula: ");
    if (scanf("%d", &n->matricula) != 1) n->matricula = 0;
    limpiarEntrada();


    n->activo = 1;

    Usuario **tmp = redimensionarUsuarios(usrArr, (*totUsr) + 1);
    if (!tmp) { free(n); return usrArr; }
    tmp[*totUsr] = n;
    *totUsr += 1;
    printf("Usuario agregado con ID %d\n", n->id);
    return tmp;
}

Usuario *buscarUsuarioPorId(Usuario **usrArr, int totUsr, int id) {
	int i;
    for (i = 0; i < totUsr; ++i) if (usrArr[i]->id == id) return usrArr[i];
    return NULL;
}

void mostrarUsuarios(Usuario **usrArr, int totUsr) {
    if (totUsr == 0) { printf("No hay usuarios.\n"); return; }
    printf("ID | Nombre | Carrera | Matricula | Activo\n");
    int i;
    for (i = 0; i < totUsr; ++i) {
        printf("%d | %s | %s | %d | %d\n",
               usrArr[i]->id, usrArr[i]->nombre, usrArr[i]->carrera,
               usrArr[i]->matricula, usrArr[i]->activo);
    }
}

/* -------------------------
   FUNCIONES PARA PRESTAMOS
   ------------------------- */
Prestamo **redimensionarPrestamos(Prestamo **array, int nuevo_tam) {
    Prestamo **tmp = realloc(array, sizeof(Prestamo*) * nuevo_tam);
    if (!tmp && nuevo_tam > 0) { fprintf(stderr,"realloc prestamos\n"); return array; }
    return tmp;
}

int contarPrestamosActivosUsuario(Prestamo **presArr, int totPres, int idUsuario) {
    int cont = 0;
    int i;
    for (i = 0; i < totPres; ++i) {
        if (presArr[i]->id_usuario == idUsuario && presArr[i]->devuelto == 0) cont++;
    }
    return cont;
}

/* realizarPrestamo */
Prestamo **realizarPrestamo(Prestamo **presArr, int *totPres, Libro **libArr, int totLib, Usuario **usrArr, int totUsr) {
    if (totLib == 0 || totUsr == 0) { printf("Necesitas al menos 1 libro y 1 usuario.\n"); return presArr; }

    int idL, idU;
    printf("ID libro: ");
    if (scanf("%d", &idL) != 1) { printf("Entrada invalida\n"); limpiarEntrada(); return presArr; }
    printf("ID usuario: ");
    if (scanf("%d", &idU) != 1) { printf("Entrada invalida\n"); limpiarEntrada(); return presArr; }
    limpiarEntrada();

    Libro *lib = buscarLibroPorId(libArr, totLib, idL);
    if (!lib) { printf("Libro no existe\n"); return presArr; }
    if (lib->disponible == 0) { printf("Libro no disponible\n"); return presArr; }

    Usuario *usr = buscarUsuarioPorId(usrArr, totUsr, idU);
    if (!usr) { printf("Usuario no existe\n"); return presArr; }
    if (usr->activo == 0) { printf("Usuario inactivo\n"); return presArr; }

    int activos = contarPrestamosActivosUsuario(presArr, *totPres, idU);
    if (activos >= 3) { printf("Usuario ya tiene 3 prestamos activos\n"); return presArr; }

    Prestamo *p = malloc(sizeof(Prestamo));
    if (!p) { fprintf(stderr,"malloc prestamo\n"); return presArr; }

    p->id_libro = idL;
    p->id_usuario = idU;
    /* pedir fecha_prestamo */
    printf("Fecha prestamo (DD/MM/YYYY): ");
    if (!fgets(p->fecha_prestamo, sizeof(p->fecha_prestamo), stdin)) p->fecha_prestamo[0] = '\0';
    quitarNuevaLinea(p->fecha_prestamo);
    if (!validarFecha(p->fecha_prestamo)) {
        printf("Fecha invalida. Cancelado.\n");
        free(p);
        return presArr;
    }
    /* calcular fecha devolucion +15 dias */
    calcularFechaDevolucion(p->fecha_prestamo, 15, p->fecha_devolucion);

    p->devuelto = 0;
    p->fecha_devolucion_real[0] = '\0';

    lib->disponible = 0;

    Prestamo **tmp = redimensionarPrestamos(presArr, (*totPres) + 1);
    if (!tmp) { free(p); return presArr; }
    tmp[*totPres] = p;
    *totPres += 1;

    printf("Prestamo ID %d registrado. Dev: %s\n", p->id, p->fecha_devolucion);
    return tmp;
}

/* devolverLibro por idPrestamo */
int devolverLibro(Prestamo **presArr, int totPres, Libro **libArr, int totLib, int idPrestamo) {
    Prestamo *p = NULL;
    int i;
    for (i = 0; i < totPres; ++i) if (presArr[i]->id == idPrestamo) { p = presArr[i]; break; }
    if (!p) { printf("Prestamo no encontrado\n"); return 0; }
    if (p->devuelto) { printf("Ya devuelto\n"); return 0; }

    printf("Fecha devolucion real (DD/MM/YYYY): ");
    if (!fgets(p->fecha_devolucion_real, sizeof(p->fecha_devolucion_real), stdin)) p->fecha_devolucion_real[0] = '\0';
    quitarNuevaLinea(p->fecha_devolucion_real);
    if (!validarFecha(p->fecha_devolucion_real)) {
        printf("Fecha invalida. Cancelado.\n");
        p->fecha_devolucion_real[0] = '\0';
        return 0;
    }

    p->devuelto = 1;
    /* cambiar estado libro */
    Libro *lib = buscarLibroPorId(libArr, totLib, p->id_libro);
    if (lib) lib->disponible = 1;

    /* verificar retraso simple: convertir fechas a número YYYYMMDD y comparar (sin time.h) */
    int d1,m1,y1,d2,m2,y2;
    if (sscanf(p->fecha_devolucion, "%2d/%2d/%4d", &d1,&m1,&y1) == 3 &&
        sscanf(p->fecha_devolucion_real, "%2d/%2d/%4d", &d2,&m2,&y2) == 3) {
        long lim = y1*10000 + m1*100 + d1;
        long rea = y2*10000 + m2*100 + d2;
        if (rea > lim) {
            printf("Atencion: devolucion con retraso\n");
        } else {
            printf("Devuelto a tiempo\n");
        }
    }
    printf("Prestamo %d devuelto.\n", p->id);
    return 1;
}

void mostrarPrestamosActivos(Prestamo **presArr, int totPres, Libro **libArr, int totLib, Usuario **usrArr, int totUsr) {
    int found = 0;
    printf("ID | IDlib | Titulo | IDusr | Nombre | FechaPres | FechaDev\n");
    int i;
    for (i = 0; i < totPres; ++i) {
        Prestamo *p = presArr[i];
        if (p->devuelto == 0) {
            Libro *l = buscarLibroPorId(libArr, totLib, p->id_libro);
            Usuario *u = buscarUsuarioPorId(usrArr, totUsr, p->id_usuario);
            printf("%d | %d | %s | %d | %s | %s | %s\n",
                   p->id,
                   p->id_libro,
                   l ? l->titulo : "(no encontrado)",
                   p->id_usuario,
                   u ? u->nombre : "(no encontrado)",
                   p->fecha_prestamo,
                   p->fecha_devolucion);
            found = 1;
        }
    }
    if (!found) printf("No hay prestamos activos.\n");
}

/* -------------------------
   REGISTRO INICIAL (pedir cantidad y cargar)
   ------------------------- */
void registrarLibrosInicial(int cantidad) {
    if (cantidad <= 0) return;
    int i;
    for (i = 0; i < cantidad; ++i) {
        printf("\n--- Libro %d de %d ---\n", i+1, cantidad);
        libros = agregarLibro(libros, &totalLibros);
    }
}

void registrarUsuariosInicial(int cantidad) {
    if (cantidad <= 0) return;
    int i;
    for (i = 0; i < cantidad; ++i) {
        printf("\n--- Usuario %d de %d ---\n", i+1, cantidad);
        usuarios = agregarUsuario(usuarios, &totalUsuarios);
    }
}

/* -------------------------
   MAIN: menú (estilo primera versión)
   ------------------------- */
int main(void) {
    int opcion = 0;
    int n;

    do {
        printf("\n====== MENU PRINCIPAL ======\n");
        printf("1. Registrar libros iniciales\n");
        printf("2. Registrar usuarios iniciales\n");
        printf("3. Mostrar libros\n");
        printf("4. Mostrar usuarios\n");
        printf("5. Agregar 1 libro\n");
        printf("6. Agregar 1 usuario\n");
        printf("7. Realizar prestamo\n");
        printf("8. Mostrar prestamos activos\n");
        printf("9. Liberar memoria y reiniciar datos\n");
        printf("10. Salir\n");
        printf("Opcion: ");
        if (scanf("%d", &opcion) != 1) { printf("Entrada invalida\n"); limpiarEntrada(); opcion = 0; continue; }
        limpiarEntrada();

        switch (opcion) {
            case 1:
                printf("Cuantos libros registrar inicialmente? ");
                if (scanf("%d", &n) != 1) { printf("Entrada invalida\n"); limpiarEntrada(); break; }
                limpiarEntrada();
                registrarLibrosInicial(n);
                break;
            case 2:
                printf("Cuantos usuarios registrar inicialmente? ");
                if (scanf("%d", &n) != 1) { printf("Entrada invalida\n"); limpiarEntrada(); break; }
                limpiarEntrada();
                registrarUsuariosInicial(n);
                break;
            case 3:
                mostrarLibros(libros, totalLibros);
                break;
            case 4:
                mostrarUsuarios(usuarios, totalUsuarios);
                break;
            case 5:
                libros = agregarLibro(libros, &totalLibros);
                break;
            case 6:
                usuarios = agregarUsuario(usuarios, &totalUsuarios);
                break;
            case 7:
                prestamos = realizarPrestamo(prestamos, &totalPrestamos, libros, totalLibros, usuarios, totalUsuarios);
                break;
            case 8: {
                int idp;
                printf("ID prestamo a devolver: ");
                if (scanf("%d", &idp) != 1) { printf("Entrada invalida\n"); limpiarEntrada(); break; }
                limpiarEntrada();
                devolverLibro(prestamos, totalPrestamos, libros, totalLibros, idp);
                break;
            }
            case 9:
                mostrarPrestamosActivos(prestamos, totalPrestamos, libros, totalLibros, usuarios, totalUsuarios);
                break;
            case 10:
                liberarMemoriaCompleta(&libros, &totalLibros, &usuarios, &totalUsuarios, &prestamos, &totalPrestamos);
                printf("Datos reiniciados.\n");
                break;
            case 11:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }

    } while (opcion != 11);

    /* liberar antes de salir (por si no se hizo) */
    liberarMemoriaCompleta(&libros, &totalLibros, &usuarios, &totalUsuarios, &prestamos, &totalPrestamos);
    return 0;
}

