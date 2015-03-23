#include "context/context.h"
#include "hw/hardware.h"
#include "context/sem.h"
#include "hw/hw.h"
#include <unistd.h>

/**
 * \brief Fonction permettant l'initiation d'un contexte 
 * \param[in] ctx contexte à initialiser
 * \param[in] stack_size taille du contexte à initialiser
 * \param[in] f fonctione exécuter sur la pile
 * \param[in] arg arguement de la fonctione lancé par le contexte
 * \return 0 en cas d'erreur et 1 sinon
 */
unsigned init_ctx(struct ctx_s *ctx, unsigned stack_size, func_t f, void* args);

/**
 * \brief Fonction permettant d'échanger des contextes
 * \param[in] ctx Le context qui sera actif.
 * \param[in] core Le coeur de context.
 */
void switch_to_ctx(struct ctx_s *ctx, const unsigned core);

void start_ctx();

/** Table des contextes de chaque coeur */
static struct ctxs_by_core_s * ctxs_tab[CORE_NCORE];

struct ctx_s * ctx_current[CORE_NCORE];

static int first_call = 1;

void* initial_esp;
void* initial_ebp;

unsigned init_ctx(struct ctx_s *ctx, unsigned stack_size, func_t f, void* args){

    ctx->ctx_stack = malloc(stack_size);
    
    if(!ctx->ctx_stack){
        fprintf(stderr, "ERROR: Erreur d'allocution pour la stack\n");
        return 0;
    }

    ctx->ctx_esp = ctx->ctx_ebp = ((char*)ctx->ctx_stack) +stack_size - 4;

    ctx->ctx_f = f;
    ctx->ctx_arg = args;
    ctx->ctx_state = CTX_INIT;
    ctx->ctx_magic = CTX_MAGIC;

    return 1;
}

unsigned init_ctxsys(){

    unsigned i;

    printf("Multicore context systeme was initialized.\n\n");

    for (i=0; i<CORE_NCORE; i++) {
        ctxs_tab[i] = malloc(sizeof(struct ctxs_by_core_s));
        
        if(ctxs_tab[i] == NULL){
            fprintf(stderr, "ERROR init_ctxsys: échec d'allocution mémoire.\n");
            return 0;
        }

        ctxs_tab[i]->ctxs_lenght = 0;
        ctx_current[i] = NULL;
    }

    return 1;
}

void switch_to_ctx(struct ctx_s *ctx, const unsigned core){

    printf("[%d] switch_to_ctx\n", core);

    if (ctx == NULL){
        fprintf(stderr, "ERROR: Le context passé au switch_to_context est NULL.\n");
        return;
    }

    /* si ctx est null on retourne au main */
    /*if (ctx == ctx->next) {
        first_call = 1;
        ctx_current[core] = NULL;
        ctxs_tab[core]->ctxs_ring = NULL;

        asm("movl %0, %%esp"
            : 
            :"r" (initial_esp));

        asm("movl %0, %%ebp"
            :
            :"r" (initial_ebp));

        return;
        }*/


    if(ctx->ctx_state == CTX_END) {
        if(ctxs_tab[core]->ctxs_ring == ctx){
            ctxs_tab[core]->ctxs_ring = ctx_current[core];
        }

        ctx_current[core]->next = ctx->next;
        free(ctx->ctx_stack);
        free(ctx);
        yield();
    }

    /*while(ctx->ctx_state == CTX_STP){
        ctx = ctx->next;
        if(ctx == ctx_current[core]){
            fprintf(stderr, "ERROR: coeur %d bloqués!\n", core);
            return;
        }
    }*/

    if(first_call){
        first_call = 0;
        asm("movl %%esp, %0"
            : "=r" (initial_esp)
            : );
        asm("movl %%ebp, %0"
            : "=r" (initial_ebp)
            : );
    }

    if(ctx_current[core]){

        if(ctx->ctx_magic != CTX_MAGIC){
	    fprintf(stderr, "ERROR: valeur magique incorrecte.\n");
	    return;
	}

        asm("movl %%esp, %0"
            : "=r" (ctx_current[core]->ctx_esp)
            : );
        asm("movl %%ebp, %0"
            : "=r" (ctx_current[core]->ctx_ebp)
            : );
    }

    
    ctx_current[core] = ctx;
    asm("movl %0, %%esp"
        : 
        :"r" (ctx->ctx_esp));
    asm("movl %0, %%ebp"
        :
        :"r" (ctx->ctx_ebp));
  

    start_ctx();
}

void start_ctx() {

    unsigned core = _in(CORE_ID);

    printf("[%d] start_ctx\n", core);

    assert(ctx_current[core] != NULL);

    if(ctx_current[core]->ctx_state == CTX_INIT){
        ctx_current[core]->ctx_state = CTX_EXQ;
        ctx_current[core]->ctx_f(ctx_current[core]->ctx_arg);
        ctx_current[core]->ctx_state = CTX_END;

        /* retour fonction d'origin du coeur */
        if(ctx_current[core] == ctx_current[core]->next) {
            first_call = 1;
            ctx_current[core] = NULL;
            ctxs_tab[core]->ctxs_ring = NULL;
            asm("movl %0, %%esp"
                : 
                :"r" (initial_esp));
            asm("movl %0, %%ebp"
                :
                :"r" (initial_ebp));
        }

        yield();
    }
}

int create_ctx(int stack_size, func_t f, void* arg, const unsigned core) {
    
    irq_disable();
    
    struct ctx_s * new_ctx;
    new_ctx = malloc(sizeof (struct ctx_s));
  
    if(new_ctx == NULL){
        fprintf(stderr, "ERROR: echec allocution du nouveau contexte.\n");
        return 0;
    }

    printf("[%d] create_ctx\n", core);

    if (ctxs_tab[core]->ctxs_lenght == 0) {
        /* si ce coeur n'a pas de contexte */
        ctxs_tab[core]->ctxs_ring = new_ctx;
        new_ctx->next = new_ctx;
        ctxs_tab[core]->ctxs_lenght++;
    } else {
        /* sinon on insert un contexte */
        new_ctx->next = ctxs_tab[core]->ctxs_ring->next;
        ctxs_tab[core]->ctxs_ring->next = new_ctx;
    }

    irq_enable();

    return init_ctx(new_ctx, stack_size, f, arg);
}

void yield() {

    irq_disable();

    unsigned core = _in(CORE_ID);

    /*int status;*/
    /*_out(TIMER_ALARM, 0xFFFFFFFF-20);*/

    /*while(status != 1) status = _in(CORE_LOCK);*/
    if(ctx_current[core]){
	/*printf("[%d] yield: current is not NULL.\n", core);*/
	switch_to_ctx(ctx_current[core]->next, core);
    } else if(ctxs_tab[core]->ctxs_lenght != 0){
	/* premier passage */
	/*printf("[%d] yield: current is NULL.\n", core);*/
	switch_to_ctx(ctxs_tab[core]->ctxs_ring, core);
    }
    irq_enable();
    /*_out(CORE_UNLOCK, 0);*/
}



