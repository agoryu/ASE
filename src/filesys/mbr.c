#include "filesys/mbr.h"


struct mbr_s mbr;
int current_vol;


/* Fonction du fichier mbr.c
 *
 * fonction qui permet de calculer la valeur du secteur et du cylindre
 * à partir du volume et du nombre de bloc
 */
void calc_secteur_cylindre(const unsigned vol, 
			   const unsigned nbloc,
			   unsigned *secteur,
			   unsigned *cylindre) {

  unsigned fs, fc;

  fs = mbr.mbr_vol[vol].vol_first_sector;
  fc = mbr.mbr_vol[vol].vol_first_cylinder;

  *secteur = (fs + nbloc) % HDA_MAXSECTOR;
  *cylindre = fc + (fs + nbloc) / HDA_MAXSECTOR;
}


/* Fonction du fichier mbr.c
 *
 * retourne 1 si l'espace (a partir de cylinder et sector) est libre 
 * sur nbloc blocs.
 */
int is_free_space(const unsigned cylinder, 
		  const unsigned sector,
		  const unsigned nbloc){
  
  unsigned new_deb, new_fin;
  unsigned i, i_deb, i_fin;


  /* debut et fin du volume donne en parametre, d'un point de vue unidimensionnel */
  new_deb = cylinder * HDA_MAXSECTOR + sector;
  new_fin = new_deb + nbloc;

  for(i=0; i<mbr.mbr_n_vol; i++){

    i_deb = mbr.mbr_vol[i].vol_first_cylinder * HDA_MAXSECTOR + mbr.mbr_vol[i].vol_first_sector;
    i_fin = i_deb + mbr.mbr_vol[i].vol_n_sector;

    if( !((i_deb<new_deb && i_fin<new_deb) || (new_fin<i_deb && new_fin<i_fin)) ){
      return 0;
    }
  }  

  return 1;
}


void init_mbr(){
  mbr.mbr_n_vol = 0;
  mbr.mbr_magic = MBR_MAGIC;
}


int load_mbr() {
  
  char reponse_utilisateur;

  /* si le mbr est plus grand que le secteur on ne pourra pas le mettre dedans */
  if(sizeof(struct mbr_s) > HDA_SECTORSIZE) {
    fprintf(stderr, "ERROR: le secteur size est plus petit que la taille du mbr");
    return 0;
  }

  /* lecture du mbr */
  read_sector_n(0, 0, (unsigned char*)&mbr, sizeof(struct mbr_s));

  /* premiere fois que le disque est utilisé */
  if(mbr.mbr_magic != MBR_MAGIC) {

    printf("Le disque est vierge, voulez-vous continuer les traitements? (y/n) ");
    reponse_utilisateur = getchar();
    
    if(reponse_utilisateur == 'y'){
      init_mbr();
    } else {
      printf("Aucun changement effectue sur le disque.\n");
      exit(EXIT_SUCCESS);
    }
  }

  return 1;
}


int save_mbr() {
  write_sector_n(0, 0, (unsigned char*)&mbr, sizeof(struct mbr_s));
  return 1;
}


void read_bloc_n(const unsigned vol, 
         const unsigned nbloc, 
         unsigned char* buffer,
         const size_t nb_bloc) {
  
  unsigned secteur, cylindre;

  calc_secteur_cylindre(vol, nbloc, &secteur, &cylindre);

  read_sector_n(cylindre, secteur, buffer, nb_bloc);
}


void read_bloc(const unsigned vol, 
	       const unsigned nbloc, 
	       unsigned char* buffer) {

  unsigned secteur, cylindre;

  calc_secteur_cylindre(vol, nbloc, &secteur, &cylindre);

  read_sector(cylindre, secteur, buffer);
}


void write_bloc_n(const unsigned vol, 
         const unsigned  nbloc, 
         unsigned char* buffer,
         const size_t nb_bloc) {

  unsigned secteur, cylindre;

  calc_secteur_cylindre(vol, nbloc, &secteur, &cylindre);

  write_sector_n(cylindre, secteur, buffer, nb_bloc);
}


void write_bloc(const unsigned vol, 
		const unsigned nbloc, 
	        const unsigned char* buffer) {

  unsigned secteur, cylindre;

  calc_secteur_cylindre(vol, nbloc, &secteur, &cylindre);

  write_sector(cylindre, secteur, buffer);
}


void format_vol(const unsigned vol) {

  unsigned secteur, cylindre;
  unsigned fs, fc, nb_bloc;
  unsigned i;

  fs = mbr.mbr_vol[vol].vol_first_sector;
  fc = mbr.mbr_vol[vol].vol_first_cylinder;
  nb_bloc = mbr.mbr_vol[vol].vol_n_sector;

  for(i=0; i<nb_bloc; i++){

    /* passage de cylindre a volume et de secteur a bloc */
    secteur = (fs + i) % HDA_MAXSECTOR;
    cylindre = fc + (fs + i) / HDA_MAXSECTOR;

    /* revoir la valeur a mettre dans le dernier parametre */
    format_sector(cylindre, secteur, 1, 0);

  }
}


int make_vol(const unsigned cylinder, 
	     const unsigned sector, 
	     const unsigned nbloc){

  if(mbr.mbr_n_vol >= MAX_VOL){
    fprintf(stderr, "ERROR: Impossible de creer un nouveau volume.\n");
    fprintf(stderr, "ERROR: Le nombre de volume sur le disque est a son maximum.\n");
    return 0;
  }

  if(is_free_space(cylinder, sector, nbloc)){

    mbr.mbr_vol[mbr.mbr_n_vol].vol_first_cylinder = cylinder;
    mbr.mbr_vol[mbr.mbr_n_vol].vol_first_sector = sector;
    mbr.mbr_vol[mbr.mbr_n_vol].vol_n_sector = nbloc;
    mbr.mbr_vol[mbr.mbr_n_vol].vol_type = VOLT_PR;
    
    current_vol = mbr.mbr_n_vol;
    
    mbr.mbr_n_vol++;

    return 1;

  }

  return 0;
}


unsigned display_vol(){
  unsigned i, nvol;

  if(mbr.mbr_n_vol == 0){
    printf("Aucun volume sur le disque.\n");
    return 0;
  }
  
  nvol = mbr.mbr_n_vol;

  printf("Il y a %d volume(s) sur le disque.\n\n", nvol);

  for(i=0; i<nvol; i++){
    printf("Volume indice %d:\n", i);
    printf("\t - Commence au cylindre %d.\n", mbr.mbr_vol[i].vol_first_cylinder); 
    printf("\t - Commence au secteur %d.\n", mbr.mbr_vol[i].vol_first_sector);
    printf("\t - Nombre de blocs : %d.\n", mbr.mbr_vol[i].vol_n_sector);
    printf("\t - Type de volume: ");
    switch(mbr.mbr_vol[i].vol_type){
    case VOLT_PR:
      printf("PRIMAIRE.\n");
      break;
    case VOLT_SND:
      printf("SECONDAIRE.\n");
      break;
    case VOLT_OTHER:
      printf("AUTRE.\n");
      break;
    }
    printf("\n");
  }
  return 1;

}
