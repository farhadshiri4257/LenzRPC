#ifndef __RTC_UTIL_H__
#define __RTC_UTIL_H__

#include <stdint.h>

#define RTC_SYSTEM_TIME  0x01     // ����ϵͳʱ��
#define RTC_SYSTEM_DATE  0x02     // ����ϵͳ����

typedef struct{
    uint16_t  usYear;			  // ��(1970~2038)
    uint8_t   ucMonth;            // ��(1~12)
    uint8_t   ucDay;              // ��(1~31)
    uint8_t   ucDayOfWeek;        // ����(0~6)
    uint8_t   ucHour;             // ʱ(0~23)
    uint8_t   ucMinute;           // ��(0~59)
    uint8_t   ucSecond;           // ��(0~59)
}tRTCInfo;

extern int rtc_check_leap_year(int usYear);
extern uint32_t rtc_make_time_to_second(const tRTCInfo * ptDateTime);
extern void rtc_make_second_to_time(const uint32_t res, tRTCInfo *ptDateTime);
extern int rtc_set_date_time(const tRTCInfo * ptDateTime, uint8_t ucMode);
extern int rtc_get_date_time(tRTCInfo * ptDateTime);
extern int rtc_make_greenwich_time(const tRTCInfo * ptCurrentTime, tRTCInfo * ptGreenwich);
extern uint32_t rtc_calculate_day(int iYear, uint8_t ucMon, uint8_t ucDay);
extern int rtc_calculate_week(int iYear, uint8_t ucMonth, uint8_t ucDay);

#endif /* __RTC_UTIL_H__ */

