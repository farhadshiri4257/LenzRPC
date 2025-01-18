/*
 * @Author: user
 * @Date: 2020-08-26 10:10:56
 * @LastEditTime: 2020-10-14 15:52:32
 * @LastEditors: user
 * @Description: 
 * @FilePath: \src\lcd\inc\lcd.h
 * @��Ҳ���������˲�˯!
 */
#ifndef __LCD_H__
#define __LCD_H__
#include <stdio.h>
#include "cutils/log.h"

#include "minui/minui.h"
#include "freetype/include/ft2build.h"
#include "freetype/include/freetype.h"
#include "graphics.h"

#include "pixelflinger/pixelflinger.h"

//#ifdef LOG_TAG
//#undef LOG_TAG
//#define LOG_TAG   "minui"
//#endif

#define DEFAULT_FONT_PATH "/system/etc/mmi/fonts.ttf"

/**One line in screen, */
#define DEFAULT_UNICODE_STR_LEN 256

//  lcdDisplay��ʾ�����С0x00-0x99
#define	FONT_SIZE8		8//0x0001       //  С������ʾ(6*8)
#define	FONT_SIZE12		12//0x0002//      //  ��������ʾ(6*12)
#define	FONT_SIZE16		16//0x0004//     //  ��������ʾ(8*16)
#define	FONT_SIZE20		20//0x0008//     //  ��������ʾ(8*20)
#define	FONT_SIZE24		24//0x0010//      //  ��������ʾ(12*24)
#define	FONT_SIZE32		32//0x0020//      //  ��������ʾ(20*32)
#define	FONT_SIZE48		48//0x0040//      //  ��������ʾ(24*48)
#define	FONT_SIZE64		64//0x0080//     //  ��������ʾ(32*64)

//��ʾģʽ
#define	DISP_CLRLINE	0x0100   //  �����ʾռ�õ���
#define	DISP_MEDIACY	0x0200   //  ������ʾ
#define	DISP_INVCHAR	0x0400   //  ������ʾ���ַ���
#define	DISP_INVLINE	0x0800   //  ������ʾ�����У�
#define	DISP_RIGHTAT	0x1000   //  �Ҷ�����ʾ

// ��ɫ����
#define DISP_COLOR_BLACK 	0x01  	// ��ɫ
#define DISP_COLOR_DRBLU 	0x02 	// ����ɫ
#define DISP_COLOR_WHITE 	0x03 	// ��ɫ	
#define DISP_COLOR_SKBLU 	0x04 	// ����ɫ
#define DISP_COLOR_YELLOW 	0x05 	// ��ɫ
#define DISP_COLOR_LGREEN	0x06	// ǳ��ɫ
#define DISP_COLOR_RED		0x07	//��ɫ
#define DISP_COLOR_BACKGROUND  0X6085FF     //0X7B68EE

#define DISP_COLOR_XMLQ_BLUE1 	0xF1 	// ����·���õ�����ɫ1 ����ɫ
#define DISP_COLOR_XMLQ_BLUE2 	0xF2 	// ����·���õ�����ɫ2 ����ɫ


void lcd_init(void);
int lcd_cls(void);
int lcd_clrline(int begline, int endline);
int lcd_goto(int x, int y);
int lcd_set_inv(int mode);
int lcd_set_font(int font);
int lcd_plot(int x, int y, int color);
int lcd_draw_line(int StartX, int StartY, int EndX, int EndY, int Mode);
int lcd_printf(const char *format, ...);
int lcd_display(int x, int y, int disp_mode, const char *format, ...);
int lcd_bmp_disp(int x, int y, const void *bmpdata);
int lcd_bmp_path_disp(int x, int y, const void *bmpdata);
int lcd_png_disp(int x, int y, const void *pngdata);
void lcd_refresh(void);
int lcd_draw_yellow_line(int StartX, int StartY, int EndX, int EndY, int Mode);

int lcd_cls_color(int bg_color);									// ָ������ɫ LCD���� 
int lcd_clrline_color(int begline, int endline, int bg_color); 	// ָ������ɫ���ָ���ļ�����Ϣ
int lcd_display_color(int x, int y,  int disp_mode, int font_color, int bg_color, const char *format, ...);

int lcd_clear(void);			// �ض�����������bjbus

int lcd_display_persian(int x, int y,  int disp_mode, const char *format, ...);
int lcd_display_color_right_alignment(int x, int y,  int disp_mode, int font_color, int bg_color, const char *format, ...);

#if 0
typedef struct{
	uint8_t r;
	uint8_t g;
	uint8_t b;
//	uint8_t a;
}LCD_COLOR_RGB_STR;


int lcd_display_color_rgb(int x, int y,  int disp_mode,

									LCD_COLOR_RGB_STR st_fnc, 
									LCD_COLOR_RGB_STR st_bgc, 
									const char *format, ...);
#endif


/*
*rgb_fn,rgb_bg :0x000000-0xFFFFFF
*/
int lcd_display_color_rgb(int x, int y,  int disp_mode,
										unsigned int rgb_fn,
										unsigned int rgb_bg,
										const char *format, ...);

//Clear the line according with the set color
int lcd_cls_color_rgb(unsigned int rgb_bg);

int lcd_clrline_color_rgb(int begline, int endline, unsigned int rgb_bg);


#endif
