#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
      char name[20];
      char matricula[20];
      char option[1];
      int counter = 0;
      FILE *f = fopen("Datalog.txt", "w");
      if (f == NULL)
      {
            fprintf(stderr,"Error opening file!\n");
            exit(1);
      }
      do{
            printf("Enter Name:\n");
            scanf("%s",&name);
            printf("Enter ID:\n");
            scanf("%s",&matricula);
            counter++;
            printf("Do you wish to add more [Y/n]:\n");
            scanf("%s", &option);
            
            fprintf(f, "Nombre: %s\t", name);
            fprintf(f, "ID: %s\n", matricula);
              
      }while((!strcmp(option,"Y")) && (counter < (atoi(argv[2]))));
      fclose(f);
      return 0;
}
