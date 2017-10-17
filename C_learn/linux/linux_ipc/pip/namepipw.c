#include <stdio.h>
#include <unistd.h>

void main(){

  FILE* out_file;
  int count = 1;
  char buf[80];
  out_file = fopen("namepip", "w");
  if(out_file == NULL){
    printf("Error open pip \n");
    exit(1);
  }

  sprintf(buf, "this is test data from name pip write process \n");
  fwrite(buf, 1, 80, out_file);
  fclose(out_file);
}
