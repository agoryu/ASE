#include "vol.h"

static void empty_it(){
    return;
}

int main(int argc, char**argv){

  int i;

  /* init hardware */
  if(!init_hardware(HW_CONFIG)){
    perror("Initialization error\n");
    exit(EXIT_FAILURE);
  }

  /* Interreupt handlers */
  for(i=0; i<NB_EMPTY_FUNCTION; i++)
    IRQVECTOR[i] = empty_it;

  /* Allows all IT */
  _mask(1);
	
  /* chargement du mbr */
  if(!load_mbr()){
    perror("Erreur lors du chargement du Master Boot Record.\n");
    exit(EXIT_FAILURE);
  }

  /* afficher les volumes */
  load_super(current_vol);  
  display_vol();


  printf("Il reste %d espace dans le volume courant.\n", get_nb_free_bloc());

  exit(EXIT_SUCCESS);
}
