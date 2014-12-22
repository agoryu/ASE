#include "vol.h"

static void empty_it(){
  return;
}

int main(int argc, char**argv){

  unsigned int i;
  unsigned fc = 0;
  unsigned fs = 1;
  unsigned nb_bloc = 10;

  /* init hardware */
  if(!init_hardware(HW_CONFIG)) {
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

  if(mbr.mbr_n_vol < 1) {

    /* creation d'un volume bidon */
    if(!make_vol(fc, fs, nb_bloc)) {
      fprintf(stderr, "Erreur a la creation d'un volume bidon.\n");
      exit(EXIT_FAILURE);
    }

    /* initialise le super du volume 1 */
    init_super(current_vol);

    printf("Le volume principale a été créé avec succès.\n");

    /* sauvegarde de tous les changements effectué sur le mbr */
    save_mbr();
  } else {
    printf("Le volume principale a déjà été créé!\n");
  }

  exit(EXIT_SUCCESS);
}
