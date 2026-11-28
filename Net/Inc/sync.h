#ifndef SYNC_H
#define SYNC_H

#include <stdint.h>


// network time (microseconds), wraps back to 0 every 71.6 min
uint32_t sync_now_us(void);

void sync_init(uint8_t node_id);

#endif /* SYNC_H */