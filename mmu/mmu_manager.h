#ifndef _MMU_MANAGER_H_
#define _MU_MANAGER_H_

#include "swap.h"
#include "my_syscall.h"

struct pm_mapping_s {
  unsigned pm_mapped : 1;
  unsigned pm_vpage : 12;
};

struct vm_mapping_s {
  unsigned vm_mapped : 1;
  unsigned vm_ppage : 8;
};

#endif /* _MU_MANAGER_H_ */
