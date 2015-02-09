#include "drive/drive.h"


/* Fonction du fichier drive.c
 *
 * Retourne vrai si le cylindre en parametre est correct
 */
unsigned is_valid_cylinder(const unsigned cylinder){
  if(cylinder>HDA_MAXCYLINDER){
    return 0;
  }
  return 1;
}


/* Fonction du fichier drive.c
 *
 * Retourne vrai si le secteur en parametre est correct
 */
unsigned is_valid_sector(const unsigned sector){
  if(sector>HDA_MAXSECTOR){
    return 0;
  }
  return 1;
}


/* Fonction du fichier drive.c
 *
 * Retourne vrai si le cylindre et le secteur en parametre sont corrects.
 */
unsigned is_valid_parameter(const unsigned cylinder, const unsigned sector){
  return is_valid_cylinder(cylinder) && is_valid_sector(sector);
}


/* Fonction du fichier drive.c
 *
 * Change la position de la tete de lecteur du disque
 */
static unsigned seek_sector(const unsigned int cylinder, const unsigned int sector){

  if(!is_valid_cylinder(cylinder)){
    fprintf(stderr, "ERROR: Cylindre invalide.\n");
    return 0;
  }

  if(!is_valid_sector(sector)){
    fprintf(stderr, "ERROR: Secteur invalide.\n");
    return 0;
  }


  /* On se position sur le disque dur */
  _out(HDA_DATAREGS, (cylinder>>8 & 0xFF) );
  _out(HDA_DATAREGS+1, (cylinder & 0xFF) );
  _out(HDA_DATAREGS+2, (sector>>8 & 0xFF) );
  _out(HDA_DATAREGS+3, (sector & 0xFF) );

  _out(HDA_CMDREG, CMD_SEEK);
  _sleep(HDA_IRQ);

  return 1;

}


void read_sector(const unsigned int cylinder, 
		 const unsigned int sector, 
		 unsigned char* buffer){
  read_sector_n(cylinder, sector, buffer, HDA_SECTORSIZE);
}


void read_sector_n(const unsigned int cylinder, 
		   const unsigned int sector, 
		   unsigned char* buffer, 
		   const size_t n){

  unsigned i;  

  if(!is_valid_parameter(cylinder, sector)){
    return;
  }

  /* TODO prendre verror, si pris yeild */
  printf("read %d %d\n", cylinder, sector);

  if( !seek_sector(cylinder, sector) ){
    buffer = NULL;
    return;
  }


  /* Mettre dans MASTERBUFFER les donnees a cette emplacement */
  _out(HDA_DATAREGS, 1);
  _out(HDA_CMDREG, CMD_READ);
  _sleep(HDA_IRQ);


  /* exploiter les donnees de MASTERBUFFER */
  for(i=0; i<HDA_SECTORSIZE && i<n; i++){
    buffer[i] = (unsigned char)MASTERBUFFER[i];
  }
  
  /* TODO laché verrou */
  
}


void write_sector(const unsigned int cylinder, 
		  const unsigned int sector, 
		  const unsigned char* buffer){
  write_sector_n(cylinder, sector, buffer, HDA_SECTORSIZE-1);
}


void write_sector_n(const unsigned int cylinder, 
		    const unsigned int sector, 
		    const unsigned char* buffer,
		    const size_t n){
  
  unsigned i;

  if(!is_valid_parameter(cylinder, sector)){
    return;
  }

  /* TODO prendre verror, si pris yeild */
  printf("write %d %d\n", cylinder, sector);

  if( !seek_sector(cylinder, sector) ){
    buffer = NULL;
    return;
  }


  /* ecrire les donnees dans MASTERBUFFER */
  for(i=0; i<HDA_SECTORSIZE-1 && i<n; i++){
    MASTERBUFFER[i] = buffer[i];
  }
  
  
  /* Mettre dans le disque les donnees de MASTERBUFFER */
  _out(HDA_DATAREGS, 0);
  _out(HDA_DATAREGS, 1);
  _out(HDA_CMDREG, CMD_WRITE);
  _sleep(HDA_IRQ);

  /* TODO laché verrou */
}


void format_sector(const unsigned int cylinder, 
		   const unsigned int sector, 
		   const unsigned int nsector, 
		   const unsigned int value){

  if(!is_valid_parameter(cylinder, sector)){
    fprintf(stderr, "ERROR: Tentative de formatage sur cylindre ou secteur invalide.\n");
    return;
  }

  /* TODO prendre verror, si pris yeild */
  
  if( !seek_sector(cylinder, sector) ){
    return;
  }

  _out(HDA_DATAREGS, (nsector>>8 & 0xFF) );
  _out(HDA_DATAREGS+1, (nsector & 0xFF) );
  _out(HDA_DATAREGS+2, (value>>24 & 0xFF) );
  _out(HDA_DATAREGS+3, (value>>16 & 0xFF) );
  _out(HDA_DATAREGS+4, (value>>8 & 0xFF) );
  _out(HDA_DATAREGS+5, (value & 0xFF) );

  _out(HDA_CMDREG, CMD_FORMAT);
  _sleep(HDA_IRQ);

  /* TODO laché verrou */

}

void format_all(const unsigned int value){

  unsigned int i, j;

  for(i=0; i<HDA_MAXCYLINDER; i++){
    for(j=0; j<HDA_MAXSECTOR; j++){
      format_sector(i, j, 0, value);
    }
  }
}

