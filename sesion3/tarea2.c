#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int nproc = 5;
    int childpid;
    pid_t hijos[nproc]; 
    for (size_t i = 0; i < nproc; ++i)
    {
        if ((childpid = fork()) < 0)
        {
            perror("Error en el fork (creación de procesos hijo)");
            exit(EXIT_FAILURE);
        }

        if (!childpid)
        {
            printf("Soy el hijo con pid %d\n", getpid());
            exit(EXIT_SUCCESS); 
        }
        else
        {
            hijos[i] = childpid; 
        }
    }

    if (childpid)
    {
        pid_t pid;
        int hijos_vivos = nproc;

        
        for (size_t i = 0; i < nproc; i += 2)
        {
            pid = waitpid(hijos[i], NULL, 0); 
            if (pid > 0)
            {
                printf("Acaba de finalizar mi hijo impar con pid %d\n", pid);
                printf("Solo me quedan %d hijos vivos\n", --hijos_vivos);
            }
        }

        
        for (size_t i = 1; i < nproc; i += 2)
        {
            pid = waitpid(hijos[i], NULL, 0); 
            if (pid > 0)
            {
                printf("Acaba de finalizar mi hijo par con pid %d\n", pid);
                printf("Solo me quedan %d hijos vivos\n", --hijos_vivos);
            }
        }
    }

    return 0;
}
