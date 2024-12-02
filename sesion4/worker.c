#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

int es_primo(int n) {
    int counter = 0;
    if (n <= 1) return 0;
    for (int k = 2; k <= sqrt(n); ++k) {
        if (n % k == 0) counter++;
    }
    if(counter < 2) return 1;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <inicio_intervalo> <fin_intervalo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int inicio = atoi(argv[1]);
    int fin = atoi(argv[2]);

    for (int i = inicio; i <= fin; ++i) {
        if (es_primo(i)) {
            write(STDOUT_FILENO, &i, sizeof(int));
        }
    }

    return 0;
}
