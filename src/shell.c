#include "hw/hw.h"

#include "shell/commande.h"
#include "filesys/ifile.h"

char* current_path;

int main() {

    char entry[MAX_ENTRY];
    char* command[MAX_COMMAND];
    unsigned cpt = 0, num_option = 0, command_length = 0;

    /* init hardware */
    if(!boot()){
        fprintf(stderr, "FATAL: L'initialisation du matériels a échoué.\n");
        exit(EXIT_FAILURE);
    }

    /* chargement du mbr */
    if(!load_mbr()){
        fprintf(stderr, "FATAL: Erreur lors du chargement du Master Boot Record.\n");
        exit(EXIT_FAILURE);
    }

    if(!mount(MAIN_VOLUME)){
        fprintf(stderr, "FATAL: Impossible de monté le disque principal.\n");
        exit(EXIT_FAILURE);
    }

    /* allocation des options de la commande */
    for(cpt=0; cpt < MAX_COMMAND; cpt++) {
        command[cpt] = malloc(MAX_OPTION * sizeof(char));
    }
    cpt = 0;

    current_path = malloc(MAX_PATH * sizeof(char));
    current_path[0] = *ROOTNAME;
    
    /* lancement du shell */
    while(strcmp(entry, "exit\n") != 0) {

        /* reinitialisation de tous les parametre pour la prochaine commande */
        memset(entry, '\0', MAX_ENTRY);
        for(cpt=0; cpt<MAX_COMMAND; cpt++) {
            memset(command[cpt], '\0', MAX_OPTION);
        }
        cpt = 0;
        num_option = 0;
        command_length = 0;

        printf("prompt %s$ ", current_path);
        if(fgets(entry, MAX_ENTRY, stdin) == NULL) {
            fprintf(stderr, "ERROR: Une erreur est survenu lors de l'entré de votre commande\n");
            continue;
        }

        while(entry[cpt] != '\n') {

            /* creation de la commande */
            if(entry[cpt] == ' ') {
                num_option++;
                command_length = 0;
            } else {
                command[num_option][command_length++] += entry[cpt];
            }
            cpt++;
        }

        if(strcmp(entry, "exit\n") != 0) {
            printf("%d\n", num_option);

            if(command[0][0] == '&') {
                printf("not yet\n");
            }
            if(execute(num_option, command) == RETURN_FAILURE){
                fprintf(stderr, "ERROR: La commande n'a pas fonctionné.\n");
            } 
        } else {
            printf("au revoir HAL\n");
        }
    
    }

    umount();

    return EXIT_SUCCESS;
}
