#include "cart/cart.h"
#include "platform/pico2/platform.h"
#include "wasm4/runtime.h"
static uint8_t* mem = NULL;
static cartInstance instance;
static w4_Disk disk = {0};
int main(int argc, char* argv[]){
    platform_init();
    w4_runtimeInit(mem, &disk);
    cartInstantiate(&instance, NULL);
    cart_start(&instance);
    while(platform_update()){
        w4_
        cart_update(&instance);
        platform_draw();
    }
    cartFreeInstance(&instance);
    platform_deinit();
}