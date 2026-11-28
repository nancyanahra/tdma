#include "sync.h"
#include "main.h"
#include "stm32f446xx.h"
#include <stdint.h>

static uint8_t s_node_id;
static uint32_t s_offset_us = 0; //network time minus local time, but 0 for now

void sync_init(uint8_t node_id){
    s_node_id = node_id;
    s_offset_us =0;
}

uint32_t sync_now_us(void){

    return TIM2->CNT + s_offset_us; // 1 tick is 1 us
}