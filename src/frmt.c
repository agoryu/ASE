#include "drive/drive.h"

static void empty_it(){
  return;
}

void usage(char* name){
  printf("Format Disk\n");
  printf("\tFormat all sector of the disk.\n");
  printf("\n");
  printf("Usage:\n");
  printf("\t$ %s\n", name);
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){

  int i;

  if(argc!=1){
    usage(argv[0]);
  }

  /* init hardware */
  if(!init_hardware(HW_CONFIG)){
    fprintf(stderr, "Error: Initialization error\n");
    exit(EXIT_FAILURE);
  }

  /* Interreupt handlers */
  for(i=0; i<16; i++)
    IRQVECTOR[i] = empty_it;

  /* Allows all IT */
  _mask(1);

  format_all(0);

  exit(EXIT_SUCCESS);
}
