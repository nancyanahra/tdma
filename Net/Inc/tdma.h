#ifndef TDMA_H
#define TDMA_H

#include <stdint.h>

// tdma slot schedule

#define TDMA_SLOT_US 2048u // about 2ms
#define TDMA_SLOTS_PER_FRAME 8u // pwr of 2
#define TDMA_FRAME_US (TDMA_SLOT_US * TDMA_SLOTS_PER_FRAME) //16384 us
#define TDMA_GUARD_US 500u //buffer between

#define TDMA_NODE_NONE 0xFFu // marker for slots 6 and 7, noone transmits

_Static_assert((TDMA_FRAME_US & (TDMA_FRAME_US - 1u)) == 0u, "TDMA_FRAME_US must be a power of 2 so the 32-bit counter wraps on a slot boundary");
_Static_assert(2u * TDMA_GUARD_US < TDMA_SLOT_US, "guard too big for slot");

// where network time t falls in the schedule
typedef struct{
    uint32_t frame; // frame number
    uint8_t slot;
    uint32_t us_into_slot;
    uint32_t us_to_next;

}tdma_pos_t;

tdma_pos_t tdma_position(uint32_t t_us);
uint8_t tdma_slot_owner(uint8_t slot);

#endif /* TDMA_H */