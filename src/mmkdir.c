#include "filesys/dir.h"
#include "shell/commande.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {

	unsigned int status;
	unsigned int i, cpt = 0;
	unsigned int size;
	unsigned char* dir_path;
	unsigned char* test_path;
	unsigned char* tmp;

	if(argc < 1) {
		fprintf(stderr, "Erreur : il manque le nom du dossier à créer\n");
		exit(EXIT_FAILURE);
	}

	size = strlen(argv[1]);
	dir_path = malloc(sizeof(char) * MAX_PATH);
	tmp = malloc(sizeof(char) * size);

	strcpy(dir_path, current_path);

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