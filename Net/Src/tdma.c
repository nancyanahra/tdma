#include "tdma.h"
#include <stdint.h>


tdma_pos_t tdma_position(uint32_t t_us){
    tdma_pos_t pos;
    pos.frame = t_us / TDMA_FRAME_US;
    pos.slot = (t_us / TDMA_SLOT_US) % TDMA_SLOTS_PER_FRAME;
    pos.us_into_slot = t_us % TDMA_SLOT_US;
    pos.us_to_next = TDMA_SLOT_US - pos.us_into_slot;

    return pos;
}

uint8_t tdma_slot_owner(uint8_t slot){
    static const uint8_t tdma_table[] ={
        0,
        1,
        2,
        0,
        1,
        2,
        TDMA_NODE_NONE,
        TDMA_NODE_NONE
    
    };
    _Static_assert(sizeof(tdma_table) == TDMA_SLOTS_PER_FRAME, "tdma slot table needs 1 entry per slot");

    if (slot >= TDMA_SLOTS_PER_FRAME){
        return TDMA_NODE_NONE;
    }
    return tdma_table[slot];
}