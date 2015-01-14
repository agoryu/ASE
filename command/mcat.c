#include "commande.h"

int main(int argc, char** argv) {
	
	char* filename;
	file_desc_t fd;
	char* buf;

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

	return EXIT_SUCCESS;
}