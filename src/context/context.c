#include "context/context.h"

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

static struct ctx_s** ctx_ring;
struct ctx_s** ctx_current;

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

    ctx_ring = malloc(sizeof(struct ctx_s*) * CORE_NCORE);
    if(!ctx_ring){
        fprintf(stderr, "ERROR: echec allocution des anneaux de contextes pour chaque coeur.\n");
        return 0;
    }

    ctx_current = malloc(sizeof(struct ctx_s*) * CORE_NCORE);
    if(!ctx_current){
        fprintf(stderr, "ERROR: echec allocution des contextes courrantes pour chaque coeur.\n");
        return 0;
    }

    *ctx_ring = NULL; 
    *ctx_current = NULL;

    return 1;
}

void switch_to_ctx(struct ctx_s *ctx){

    /* si ctx est null on retourne au main */
    if (ctx == ctx->next) {
        first_call = 1;
        ctx_current[0] = NULL;
        ctx_ring[0] = NULL;

        asm("movl %0, %%esp"
            : 
            :"r" (initial_esp));
        asm("movl %0, %%ebp"
            :
            :"r" (initial_ebp));

        return;
    }

    if(ctx->ctx_state == CTX_END) {
        if(ctx_ring[0] == ctx){
            ctx_ring[0] = ctx_current[0];
        }

        ctx_current[0]->next = ctx->next;
        free(ctx->ctx_stack);
        free(ctx);
        yield();
    }

    while(ctx->ctx_state == CTX_STP){
        ctx = ctx->next;
        if(ctx == ctx_current[0]){
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
  

    if(ctx_current[0]){
        assert(ctx->ctx_magic == CTX_MAGIC);
        asm("movl %%esp, %0"
            : "=r" (ctx_current[0]->ctx_esp)
            : );
        asm("movl %%ebp, %0"
            : "=r" (ctx_current[0]->ctx_ebp)
            : );
    }

    ctx_current[0] = ctx;
    asm("movl %0, %%esp"
        : 
        :"r" (ctx->ctx_esp));
    asm("movl %0, %%ebp"
        :
        :"r" (ctx->ctx_ebp));
  
    start_ctx();
}

void start_ctx() {
    if(ctx_current[0]->ctx_state == CTX_INIT){
        ctx_current[0]->ctx_state = CTX_EXQ;
        ctx_current[0]->ctx_f(ctx_current[0]->ctx_arg);
        ctx_current[0]->ctx_state = CTX_END;
    
        /* retour au main */
        if(ctx_current[0] == ctx_current[0]->next) {
            first_call = 1;
            ctx_current[0] = NULL;
            ctx_ring[0] = NULL;
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

    if (ctx_ring[0] != NULL) {
        /* premier passage dans le create */
        new_ctx->next = ctx_ring[0]->next;
        ctx_ring[0]->next = new_ctx;
    } else {
        /* tous les autres passages */
        ctx_ring[0] = new_ctx;
        new_ctx->next = new_ctx;
    }

    return init_ctx(new_ctx, stack_size, f, arg);
}

void yield() {
    if(ctx_current[0]){
        switch_to_ctx(ctx_current[0]->next);
    } else {
        /* premier passage */
        switch_to_ctx(ctx_ring[0]);
    }
}

