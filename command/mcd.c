#include "commande.h"

int main(int argc, char** argv) {
	
	int inumber;
	char* filename;

	if(arg == NULL) {
		return;
	}

	if(arg[1][0] == '/') {
		
		inumber = inumber_of_path(arg[1]);
		if(inumber > 0) {
			strcpy(current_path, arg[1]);
		}

	} else {

		filename = malloc((strlen(current_path) + strlen(arg[1])) * sizeof(char));
		strcat(filename, current_path);
		strcat(filename, arg[1]);

		inumber = inumber_of_path(filename);
		if(inumber > 0) {
			strcpy(current_path, filename);
		}
	}

	return EXIT_SUCCESS;
}