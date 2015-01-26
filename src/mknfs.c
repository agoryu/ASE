#include "filesys/vol.h"

static void empty_it(){
  return;
}

void usage(char* name){
  printf("Make New FileSystem\n");
  printf("\tInitialaze the current volume\n");
  printf("\n");
  printf("Usage:\n");
  printf("\t$ %s\n", name);
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){

  unsigned int i;
  char* root_name;

  if(argc != 1){
    usage(argv[0]);
  }

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

  /* initialise le super du volume 1 */
  init_super(CURRENT_VOLUME);

  if(!load_super(CURRENT_VOLUME)){
    fprintf(stderr, "Super bloc invalide.\n");
  }
  
  root_name = malloc(sizeof(char) * ENTRYMAXLENGTH);
  *root_name = '/';

  /* creation de la racine si elle n'existe pas */
  /* creation un peu radical qui fait des degat */
  if(inumber_of_path("/") == 0) {
    inumber_racine = create_ifile(IT_DIR);
    add_entry(inumber_racine, inumber_racine, root_name);
  } else {
    fprintf(stderr, "Racine existante\n");
  }

  init_root();

  printf("Le volume principale a été créé avec succès.\n");

  /* sauvegarde de tous les changements effectué sur le mbr */
  save_mbr();
} else {
  printf("Le volume principale a déjà été créé!\n");
 }

exit(EXIT_SUCCESS);
}
