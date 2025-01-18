#ifndef __INPUT_LISTENER_H__
#define __INPUT_LISTENER_H__
#include <stdio.h>
#include <cutils/log.h>

#include "minui.h"

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG   "minui"
#endif
int kb_getkey(int wait);
int kb_hit(void);
void kb_flush(void);
int init_input_listener(void);


#endif