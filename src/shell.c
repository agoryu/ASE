#include "shell/commande.h"
#include "filesys/vol.h"

char* current_path;

int main() {

    char entry[MAX_ENTRY];
    char* command[MAX_COMMAND];
    int cpt = 0, num_option = 0, command_length = 0;

    /* allocation des options de la commande */
    for(cpt=0; cpt < MAX_COMMAND; cpt++) {
        command[cpt] = malloc(MAX_OPTION * sizeof(char));
    }
    cpt = 0;

    mount(CURRENT_VOLUME);

    /* allocation du path */
    current_path = malloc(sizeof(char) * MAX_PATH);
    *current_path = '/';

    /* creation de la racine si elle n'existe pas */
    /*create_file("/", IT_DIR);*/
    

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

        printf("prompt %s> ", current_path);
        if(fgets(entry, MAX_ENTRY, stdin) == NULL) {
            fprintf(stderr, "Une erreur est survenu lors de l'entré de votre commande\n");
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

        printf("entry = %s\n", entry);

        if(strcmp(entry, "exit\n") != 0) {
            if(!execute(num_option, command)) {
                fprintf(stderr, "La commande n'a pas fonctionné.\n");
            } 
        } else {
            printf("en revoir HAL\n");
        }
    
    }

    umount();

    return EXIT_SUCCESS;
}
