#include "hw/hw.h"
#include "filesys/super.h"

#define GOOD_SUM 42

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
  printf("\t$ %s -fc 0 -fs 1 -s 32767\n", name);
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){
	
  unsigned fc = HDA_MAXCYLINDER+1;
  unsigned fs = HDA_MAXSECTOR+1;
  unsigned nb_bloc = 4;
  unsigned vol, i;
  char reponse_utilisateur;
  unsigned check_sum = 0;

  if(argc != 7){
    usage(argv[0]);
  }

  /* init materiels */
  if(!boot()){
    fprintf(stderr, "FATAL: L'initialisation du matériels a échoué.\n");
    exit(EXIT_FAILURE);
  }

  /* chargement du mbr */
  if(!load_mbr()){
    printf("Le disque est vierge, voulez-vous continuer les traitements? (y/n) ");
    reponse_utilisateur = getchar();
    
    if(reponse_utilisateur == 'y'){
      init_mbr();
    } else {
      printf("Aucun changement effectue sur le disque.\n");
      exit(EXIT_SUCCESS);
    }
  }

  /* recuperer les arguments */
  for(i=1; i<argc; i++){

    if((check_sum>=4 && check_sum<10) ||
       check_sum==20 || check_sum==22 || check_sum==34){
      break;
    }

    if(strcmp(argv[i], "-s") == 0){
      nb_bloc = atoi(argv[i+1]);
      check_sum += 10;
    }

    if(strcmp(argv[i], "-fc") == 0){
      fc = atoi(argv[i+1]);
      check_sum += 30;
    }

    if(strcmp(argv[i], "-fs") == 0){
      fs = atoi(argv[i+1]);
      check_sum += 2;
    }
  }

  if(check_sum != GOOD_SUM){
    fprintf(stderr, "WARNING: Arguments frauduleux !\n");
    usage(argv[0]);
  }

  /* gestion des erreur d'arguments */
  if(nb_bloc <= 0 || nb_bloc>=(HDA_MAXCYLINDER*HDA_MAXSECTOR)){
    fprintf(stderr, "ERROR: La taille du volume impossible.\n");
    exit(EXIT_FAILURE);
  }

  if(fc>HDA_MAXCYLINDER){
    fprintf(stderr, "ERROR: Numéro de cylindre supérieur au nombre de cylindre possible.\n");
    exit(EXIT_FAILURE);
  }

  if(fs>HDA_MAXSECTOR){
    fprintf(stderr, "ERROR: Numéro de secteur supérieur au nombre de secteur possible.\n");
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

  vol = mbr.mbr_n_vol-1;

  init_super(vol);

  /* sauvegarde de tous les changements effectué sur le mbr */
  save_mbr();

  exit(EXIT_SUCCESS);
}
