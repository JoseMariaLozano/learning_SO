#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<unistd.h>
int main()
{
  int nproc = 5;
  int childpid;
  for (size_t i = 0; i < nproc; ++i)
  {
    if( ( childpid = fork() ) < 0)
    {
      perror("Error en el fork (creación de procesos hijo)");
      exit(EXIT_FAILURE);
    }

    if(!childpid)
    {
      printf( "Soy el hijo con pid %d\n", getpid() );
      break;
    }
  }

  if(childpid){
    pid_t pid;
    while( (pid = wait(NULL) ) > 0)
    {
      printf( "Acaba de finalizar mi hijo con pid %d\n", pid );
      printf( "Solo me quedan %d hijos vivos\n", --nproc)
    }
  }
  

  return 0;
}