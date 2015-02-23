#include "shell/commande.h"

char* current_path;

int main(int argc, char** argv) {
	
    char* filename;
    file_desc_t fd;
    char* buf;

    if(argc == 0) {
        exit(EXIT_SUCCESS);
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

    return EXIT_SUCCESS;
}
