Un systeme de fichiers a la Unix
================================



# Auteurs

- Elliot VANEGUE
- Gaëtan DEFLANDRE

* * *


# Table des matières

- TP5 : Accès au materiel
- TP6 : Gestion de volumes
- TP7 : Structure d'un volume
- TP8 : Structure d'un fichier

* * *


# TP5 : Accès au materiel


## Description


### Bibliotheque drive

La bibliotheque d'acces au materiel *drive* est ecrite dans 
les fichers suivant:

- drive.h
- drive.c

Ces fonctionnalites font partie de la premiere couche d'abtraction du materiels.

On y retrouve les fonctions:

	int is_valid_cylinder(const unsigned int cylinder);

	int is_valid_sector(const unsigned int sector);

	int seek_sector(const unsigned int cylinder, const unsigned int sector);

	void read_sector(const unsigned int cylinder, 
					 const unsigned int sector, 
					 unsigned char* buffer);

	void read_sector_n(const unsigned int cylinder, 
				       const unsigned int sector, 
				       unsigned char* buffer, 
				       const size_t n);

	void write_sector(const unsigned int cylinder, 
					  const unsigned int sector, 
					  const unsigned char* buffer);

	void write_sector_n(const unsigned int cylinder, 
						const unsigned int sector, 
						const unsigned char* buffer,
						const size_t n);

	void format_sector(const unsigned int cylinder, 
					   const unsigned int sector, 
					   const unsigned int nsector, 
					   const unsigned int value);

	void format_all(const unsigned int value);

*Plus d'informations sur les fonctions et structure de données dans la documentation.*


### Commandes dmps et frmt 

Les fichier *dmps.c* et *frmt.c* contiennent le code des 
commandes *dmps* et *frmt* qui servent:

- dmps: lire d'une donnee a un cylindre et sector en parametre.
- frmt: format l'ensemble du materiel. 


### Utilisation

    $ ./dmps c s

Lire la donnee au cylindre c et au secteur s.

    $ ./frmt

Format les donnees du disque.


  
* * *
  

# TP6 : Gestion de volumes


## Description


### Bibliotheque mbr

La bibliotheque de gestion des volumes *mbr* est écrite dans les 
fichiers:

 - mbr.h
 - mbr.c

Ces fonctionnalites font partie de la deuxieme couche logiciel qui interagissent 
directement avec les fonction de la bicliotheque *drive* (premiere couche).

On y retrouve les fonctions:

	void init_mbr();

	int load_mbr();

	int save_mbr();

	void read_bloc(const unsigned int vol, 
				   const unsigned int nbloc, 
				   unsigned char* buffer);

	void write_bloc(const unsigned int vol, 
					const unsigned int nbloc, 
					const unsigned char* buffer);

	void format_vol(const unsigned int vol);

	int make_vol(const unsigned cylinder, 
				 const unsigned sector, 
				 const unsigned nbloc);

	void display_vol();

*Plus d'informations sur les fonctions et structure de données dans la documentation.*


### Commandes mkvol et dvol

Les commandes **mkvol** et **dvol** sont fonctionnelles et peuvent s'executer.


#### La Commande mkvol (make volume)

Elle permet de creer un volume sur le disque, en fonction de ces arguments.
Ces argument sont:
 - s: le nombre de blocs pour le volume.
 - fc: le cylindre a partir duquel le volume commence.
 - fs: le secteur a partir duquel le volume commence.

Cette fonctonnalite gere le chevauchement.

##### Utilisation

    $ ./mkvol -fc 0 -fs 1 -s 12
    
Creation d'un volume, cylindre 0, secteur 1 de 12 blocs.


#### La commande dvol (display volume)

Elle permet l'affichage des informations des volumes sur le disque.

##### Utilisation

    $ ./dvol
    
