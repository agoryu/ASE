#include "commande.h"

char* current_path;

int main(int argc, char** argv) {
	
	char entry[MAX_ENTRY_LENGTH];
	char* command[MAX_COMMAND_LENGTH];
	int cpt = 0, num_option = 0, command_length = 0;

	for(cpt=0; cpt < MAX_COMMAND_LENGTH; cpt++) {
		command[cpt] = malloc(MAX_OPTION_LENGTH * sizeof(char));
	}

	while(strcmp(entry, "exit") != 0) {

		printf("prompt> ");
		fgets(entry, MAX_ENTRY_LENGTH, stdin);

		while(entry[cpt] != '\n') {
			if(entry[cpt] == ' ') {
				num_option++;
			} else {
				command[num_option][command_length++] += entry[cpt];
			}
			cpt++;
		}

		execvp(command[0], command);
	}

	return EXIT_SUCCESS;
}