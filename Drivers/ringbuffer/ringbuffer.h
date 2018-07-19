#ifndef RINGBUFFER_H_INCLUDED
#define RINGBUFFER_H_INCLUDED

#include <stdint.h>
//#include "stm32f4xx_hal.h"
#include "stm32f1xx_hal.h"
typedef struct
{
    uint8_t * data;
    uint32_t size;
    DMA_HandleTypeDef * hdma;
    uint8_t const * tail_ptr;
} RingBuffer;

void RingBuffer_Init(RingBuffer * buffer, DMA_HandleTypeDef * hdma, uint8_t * data, uint32_t size);
uint8_t RingBuffer_GetByte(RingBuffer * buffer);
uint32_t RingBuffer_Count(RingBuffer * buffer);

#endif /* RINGBUFFER_H_INCLUDED */
