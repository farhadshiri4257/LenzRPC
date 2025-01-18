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
	int		SigLevel;		//  �ź�ǿ��	
	char		SSID[64];		//  SSID	
	int		SecurityType;	//  Sets the Wireless LAN security type.(0:open; 1:shared; 2:WPA; 3:WPA2)
	int		EncryptType;	//  Sets the Wireless LAN encrypt type (0: none; 1:WEP; 2:TKIP; 3: AES)
	//char		WPAPSK[64];		//  Personal Shared Key Pass-Phrase.	
	//char		WEPKey[32];		//  WEP Key 
}WiFiStatus_t;  // Wi-Fi����״̬

typedef struct{
	int		Channel;		// ͨ��
	char		SSID[64];		//  SSID
	char		BSSID[20];
	int		SecurityType;	//  Sets the Wireless LAN security type.(0:open; 1:shared; 2:WPA; 3:WPA2)
	int		EncryptType;	//  Sets the Wireless LAN encrypt type (0: none; 1:WEP; 2:TKIP; 3: AES)	
	int		SigLevel;		// �ź�ǿ��
}WiFiAPInfo_t;      //  available ap info in surrounding area
/*
typedef struct{
	int 		network_id;
	char		SSID[64];		//  SSID
	int 		type; //0 DISABLE ; 1 CURRENT	
}WifiNetwork_t; //������ȵ���Ϣ
*/
//A ��ʹ�ô�����������һ���ȵ�ʱ��LIST_NETWORK�Ὣ���ȵ㵱���������ȵ㡣
//B ��ʹ����ȷ���������ȵ㣬�����䲻��IPʱ��LIST_NETWORK�õ�����ϢҲ��������ġ�
//���ԣ�һ����˵������STATUS���������Ч�����ܵõ���ǰ��������״̬�Ƿ�ΪCOMPLETED



int WifiInit(void);//��ʼ��wifi����
int WifiEnable(void);//����wifi
int WifiDisable(void);//�ر�wifi

int WifiScanAP(int maxlistnum, WiFiAPInfo_t APlist[]);//ɨ��AP
int WifiConnectAP(const WiFiAP_t *APxParms);//����AP
int WifiCheckAPStatus(WiFiStatus_t *CurStatus);//��ȡ��ǰAP״̬


//int WifiPing(char *ipstr, int timeoutms);
int WifiGetMAC(void *macaddr);//��ȡMAC��ַ

#endif

