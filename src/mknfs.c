#include "hw/hw.h"
#include "filesys/ifile.h"
#include "filesys/dir.h"


void usage(char* name){
  printf("Make New FileSystem\n");
  printf("\tCreate the file systeme in the chosen device.\n");
  printf("\tIf no device are chosen, the NFS is created for the main Volume.\n");
  printf("\n");
  printf("Usage:\n");
  printf("\t$ %s [VOLUME_ID]\n", name);
  printf("\n");
  printf("Exemple:\n");
  printf("\t$ %s 0\n", name);
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){

  unsigned iroot;
  unsigned chosen_vol;
  
  if(argc == 1){
    chosen_vol = MAIN_VOLUME;
  } else if(argc == 2){
    chosen_vol = atoi(argv[1]);
  } else {
    usage(argv[1]);
  }

  /* init hardware */
  if(!boot()) {
    fprintf(stderr, "FATAL: L'initialisation du matériels a échoué.\n");
    exit(EXIT_FAILURE);
  }

  /* chargement du mbr */
  if(!load_mbr()){
    fprintf(stderr, "WARNING: Disque viège ou corrompu.\n");
    exit(EXIT_SUCCESS);
  }

  if(!mount(chosen_vol)){
    fprintf(stderr, "ERROR: Super bloc invalide.\n");
    exit(EXIT_FAILURE);
  }
  
  if(!has_root()){
    iroot = create_ifile(IT_DIR);
    init_root(iroot);
    add_entry(iroot, iroot, ROOTNAME);
  } else {
    fprintf(stderr, "WARNING: Le root existe déjà.\n");
    exit(EXIT_SUCCESS);
  }

  if(!umount()){
    fprintf(stderr, "ERROR: Problème lors du démontage du disque.\n");
    exit(EXIT_FAILURE);
  }

  printf("Le système de fichiers a été créé avec succès.\n");  

  /* sauvegarde de tous les changements effectué sur le mbr */
  save_mbr();

  exit(EXIT_SUCCESS);
}
