/**
 *              0 - Not registered, MT is not currently searching
 *                  a new operator to register
 *              1 - Registered, home network
 *              2 - Not registered, but MT is currently searching
 *                  a new operator to register
 *              3 - Registration denied
 *              4 - Unknown
 *              5 - Registered, roaming
*/

 /*			网络连接状态
		case 0:
            return STATE_OUT_OF_SERVICE;
        case 1:
            return STATE_IN_SERVICE;
        case 2: // 2 is "searching", fall through
        case 3: // 3 is "registration denied", fall through
        case 4: // 4 is "unknown", not valid in current baseband
            return STATE_OUT_OF_SERVICE;
        case 5:// 5 is "Registered, roaming"
            return STATE_IN_SERVICE;
*/
typedef struct{
    int gw_csq;
    int gw_bitErrorRate;
    int cdma_dbm;
    int cdma_ecio;
    int evdo_dbm;
    int evdo_ecio;
    int evdo_noiseratio;
    int lte_csq; //4g信号强度
    int lte_rsrp;
    int lte_rsrq;
    int lte_rssnr;
    int lte_cqi;
}FYL_RADIO_CSQ;

extern int setupDataCallofC(const char* ,const char* ,const char* ,const char*  ,const char*  ,const char*  ,const char* );
extern int queryDataStateOfC();
extern int connect_socket_of_c(void);//直接注册网络，不用配置
extern int close_read_of_c(void);
//extern int call_test_Of_C(void);
extern int closeDataConnofC(void);
extern int get_local_ip_of_C(char *rmnet_data0_ip);
extern int getSingalStrength(FYL_RADIO_CSQ *radio_csq);
extern int AirPalaneMode();		// 设置开启飞行模式

extern int WnetGetBaseStationInfo(uint32_t *puiLac, uint32_t *puiCellId);
extern int WnetGetBaseInfo(uint32_t *puiMcc, uint32_t *puiMnc, uint32_t *puiLac, uint32_t *puiBsic, uint32_t *puiCellId);
extern int getIMEI(char *imei);
extern int getModuleSN(char *moduleSN);
extern int getIMSI(char *imsi);
extern int getIccid(char *iccid);