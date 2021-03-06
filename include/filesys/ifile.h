/* ------------------------------
   $Id: ifile.h,v 1.1 2009/11/16 05:38:07 marquet Exp $
   ------------------------------------------------------------

   File manipulation. 
   Basic version; manipulate files via their inode number. 
   Philippe Marquet, october 2009
   
*/

#ifndef _IFILE_H_
#define _IFILE_H_

/**
 * \file ifile.h
 * \brief Gestionnaire de fichier par leur inode
 * \author Philippe Marquet
 * \version 1
 */

#include "filesys/inode.h"
#include "filesys/tools.h"


/* Despite the user does not need these definitions, our first
   implementation allocates file descriptors in the user space. Thus
   the definitions... */
struct file_desc_s {
    unsigned int fds_inumber;   /* inode number of the file */
    unsigned int fds_size;      /* file size in char */
    unsigned int fds_pos;       /* cursor in the file, in char */
    unsigned char fds_buf[BLOC_SIZE]; /* memory copy of bloc at fds_pos */
    char fds_dirty;             /* buffer write back is needed */
};

typedef enum inode_type_e file_type_t;
typedef struct file_desc_s file_desc_t; 

#define READ_EOF		-2
#if READ_EOF >= 0
#  error "READ_EOF must be negative"
#endif


unsigned int create_ifile(file_type_t type); 
int delete_ifile(unsigned int inumber);

int open_ifile(file_desc_t *fd, unsigned int inumber);
void close_ifile(file_desc_t *fd);
void flush_ifile(file_desc_t *fd);
void seek_ifile(file_desc_t *fd, int r_offset);  
void seek2_ifile(file_desc_t *fd, int a_offset); 

int readc_ifile(file_desc_t *fd);
int writec_ifile(file_desc_t *fd, char c);
int read_ifile(file_desc_t *fd, void *buf, unsigned int nbyte);
int write_ifile(file_desc_t *fd, const void *buf, unsigned int nbyte);

/* the inode is already loaded, shortcut the begining of open_ifile() */
int iopen_ifile(file_desc_t *fd, unsigned int inumber, struct inode_s *inode);

#endif
