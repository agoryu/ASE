#include "context/sem.h"


void sem_init(struct sem_s * sem, unsigned val){
    sem->sem_cpt = val;
    sem->sem_ctx = NULL;
}

void sem_down(struct sem_s * sem){

    irq_disable();

    sem->sem_cpt --;
    if(sem->sem_cpt < 0){
	ctx_current[0]->ctx_state = CTX_STP;
	ctx_current[0]->ctx_sem_next = sem->sem_ctx;
	sem->sem_ctx = ctx_current[0];
	irq_enable();
	yield();
    } else {
	irq_enable();
    }
}

void sem_up(struct sem_s * sem){

    irq_disable();

    sem->sem_cpt++;
    if(sem->sem_cpt <= 0){
	sem->sem_ctx->ctx_state = CTX_EXQ;
	sem->sem_ctx = sem->sem_ctx->ctx_sem_next;
    }

    irq_enable();

}

