#ifndef GPS_H_
#define GPS_H_


#define GPS_SUCCESS			0
#define GPS_ERR_PARA      	-2800
#define GPS_RECV_TIMEOUT  	-2801
#define GPS_ERR_OVERFLOW  	-2802


/** GPS status event values. */
typedef uint16_t GpsStatusValue;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
/** GPS status unknown. δ֪*/
#define GPS_STATUS_NONE             0 
/** GPS has begun navigating. ���ڶ�λ*/
#define GPS_STATUS_SESSION_BEGIN    1
/** GPS has stopped navigating. ֹͣ��λ*/
#define GPS_STATUS_SESSION_END      2
/** GPS has powered on but is not navigating. ������δ��λ*/
#define GPS_STATUS_ENGINE_ON        3
/** GPS is powered off. δ����GPS*/
#define GPS_STATUS_ENGINE_OFF       4


// User interface structure
typedef struct
{
	int8_t year;
	int8_t month;
	int8_t day;
	int8_t hour;
	int8_t minute;
	int8_t second;
}GPS_TIME;

typedef struct
{
	int location_status; 		// whether the positioning is successful. 0 is FAILURE, 1 IS SUCCESS 
	unsigned char WDNS;			// Latitude N'/'S' （Geographic coordinates）
	double WD;					// Latitude
	unsigned char JDWE;			// Longitude  E/W（Geographic coordinates）
	double JD;					// Longitude 
	GpsStatusValue status;	 	// GPS status. 0 is unknown, 1 is being 
								// positioning, 2 stops positioning, 3 starts but not positioned, 4 does not start.    
	unsigned int WXS;			// the number of satellites used 
	float HDOP;					// horizontal accuracy
	float SPEED;				// ground speed, unit: knot
	float XHQD;					// ground heading, unit: degree
	int64_t TIME;				// seconds since 1970-01-01 0:0:0 
}GPS_INF;

typedef int32_t (*nmea_cb)(const char *sNmeaString, uint32_t sNmeaStringLength);


int Gps_Open(void);
void Gps_Close(void);
int32_t Get_GPS_Info(GPS_INF *Gps_Info, uint32_t WaitTime);
int32_t reportNmea(nmea_cb lh_nmea_cb);



#endif

