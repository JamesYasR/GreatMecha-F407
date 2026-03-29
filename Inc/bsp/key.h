#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
extern uint8_t ucKey_val,ucKey_down,ucKey_up,ucKey_old;
extern uint8_t KEY_DOWN,KEY_UP;
void Key_Proc(void);
#ifdef __cplusplus
}
#endif

#endif
