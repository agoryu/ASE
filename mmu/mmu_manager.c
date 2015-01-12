#include "mmu_manager.h"

static struct vm_mapping_s vm_mapping[VM_PAGES];
static struct pm_mapping_s pm_mapping[PM_PAGES];	

int mmu_handler() {
	
	int vaddr = _in(MMU_FAULT_ADDR);
	static int rr = 0;
	int ppage = 0;
	
	if(!is_vaddr(vaddr)){
		fprintf(stderr, "Erreur l'adresse virtuel n'est pas valide.\n");
		return -1;
	}
	
	//mais ou est vpage????
	if(vm_mapping[vpage].vm_mapped) {
		tlb.tlb_vpage = vpage;
		tlb.tlb_ppage = 1; //yen a pas qu'un baka
		tlb.tlb_acces = 7;
		tlb.tlb_active = 1;
		_out(TLB_ADD_ENTRY, tlb);
	}
	ppage = (rr++ % PM_PAGES - 1) + 1;
	store_to_swap(pm_mapping[ppage].pm_vpage, ppage);
	
	tlb.tlb_ppage = ppage;
	_out(TLB_ADD_ENTRY, tlb);
	
}
