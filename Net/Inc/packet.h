#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

#define PKT_BEACON 1u

typedef struct {
    uint8_t type; // beacon or otherwise
    uint8_t src; // source, who tx this packet
    uint8_t origin; // who created the packet
    uint8_t seq; // origin's sequence numbers
    uint8_t data[4]; // timestamp in the sync phase
} pkt_t;

//check during compilations
_Static_assert(sizeof(pkt_t) == 8, "pkt_t must be 8 bytes");

#define NRF24_PAYLOAD_LEN ((uint8_t)sizeof(pkt_t))


#endif