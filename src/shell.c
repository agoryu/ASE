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
    if(inumber_of_path("/") != 0) {
        add_entry(0, 0, "/");
    }

    /* lancement du shell */
    while(strcmp(entry, "exit\n") != 0) {

        printf("prompt %s> ", current_path);
        if(fgets(entry, MAX_ENTRY, stdin) == NULL) {
            fprintf(stderr, "Une erreur est survenu lors de l'entré de votre commande\n");
            continue;
        }

        while(entry[cpt] != '\n') {

            /* creation de la commande */
            if(entry[cpt] == ' ') {
                num_option++;
            } else {
                command[num_option][command_length++] += entry[cpt];
            }
            cpt++;
        }

        command[num_option][command_length++] += entry[cpt];

        if(strcmp(entry, "exit\n") != 0) {
            execute(num_option, command);
        }

        cpt = 0;
        num_option = 0;
        command_length = 0;
    }

    umount();

    return EXIT_SUCCESS;
}
