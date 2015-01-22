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

    /* TODO remplacer 0 par le define */
    mount(0);

    /* allocation du path */
    current_path = malloc(sizeof(char) * MAX_PATH);
    *current_path = '/';

    /* lancement du shell */
    while(strcmp(entry, "exit\n") != 0) {

        printf("prompt %s> ", current_path);
        fgets(entry, MAX_ENTRY, stdin);

        while(entry[cpt] != '\n') {

            /* creation de la commande */
            if(entry[cpt] == ' ') {
                num_option++;
            } else {
                command[num_option][command_length++] += entry[cpt];
            }
            cpt++;
        }

        cpt = 0;
        num_option = 0;
        command_length = 0;

        execute(num_option, command);
    }

    umount();

    return EXIT_SUCCESS;
}
