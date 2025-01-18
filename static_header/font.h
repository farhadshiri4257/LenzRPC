#ifndef FONT_H_
#define FONT_H_

#ifdef __cplusplus
 extern "C" {
#endif 

#define   SYS_FONT_LEN_OFFSET				0
#define	  SYS_FONT_LEN_SIZE					4u
#define   SFLASH_SYS_FONT_OFFSET			0

#define CHARSET_ASCII       0x10
#define CHARSET_GB2312_ZF   0x21
#define CHARSET_GB2312_HZ   0x22
#define CHARSET_GB18030_ZF  0x31
#define CHARSET_GB18030_HZ  0x32
#define CHARSET_GB18030_TW  0x33
#define CHARSET_GB18030_IME 0x3A
     
#define CHARSET_CP932		(0x41)		// 日文
#define CHARSET_CP936		(0x42)		// 简体中文
#define CHARSET_CP949		(0x43)		// 朝鲜语
#define CHARSET_CP950		(0x44)		// BIG5繁体中文
#define CHARSET_CP874		(0x45)		// 泰国语
#define CHARSET_CP1250		(0x46)		// 拉丁文(中欧)
#define CHARSET_CP1251		(0x47)		// 西里尔文
#define CHARSET_CP1252		(0x48)		// 拉丁文(西欧)
#define CHARSET_CP1253		(0x49)		// 希腊语
#define CHARSET_CP1254		(0x4A)		// 土耳其语
#define CHARSET_CP1255		(0x4B)		// 希伯来语
#define CHARSET_CP1256		(0x4C)		// 阿拉伯语
#define CHARSET_CP1257		(0x4D)		// 波罗的海文
#define CHARSET_CP1258		(0x4E)		// 越南语     

#define	PIXELS_PER_BYTE					8
#define	FONT_INDEX_TAB_SIZE				4    //单个字符对应的字体检索信息长度为 4Byte  (b0~b25: 记录点阵信息的起始地址, b26~b31: 记录当前字符的象素宽度)
#define	GET_FONT_WIDTH(charinfo)		((charinfo) >> 26)
#define	GET_FONT_OFFADDR(charinfo)		((charinfo) & 0x3ffffff)

#define SYSTEM_FONT_PATH			"/system/etc/K21Font.fnt"
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

typedef struct{
	uint16_t	First;        
	uint16_t	Last;         
	uint32_t	OffAddr;     
} FL_SECTION_INF, *PFL_SECTION_INF;

typedef struct{
	uint8_t 	magic[4];   //'U'(or ’M’), 'F', 'L', X---Unicode(or MBCS) Font Library, X: 表示版本号. 分高低4位。如 0x12表示 Ver 1.2
	uint32_t 	Size;            
	uint8_t 	nSection; 	// 共分几段数据，主要针对 UNICODE 编码有效。
	uint8_t 	YSize;                 
	uint16_t	wCpFlag;    // codepageflag:  bit0~bit13 每个bit分别代表一个CodePage 标志，如果是1，则表示当前CodePage 被选定，否则为非选定。
	uint8_t 	reserved[4];      // 预留字节   	
} FL_HEADER, *PFL_HEADER;

#define FONT_HEAD_LEN       1024
#define SFLASH_SYS_FONT_MAX_SIZE	(10*1024*1024)

/************************* GB18030 Specific Operations ************************/
#define GB18030_REG1_NUM			846						/* 汉字1区个数 */
#define GB18030_REG2_NUM			6768					/* 汉字2区个数 */
#define GB18030_REG3_NUM			6080					/* 汉字3区个数 */
#define GB18030_REG4_NUM			8160					/* 汉字4区个数 */
#define GB18030_REG5_NUM			192						/* 汉字5区个数 */
#define GB18030_4_BYTES_NUM			(161 + 1260 *5 +69)		/* 4字节汉字个数 */

#define GB18030_REG1_OFFSET			0
#define GB18030_REG5_OFFSET			0
#define GB18030_REG2_OFFSET			0
#define GB18030_REG3_OFFSET			(GB18030_REG2_OFFSET + GB18030_REG2_NUM)
#define GB18030_REG4_OFFSET			(GB18030_REG3_OFFSET + GB18030_REG3_NUM)
#define GB18030_4_BYTES_OFFSET		(GB18030_REG4_OFFSET + GB18030_REG4_NUM)


typedef struct
{
    int     	charset;        //  字符集
    int     	height;         //  字体高度
    int    	 	width;          //  字体宽度
    uint32_t    offaddr;        //  字体起始的偏移位置
    uint32_t    size;           //  字体文件的大小
}fontinfo_t;


/*字库文件的格式
   0 -  31:       字库识别名        //  如"LH01FONT V1.1"
  32 -  35:       字库包个数        //  指明了字库文件包含字库包的数量，int型小端存储
  36 -  39:       IME表个数         //  指明了字库文件包含IME否，int型小端存储
  39 - 255:       预留
 256 -1023:       字库包信息+IME表信息
1024 -size:       每个字库包的内容
*/

int font_load_default(void);
int font_load_all(void);
int s_font_match(int fontheight, int charset);
int font_get_dot(int fontheight, const char *string, int *width, void *fontmap);
int font_get_width(int fontheight, const char *string, int charnum, int maxwidth);
int sys_font_detect(int language, int height);

#ifdef __cplusplus
}
#endif

#endif 
