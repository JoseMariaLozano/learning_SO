#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>

#define MY_ISREG(mode) (((mode) & 0170000) == 0100000)

int main(int argc, char *argv[]){
    
    int i;
    struct stat atributos;
    char tipoArchivo[30];
    if(argc<2) {
        printf("\nSintaxis de ejecucion: tarea4_exe [<nombre_archivo>]+\n\n");
        exit(EXIT_FAILURE);
    }
    for(i=1;i<argc;i++) {
        printf("%s: ", argv[i]);
        if(lstat(argv[i],&atributos) < 0) {
            printf("\nError al intentar acceder a los atributos de %s",argv[i]);
            perror("\nError en lstat");
        }

        if (MY_ISREG(atributos.st_mode)) strcpy(tipoArchivo,"Regular");
        else {
            printf("%s no es un archivo regular\n", argv[i]);
            exit(EXIT_FAILURE);
        }

        printf("%s\n",tipoArchivo);
    }
    return 0;
}
