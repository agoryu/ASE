#include "hw/hw.h"
#include "filesys/super.h"


void usage(char* name){
  printf("Make New FileSystem\n");
  printf("\tInitialaze the current volume\n");
  printf("\n");
  printf("Usage:\n");
  printf("\t$ %s\n", name);
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){

  char* root_name;

  if(argc != 1){
    usage(argv[0]);
  }

  /* init hardware */
  if(!boot()) {
    fprintf(stderr, "FATAL: L'initialisation du matériels a échoué.\n");
    exit(EXIT_FAILURE);
  }

  /* chargement du mbr */
  if(!load_mbr()){
    fprintf(stderr, "ERROR: Erreur lors du chargement du Master Boot Record.\n");
    exit(EXIT_FAILURE);
  }

  /* initialise le super du volume 1 */
  init_super(CURRENT_VOLUME);

  if(!load_super(CURRENT_VOLUME)){
    fprintf(stderr, "ERROR: Super bloc invalide.\n");
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
