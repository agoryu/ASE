#include "drive/drive.h"
#include "hw/hw.h"


void usage(char* name){
  printf("Store Sector\n");
  printf("\tStore a buffer value in a sector.\n");
  printf("\n");
  printf("Usage:\n");
  printf("\t$ %s cylinder sector buffer\n", name);
  printf("\n");
  printf("Exemple:\n");
  printf("\t$ %s 0 2 \"Hello World\"\n", name);
  exit(EXIT_SUCCESS);
}


int main(int argc, char* argv[]){

  unsigned cylinder, sector;
  unsigned buf_size = HDA_SECTORSIZE * sizeof(char);
  unsigned char* buffer = malloc(buf_size);

  if(argc!=4 || (argc>1  && strcmp(argv[1], "-h")==0) ){
    usage(argv[0]);
  }

  cylinder = atoi(argv[1]);
  sector = atoi(argv[2]);

  /* init hardware */
  if(!boot()){
    fprintf(stderr, "FATAL: L'initialisation du matériels a échoué.\n");
    exit(EXIT_FAILURE);
  }

  if(!memcpy((void*)buffer, (void*)argv[3], buf_size)){
    fprintf(stderr, "ERROR: Couldn't copy the buffer argument: %s\n", argv[3]);
    exit(EXIT_FAILURE);
  }
  
  write_sector(cylinder, sector, buffer);
  
  free(buffer);
  exit(EXIT_SUCCESS);
}
