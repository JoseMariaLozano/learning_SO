#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

static volatile sig_atomic_t sigint_count = 0;
static void 
catch_sigint(int sig) 
{
    if(sig = SIGINT)
        sigint_count++;
}

static int 
install_sigint(void)
{
    struct sigaction act;
    memset(&act, 0, sizeof(act)); // inicializar a 0 para asegurar acción by default
    sigemptyset(&act.sa_mask);
    act.sa_handler = catch_sigint;
    act.sa_flags = 0;

    if(sigaction(SIGINT, &act, NULL) == -1)
    {
        return errno;
    }


    return EXIT_SUCCESS;
}
int main(int argc, char **argv)
{
    struct timespec duration;
    int result;

    if (install_sigint())
    {
        fprintf(stderr, "No se pudo instalar el manejador de SIGINT con error, %d\n", errno);
        return EXIT_FAILURE;
    }

    duration.tv_sec = 360;
    duration.tv_nsec = 0;

    printf("Durmiendo por %ld segundos\n", duration.tv_sec);
    fflush(stdout);

    while(1)
    {
        result = nanosleep(&duration, &duration);

        if(sigint_count > 0)
        {
            printf("Le has dado al Ctrl+C %d ve%s\n", sigint_count, (sigint_count > 1) ? "ces" : "z");
        }

    }

    return 0;
}
