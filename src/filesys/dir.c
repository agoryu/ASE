/* ------------------------------
   $Id: dir.c 7085 2013-10-18 15:37:14Z marquet $
   ------------------------------------------------------------

   Directories manipulation.
   Based on the ifile library.    
   Philippe Marquet, october 2002

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filesys/tools.h"
#include "filesys/ifile.h"
#include "filesys/dir.h"
#include "filesys/super.h"

/*------------------------------
  Private entry manipulation.
  ------------------------------------------------------------*/

int min(int a, int b){
    return (a<b)?a:b;
} 

/* return a new entry index.
   the file descriptor must the one of an open directory */
static unsigned int new_entry(file_desc_t *fd){

    struct entry_s entry;
    unsigned int ientry = 0; /* the new entry index */

    /* seek to begin of dir */
    seek2_ifile(fd, 0);
    
    /* look after a null inumber in an entry */
    while (read_ifile (fd, &entry, sizeof(struct entry_s)) != READ_EOF) {
	if (! entry.ent_inumber)
	    return ientry;
	ientry++;
    }

    /*printf("not new entry\n");*/
    
    /* need to append an entry in the dir */
    return ientry;
}

/* return the entry index or RETURN_FAILURE.
   the file descriptor must the one of an open directory */
static int find_entry(file_desc_t *fd, const char *basename){

    struct entry_s entry;
    unsigned int ientry = 0; /* the entry index */

    /* seek to begin of dir */
    seek2_ifile(fd, 0);
    
    /* look after the right entry */
    while (read_ifile (fd, &entry, sizeof(struct entry_s)) != READ_EOF){
    	if (entry.ent_inumber && ! strcmp(entry.ent_basename, basename)){
    	    return ientry;
    	}
    	ientry++;
    }

    /* not found */
    return RETURN_FAILURE;
}

/*------------------------------
  Create and delete entry
  ------------------------------------------------------------*/
int add_entry(unsigned int idir, unsigned int inumber, const char *basename){

    struct inode_s inode; 
    file_desc_t _fd, *fd = &_fd;
    struct entry_s entry; 
    unsigned int ientry = 0;
    int nbyte; 
    /* TODO confirmation de la suppression */
    /*int status;*/

    printf("FUNCTION: add_entry\n");
    printf("ARG: idir -> %d\n", idir);
    printf("ARG: inumber -> %d\n", inumber);
    printf("ARG: basename -> %s\n", basename);
    
    /* a directory inode? */
    read_inode(idir, &inode); 
    if (inode.in_type != IT_DIR) {
    	fprintf(stderr, "Erreur : Le idir de add_entry n'est pas un dossier\n");
    	return RETURN_FAILURE;
    }
    
    /* open the dir */
    /*status =*/ iopen_ifile(fd, idir, &inode);

    /* the new entry position in the file */
    ientry = new_entry(fd);
    
    /* built the entry */
    entry.ent_inumber = inumber;
    strncpy(entry.ent_basename, basename, ENTRYMAXLENGTH);
    entry.ent_basename[ENTRYMAXLENGTH-1] = 0;

    /* seek to the right position */
    seek2_ifile(fd, ientry * sizeof(struct entry_s));
    
    /* write the entry */
    nbyte = write_ifile(fd, &entry, sizeof(struct entry_s));
    
    /* done */
    close_ifile(fd); /* even in case of write failure */

    if (nbyte == sizeof(struct entry_s)){
	   return RETURN_SUCCESS;
    } else {
	   return RETURN_FAILURE;
    }
}

int del_entry(unsigned int idir, const char *basename){

    struct inode_s inode; 
    file_desc_t _fd, *fd = &_fd;
    struct entry_s entry; 
    unsigned int ientry;
    int status;
    
    /* a directory inode? */
    read_inode(idir, &inode); 
    if (inode.in_type != IT_DIR) 
	return RETURN_FAILURE;
    
    /* open the dir */
    iopen_ifile(fd, idir, &inode);

    /* the entry position in the file */
    status = find_entry(fd, basename);
    if (status == RETURN_FAILURE) {
	close_ifile(fd);
	return RETURN_FAILURE;
    }
    ientry = status; 

    /* built a null entry */
    memset(&entry, 0, sizeof(struct entry_s));
    
    /* seek to that entry */
    seek2_ifile(fd, ientry * sizeof(struct entry_s));

    /* delete the entry = write the null entry */
    status = write_ifile(fd, &entry, sizeof(struct entry_s));

    /* close, and report status */
    close_ifile(fd);
    return status;
}

/*------------------------------
  Looking after entries
  ------------------------------------------------------------*/

