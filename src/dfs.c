#include "hw/hw.h"
#include "filesys/super.h"


void usage(char* name){
    printf("Display FileSystem\n");
    printf("\tDisplay the partitions and the amount of available disk space for the current one.\n");
    printf("\n");
    printf("Usage:\n");
    printf("\t$ %s\n", name);
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){

    if(argc!=1){
        usage(argv[0]);
    }

    /* init hardware */
    if(!boot()){
        fprintf(stderr, "FATAL: L'initialisation du matériels a échoué.\n");
    }
	
    /* chargement du mbr */
    if(!load_mbr()){
        fprintf(stderr, "WARNING: Disque viège ou corrompu.\n");
        exit(EXIT_SUCCESS);
    }

    if(!load_super(MAIN_VOLUME)){
        fprintf(stderr, "ERROR: Super bloc invalide.\n");
        exit(EXIT_FAILURE);
    }

    /* afficher les volumes */
    if(!display_vol()){
        exit(EXIT_FAILURE);
    }

    printf("Il reste %d espace dans le volume courant.\n", get_nb_free_bloc());
    exit(EXIT_SUCCESS);
}
