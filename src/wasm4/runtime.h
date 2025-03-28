#ifndef runtime_H
#define runtime_H

typedef struct {
    uint16_t size;
    uint8_t data[1024];
} w4_Disk;
void w4_runtimeInit (uint8_t* memory, w4_Disk* disk);
#endif