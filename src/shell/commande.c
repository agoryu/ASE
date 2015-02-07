#include "shell/commande.h"

int execute(int argc, char** argv) {

    if(argc < 0) {
        fprintf(stderr, "Erreur : pas assez d'argument dans la fonction d'execution\n");
        return 0;
    }

    if(strcmp(argv[0], "cd") == 0) {
        return mcd(argc, argv);

    } else if(strcmp(argv[0], "mkdir") == 0) {
        return mmkdir(argc, argv);

    } else if(strcmp(argv[0], "cat") == 0) {
        return mcat(argc, argv);

    } else if(strcmp(argv[0], "ls") == 0) {
        return mls(argc, argv);

    } else {
        printf("Commande non reconnu\n");
        return 0;
    }
}

int mcd(int argc, char** argv) {
    int inumber;
    char* filename;

    if(argc < 1) {
        strcpy(current_path, "/");
        return 1;
    }

    if(argv[1][0] == '/') {

        inumber = inumber_of_path(argv[1]);
        if(inumber > 0) {
            strcpy(current_path, argv[1]);
        }

    } else {

        filename = malloc((strlen(current_path) + strlen(argv[1])) * sizeof(char));
        strcat(filename, current_path);
        strcat(filename, argv[1]);

        inumber = inumber_of_path(filename);
        if(inumber > 0) {
            strcpy(current_path, filename);
        }
    }

    return 1;
}

int mmkdir(int argc, char** argv) {

    unsigned int status = 0;
    char* dir_path;

    if(argc < 1) {
        fprintf(stderr, "Erreur : il manque le nom du dossier à créer\n");
        return 0;
    }

    /* future chaine contenant le chemin absolu du nouveau dossier */
    dir_path = malloc(sizeof(char) * MAX_PATH);
    
    /* on met la racine dans dir path */
    strcpy(dir_path, current_path);

    /* chemin different si on est a la racine */
    if(strcmp(dir_path, "/") == 0) {
        sprintf(dir_path, "/%s", argv[1]);
    } else {
        sprintf(dir_path, "%s/%s", dir_path, argv[1]);
    }
        
    status = create_file(dir_path, IT_DIR);

    return status;
}

int mcat(int argc, char** argv) {

    file_desc_t fd;
    char* buf;
    char* filename;

    if(argc == 0) {
        return 1;
    }

    filename = malloc((strlen(current_path) + strlen(argv[1])) * sizeof(char));
    strcat(filename, current_path);
    strcat(filename, argv[1]);

    if(open_file(&fd, filename)) {
        buf = malloc(sizeof(char) * fd.fds_size);

        if(buf == NULL) {
            fprintf(stderr, "Erreur : l'allocation du buffer a échoué");
            exit(EXIT_FAILURE);
        }

        read_file(&fd, buf, fd.fds_size);

        printf("%s", buf);
    }

    return 1;
}

int mls(int argc, char** argv) {

    char* entry = malloc(sizeof(char) * 1024);

    printf("pathname = %s\n", current_path);
    
    if(!get_entry(current_path, entry)) {
        fprintf(stderr, "Erreur lors de la lecture du repertoir.\n");
        return 0;
    } 

    if(strcmp(entry, "") == 0) {
        printf("Le dossier est vide\n");
    } else {
        printf("%s\n", entry);
    }

    return 1;
}
