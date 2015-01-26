#include "filesys/vol.h"
#include <unistd.h>

static void empty_it(){
    return;
}

void usage(char* name){
  printf("Make Volume\n");
  printf("\tPermets la creation d'un volume.\n");
  printf("\n");
  printf("Usage:\n");
  printf(" -s \t l'argument qui suit est le nombre de blocs pour le volume\n");
  printf(" -fc \t l'argument qui suit est le cylindre a partir duquel le volume commence\n");
  printf(" -fs \t l'argument qui suit est le secteur a partir duquel le volume commence\n");
  printf("\t $ %s -s NB_BLOC -fc CYLINDRE -fs SECTOR\n", name);
  printf("\n");
  printf("Exemple:\n");
  printf("\t$ %s -fc 0 -fs 1 -s 12\n", name);
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){
	
  unsigned fc = HDA_MAXCYLINDER+1;
  unsigned fs = HDA_MAXSECTOR+1;
  unsigned nb_bloc = 4;
  int i;


  /*vérification des arguments entree par l'utilisateur */
  if(argc != 7 || (argc>1  && strcmp(argv[1], "-h")==0)){
    usage(argv[0]);
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

  /* recuperer les arguments */
  for(i=1; i<argc; i++){

    if(strcmp(argv[i], "-s") == 0){
      nb_bloc = atoi(argv[i+1]);
    }

    if(strcmp(argv[i], "-fc") == 0){
      fc = atoi(argv[i+1]);

    }

    if(strcmp(argv[i], "-fs") == 0){
      fs = atoi(argv[i+1]);
    }
  }

  /* gestion des erreur d'arguments */
  if(nb_bloc <= 0 || nb_bloc>=(HDA_MAXCYLINDER*HDA_MAXSECTOR)){
    perror("La taille du volume impossible.\n");
    exit(EXIT_FAILURE);
  }

  if(fc>HDA_MAXCYLINDER){
    perror("Cylindre possible.\n");
    exit(EXIT_FAILURE);
  }

  if(fs>HDA_MAXSECTOR){
    perror("Secteur possible.\n");
    exit(EXIT_FAILURE);
  }

  if(fc==0 && fs==0) {
    fprintf(stderr, "Erreur: Possible de creer un volume a la place du Master Boot Record.\n");
    exit(EXIT_FAILURE);
  }
  
  /* creation du volume */
  if(!make_vol(fc, fs, nb_bloc)){
    fprintf(stderr, "Erreur lors de la creation du volume.\n");
    fprintf(stderr, "La creation du volume est impossible ");
    fprintf(stderr, "avec les carateristiques donnees en parametre.\n");
    exit(EXIT_FAILURE);
  }

  /* initialise le super du volume 1 */
  init_super(current_vol);

  /* sauvegarde de tous les changements effectué sur le mbr */
  save_mbr();

  exit(EXIT_SUCCESS);
}
