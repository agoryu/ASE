#include <stdio.h>
#include <stdlib.h>

static void* esp;
static void* ebp;


/**
 * La fonction foo pour afficher la pile des variables.
 */
int foo(int arg_foo, int arg_bar){

  int a_foo = 1;
  int b_foo = 2;
  int c_foo = 3;
  int res_bar = 123;

  int i;
  int* tmp;

  asm("movl %%esp, %0"
      : "=r" (esp)
      : 
      );
  asm("movl %%ebp, %0"
      : "=r" (ebp)
      : 
      );

  printf("\nFonction foo:\n");
  printf("Adresse de esp \t\t\t : %p\n", esp);
  printf("Adresse de ebp \t\t\t : %p\n", ebp);
  printf("Adr de la 1ere var auto \t : %p\n", &a_foo);
  printf("Adr de la derniere var auto \t : %p\n", &tmp);

  printf("\nPile:\n");
  printf("\n");
  printf("fct \t \\ adresse \t \\ entier\n");
  for(i=0; esp+i<ebp; i=i+sizeof(void*)){
    tmp = (int*) (esp+i);
    printf("foo \t | %p \t | %d\n", (esp+i), *tmp );
  }

  res_bar = bar(arg_bar);

  return arg_foo + a_foo + b_foo + c_foo;
}

/**
 * La fonction bar pour afficher la pile des variables.
 */
int bar(int arg_bar){

  int a_bar = 5;
  int b_bar = 6;
  int c_bar = 7;

  int i;
  int* tmp;

  asm("movl %%esp, %0"
      : "=r" (esp)
      : 
      );
  asm("movl %%ebp, %0"
      : "=r" (ebp)
      : 
      );

  printf("\nFonction bar:\n");
  printf("Adresse de esp \t\t\t : %p\n", esp);
  printf("Adresse de ebp \t\t\t : %p\n", ebp);
  printf("Adr de la 1ere var auto \t : %p\n", &a_bar);
  printf("Adr de la derniere var auto \t : %p\n", &tmp);

  printf("\nPile:\n");
  printf("\n");
  printf("fct \t \\ adresse \t \\ entier\n");
  for(i=0; esp+i<ebp; i=i+sizeof(void*)){
    tmp = (int*) (esp+i);
    printf("bar \t | %p \t | %d\n", (esp+i), *tmp );
  }

  return arg_bar + a_bar + b_bar + c_bar;
}


int main(int argc, char* argv[]){

  int a = 0;
  int b = 4;
  
  int c = foo(a, b);

  return a+b+c;
}
