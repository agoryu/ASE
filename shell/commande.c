#include "commande.h"

char* current_path;

void ls(const char** arg) {

	
}

void cat(const char** arg) {

	char* filename;
	file_desc_t fd;
	char* buf;
	unsigned int i;

	if(arg == NULL) {
		return;
	}

	filename = malloc((strlen(current_path) + strlen(arg[1])) * sizeof(char));
	strcat(filename, current_path);
	strcat(filename, arg[1]);

	if(open_file(&fd, filename)) {
		buf = malloc(sizeof(char) * fd.fds_size);

		if(buf == NULL) {
			fprintf(stderr, "oups");
			return;
		}

		read_file(&fd, buf, fd.fds_size);

		printf("%s", buf);
	}

}

void cd(const char** arg) {

	int inumber;
	char* filename;

	if(arg == NULL) {
		return;
	}

	if(arg[1][0] == '/') {
		
		inumber = inumber_of_path(arg[1]);
		if(inumber > 0) {
			current_path = arg[1];
		}

	} else {

		filename = malloc((strlen(current_path) + strlen(arg[1])) * sizeof(char));
		strcat(filename, current_path);
		strcat(filename, arg[1]);

		inumber = inumber_of_path(filename);
		if(inumber > 0) {
			current_path = filename;
		}
	}

}