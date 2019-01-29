//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2018-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(STM32F4ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PG12��SCL��
//              D1   ��PD5��SDA��
//              RES  ���濪���帴λ������
//              DC   ��PD15
//              CS   ��PD1               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//  ��������   : 2018-0101
//All rights reserved
//******************************************************************************/

#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    

//-----------------����LED�˿ڶ���---------------- 


//-----------------OLED�˿ڶ���----------------  					   


#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET)//CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11,GPIO_PIN_RESET)//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_11,GPIO_PIN_SET)

//#define OLED_SDIN_READ() HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_1)

#define OLED_RST_Clr() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_12,GPIO_PIN_RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_12,GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(GPIOF,GPIO_PIN_13,GPIO_PIN_SET)
 		     
#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_14,GPIO_PIN_SET)

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);

extern void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size);
void OLED_ShowChar2(u8 x,u8 y,u8 chr,u8 size);
extern void OLED_ShowChar_2(u8 x,u8 y,u8 chr,u8 size);

void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);

void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size);
void OLED_ShowString2(u8 x,u8 y,u8 *chr,u8 size);

void OLED_ShowString_2(u8 x,u8 y,u8 *chr,u8 size);  //����

extern void OLED_ShowString_Size22(u8 x,u8 y,u8 *chr,u8 size);

void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



