#include "drive/drive.h"
#include "hw/hw.h"


void usage(char* name){
  printf("Format Disk\n");
  printf("\tFormat all sector of the disk.\n");
  printf("\n");
  printf("Usage:\n");
  printf("\t$ %s\n", name);
  exit(EXIT_SUCCESS);
}


int main(int argc, char* argv[]){

  if(argc!=1){
    usage(argv[0]);
  }

  /* init hardware */
  if(!boot()){
    fprintf(stderr, "FATAL: L'initialisation du matériels a échoué.\n");
    exit(EXIT_FAILURE);
  }

  format_all(0);

  exit(EXIT_SUCCESS);
}
