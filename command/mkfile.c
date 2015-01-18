#include "inode.h"
#include "ifile.h"
#include "tools.h"

static void empty_it(){
  return;
}

void usage(){
  printf("Utilisation: mkfile\n");
  printf("\n");
  printf("Retourne et affiche l'inoeud du fichier créé.\n");
  printf("\n");
  printf("Exemple:\n");
  printf("\t$mkfile\n");
}

int main(int argc, char**argv){

  unsigned int i;
  unsigned int inumber;

  /* gestion des arguments */
  if(argc != 1){
    usage();
    exit(EXIT_SUCCESS);
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

  /* initialise le super du premier volume */
  init_super(CURRENT_VOLUME);

  /* charge le super du premier volume dans la variable globale */
  load_super(CURRENT_VOLUME);

  /* création du fichier */
  inumber = create_ifile(IT_FILE);
  if(!inumber){
    fprintf(stderr, "Erreur lors de la creation du fichier\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Création d'un fichier:\n");
  printf("\tinumber: %d.\n", inumber);

  exit(inumber);
}
