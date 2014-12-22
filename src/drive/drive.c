#include "drive.h"


/* Fonction du fichier drive.c
 *
 * Retourne vrai si le cylindre en parametre est correct
 */
int is_valid_cylinder(const unsigned int cylinder){
  if( cylinder<0 || cylinder>=HDA_MAXCYLINDER ){
    perror("Invalid cylinder\n");
    return 0;
  }
  return 1;
}


/* Fonction du fichier drive.c
 *
 * Retourne vrai si le secteur en parametre est correct
 */
int is_valid_sector(const unsigned int sector){
  if( sector<0   || sector>=HDA_MAXSECTOR ){
    perror("Invalid sector\n");
    return 0;
  }
  return 1;
}


/* Fonction du fichier drive.c
 *
 * Change la position de la tete de lecteur du disque
 */
int seek_sector(const unsigned int cylinder, const unsigned int sector){

  if(!is_valid_cylinder(cylinder) || !is_valid_sector(sector)){
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

  int i;  


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
}


void write_sector(const unsigned int cylinder, 
		  const unsigned int sector, 
		  const unsigned char* buffer){
  write_sector_n(cylinder, sector, buffer, HDA_SECTORSIZE);
}


void write_sector_n(const unsigned int cylinder, 
		    const unsigned int sector, 
		    const unsigned char* buffer,
		    const size_t n){
  
  int i;

  if( !seek_sector(cylinder, sector) ){
    buffer = NULL;
    return;
  }

  /* ecrire les donnees dans MASTERBUFFER */
  for(i=0; i<HDA_SECTORSIZE && i<n; i++){
    MASTERBUFFER[i] = buffer[i];
  }

  /* Mettre dans le disque les donnees de MASTERBUFFER */
  _out(HDA_DATAREGS, 0);
  _out(HDA_DATAREGS, 1);
  _out(HDA_CMDREG, CMD_WRITE);
  _sleep(HDA_IRQ);

}


void format_sector(const unsigned int cylinder, 
		   const unsigned int sector, 
		   const unsigned int nsector, 
		   const unsigned int value){
  
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

}

void format_all(const unsigned int value){

  unsigned int i, j;

  for(i=0; i<HDA_MAXCYLINDER; i++){
    for(j=0; j<HDA_MAXSECTOR; j++){
      format_sector(i, j, 0, value);
    }
  }
}

