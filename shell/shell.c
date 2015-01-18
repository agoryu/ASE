#include "commande.h"
#include "../file/vol.h"

char* current_path;

static void empty_it(){
  return;
}

int main(int argc, char** argv) {
	
	char entry[MAX_ENTRY];
	char* command[MAX_COMMAND];
	int cpt = 0, num_option = 0, command_length = 0;
	char* command_path = malloc(sizeof(char) * (MAX_COMMAND + strlen(COMMAND_PATH)));

	unsigned fc = 0;
  	unsigned fs = 1;
  	unsigned nb_bloc = 512;

	/* on met le path de la commande dans une variable */
	strcpy(command_path, COMMAND_PATH);

	/* allocation des options de la commande */
	for(cpt=0; cpt < MAX_COMMAND; cpt++) {
		command[cpt] = malloc(MAX_OPTION * sizeof(char));
	}
	cpt = 0;

	/* allocation du path */
	current_path = malloc(sizeof(char) * MAX_PATH);
	*current_path = '/';

	/* init hardware */
  	if(!init_hardware(HW_CONFIG)){
    	perror("Initialization error\n");
    	exit(EXIT_FAILURE);
  	}

  	/* Interreupt handlers */
  	for(cpt=0; cpt<16; cpt++) {
    	IRQVECTOR[cpt] = empty_it;
    }
    cpt = 0;

  	/* Allows all IT */
  	_mask(1);

  	/* chargement du mbr */
  	if(!load_mbr()){
    	perror("Erreur lors du chargement du Master Boot Record.");
    	exit(EXIT_FAILURE);
  	}

  	/* on créé le volume principale */
  	if(mbr.mbr_n_vol < 1) {

    	/* creation d'un volume bidon */
    	if(!make_vol(fc, fs, nb_bloc)) {
      		fprintf(stderr, "Erreur a la creation d'un volume bidon.\n");
      		exit(EXIT_FAILURE);
    	}

    	/* initialise le super du volume 1 */
    	init_super(current_vol);

    	/* sauvegarde de tous les changements effectué sur le mbr */
    	save_mbr();

    	/* charge le super du premier volume dans la variable globale */
  		load_super(CURRENT_VOLUME);
  	}

	/* lancement du shell */
	while(strcmp(entry, "exit\n") != 0) {

		printf("prompt %s> ", current_path);
		fgets(entry, MAX_ENTRY, stdin);

		while(entry[cpt] != '\n') {
			
			/* creation de la commande */
			if(entry[cpt] == ' ') {
				num_option++;
			} else {
				command[num_option][command_length++] += entry[cpt];
			}
			cpt++;
		}
		
		cpt = 0;
		num_option = 0;
		command_length = 0;

		/* on va chercher les commande dans le bon dossier */
		strcat(command_path, command[0]);
		execvp(command_path, command);
		sprintf(command_path, "%s", COMMAND_PATH);
	}

	return EXIT_SUCCESS;
}