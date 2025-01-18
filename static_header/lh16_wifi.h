#ifndef __LH16_WIFI_H_
#define __LH16_WIFI_H_

#define	WIFI_SECURITY_TYPE_OPEN		0	  	// open mode
#define	WIFI_SECURITY_TYPE_SHARED	1		// shared mode
#define	WIFI_SECURITY_TYPE_WPA		2		// WPA
#define	WIFI_SECURITY_TYPE_WPA2		3		// WPA2

#define	WIFI_ENCRYPT_TYPE_NONE		0		// none encryption
#define	WIFI_ENCRYPT_TYPE_WEP		1		// WEP encryption
#define	WIFI_ENCRYPT_TYPE_TKIP		2		// TKIP encryption
#define	WIFI_ENCRYPT_TYPE_AES			3		// AES encryption



typedef struct{
	char		SSID[64];		//  SSID
	int		SecurityType;	//  Sets the Wireless LAN security type.(0:open; 1:shared; 2:WPA; 3:WPA2)
	int		EncryptType;	//  Sets the Wireless LAN encrypt type (0: none; 1:WEP; 2:TKIP; 3: AES)
	char		WPAPSK[64];		//  Personal Shared Key Pass-Phrase.
	int		WepKeyIndex;	// WEP Key Index
	char		WEPKey[4][32];	//  WEP Key Array
}WiFiAP_t;       //  can be configured up to 10 sets(0~9), the default set is No.0.

typedef struct{
	int		Status;             /*  port status:    0:Connected;
							1:Disconnected;
							2:RF Off; */	
	int		SigLevel;		//  信号强度	
	char		SSID[64];		//  SSID	
	int		SecurityType;	//  Sets the Wireless LAN security type.(0:open; 1:shared; 2:WPA; 3:WPA2)
	int		EncryptType;	//  Sets the Wireless LAN encrypt type (0: none; 1:WEP; 2:TKIP; 3: AES)
	//char		WPAPSK[64];		//  Personal Shared Key Pass-Phrase.	
	//char		WEPKey[32];		//  WEP Key 
}WiFiStatus_t;  // Wi-Fi连接状态

typedef struct{
	int		Channel;		// 通道
	char		SSID[64];		//  SSID
	char		BSSID[20];
	int		SecurityType;	//  Sets the Wireless LAN security type.(0:open; 1:shared; 2:WPA; 3:WPA2)
	int		EncryptType;	//  Sets the Wireless LAN encrypt type (0: none; 1:WEP; 2:TKIP; 3: AES)	
	int		SigLevel;		// 信号强度
}WiFiAPInfo_t;      //  available ap info in surrounding area
/*
typedef struct{
	int 		network_id;
	char		SSID[64];		//  SSID
	int 		type; //0 DISABLE ; 1 CURRENT	
}WifiNetwork_t; //保存的热点信息
*/
//A 当使用错误密码连接一个热点时，LIST_NETWORK会将此热点当成已连接热点。
//B 当使用正确密码连接热点，但分配不到IP时，LIST_NETWORK得到的信息也是有问题的。
//所以，一般来说，还是STATUS命令更加有效，它能得到当前的网卡的状态是否为COMPLETED



int WifiInit(void);//初始化wifi驱动
int WifiEnable(void);//开启wifi
int WifiDisable(void);//关闭wifi

int WifiScanAP(int maxlistnum, WiFiAPInfo_t APlist[]);//扫描AP
int WifiConnectAP(const WiFiAP_t *APxParms);//连接AP
int WifiCheckAPStatus(WiFiStatus_t *CurStatus);//获取当前AP状态


//int WifiPing(char *ipstr, int timeoutms);
int WifiGetMAC(void *macaddr);//获取MAC地址

#endif

