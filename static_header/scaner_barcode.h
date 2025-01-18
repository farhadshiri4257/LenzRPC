#ifndef __SCANER_BARCODE_H__
#define __SCANER_BARCODE_H__

#include "time.h"
#include <pthread.h>
typedef int32_t (*scaner_result)(const char *str, uint32_t size);

#define DISTANCE_LOW 0
#define DISTANCE_HEIGHT 1
#define BEEP_OFF 0
#define BEEP_ON	 1

int start_scaner_barcode(scaner_result sc_rt, int distance, int beep ,int out_time);
int set_beep(int beep, int msecs);
int stop_scaner_barcode(void);
int set_out_time(int out_time);
int save_camera_image(const char *filename);
int set_led(int mode);
#endif