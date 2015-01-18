#include "inode.h"
#include "ifile.h"
#include "tools.h"

static void empty_it(){
  return;
}

void usage(){
  printf("Utilisation: writefile INUMBER BUFFER\n");
  printf("\n");
  printf("Permets l'écriture sur un fichier existant à partir de son inumber.\n");
  printf("\n");
  printf("Exemple:\n");
  printf("\t$writefile 1 \"Contenu texte du fichier d'inumber 1\"\n");
}

int main(int argc, char**argv){

  unsigned int i;
  unsigned int inumber = 0;
  file_desc_t fd;

  /* gestion des arguments */
  if(argc!=3){
    usage();
    exit(EXIT_SUCCESS);
  }

  inumber = atoi(argv[1]);
  if(inumber==0){
    usage();
    exit(EXIT_FAILURE);
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


  /* manipulation du fichier */
  if(open_ifile(&fd, inumber) == RETURN_FAILURE){
    fprintf(stderr, "Erreur lors de l'ouverture du fichier\n");
    exit(EXIT_FAILURE);
  }

  if(write_ifile(&fd, argv[2], strlen(argv[2])+1) == RETURN_FAILURE){
    fprintf(stderr, "Erreur lors de l'ecriture dans le fichier\n");
    exit(EXIT_FAILURE);
  }

  flush_ifile(&fd);
  close_ifile(&fd);

  exit(EXIT_SUCCESS);
}
