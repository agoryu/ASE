#include "filesys/vol.h"

static void empty_it(){
    return;
}

void usage(char* name){
  printf("Display FileSystem\n");
  printf("\tDisplay the partitions and the amount of available disk space for the current one.\n");
  printf("\n");
  printf("Usage:\n");
  printf("\t$ %s\n", name);
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){

  unsigned i;

  if(argc!=1){
    usage(argv[0]);
  }

  /* init hardware */
  if(!init_hardware(HW_CONFIG)){
    fprintf(stderr, "Initialization error\n");
    exit(EXIT_FAILURE);
  }

  /* Interreupt handlers */
  for(i=0; i<NB_EMPTY_FUNCTION; i++)
    IRQVECTOR[i] = empty_it;

  /* Allows all IT */
  _mask(1);
	
  /* chargement du mbr */
  if(!load_mbr()){
    fprintf(stderr, "Erreur lors du chargement du Master Boot Record.\n");
    exit(EXIT_FAILURE);
  }

  /* afficher les volumes */
  load_super(current_vol);  
  if(!display_vol()){
    exit(EXIT_SUCCESS);
  }


  printf("Il reste %d espace dans le volume courant.\n", get_nb_free_bloc());

  exit(EXIT_SUCCESS);
}
