#ifndef _CLOCKS_H_
#define _CLOCKS_H_
#include <pico/stdlib.h>
#include "hardware/vreg.h"
#include "hardware/clocks.h"
#define SYS_CLK	384000
//#define SYS_CLK	96000
void init_sys();
#endif