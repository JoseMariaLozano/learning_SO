#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv)
{
  if( argc < 2 )
  {
    perror( "Debes por lo menos proporcionar el nombre de un ejecutable" );
    exit(EXIT_FAILURE);
  }

  // Revisar si "bg" está entre los argumentos y eleminarlo
  for (size_t i = 1; i < argc; ++i) { 
    if (strcmp("bg", argv[i]) == 0) {
      argv[i] = NULL; 
      break;
    }
  }
  pid_t childpid;
  if ( ( childpid = fork() ) < 0)
  {
    perror( "Error creando al proceso hijo ");
    exit(EXIT_FAILURE);
  }


  if(!childpid)
  {
    if(execvp(argv[1], &argv[1]) < 0)
    {
      perror("Error en la llamada a exec");
      exit(EXIT_FAILURE);
    }
  }

  int estado;

  if (waitpid(childpid, &estado, NULL) < 0)
  {
    perror("Error esperando al hijo");
    exit(EXIT_FAILURE);
  }

  printf("El proceso hijo termino con el estado de salida %d\n", &estado>>8);
  return 0;

}