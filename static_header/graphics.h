/*
 * Copyright (c) 2014, Qualcomm Technologies, Inc. All Rights Reserved.
 * Qualcomm Technologies Proprietary and Confidential.
 */

#ifndef __MMI_GRAPHICS_H__
#define __MMI_GRAPHICS_H__

typedef void (*gr_text_func) (int x, int y, const char *s, int bold);

void gr_deinit_multi(void);
int gr_init_multi(void);
void gr_text_multi(int x, int y, const char *s, int bold);
int set_font_size(int font_size);
void gr_font_size_multi(int *x, int *y);

#endif
