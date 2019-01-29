#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"

#include "common.h"

extern uint8_t Config_Display_Brightness_Arr[];


//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;

//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 

void OLED_WR_Byte2(u8 dat,u8 cmd)    //����
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
			OLED_SDIN_Clr();
		  
		else 
		    OLED_SDIN_Set();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 


	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte((((x+2)&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte(((x+2)&0x0f),OLED_CMD); 
}   	  

/*
uint8_t OLED_RE_Byte(u8 x,u8 y)
{	
	u8 i;			
	uint8_t temp=0;	
	
	OLED_Set_Pos(x,y);     //�������йء�����  
	
	OLED_DC_Set();    // dc 1
	
	OLED_CS_Set();
	
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)
	{		
		temp <<= 1;
		OLED_SCLK_Set();
		if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_1)){
		   temp |= 0x01;
		}
		OLED_SCLK_Clr();
		
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   

	printf(" %x ",temp);   // 00 ff 00 ff ...
	
		return temp;
} 

*/



//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ    OLED_WR_Byte (0x02,OLED_CMD);  
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 

extern void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(size ==16){           ///------------------��һ������-----------------
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
				OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
				OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}else if(size == 12){	    ///-------------------��2������-----------------
				OLED_Set_Pos(x,y+1);
				//for(i=0;i<6;i++)
			for(i=0;i<8;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);	
		}else if(size == 32){   //oled_audio/video  display  -------��3������-----------------
			OLED_Set_Pos(x,y);	
			for(i=0;i<16;i++)
				OLED_WR_Byte(F32X32[c*64+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<16;i++)
				OLED_WR_Byte(F32X32[c*64+i+16],OLED_DATA);
			OLED_Set_Pos(x,y+2);	
			for(i=0;i<16;i++)
				OLED_WR_Byte(F32X32[c*64+i+32],OLED_DATA);
			OLED_Set_Pos(x,y+3);
			for(i=0;i<16;i++)
				OLED_WR_Byte(F32X32[c*64+i+48],OLED_DATA);
		}
}
extern void OLED_ShowChar_2(u8 x,u8 y,u8 chr,u8 size)   //����
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(size ==16){            ///------------------��һ������-----------------
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte2(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte2(F8X16[c*16+i+8],OLED_DATA);
		}else if(size == 12){	    ///-------------------��2������-----------------
				OLED_Set_Pos(x,y+1);
			//	for(i=0;i<6;i++)
			for(i=0;i<8;i++)
				OLED_WR_Byte2(F6x8[c][i],OLED_DATA);	
		}else if(size == 32){   //oled_audio/video  display  -------��3������-----------------
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
				OLED_WR_Byte2(F32X32[c*32+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
				OLED_WR_Byte2(F32X32[c*32+i+8],OLED_DATA);
			OLED_Set_Pos(x,y+2);	
			for(i=0;i<8;i++)
				OLED_WR_Byte2(F32X32[c*32+i+16],OLED_DATA);
			OLED_Set_Pos(x,y+3);
			for(i=0;i<8;i++)
				OLED_WR_Byte2(F32X32[c*32+i+24],OLED_DATA);
		}
}



/*
uint8_t page_3_buf[128];
uint8_t page_5_buf[128];

void OLED_ShowChar2(u8 x,u8 y,u8 chr,u8 size)
{      	
	unsigned char c=0,i=0;	
	uint8_t temp=0;
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(size ==16){   //�� 2ҳ page  16��
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X13[c*16+i] ,OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++){
				OLED_WR_Byte(F8X13[c*16+i+8],OLED_DATA);
				page_3_buf[x+i]= F8X13[c*16+i+8];       //!!!!!!!!!!!	page_3_buf
			}
		}else if(size ==14){   //�� 2ҳ page  16��
				OLED_Set_Pos(x,y);	
				for(i=0;i<8;i++){
					temp = F8X14[c*24+i] | page_5_buf[x+i] ; //!!!!!!!!!!!!!!!!!!!!!!!!!!	page_5_buf 
					OLED_WR_Byte(temp ,OLED_DATA);
				}
				OLED_Set_Pos(x,y+1);
				for(i=0;i<8;i++){
					OLED_WR_Byte(F8X14[c*24+i+8],OLED_DATA);    
				}
		}else if(size == 32){   //oled_audio/video  display
			OLED_Set_Pos(x,y);	
				for(i=0;i<8;i++){
					temp = F16X16[c*24+i] | page_5_buf[x+i] ; //!!!!!!!!!!!!!!!!!!!!!!!!!!	page_5_buf 
					OLED_WR_Byte(temp ,OLED_DATA);
				}
				OLED_Set_Pos(x,y+1);
				for(i=0;i<8;i++){
					OLED_WR_Byte(F16X16[c*24+i+8],OLED_DATA);    
				}
		}else{	   //size ==12    �պ�һҳ page  8��
				OLED_Set_Pos(x,y+1);
		//		for(i=0;i<6;i++)
			for(i=0;i<8;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
		}
}
*/

/*
void OLED_ShowChar_Size22(u8 x,u8 y,u8 chr,u8 size)
{      	
	unsigned char c=0,i=0;	
	uint8_t temp=0;
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(size == 22)    //�� 3 ҳ  page
			{
			OLED_Set_Pos(x,y);		
			for(i=0;i<8;i++){
				temp = F8X20[c*24+i] | page_3_buf[x+i];   //!!!!!!!!!!!	page_3_buf
				OLED_WR_Byte( temp,OLED_DATA);
			}
			
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++){
			  OLED_WR_Byte(F8X20[c*24+i+8] ,OLED_DATA);
			}
			
		  OLED_Set_Pos(x,y+2);
			for(i=0;i<8;i++){
				OLED_WR_Byte(F8X20[c*24+i+16],OLED_DATA);
				page_5_buf[x+i]= F8X20[c*24+i+16];       //!!!!!!!!!!!!!!!!!!!!!!!!!!	page_5_buf 
			}
			}
			
}

*/

//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)    //size=16
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size); 
	}
} 

//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],size);
			if(size == 32)
				x+=16;
			else
				x+=8;
			
		if(x>120){x=0;y+=2;}
			j++;
	}
}

void OLED_ShowString_2(u8 x,u8 y,u8 *chr,u8 size)  //���� 
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar_2(x,y,chr[j],size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

/*
void OLED_ShowString2(u8 x,u8 y,u8 *chr,u8 size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar2(x,y,chr[j],size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
*/

/*
extern void OLED_ShowString_Size22(u8 x,u8 y,u8 *chr,u8 size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar_Size22(x,y,chr[j],size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
*/

//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//��ʼ��SSD1306			



void OLED_Init(void)
{ 	 		 
  GPIO_InitTypeDef  GPIO_InitStruct;
	
	/* PF10 PF11 PF12 PF13 PF14 */
	
		GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	

	/*
	//GPIO��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 |  GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	
	
		//GPIO��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
	*/
	
 
  OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(200);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	if(Config_Display_Brightness_Arr[0] == MENU_SET)
		OLED_WR_Byte(0xff,OLED_CMD); // Set SEG Output Current Brightness          // ���öԱȶ�  00 ���� ff
	else
		OLED_WR_Byte(0x0f,OLED_CMD); 
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
}  






























