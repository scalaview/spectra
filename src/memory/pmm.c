#include "pmm.h"
#include "bitmap_pmm.h"

const struct pmm_manager* pmm_manager;

static void init_pmm_manager()
{
    pmm_manager = &bitmap_pmm_manager;
}

void init_pmm()
{
    init_pmm_manager();
    pmm_manager->init();
}