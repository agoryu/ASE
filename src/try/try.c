#include "try.h"

int try(struct ctx_s * ctx, func_t f, int arg) {

	ctx->ctx_magic = CTX_MAGIC;
	asm("movl %%esp, %0"
			: "=r" (ctx->ctx_esp)
			: );
	asm("movl %%ebp, %0"
			: "=r" (ctx->ctx_ebp)
			: );
	printf("esp try -> %p\n", ctx->ctx_esp);
	printf("ebp try -> %p\n", ctx->ctx_ebp);
	return f(arg);
}

int throw(struct ctx_s * pctx, int r) {
       	static int throw_r = 0;
	throw_r = r;

	if(pctx -> ctx_magic != CTX_MAGIC) {
		perror("message d'erreur");
		return 0;
	}	

	asm("movl %0, %%esp"
			: 
			:"r" (pctx->ctx_esp));
	asm("movl %0, %%ebp"
			:
			:"r" (pctx->ctx_ebp));

	printf("esp throw -> %p\n", pctx->ctx_esp);
	printf("ebp throw -> %p\n", pctx->ctx_ebp);
	pctx->ctx_magic = 0;

	return throw_r;
}
