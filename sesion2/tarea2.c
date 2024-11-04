#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <string.h>


void print_permisos(mode_t mode) {
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}


int main(int argc, char **argv)
{
    if (argc != 3) 
    {
        perror("Error al llamar a la funcion. Uso: ./tarea2_exe <pathname> <mode> (mode has to be octal num)");
        exit(EXIT_FAILURE);
    }

    char * path = argv[1];
    mode_t mode = strtol(argv[2], NULL, 0); // base = 0 detecta automaticamente la base del numero

    DIR * dir = opendir(path); // abrimos el directorio

    if (dir == NULL) 
    {
        printf("Error en opendir %d \n", errno);
        exit(EXIT_FAILURE);
    }

    struct dirent *entrada; // cada entrada del directorio
    struct stat stat_fichero; // para acceder al modo del archivo seleccionado

    char path_archivo[PATH_MAX];

    // leer cada entrada del directorio
    while ( (entrada = readdir(dir)) != NULL ) 
    {
        // si son el directorio activo o el padre  no nos interesa
        if ( (strcmp(".", entrada->d_name) == 0 || strcmp("..", entrada->d_name) == 0) )
        {
            continue;
        }

        // el path del archivo es el path de directorio/nombre_archivo (entrada->d_name)
        snprintf(path_archivo, PATH_MAX, "%s/%s", path, entrada->d_name);

        // accedemos a los metadatos del archivo
        if(stat(path_archivo, &stat_fichero) == -1)
        {
            perror("Error en stat");
            exit(EXIT_FAILURE);
        }

        // comprobamos si hemos accedido a un directorio o un archivo
        char tipoArchivo[30];
        if(S_ISDIR(stat_fichero.st_mode)) strcpy(tipoArchivo,"Directorio");
        else strcpy(tipoArchivo,"Regular");

        // Formateamos output para mostrar los permisos antiguos y los nuevos
        printf("%s: %s Permisos antiguos: ", tipoArchivo, entrada->d_name);
        print_permisos(stat_fichero.st_mode);

        if (chmod(path_archivo, mode) == -1) {
            fprintf(stdout, " %s\n", strerror(errno));

        } else {
            printf(" Nuevos permisos: ");
            print_permisos(mode);
            printf("\n");
        }
        printf("\n");
    }

    closedir(dir); // cerramos directorio

    return EXIT_SUCCESS; 
}
