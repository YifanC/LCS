#include <stdio.h>
#include <unistd.h>
#include <signal.h>

static int stop = 0;

void CtrlHandler(int sig)
{
   if(sig==SIGINT)
   printf ("\nShutting down.");
     stop = 1;
}


int main(int argc, char **argv)
{
  if (signal(SIGINT, CtrlHandler) == SIG_ERR)
  printf("\ncan't catch SIGINT\n");


    int i = 0;
    for (i = 0; i < argc; ++i)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    int j = 0;
       for (j = 0; j < 10000; j++){
           printf("\rIn progress %d", j);
           usleep(1000);
           if (stop){
                break;
           }
       }
       printf("\n");
}