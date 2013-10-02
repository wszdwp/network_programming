/*fork a child and then calls exterm to show the commands*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>    //gethostbyname

#include "unp.h"

main(int argc, char **argv) {

  char *ptr, **pptr;
  char str[INET_ADDRSTRLEN];
  struct hostent *hptr;

  pid_t pid;
  int   stat;
  int   pfd[2];

  while(--argc > 0) {
    ptr = *++argv;
    if ( (hptr = gethostbyname(ptr)) == NULL) {
      printf("gethostbyname error for host: %s: %s", ptr, hstrerror(h_errno));
      continue;
    }
    printf("official hostname: %s\n", hptr->h_name);

    for(pptr = hptr->h_aliases; *pptr != NULL; pptr++)
      printf("\talias: %s\n", *pptr);

    switch (hptr->h_addrtype) {
      case AF_INET:
        pptr = hptr->h_addr_list;
          for( ; *pptr != NULL; pptr++)
              printf("\taddress: %s\n", inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
          break;
      default:
        printf("unkown address type");
        break;
    }
  }

  //fork 
  if (pipe(pfd) == -1)
  {
    perror("pipe failed");
    exit(1);
  }
  if ((pid = fork()) < 0)
  {
    perror("fork failed");
    exit(2);
  }

  if (pid == 0)
    {
      /*child process*/
      printf("forded a child\n");
      close(pfd[1]);        
      dup2(pfd[0], 0);
      close(pfd[0]);

      if ( (execlp("xterm", "xterm", "-e", "./test", "127.0.0.1", (char *) 0)) < 0) {
            /*print an appropriate error message here*/
            exit(1);
         }
      //execlp("wc", "wc", (char *) 0);
      perror("wc failed");
      pid = wait(&stat);  
      printf("child terminated\n");
      exit(3);
    } else {                
      /*parent process*/
      close(pfd[0]);
      dup2(pfd[1], 1);
      close(pfd[1]);
      //execlp("ls", "ls", (char *) 0);
      perror("ls failed");
      exit(4);
    } 
}