#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

void leer_primos(int fd) {
    int primo;
    while (read(fd, &primo, sizeof(int)) > 0) {
        printf("%d\n", primo);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <inicio_intervalo> <fin_intervalo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int inicio = atoi(argv[1]);
    int fin = atoi(argv[2]);
    int mitad = inicio + (fin - inicio) / 2;

    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Error al crear los cauces");
        exit(EXIT_FAILURE);
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Proceso esclavo 1
        close(pipe1[0]); // Cerrar extremo de lectura
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[1]);
        execlp("./worker", "./worker", argv[1], &argv[2][mitad - inicio + 1], NULL);
        perror("Error en execlp esclavo 1");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Proceso esclavo 2
        close(pipe2[0]); // Cerrar extremo de lectura
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[1]);
        execlp("./worker", "./worker", &argv[2][mitad - inicio + 1], argv[2], NULL);
        perror("Error en execlp esclavo 2");
        exit(EXIT_FAILURE);
    }

    // Proceso maestro
    close(pipe1[1]); // Cerrar extremos de escritura
    close(pipe2[1]);

    printf("Números primos en el intervalo [%d, %d]:\n", inicio, fin);
    leer_primos(pipe1[0]);
    leer_primos(pipe2[0]);

    close(pipe1[0]);
    close(pipe2[0]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
