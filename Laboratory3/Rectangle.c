#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
      int i=0;
      //printf("%d",argv[1]);
      if(!strcmp(argv[1],"-a"))
      {
            printf("area =%d units\n", atoi(argv[3])*atoi(argv[5]));
      }
      if(!strcmp(argv[1],"-p"))
      {
            printf("perimeter =%d units\n", 2*(atoi(argv[3])+atoi(argv[5])));
      }
      return 0;
}
