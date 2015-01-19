#include "drive/drive.h"

static void empty_it(){
    return;
}

void usage(char* name){
  printf("Dump Sector\n");
  printf("\tShow the value of a sector.\n");
  printf("\n");
  printf("Usage:\n");
  printf("\t$ %s cylinder sector\n", name);
  printf("\n");
  printf("Exemple:\n");
  printf("\t$ %s 0 2\n", name);
  printf("\t$ Data from cylinder 0 and sector 2\n");
  exit(EXIT_SUCCESS);
}


int main(int argc, char* argv[]){

  unsigned cylinder, sector;
  unsigned char* buffer = malloc(HDA_SECTORSIZE * sizeof(char));
  int i;

  if(argc!=3 || (argc>1  && strcmp(argv[1], "-h")==0) ){
    usage(argv[0]);
  }

  cylinder = atoi(argv[1]);
  sector = atoi(argv[2]);

  /* init hardware */
  if(!init_hardware(HW_CONFIG)){
    fprintf(stderr, "Error: Initialization error\n");
    exit(EXIT_FAILURE);
  }

  /* Interreupt handlers */
  for(i=0; i<16; i++){
    IRQVECTOR[i] = empty_it;
  }

  /* Allows all IT */
  _mask(1);

  read_sector(cylinder, sector, buffer);

  if(!buffer){
    fprintf(stderr, "Error: Couldn't read hardware\n");
    exit(EXIT_FAILURE);
  } 

  printf("%s\n", buffer);

  exit(EXIT_SUCCESS);
}
