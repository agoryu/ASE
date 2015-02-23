#include "mmu/mi_syscall.h"


static int current_process;

int ppage_of_vaddr(const int process, const unsigned vaddr);

static void switch_to_process0(void) {

    current_process = 0;
    _out(MMU_CMD, MMU_RESET);
}

static void switch_to_process1(void) {

    current_process = 1;
    _out(MMU_CMD, MMU_RESET);
}

unsigned is_vaddr(const unsigned vaddr){
    if(vaddr < VM_BEGIN || vaddr > VM_END) {
        return 0;
    }
    return 1;
}

int ppage_of_vaddr(const int process, const unsigned vaddr) {

    int vpage;
    int ppage;

    /* verification de la validité de l'adresse virtuel */
    if(!is_vaddr(vaddr)) {
        fprintf(stderr, "Erreur l'adresse virtuel n'est pas valide.\n");
        return -1;
    }


    vpage = (vaddr >> 12) & 0xFFF;

    /* ya pas de raison de passé la dedans avec la verif du dessus */
    if(vpage > N/2) {
        fprintf(stderr, "Erreur page virtuelle incorrecte.\n");
        return -1;
    }

    if(process == 0)
        ppage = vpage + 1;
    else if(process == 1)
        ppage = vpage + N/2 + 1;

    return ppage;
}


unsigned init() {

    /* init hardware */
    if(!init_hardware("etc/hardware.ini")){
        fprintf(stderr, "Erreur lors de l'initialisation hardware.\n");
        return 0;
    }

    IRQVECTOR[MMU_IRQ] = mmuhandler;
    IRQVECTOR[SYSCALL_SWITCH_0] = switch_to_process0;
    IRQVECTOR[SYSCALL_SWITCH_1] = switch_to_process1;
    _mask(0x1001);
    return 1;
}


void mmuhandler() {

    struct tlb_entity_s tlb;
    unsigned vaddr = _in(MMU_FAULT_ADDR);
    int ppage = ppage_of_vaddr(current_process, vaddr);

    if(ppage == -1) {
        fprintf(stderr, "Erreur lors de la recherche d'une page physique pour une adresse vaddr.\n");
        return;
    }

    tlb.tlb_vpage = (vaddr >> 12) & 0xFFF;
    tlb.tlb_ppage = ppage;
    tlb.tlb_acces = 0x7;
    tlb.tlb_active = 1;

    _out(TLB_ADD_ENTRY, *(int*)(&tlb));
}


int main() {

    if(!init()){
        fprintf(stderr, "Erreur lors de l'intialisation global.\n");
        exit(EXIT_FAILURE);
    }

    user();

    exit(EXIT_SUCCESS);
}
