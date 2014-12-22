#ifndef _TRY
#define _TRY
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CTX_MAGIC 0xCAFEBABE

struct ctx_s {
	void * ctx_esp;
	void * ctx_ebp;
	unsigned ctx_magic;
};

typedef int (*func_t)(int);

/** save the context 
  */
int try(struct ctx_s * ctx, func_t f, int arg);

/** modify the context with pctx
  */
int throw(struct ctx_s * pctx, int r);
#endif
