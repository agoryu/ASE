#ifndef _SWAP_H_
#define _SWAP_H_

#include <stdio.h>
#include <stdint.h>

#include "hardware.h"
/*#include "hw_config.h"*/

static char init_swap(const char *path);

static char store_to_swap(int vpage, int ppage);

static char fetch_from_swap(int vpage, int ppage);

#endif /* _SWAP_H_ */
