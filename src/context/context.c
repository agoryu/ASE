#include "context/context.h"
#include "hw/hardware.h"

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
 * \param[in] ctx context qui sera actif
 */
void switch_to_ctx(struct ctx_s *ctx);

void start_ctx();

/** Table des contextes de chaque coeur */
static struct ctxs_by_core_s ctxs_tab[CORE_NCORE];

struct ctx_s * ctx_current[CORE_NCORE];

static int first_call = 1;
void* initial_esp;
void* initial_ebp;

int num_core;

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

    for (i=0; i<CORE_NCORE; i++) {
        ctxs_tab[i].ctxs_lenght = 0;
        ctx_current[i] = NULL;
    }

    return 1;
}

void switch_to_ctx(struct ctx_s *ctx){

    unsigned core = _in(CORE_ID);

    /* si ctx est null on retourne au main */
    if (ctx == ctx->next) {
        first_call = 1;
        ctx_current[core] = NULL;
        ctxs_tab[core].ctxs_ring = NULL;

        asm("movl %0, %%esp"
            : 
            :"r" (initial_esp));
        asm("movl %0, %%ebp"
            :
            :"r" (initial_ebp));

        return;
    }

    if(ctx->ctx_state == CTX_END) {
        if(ctxs_tab[core].ctxs_ring == ctx){
            ctxs_tab[core].ctxs_ring = ctx_current[core];
        }

        ctx_current[core]->next = ctx->next;
        free(ctx->ctx_stack);
        free(ctx);
        yield();
    }

    while(ctx->ctx_state == CTX_STP){
        ctx = ctx->next;
        if(ctx == ctx_current[core]){
            fprintf(stderr, "ERROR: tous bloqués!\n");
        }
    }

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
        /* TODO voir si on peu changer */
        assert(ctx->ctx_magic == CTX_MAGIC);
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

    if(ctx_current[core]->ctx_state == CTX_INIT){
        ctx_current[core]->ctx_state = CTX_EXQ;
        ctx_current[core]->ctx_f(ctx_current[core]->ctx_arg);
        ctx_current[core]->ctx_state = CTX_END;
    
        /* retour fonction d'origin du coeur */
        if(ctx_current[core] == ctx_current[core]->next) {
            first_call = 1;
            ctx_current[core] = NULL;
            ctxs_tab[core].ctxs_ring = NULL;
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

int create_ctx(int stack_size, func_t f, void* arg) {
    
    struct ctx_s * new_ctx;
    new_ctx = malloc(sizeof (struct ctx_s));
  
    if(new_ctx == NULL){
        fprintf(stderr, "ERROR: echec allocution du nouveau contexte.\n");
        return 0;
    }

    if (ctxs_tab[num_core].ctxs_lenght == 0) {
        /* si ce coeur n'a pas de contexte */
        ctxs_tab[num_core].ctxs_ring = new_ctx;
        new_ctx->next = new_ctx;
    } else {
        /* sinon on insert un contexte */
        new_ctx->next = ctxs_tab[num_core].ctxs_ring->next;
        ctxs_tab[num_core].ctxs_ring->next = new_ctx;
        
    }

    num_core = (num_core + 1) % CORE_NCORE;

    return init_ctx(new_ctx, stack_size, f, arg);
}

void yield() {

    unsigned core = _in(CORE_ID);
        
    if(ctx_current[core]){
        printf(" coeur en cours -> %d\n", core);
        switch_to_ctx(ctx_current[core]->next);
    } else {
        /* premier passage */
        printf(" coeur en cours -> %d\n", core);
        switch_to_ctx(ctxs_tab[core].ctxs_ring);
    }
}

