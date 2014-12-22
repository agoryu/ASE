#include "context.h"

struct ctx_s ctx_ping; 
struct ctx_s ctx_pong;
struct ctx_s ctx_pang;

void f_ping(void *arg);
void f_pong(void *arg);
void f_pang(void *arg);

int main(int argc, char *argv[])
{
  init_ctx(&ctx_ping, 16384, f_ping, NULL);
  init_ctx(&ctx_pong, 16384, f_pong, NULL);
  init_ctx(&ctx_pang, 16384, f_pang, NULL);
  switch_to_ctx(&ctx_ping);

  printf("\nRetour au main\n");

  init_ctx(&ctx_ping, 16384, f_ping, NULL);
  init_ctx(&ctx_pong, 16384, f_pong, NULL);
  init_ctx(&ctx_pang, 16384, f_pang, NULL);
  switch_to_ctx(&ctx_ping);

  printf("\nFin du programme dans le main\n");

  exit(EXIT_SUCCESS);
}

void f_ping(void *args)
{

    printf("A") ;
    switch_to_ctx(&ctx_pong);
    printf("B") ;
    switch_to_ctx(&ctx_pang);
    printf("C") ;
    switch_to_ctx(&ctx_pang);

}

void f_pong(void *args)
{
    printf("1") ;
    switch_to_ctx(&ctx_pang);
    printf("2") ;
    switch_to_ctx(&ctx_ping);

}

void f_pang(void *args)
{
    printf("a") ;
    switch_to_ctx(&ctx_ping);
    printf("b") ;
    switch_to_ctx(&ctx_pong);
    printf("c") ;
    switch_to_ctx(&ctx_ping);
} 
