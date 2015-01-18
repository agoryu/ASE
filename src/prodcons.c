#include "context/sem.h"

#define STACK_SIZE 16384
#define N 10                       /* nombre de places dans le tampon */

typedef unsigned objet_t;
struct sem_s mutex, vide, plein;
int cpt;

void producteur(void*);
void consommateur(void*);

static objet_t buf[1];

int main()
{
  /* controle d'acces au tampon */
  sem_init(&mutex, 1);
  /* nb de places libres */
  sem_init(&vide, N);
  /* nb de places occupees */
  sem_init(&plein, 0);

  if( ! create_ctx(STACK_SIZE, producteur, NULL)){
    fprintf(stderr, "Erreur creation de context");
    exit(EXIT_FAILURE);
  }
  if( ! create_ctx(STACK_SIZE, consommateur, NULL)){
    fprintf(stderr, "Erreur creation de context");
    exit(EXIT_FAILURE);
  }


  start_sched(yield);

  printf("\nRetour au main\n");

  exit(EXIT_SUCCESS);
}

void produire_objet(objet_t* objet) {
  *objet = 1;
  printf("je produit une potion magique\n");
}

void mettre_objet(objet_t objet) {
  buf[0] = objet;
  printf("cpt = %d\n", cpt++);
}

void retirer_objet ( objet_t* objet ) {
  *objet = buf[0];
  printf("cpt = %d\n", cpt--); 
}

void utiliser_objet( objet_t objet ) {
  objet--;
  printf("j'ai utilisé la potion magique\n");
}

void producteur (void* arg)
{

  if(arg){
    fprintf(stderr, "Erreur: fonction producteur sans arguments");
    return;
  }

  objet_t objet ;

  while (1) {
    produire_objet(&objet);           /* produire l'objet suivant */
    sem_down(&vide);                  /* dec. nb places libres */
    sem_down(&mutex);                 /* entree en section critique */
    mettre_objet(objet);              /* mettre l'objet dans le tampon */
    sem_up(&mutex);                   /* sortie de section critique */
    sem_up(&plein);                   /* inc. nb place occupees */
  }
}

void consommateur (void* arg)
{

  if(arg){
    fprintf(stderr, "Erreur: fonction consommateur sans arguments");
    return;
  }

  objet_t objet ;

  while (1) {
    sem_down(&plein);                 /* dec. nb emplacements occupes */
    sem_down(&mutex);                 /* entree section critique */
    retirer_objet (&objet);           /* retire un objet du tampon */
    sem_up(&mutex);                   /* sortie de la section critique */
    sem_up(&vide);                    /* inc. nb emplacements libres */
    utiliser_objet(objet);            /* utiliser l'objet */
  }
}
