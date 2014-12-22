#include "inode.h"

void read_inode(const unsigned int inumber, struct inode_s *inode){
  read_bloc_n(current_vol, inumber, (unsigned char*)inode, sizeof(struct inode_s));
}

void write_inode(const unsigned int inumber, const struct inode_s *inode){
  write_bloc_n(current_vol, inumber, (unsigned char*)inode, sizeof(struct inode_s));
}

unsigned int create_inode(const enum inode_type_e type){
  
  unsigned int inumber;
  struct inode_s inode;

  if( !memset(&inode, 0, sizeof(struct inode_s)) ){
    fprintf(stderr, 
	    "Erreur lors du remplissage de la zone mémoire de l'inode lors de sa création.\n");
    return 0;
  }

  inode.in_type = type;
  inumber = new_bloc();

  if( !inumber ){
    fprintf(stderr, "Disque plein, impossible de créer un nouvel inode.\n");
    return 0;
  }

  write_inode(inumber, &inode);

  return inumber;
}

int delete_inode(const unsigned int inumber){
  
  struct inode_s inode;
  unsigned int blocs[NNBPB];
  unsigned int blocs2[NNBPB];
  int i;

  read_inode(inumber, &inode);
  free_blocs(inode.in_direct, N_DIRECT);

  read_bloc(current_vol, inode.in_indirect, (unsigned char*)blocs);
  free_blocs(blocs, NNBPB);
  free_bloc(inode.in_indirect);

  read_bloc(current_vol, inode.in_d_indirect, (unsigned char*)blocs);

  for(i=0; i<NNBPB; i++) {
    read_bloc(current_vol, blocs[i], (unsigned char*)blocs2);
    free_blocs(blocs, NNBPB);
  }

  free_bloc(inode.in_d_indirect);

  return 1;
}


