#include "hw/hw.h"
#include "filesys/mbr.h"

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

  /* init materiels */
  if(!boot()){
    fprintf(stderr, "FATAL: L'initialisation du matériels a échoué.\n");
  }

  /* chargement du mbr */
  if(!load_mbr()){
    fprintf(stderr, "ERROR: lors du chargement du Master Boot Record.");
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
    fprintf(stderr, "ERROR: La taille du volume impossible.\n");
    exit(EXIT_FAILURE);
  }

  if(fc>HDA_MAXCYLINDER){
    fprintf(stderr, "ERROR: Cylindre possible.\n");
    exit(EXIT_FAILURE);
  }

  if(fs>HDA_MAXSECTOR){
    perror("Secteur possible.\n");
    exit(EXIT_FAILURE);
  }

  if(fc==0 && fs==0) {
    fprintf(stderr, "ERROR: Possible de creer un volume a la place du Master Boot Record.\n");
    exit(EXIT_FAILURE);
  }
  
  /* creation du volume */
  if(!make_vol(fc, fs, nb_bloc)){
    fprintf(stderr, "ERROR: La creation du volume est impossible ");
    fprintf(stderr, "avec les carateristiques donnees en parametre.\n");
    exit(EXIT_FAILURE);
  }

  /* sauvegarde de tous les changements effectué sur le mbr */
  save_mbr();

  exit(EXIT_SUCCESS);
}
