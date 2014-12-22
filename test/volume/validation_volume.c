#include "vol.h"

static void empty_it(){
  return;
}

int main() {

  unsigned int cpt = 0, i;

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

  /* creation d'un volume bidon */
  if(!make_vol(1, 1, 10)) {
    perror("Erreur a la creation d'un volume bidon.");
    exit(EXIT_FAILURE);
  }

  /* initialise le super du premier volume */
  init_super(CURRENT_VOLUME);

  /* charge le super du premier volume dans la variable globale */
  load_super(CURRENT_VOLUME);

  /* creation de nouveau bloc jusqu'a ce qu'il y est une erreur */
  while(new_bloc());

  if(is_full()) {
    free_bloc(2);
    free_bloc(5);
    free_bloc(6);

    printf("nombre de bloc libre = %d\n", get_nb_free_bloc());

    while(new_bloc()) cpt++;

    printf("le nombre de bloc alloué est %d\n", cpt);
				
  } else {
    printf("le disque n'est pas encore rempli\n");
  }

  exit(EXIT_SUCCESS);
}
