#include "filesys/super.h"
#include "filesys/inode.h"


struct super_s current_super;


unsigned int is_init_super(const struct super_s super);

unsigned int is_init_super(const struct super_s super){
  return super.super_magic == SUPER_MAGIC;
}

unsigned int is_correct_volume(const unsigned vol){
  if(vol > MAX_VOL) {
    fprintf(stderr, 
	    "ERROR: Numero de volume supérieur au nombre de volumes autorisés sur le disque.\n");
    return 0;
  }

  if(mbr.mbr_magic != MBR_MAGIC){
    fprintf(stderr, "ERROR: Le master boot record n'est pas initialisé.\n");
    return 0;
  }

  if(mbr.mbr_n_vol == 0){
    fprintf(stderr, "ERROR: Il n'y a pas de volume dans votre disque.\n");
    return 0;
  }

  if(vol >= mbr.mbr_n_vol){
    fprintf(stderr, "ERROR: Le volume %d n'existe pas, sur le disque.\n", vol+1);
    return 0;
  }

  return 1;
}


void init_super(const unsigned vol) {

  struct super_s super;
  unsigned free_size;

  if(!is_correct_volume(vol)){
    fprintf(stderr, "ERROR: Impossible d'initialiser un super bloc pour le volume %d.\n", vol+1);
    return;
  }

  read_bloc_n(vol, SUPER_BLOC, (unsigned char*)&super, sizeof(struct super_s));
  if(is_init_super(super)){
    return;
  }

  super.super_magic = SUPER_MAGIC;
  super.super_first_free = 1;

  free_size = mbr.mbr_vol[vol].vol_n_sector - 1;

  super.super_n_free = free_size;
  super.super_iroot = 0;
  current_vol = vol;

  write_bloc_n(vol, SUPER_BLOC, (unsigned char*)&super, sizeof(struct super_s));

}

unsigned load_super(const unsigned vol) {

  if(!is_correct_volume(vol)){
    fprintf(stderr, "ERROR: Impossible de charger le super bloc du volume %d.\n", vol+1);
    return 0;
  }

  current_vol = vol;
  read_bloc_n(vol, SUPER_BLOC, (unsigned char*)&current_super, sizeof(struct super_s));
	
  return is_init_super(current_super);
}

unsigned init_root(const unsigned root_inumber){

  if(!is_init_super(current_super)){
    fprintf(stderr, "ERROR: Super bloc non initialisé.\n");
    return 0;
  }

  if(root_inumber == 0) {
    fprintf(stderr, "ERROR: mauvaise valeur passé dans init_root.\n");
    return 0;
  }

  current_super.super_iroot = root_inumber;
  return 1;

}

unsigned has_root(){
  
  /*struct inode_s ind_root;

  read_inode(current_super.super_iroot, &ind_root);

  if(ind_root.in_magic != INODE_MAGIC){
    return 0;
  }
  return 1;*/
  return !(current_super.super_iroot == 0);
}

unsigned get_iroot(){
  return current_super.super_iroot;
}

unsigned mount(unsigned vol){
  return load_super(vol);
}

unsigned umount(){
  return save_current_super();
}

unsigned save_current_super(){
  
  if(!is_correct_volume(current_vol)){
    fprintf(stderr, "ERROR: Volume %d correct.\n", current_vol+1);
    return 0;
  }

  write_bloc_n(current_vol, SUPER_BLOC, (unsigned char*)&current_super, sizeof(struct super_s));
  return 1;
}


