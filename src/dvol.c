#include "filesys/mbr.h"

static void empty_it(){
    return;
}

void usage(char* name){
  printf("Display Volume:\n");
  printf("\tPermet d'afficher les volumes");
  printf("\n");
  printf("Usage:\n");
  printf("\t$%s\n", name);
}

int main(int argc, char**argv){

  int i;

  if(argc!=1){
    usage(argv[0]);
    exit(EXIT_SUCCESS);
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
	
  /* chargement du mbr */
  if(!load_mbr()){
    fprintf(stderr, "Erreur lors du chargement du Master Boot Record.");
    exit(EXIT_FAILURE);
  }

  /* afficher les volumes */
  display_vol();

  exit(EXIT_SUCCESS);
}
