#include "file_system/inode.h"
#include "file_system/ifile.h"
#include "file_system/tools.h"


static void empty_it(){
  return;
}

void usage(){
  printf("Utilisation: readfile INUMBER\n");
  printf("\n");
  printf("Permets de lire un fichier existant à partir de son inumber.\n");
  printf("\n");
  printf("Exemple:\n");
  printf("\t$readfile 1\n");
}

int main(int argc, char**argv){

  unsigned int i;
  unsigned int inumber = 0;
  file_desc_t fd;

  char* buf = NULL;

  /* gestion des arguments */
  if(argc != 2){
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

  buf = malloc(fd.fds_size*sizeof(char));
  if(!buf){
    fprintf(stderr, "Erreur lors de l'allocation du buffer.\n");
  }

  if(read_ifile(&fd, buf, fd.fds_size) == RETURN_FAILURE){
    fprintf(stderr, "Erreur lors de la lecture dans le fichier\n");
    exit(EXIT_FAILURE);
  }

  close_ifile(&fd);
  
  for (i = 0; i < fd.fds_size; i++) {
    printf("%c", buf[i]);
  }
  printf("\n");

  free(buf);
  buf = NULL;
  exit(EXIT_SUCCESS);
}
