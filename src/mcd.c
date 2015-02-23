#include "shell/commande.h"
#include "filesys/dir.h"

char* current_path;

int main(int argc, char** argv) {
        
    int inumber;
    char* filename;

    if(argc == 0) {
        strcpy(current_path, "/");
        return EXIT_SUCCESS;
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

    return EXIT_SUCCESS;
}
