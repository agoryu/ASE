#include "mbr.h"

static void empty_it(){
    return;
}

void usage(){
  printf("Utilisation: dvol\n");
  printf("\n");
  printf("Permet d'afficher les volumes");
  printf("\n");
  printf("Exemples:\n");
  printf("\t$dvol\n");
}

int main(int argc, char**argv){

  int i;

  if(argc!=1){
    usage();
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
	
  /* chargement du mbr */
  if(!load_mbr()){
    perror("Erreur lors du chargement du Master Boot Record.");
    exit(EXIT_FAILURE);
  }

  /* afficher les volumes */
  display_vol();

  exit(EXIT_SUCCESS);
}
