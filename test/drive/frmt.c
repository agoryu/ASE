#include "drive.h"

static void empty_it(){
    return;
}


int main(int argc, char* argv[]){

  int i;

  if(argc>1 || (argc>1  && strcmp(argv[1], "-h")==0) ){
    printf("Usage:\n");
    printf("\t$dmps cylindre secteur\n");
    exit(EXIT_SUCCESS);
  }

  /* init hardware */
  if(!init_hardware(HW_CONFIG)){
    perror("Initialization error\n");
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
