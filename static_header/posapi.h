#ifndef _POS_API_H_
#define _POS_API_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <linux/input.h>
#include "lib_def.h"

#include "cutils/log.h"
#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG   "e60demo"
#endif

#include "ERRCode.h"

#include "m3_iccard.h"
#include "m3_serial.h"
#include "m3_sflash.h"
#include "m3_sled.h"
#include "m3_system.h"

#include "lcd.h"
#include "keyboard.h"

#include "rfid.h"
#include "nfc.h"

#include "rild_util.h"
#include "rtc_util.h"

#include "scaner_barcode.h"

#include "ecclib.h"
#include "sm3.h"

#include "serial.h"

#include "gps.h"
#include "lh16_wifi.h"
#include "lh16_fram.h"

#include "system.h"
#include "dac_api.h"

#endif

