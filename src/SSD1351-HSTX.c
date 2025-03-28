// Copyright (c) 2024 Raspberry Pi (Trading) Ltd.

// Drive a ST7789 SPI LCD using the HSTX. The SPI clock rate is fully
// independent of (and can be faster than) the system clock.

// You'll need an LCD module for this example. It was tested with: WaveShare
// 1.3 inch ST7789 module. Wire up the signals as per PIN_xxx defines below,
// and don't forget to connect GND and VCC to GND/3V3 on your board!
//
// Theory of operation: Each 32-bit HSTX record contains 3 x 8-bit fields:
//
// 27:20  CSn x8    (noninverted CSn pin)
// 17:10  !DC x 8   (inverted DC pin)
//  7: 0  data bits (DIN pin)
//
// SCK is driven by the HSTX clock generator. We do issue extra clocks whilst
// CSn is high, but this should be ignored by the display. Packing the
// control lines in the HSTX FIFO records makes it easy to drive them in sync
// with SCK without having to reach around and do manual GPIO wiggling.

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#include "hstx.h"
#define SYS_CLK	384000
#define SCREEN_W 128
#define SCREEN_H 128
uint16_t frame_buffer[2][SCREEN_H*SCREEN_W] = {0xFFFF};
bool draw_buffer = false;
#define DUMMY_FLAG_VALUE 1

void init_sys(){
    vreg_set_voltage(VREG_VOLTAGE_MAX);
	sleep_ms(1);
    set_sys_clock_khz(SYS_CLK, true);
    stdio_init_all();
}
typedef struct
{
    void (*func)();
} queue_entry_t;

queue_t call_queue;
queue_t results_queue;
void core1_hstx_fn(){
    bool local_draw_buffer = !draw_buffer;
    lcd_start_pixels();
    for (uint16_t i = 0; i < SCREEN_H*SCREEN_W; ++i) {
        uint16_t rgb = frame_buffer[local_draw_buffer][i];
        lcd_put_data(rgb >> 8);
        lcd_put_data(rgb & 0xff);
    }
}
void core1_hstx(){
    while (true){
        queue_entry_t entry;
        queue_remove_blocking(&call_queue, &entry);
        entry.func();
    }
}

int main() {
    init_sys();
    init_hstx();
    queue_init(&call_queue, sizeof(queue_entry_t), 2);

    multicore_launch_core1(core1_hstx);
    queue_entry_t entry = {core1_hstx_fn};
   
    
    sleep_ms(10);
    uint t = 0;
    while (1){
        draw_buffer = !draw_buffer;
        for (uint y = 0; y < SCREEN_H; ++y) {
            for (uint x = 0; x < SCREEN_W; ++x) {
                uint16_t r = (x + t) & 0x1f;
                uint16_t b = (y - t) & 0x1f;
                uint16_t g = (x + y + (t >> 1)) & 0x3f;
                uint16_t rgb = (r << 11) + (g << 5) + b;
                frame_buffer[draw_buffer][y|(x<<7)]=rgb;
            }
        }
        queue_add_blocking(&call_queue, &entry);
        ++t;
    }
}