#include "runtime.h"
#define WIDTH 160
#define HEIGHT 160

#pragma pack(1)
typedef struct {
    uint8_t _padding[4];
    uint32_t palette[4];
    uint8_t drawColors[2];
    uint8_t gamepads[4];
    int16_t mouseX;
    int16_t mouseY;
    uint8_t mouseButtons;
    uint8_t systemFlags;
    uint8_t _reserved[128];
    uint8_t framebuffer[WIDTH*HEIGHT>>2];
    uint8_t _user[58976];
} Memory;
#pragma pack()