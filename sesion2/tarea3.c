#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <string.h>

unsigned
    num_archivos = 0,
    tam_total = 0;


void mostrar (struct stat stat, char * path)
{
    printf("%s %ld\n", path, stat.st_size);
    num_archivos++;
    tam_total += stat.st_size;
}

void buscar( char * path)
{
    DIR *dir = opendir(path);

    if (dir == NULL) 
    {
        printf("Error en opendir %d \n", errno);
        exit(EXIT_FAILURE);
    }

    struct dirent *entrada;
    struct stat stat_fichero; // para acceder al modo del archivo seleccionado
    char path_archivo[PATH_MAX];

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

        if(S_ISDIR(stat_fichero.st_mode)) buscar(path_archivo);
        else if( stat_fichero.st_mode & (S_IXGRP | S_IXOTH) ) 
        {
            mostrar(stat_fichero, path_archivo);
        } 


    }

    if (closedir(dir) == -1) {
        perror("Error al cerrar el directorio");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{   
    if(argc != 2)
    {
        printf("Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char * path = argv[1];
    buscar(path);

    printf("Existen %d archivos regulares con permisos de ejecucion para grupo y otros\n", num_archivos);
    printf("El tamaño total ocupado por dichos archivos es %d bytes", tam_total);

    return EXIT_SUCCESS;
}