unsigned int vbloc_of_fbloc(const unsigned int inumber, 
                            const unsigned int fbloc,
                            const unsigned int do_allocate){
  
  unsigned int position = fbloc;
  struct inode_s inode;
  unsigned int blocs[NNBPB];
  unsigned int blocs2[NNBPB];
  unsigned int i = 0;

  const char* all_err = "N'a pas pu allouer un nouveau bloc,";
  const char* mem_err = "Erreur memset, lors de l'allocution d'un nouveau bloc.";

  read_inode(inumber, &inode);


  /* ---- DIRECTS ---- */

  if(position<N_DIRECT){
    
    /* si le numero de bloc a cette position est null */
    if(inode.in_direct[position] == BLOC_NULL){

      if(!do_allocate){
	return BLOC_NULL;
      }

      inode.in_direct[position] = new_bloc();
      if(!inode.in_direct[position]){
	fprintf(stderr, "%s dans les blocs directs.\n", all_err);
	return 0;
      }
      write_inode(inumber, &inode);
      return inode.in_direct[position];
    }

    /* on n'a pas eu besoin de faire des allocations */
    return inode.in_direct[position];
  }


  /* ---- INDIRECTS ---- */

  position -= N_DIRECT;

  if(position<NNBPB){

    /* si le numero du bloc d'indirection est null */
    if(inode.in_direct == BLOC_NULL){

      if(!do_allocate){
	return BLOC_NULL;
      } 

      inode.in_indirect = new_bloc();
      if(!inode.in_indirect){	  
	fprintf(stderr, "%s pour le bloc d'indirection.\n", all_err);
	return 0;
      }
	
      if( !memset(blocs, 0, sizeof(blocs)) ){
	fprintf(stderr, "%s\n", mem_err);
	return 0;
      }

      blocs[position] = new_bloc();
      if(!blocs[position]){
	fprintf(stderr, "%s dans les blocs d'indirection.\n", all_err);
	return 0;
      }

      write_bloc(current_vol, inode.in_indirect, (unsigned char*)&blocs);
      write_inode(inumber, &inode);
      return blocs[position];

    }

    read_bloc(current_vol, inode.in_indirect, (unsigned char*)&blocs);

    /* si le numero de bloc a cette position est null */
    if(blocs[position] == BLOC_NULL){

      if(!do_allocate){
	return BLOC_NULL;
      }

      blocs[position] = new_bloc();
      if(!blocs[position]){
	fprintf(stderr, "%s dans les blocs d'indirection.\n", all_err);
	return 0;
      }
      write_bloc(current_vol, inode.in_indirect, (unsigned char*)&blocs);
      return blocs[position];
    }

    /* on n'a pas eu besoin de faire des allocations */
    return blocs[position];
  }


  /* ---- DOUBLE INDIRECT ---- */

  position -= NNBPB;

  if(position < NNBPB*NNBPB){

    /* si le numero du bloc de double indirection est null */
    if(inode.in_d_indirect == BLOC_NULL){

      if(!do_allocate){
	return BLOC_NULL;
      } 
      
      /* cherché l'indice de la première indirection */
      for(i=0; position>=NNBPB && i<NNBPB; i++){
	position -= NNBPB;
      }

      if(i==NNBPB){
	fprintf(stderr, "fbloc trop grand pour un inoeud");
	return 0;
      }
	
      if( !memset(blocs, 0, sizeof(blocs)) ){
	fprintf(stderr, "%s\n", mem_err);
	return 0;
      }

      if( !memset(blocs2, 0, sizeof(blocs2)) ){
	fprintf(stderr, "%s\n", mem_err);
	return 0;
      }

      /* on commence a alloué la chaine d'indirection */
      inode.in_d_indirect = new_bloc();
      if(!inode.in_d_indirect){	  
	fprintf(stderr, "%s pour le bloc de double indirection.\n", all_err);
	return 0;
      }
      write_inode(inumber, &inode);

      blocs[i] = new_bloc();
      if(!blocs[i]){
	fprintf(stderr, "%s dans le premier niveau d'indirection double.\n", all_err);
	return 0;
      }
      write_bloc(current_vol, inode.in_d_indirect, (unsigned char*)&blocs);

      blocs2[position] = new_bloc();
      if(!blocs2[position]){
	fprintf(stderr, "%s dans le deuxième niveau d'indirection double.\n", all_err);
	return 0;
      }
      write_bloc(current_vol, blocs[i], (unsigned char*)&blocs2);

      return blocs2[position];
    }

    read_bloc(current_vol, inode.in_d_indirect, (unsigned char*)&blocs);

    for(i=0; i<NNBPB; i++){

      if(position<NNBPB){
	
	if(blocs[i] == BLOC_NULL){

	  if( !memset(blocs2, 0, sizeof(blocs2)) ){
	    fprintf(stderr, "%s\n", mem_err);
	    return 0;
	  }

	  blocs2[position] = new_bloc();
	  if(!blocs2[position]){
	    fprintf(stderr, "%s dans le deuxième niveau d'indirection double.\n", all_err);
	    return 0;
	  }
	  write_bloc(current_vol, blocs[i], (unsigned char*)&blocs2);

	  return blocs2[position];
	}

	read_bloc(current_vol, blocs[i], (unsigned char*)&blocs2);
	if(blocs2[position] == BLOC_NULL){

	  if(!do_allocate){
	    return BLOC_NULL;
	  }

	  blocs2[position] = new_bloc();
	  if(!blocs2[position]){
	    fprintf(stderr, "%s dans le deuxième niveau d'indirection double.\n", all_err);
	    return 0;
	  }
	  write_bloc(current_vol, blocs[i], (unsigned char*)&blocs2);

	  return blocs2[position];
	}

	/* on n'a pas eu besoin de faire des allocations */
	return blocs2[position];
      }

      position -= NNBPB;

    }
  }
  
  /* ---- FIN BLOCS ADRESSABLE DANS UN INOEUD ---- */
  fprintf(stderr, "fbloc trop grand pour un inoeud");
  return BLOC_NULL;
}
