#ifndef cart_H
#define cart_H

#ifdef __cplusplus
extern "C" {
#endif

#include "w2c2_base.h"

typedef struct cartInstance {
wasmModuleInstance common;
wasmMemory*env__memory;
U32 g0;
} cartInstance;

void env__blitSub(void*,U32,U32,U32,U32,U32,U32,U32,U32,U32);

void f1(cartInstance*);

void f2(cartInstance*);

void cart_start(cartInstance*i);

void cart_update(cartInstance*i);

void cartInstantiate(cartInstance* instance, void* resolve(const char* module, const char* name));

void cartFreeInstance(cartInstance* instance);

#ifdef __cplusplus
}
#endif

#endif /* cart_H */

