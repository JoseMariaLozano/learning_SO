#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define BUFFER_SIZE 80
#define BLOQUE_STR_SIZE 200

int fd_escritura;
unsigned int block = 0;

/*******************************************************************************
 * Funciones de manejo de señal de interrupcion no es material de esta sesion
 * se puede implementar el codigo sin el uso de manejo de interrupciones
 */

// Manejador de la señal SIGINT (Ctrl+C)
void handle_sigint(int sig) {
    // Escribir el número de bloques en el archivo antes de salir
    lseek(fd_escritura, 0, SEEK_SET);
    char bloques_info[50];
    snprintf(bloques_info, 50, "El número de bloques es %d\n", block);
    if (write(fd_escritura, bloques_info, strlen(bloques_info)) < 0) {
        perror("\nError al escribir el número de bloques");
        exit(EXIT_FAILURE);
    }
    
    printf("\nInterrupción de teclado (Ctrl+C). Terminado de leer y escribir.\n");
    close(fd_escritura);
    exit(EXIT_SUCCESS);
}

//------------------------------------------------------------------------------

/**
 * @brief Main function
 * 
 * @param argc numero de argumentos (incluye el nombre del programa)
 * @param argv puntero a cadenas de caracteres (los argumentos) 
 * @return int 0=EXIT_SUCCES, 1=EXIT_FAILURE
 */
int main(int argc, char *argv[]) {

    int fd_lectura;

    if(argc == 1) {
        fd_lectura = STDIN_FILENO;
        printf("Debes pulsar ENTER para que se escriba la entrada (entrada estandar), pulse Ctrl+C para salir\n");

    } else {
        char *filename = argv[1];
        if ( (fd_lectura = open(filename, O_RDONLY)) < 0) { // abre el archivo con permisos de lectura solo
            printf("\nError %d en open", errno);
            perror("\nError en open");
            exit(EXIT_FAILURE);
        }
    }
    
    char buf1[BUFFER_SIZE];
    char * filename2 = "./files/archivo.txt";

    if ( (fd_escritura = open(filename2, O_CREAT | O_TRUNC | O_WRONLY, 0666)) < 0) { // abre el archivo con permisos de lectura y escritura
        printf("\nError %d en open", errno);
        perror("\nError en open");
        exit(EXIT_FAILURE);
    }


    // Configurar el manejador de señal para Ctrl+C (SIGINT)
    signal(SIGINT, handle_sigint);

    int bytes_leidos;

    // Reservar espacio para el número de bloques
    lseek(fd_escritura, 50, SEEK_SET);

    // Proceso de lectura y escritura en bloques
    while ( (bytes_leidos = read(fd_lectura, buf1, BUFFER_SIZE)) > 0) {
        char bloque_str[BLOQUE_STR_SIZE];
        snprintf(bloque_str, BLOQUE_STR_SIZE, "Bloque %d:\n// Datos leidos en el bloque %d\n", block, block);

        if (write(fd_escritura, bloque_str, strlen(bloque_str)) < 0) {
            perror("\nError en write bloque_str");
            exit(EXIT_FAILURE);
        }
        if (write(fd_escritura, buf1, bytes_leidos) < 0) {
            perror("\nError en write buf1");
            exit(EXIT_FAILURE);
        }
        if (write(fd_escritura, "\n\n", strlen("\n\n")) < 0) {
            perror("\nError en write newlines");
            exit(EXIT_FAILURE);
        }

        block++;
    }

    if (bytes_leidos < 0) {
        perror("\nError en read");
        exit(EXIT_FAILURE);
    }

    // Si el programa finaliza sin interrupción, escribir el número de bloques (no va a ocurrir)
    // lseek(fd_escritura, 0, SEEK_SET);
    // char bloques_info[50];
    // snprintf(bloques_info, 50, "El número de bloques es %d\n", block);
    // if (write(fd_escritura, bloques_info, strlen(bloques_info)) < 0) {
    //     perror("\nError al escribir el número de bloques");
    //     exit(EXIT_FAILURE);
    // }

    close(fd_escritura);
    close(fd_lectura);

    return 0;
}
