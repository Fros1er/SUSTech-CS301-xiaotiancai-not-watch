#include "mem_monitor.h"

MemoryMonitor::MemoryMonitor() : Application("Monitor") {
    label = lv_label_create(_bg);
    lv_obj_center(label);
}

void MemoryMonitor::tick() {
    lv_mem_monitor(&monitor_buf);
    lv_label_set_text_fmt(label, "Memory:\ntotal_size: %ld\nfree_cnt: %ld\nfree_size: %ld\nfree_biggest_size: %ld\nused_cnt: %ld\nmax_used: %ld\nused_pct: %d\nfrag_pct: %d\n\n2.4G status\nServer(%p)<->(%p)Me\nPacket send: %d\nPacket recv: %d\n",
                          monitor_buf.total_size,
                          monitor_buf.free_cnt,
                          monitor_buf.free_size,
                          monitor_buf.free_biggest_size,
                          monitor_buf.used_cnt,
                          monitor_buf.max_used,
                          monitor_buf.used_pct,
                          monitor_buf.frag_pct, 
                          (void*)SERVER_ADDR, (void*)device_name,
                          packet_send_cnt, 
                          packet_recv_cnt);
}
