#ifndef __M3_SLED_H__
#define __M3_SLED_H__
#include <stdint.h>  
#ifdef __cplusplus
extern "C"{
#endif

typedef enum{
	LEVEL0 = 0,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL4,
	LEVEL5,
	LEVEL6,
	LEVEL7,
}LED_LIGHT_LEVEL;

int set_sled_string(const char *str, unsigned char lightLevel);
int set_sled_value(int value, uint8_t point, unsigned char lightLevel);
int set_sled_off(void);

#ifdef __cplusplus
}
#endif

#endif	// #ifndef __M3_SLED_H__