/* consider the directory of inumber idir. 
   search after an entry of name basename (which can not contain /). 
   return the inumber of the entry, 0 if no such entry or if idir is
   not a directory.
*/
unsigned int inumber_of_basename(unsigned int idir, const char *basename){

    struct inode_s inode;
    file_desc_t _fd, *fd = &_fd;
    unsigned int ientry;
    struct entry_s entry;
    int status;

    printf("FUNCTION: inumber_of_basename\n");
    printf("ARG: idir -> %d\n", idir);
    printf("ARG: basename -> %s\n", basename);
    
    /* a directory inode? */
    read_inode(idir, &inode);
    if (inode.in_type != IT_DIR){
        fprintf(stderr, "Erreur : le chemin dont le inumber est %d n'est pas un repertoir\n", idir);
    	return 0;
    }

    /* open the dir */
    iopen_ifile(fd, idir, &inode);

    /* the entry position in the file */
    status = find_entry(fd, basename);
    if (status == RETURN_FAILURE){
        fprintf(stderr, "Erreur : l'entrée %s n'a pas été trouvé\n", basename);
	    return 0;
    }
    ientry = status; 

    /* seek to the right position */
    seek2_ifile(fd, ientry * sizeof(struct entry_s));
    
    /* read the entry */
    status = read_ifile(fd, &entry, sizeof(struct entry_s));
    close_ifile(fd); 

    /* the result */
    return entry.ent_inumber;
}

unsigned int inumber_of_path(const char *pathname){

    unsigned int icurrent; 	/* the inumber of the current dir */
    
    printf("FUNCTION: inumber_of_path\n");
    printf("ARG: pathname -> %s\n", pathname);

    /* an *absolute* pathname */
    if (*pathname != '/'){
        fprintf(stderr, "Erreur : le chemin ne commence pas à la racine dans inumber_of_path.\n");
	    return 0;
    }

    /* TODO verif les ligne en dessous */
    /* start at root */
    icurrent = current_super.super_iroot;
    /* fin TODO */
    
    while (*pathname) {
    	printf("%c\n", *pathname);
    	if (*pathname != '/') {
    	    char basename[ENTRYMAXLENGTH];
    	    char *pos;	/* the first / position */
    	    unsigned lg;     	/* the length of the first basename */
        	    
    	    /* length of the leading basename */
    	    pos = strchr(pathname, '/');
    	    lg = pos ? pos-pathname : strlen(pathname);

    	    printf("pathname: %d %s\n", (unsigned)pathname, pathname);
    	    printf("pos: %d %s\n", (unsigned)pos, pos);
    	    printf("length: %d\n", lg);

    	    /* copy this leading basename to basename */
    	    strncpy (basename, pathname, min(ENTRYMAXLENGTH, lg));
    	    basename[min(ENTRYMAXLENGTH, lg)] = 0;

    	    /* look after this basename in the directory.
    	       this entry inumber is the new current */
    	    icurrent = inumber_of_basename(icurrent, basename); 
    	    if (!icurrent){
    		    return 0;
    	    }

    	    /* skip the basename in pathname */
    	    pathname += lg;

    	    /* may end here */ 
    	    if (! *pathname) break;
    	}
    	pathname++ ;
    }
    return icurrent ;
}

unsigned dinumber_of_path(const char *pathname, const char **basename){

    char *dirname = strdup(pathname);
    unsigned int idirname = 0; 
    struct inode_s inode; 

    /* an *absolute* pathname */
    if (*pathname != '/') {
    	/* free dirname strdup() */
        fprintf(stderr, "Erreur : le chemin ne commence pas à la racine dans dinumber_of_path.\n");
    	free(dirname);
    	return idirname;
    }

    /* the last basename (there is at least a '/') */
    *basename = strrchr (pathname, '/');
    
    /*if(strcmp(pathname, ROOTNAME) == 0){*/
    if(*pathname == '/') {
    	/* if pathname isn't root, we want the basename without any '/' */
    	(*basename)++;
    } 

    /* TODO gére le nom de la racine càd '/' */

    /* la valeur initial était 1 a la place du 0 */
    if(((*basename) - pathname) != 0){
    	/* the dirname stops at the last '/'. ugly isn't it! */
    	*(dirname + ((*basename) - pathname)) = 0;
    }

     /* the dirname inumber */
    idirname = inumber_of_path(dirname);
    if (! idirname) {
    	/* free dirname strdup() */
        fprintf(stderr, "Erreur : le chemin n'a pas de inumber dans dinumber_of_path.\n");
    	free(dirname);
    	return idirname;
    }
  
    /* is dirname a directory? */
    read_inode(idirname, &inode); 
    if (inode.in_type != IT_DIR) {
        fprintf(stderr, "Erreur : le chemin n'est pas un dossier dans dinumber_of_path.\n");
        idirname = 0; 
    }
	   

    /* free dirname strdup() */
    free(dirname); 
    
    return idirname;
}

int get_entry(const char* pathname, char* contain) {

    struct inode_s inode; 
    file_desc_t _fd, *fd = &_fd;
    struct entry_s entry; 
    unsigned int idir =  inumber_of_path(pathname);
    
    /* recuperation inode */
    read_inode(idir, &inode); 
    if (inode.in_type != IT_DIR)  {
    	fprintf(stderr, "ERROR: Le premier parametre de get_entry n'est pas un dossier\n");
    	return 0;
    }
    
    /* open the dir */
    iopen_ifile(fd, idir, &inode);
    
    /* done */
    close_ifile(fd); /* even in case of write failure */

    if(read_ifile (fd, &entry, sizeof(struct entry_s)) != 0) {
    	printf("basename = %s\n", entry.ent_basename);
    	strcpy(contain, entry.ent_basename);
    	return 1;
    }

    return 0;

}