unsigned new_bloc() {
	
  struct free_bloc_s free_bloc;
  unsigned new;

  if(!is_correct_volume(current_vol)){
    fprintf(stderr, "ERROR: Le volume courant est incorrect.\n");
    return 0;
  }

  if(!is_init_super(current_super)){
    fprintf(stderr, "ERROR: Le super bloc du volume courant n'est pas initialisé.\n");
    return 0;
  }

  if(current_super.super_n_free <= 0) {
    fprintf(stderr, "ERROR: Plus de place libre sur ce volume.\n");
    return 0;
  }


  read_bloc_n(current_vol, current_super.super_first_free, (unsigned char*)&free_bloc, sizeof(struct free_bloc_s));

  new = current_super.super_first_free;

  if(free_bloc.fb_n_free == 1) {
    current_super.super_first_free = free_bloc.fb_next;
    current_super.super_n_free--;
  } else {
    current_super.super_n_free--;
    current_super.super_first_free++;
    free_bloc.fb_n_free--;

    write_bloc_n(current_vol, current_super.super_first_free, (unsigned char*)&free_bloc, sizeof(struct free_bloc_s));
  }

  if(!save_current_super()){
    fprintf(stderr, "ERROR: Impossible d'enregistrer le super bloc.\n");
    return 0;
  }
  
  return new;
}


void free_bloc(const unsigned int bloc) {

  /* nouveau bloc libre qui sera ajouté à la liste */
  struct free_bloc_s new_free_bloc;

  /* bloc libre permettant de trouver le bloc précédent
     le nouveau bloc libre */
  struct free_bloc_s current_free_bloc;

  unsigned int position = current_super.super_first_free;

  /* variable permettant de vérifier si bloc trouve sa place
     dans la chaine de bloc libre */
  unsigned int trouve = 0;

  if(!is_correct_volume(current_vol)){
    fprintf(stderr, "ERROR: Le volume courant est incorrect.\n");
    return;
  }

  if(!is_init_super(current_super)){
    fprintf(stderr, "ERROR: Le super bloc du volume courant n'est pas initialisé.\n");
    return;
  }

  if(bloc > mbr.mbr_vol[current_vol].vol_n_sector) {
    printf("ERROR: le numero de bloc n'existe pas dans le volume courant\n");
    return;
  }

  if(current_super.super_n_free == 0) {
    current_super.super_first_free = bloc;
    new_free_bloc.fb_n_free = 1;
    new_free_bloc.fb_next = bloc;

  } else if(current_super.super_first_free > bloc) {
    /* cas ou le nouveau bloc libre est avant le premier bloc libre */
    /* pour l'instant le nouveau bloc libre ne 
       sera pas fusionné a une liste de bloc qui serait
       a coté */
    new_free_bloc.fb_n_free = 1;
    new_free_bloc.fb_next = current_super.super_first_free;
    current_super.super_first_free = bloc;

  } else {

    /* on cherche le bloc précédent le nouveau bloc libre */
    while(position < bloc) {
      read_bloc_n(current_vol, position, (unsigned char*)&current_free_bloc, sizeof(struct free_bloc_s));

      /* on verifie que l'on trouve la position du bloc sinon on retourne une erreur */
      if(current_free_bloc.fb_next < bloc) {
      	trouve = 1;
      	break;
      }

      position = current_free_bloc.fb_next;
      printf("next = %d\n", position);
    }

    if(trouve) {
      /* initialisation des chanps du nouveau bloc libre */
      new_free_bloc.fb_next = current_free_bloc.fb_next;
      new_free_bloc.fb_n_free = 1;

      /* modification du champ next du bloc précédent le nouveau
	 bloc libre */
      current_free_bloc.fb_next = bloc;
    } else {
      fprintf(stderr, "ERROR: Le bloc a libérer n'existe pas\n");
      return;
    }
  }

  /* on incremente le nombre de bloc libre */
  current_super.super_n_free++;

  write_bloc_n(current_vol, bloc, (unsigned char*)&new_free_bloc, sizeof(struct free_bloc_s));
}

void free_blocs(const unsigned int * blocs, const size_t size) {

  unsigned int i;

  for(i=0; i<size; i++) {
    free_bloc(blocs[i]);
  }

}

unsigned int is_full() {
  return current_super.super_n_free == 0;
}


unsigned int get_nb_free_bloc() {
  return current_super.super_n_free;
}

