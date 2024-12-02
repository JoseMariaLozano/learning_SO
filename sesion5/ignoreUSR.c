#include <stdlib.h>
#include <signal.h>

int main(int argc, char **argv)
{
    sigset_t new_mask;

    sigemptyset(&new_mask); // vaciar la mascara

    sigaddset(&new_mask, SIGUSR1); // añadimos SIGUSR1 al listado

    sigsuspend(&new_mask); // esperamos hasta recibir una señal que no sea las que estan en new_mask

    return EXIT_SUCCESS;
}
