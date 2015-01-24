#include "shell/commande.h"

int execute(int argc, char** argv) {

    if(argc < 1) {
        fprintf(stderr, "Erreur : pas assez d'argument dans la fonction d'execution\n");
        return 0;
    }

    if(strcmp(argv[1], "cd") == 0) {
        return mcd(argc, argv);
    } else if(strcmp(argv[1], "mkdir") == 0) {
        return mmkdir(argc, argv);
    } else if(strcmp(argv[1], "mkdir") == 0) {
        return mmkdir(argc, argv);
    } else if(strcmp(argv[1], "cat") == 0) {
        return mcat(argc, argv);
    }
    return 1;
}

int mcd(int argc, char** argv) {
    int inumber;
    char* filename;

    if(argc < 1) {
        strcpy(current_path, "/");
        return 0;
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

    unsigned int status;
    unsigned int i, cpt = 0;
    unsigned int size;
    char* dir_path;
    char* tmp;

    if(argc < 1) {
        fprintf(stderr, "Erreur : il manque le nom du dossier à créer\n");
        exit(EXIT_FAILURE);
    }

    size = strlen(argv[1]);
    dir_path = malloc(sizeof(char) * MAX_PATH);
    tmp = malloc(sizeof(char) * size);

    sprintf(dir_path, "%s", current_path);

    if(strstr(argv[1], "/") == 0) {
        for(i=0; i<size - 1; i++) {
            if(argv[1][i] == '/') {
                strcat(dir_path, tmp);
                strcat(dir_path, "/");
                cpt = 0;
            } else {
                tmp[cpt++] = argv[1][i];
            }
        }

        if(inumber_of_path(dir_path) != 0) {
            strcat(dir_path, tmp);
            status = create_file(dir_path, IT_DIR);
        }

    } else {
        sprintf(dir_path, "%s/%s", dir_path, argv[1]);
        status = create_file(dir_path, IT_DIR);
    }

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
