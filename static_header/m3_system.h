#ifndef __M3_SYSTEM_H__
#define __M3_SYSTEM_H__

#include <stdint.h>  

#ifdef __cplusplus
extern "C"{
#endif

int sys_get_battery_voltage(uint32_t *puiBackBatVol);
int sys_get_hw_version(char *pVersion);
int sys_update_boot(int offset, int len, void *pbuf, int finish);
int sys_update_kernel(int offset, int len, void *pbuf, int finish);
int sys_get_product_name(char* pname);
int sys_get_boot_version(char *pVersion);
int sys_get_kernel_version(char *pVersion);
int sys_reset(void);
int sys_set_hardware_serial(uint8_t len, const char *pSerial);
int sys_get_hardware_serial(char *pSerial);
int sys_set_terminal_serial(uint8_t len, const char *pSerial);
int sys_get_terminal_serial(char *pSerial);
int sys_set_product_status(uint8_t Status);
int sys_get_product_status(uint8_t *pucStatus);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __M3_SYSTEM_H__

