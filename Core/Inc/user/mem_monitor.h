#include "application.h"
#include "ff.h"
#include <vector>

class MemoryMonitor : public Application {
    lv_mem_monitor_t monitor_buf;
    lv_obj_t *label;

   public:
    MemoryMonitor();
    void tick() override;
};