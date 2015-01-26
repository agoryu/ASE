#include "shell/commande.h"
#include "filesys/vol.h"
#include "filesys/ifile.h"
#include "filesys/inode.h"

static void empty_it(){
    return;
}

char* current_path;

int main() {

    char entry[MAX_ENTRY];
    char* command[MAX_COMMAND];
    char* root_name;
    int cpt = 0, num_option = 0, command_length = 0;
    unsigned inumber_racine;
    unsigned i;
    file_desc_t fd;
    struct inode_s inode;

    /* init hardware */
    if(!init_hardware(HW_CONFIG)){
        fprintf(stderr, "Error: Initialization error\n");
        exit(EXIT_FAILURE);
    }

    /* Interreupt handlers */
    for(i=0; i<16; i++)
        IRQVECTOR[i] = empty_it;

    /* Allows all IT */
    _mask(1);

    /* chargement du mbr */
    if(!load_mbr()){
        fprintf(stderr, "Erreur lors du chargement du Master Boot Record.");
        exit(EXIT_FAILURE);
    }

    if(!mount(CURRENT_VOLUME)){
      fprintf(stderr, "Impossible de monté le disque principal.");
      exit(EXIT_FAILURE);
    }

    /* allocation des options de la commande */
    for(cpt=0; cpt < MAX_COMMAND; cpt++) {
        command[cpt] = malloc(MAX_OPTION * sizeof(char));
    }
    cpt = 0;

    /* allocation du path */
    current_path = malloc(sizeof(char) * MAX_PATH);
    *current_path = '/';

    root_name = malloc(sizeof(char) * ENTRYMAXLENGTH);
    *root_name = '/';

    /* creation de la racine si elle n'existe pas */
    /* creation un peu radical qui fait des degat */
    if(inumber_of_path("/") == 0) {
      inumber_racine = create_ifile(IT_DIR);
      add_entry(inumber_racine, inumber_racine, root_name);
    } else {
      fprintf(stderr, "Racine existante\n");
    }
    
    open_ifile(&fd, inumber_racine);
    read_inode(inumber_racine, &inode);
    if(inode.in_type == IT_DIR){
      printf("La racine est un dossier\n");
    }
    

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