Output:

    Il y a 2 volume(s) sur le disque.

    Volume 1:
	     - Commence au cylindre 0.
	     - Commence au secteur 1.
	     - Nombre de blocs : 12.
	     - Type de volume: PRIMAIRE.

    Volume 2:
	     - Commence au cylindre 2.
	     - Commence au secteur 0.
	     - Nombre de blocs : 42.
	     - Type de volume: PRIMAIRE.




  
* * *
  

# TP7 : Structure d'un volume


## Description


### Gestion d'un volume

La gestion de l'interieur de chaque volume est traite dans les fichiers:

 - vol.h
 - vol.c
 
Les fonctions présentes dans les fichier vol.h et vol.c font partie de la 
troisieme couche logiciel, elles n'interagit qu'avec la couche inferieur 
vu precedemment.

On y retrouve les fonctions:

    void init_super(const unsigned int vol);

    int load_super(const unsigned int vol);

    unsigned int new_bloc();

    void free_bloc(const unsigned int bloc);

    unsigned int is_full();

    unsigned int get_nb_free_bloc();
    
*Plus d'informations sur les fonctions et structure de données dans la documentation.*


### Les commandes validation_volume, mknfs et dfs


#### La commande validation_volume

Cette fonction cree et verifie qu'il est possible de remplir ce volume. Puis, 
quelques blocs du volume sont libere pour verifier la coherence des informations 
du volume.

##### Utilisation

    $ ./validation_volume
    
    
#### La commande mknfs

Cette fonction cree, si ce n'est pas deja fait le volume courant par defaut.

##### Utilisation

    $ ./mknfs

Output, si le disque est vierge:

    Le disque est vierge, voulez-vous continuer les traitements? (y/n) y
    Le volume principale a été créé avec succès.

Output, si le volume courant principal est deja cree:

    Le volume principale a déjà été créé!
    
    
#### La commande dfs

Comme la commande *dvol*, elle affiche les informations des volumes et affiche 
aussi le taux d'occupation du volume courant.

##### Utilisation

    $ ./dfs

Output:

    Il y a 1 volume(s) sur le disque.

    Volume 1:
	     - Commence au cylindre 0.
	     - Commence au secteur 1.
	     - Nombre de blocs : 10.
	     - Type de volume: PRIMAIRE.

    Il reste 9 espace dans le volume courant.




  
* * *
  

# TP8 : Structure d'un fichier


## Description

Les fichiers sont indexés par les inoeuds, la gestion des inoeuds est faite 
dans les fichiers:

 - inode.h
 - inode.c

On y retrouve les fonctions:

    void read_inode(const unsigned int inumber, struct inode_s *inode);

    void write_inode(const unsigned int inumber, const struct inode_s *inode);

    unsigned int create_inode(const enum inode_type_e type);

    int delete_inode(const unsigned int inumber);

    unsigned int vbloc_of_fbloc(const unsigned int inumber, 
                                const unsigned int fbloc,
                                const unsigned int do_allocate);
                                
Les fonctions de gestions des inoeuds utilisent les fonctions de **vol.h**.  
La fonction vbloc_of_fbloc gère allocution de nouveaux blocs lorsque 
*do_allocate* est vrai.

*Plus d'informations sur les fonctions et structure de données dans la documentation.*


## Manipulation de fichiers

 - ifile.h
 - ifile.c
 
Fichiers du professeur qui gèrent la manipulation de fichiers.


## Commandes mkfile, writefile et readfile

Commandes pour tester les inoeuds.


### Commande mkfile

Commande qui retourne  et affiche l'inoeud du fichier créé.

#### Utilisation

    $ ./mkfile
    
Output:

    Création d'un fichier:
        inumber: 1.


### Commande writefile

Permets l'écriture sur un fichier existant à partir de son inumber.

#### Utilisation

    $ ./writefile 1 "Contenu texte du fichier d'inumber 1"


### Commande readfile

Permets de lire un fichier existant à partir de son inumber.

#### Utilisation

    $ ./readfile 1
   
Output:

    Contenu texte du fichier d'inumber 1
