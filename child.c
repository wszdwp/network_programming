/*child calls exterm to show the commands*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

main(int argc, char **argv) {

  pid_t childpid, pid;
  int   stat;

  printf("forking child\n");

  if ( (childpid = fork()) == 0)  {

      if ( (execlp("xterm", "xterm", "-e", "./test", "127.0.0.1", (char *) 0)) < 0)  {
            /*print an appropriate error message here*/
            exit(1);
         }
     }

  pid = wait(&stat);  
  printf("child terminated\n");
}
