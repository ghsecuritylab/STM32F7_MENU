#include "common.h"


#define SYSTEM_Y 2

//����汾��
#define sw_version_yearhi    0x20
#define sw_version_yearlo    0x19
#define sw_version_month     0x01
#define sw_version_day       0x27
#define sw_version_dayfrag   0xc2

#define fw_version_len 10
#define sw_version_len 10

extern uint8_t En_Or_Dis[En_Or_Dis_NUM];


uint8_t sw[5] = {sw_version_yearhi,sw_version_yearlo,sw_version_month,sw_version_day,sw_version_dayfrag};   //sw�汾��  ʮ������  ���� qt


uint8_t version_char[sw_version_len];   //�汾��  10���ַ�  ����         sw��fw����10���ַ�

//uint8_t sw_version[sw_version_len+1]={0};         //��Ҫ����qt    char
//uint8_t fw_version[fw_version_len+1]={0};         //��Ҫ����qt



/*************************************************
*�������ܣ� Get_SW  ��ȡ����汾�� 
*��������� 
*��������� 
*����ֵ  ��
*************************************************/
extern uint8_t* Get_SW(void)
{
	return sw;
}

/*************************************************
*�������ܣ� ��ȡӲ���汾�ţ� HW :    ����hex��ֵ  
*��������� 
*��������� 
*����ֵ  ��
*************************************************/
extern uint8_t Get_HW(void)
{
	//uint8_t HW[8]={0};
	uint8_t hw=0;
	/*
	//��4λ
	HW[0] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12);   //H
	HW[1] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
	HW[2] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);
	HW[3] = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);   //L
	//��4λ
	HW[4] = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12);   //H
	HW[5] = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_13);
	HW[6] = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_14);
	HW[7] = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_15);   //L
	*/
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12); 
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14);
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);
	
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_12); 
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_13);
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_14);
	hw <<= 1;
	hw |= HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_15);
	
	return hw;
}

//
/*************************************************
*�������ܣ� hw  ת�� char ��
*��������� 
*��������� 
*����ֵ  ��
*************************************************/
uint8_t* hw_0x_to_char(uint8_t hw_version)
{
	version_char[0] = hw_version / 16 + 0x30;
	version_char[1] = hw_version % 16 + 0x30;
	
	//version_char[2] = 0;   //��ֵ0 ����ֹoled��ʾ����
	
	return version_char;
}

//
/*************************************************
*�������ܣ� fw sw  ת�� char ��
*��������� 
*��������� 
*����ֵ  ��
*************************************************/
uint8_t* _0x_to_char(uint8_t *pstr_fw)
{
	uint8_t i=0;
//	pstr_fw = Get_FW();    //��ȡFw�汾��   ʮ������ ��0x20 0x18 0x12 0x11 0xb3 --> "20181211B3"
	
	if(pstr_fw != NULL){   //ת��char   �浽version_char[]������
		version_char[0] = pstr_fw[0] / 16 + 0x30;
		version_char[1] = pstr_fw[0] % 16 + 0x30;
		
		version_char[2] = pstr_fw[1] / 16 + 0x30;
		version_char[3] = pstr_fw[1] % 16 + 0x30;
		
		version_char[4] = pstr_fw[2] / 16 + 0x30;
		version_char[5] = pstr_fw[2] % 16 + 0x30;
		
		version_char[6] = pstr_fw[3] / 16 + 0x30;
		version_char[7] = pstr_fw[3] % 16 + 0x30;
		
		version_char[8] = pstr_fw[4] / 16 + 0x30;
		version_char[9] = pstr_fw[4] % 16 + 0x30;
		
		for(i=0; i<fw_version_len; i++){
			if(version_char[i] > 0x39)
				version_char[i] += 7;         //�������ĸ��ת����ĸ��ascii
		}
		
		return version_char;
	}else{
		printf(" version get error !\r\n");
		return NULL;
	}

}
/*************************************************
*�������ܣ� ��ʾ  SW  HW  FW �汾��               HW:Vx.x
*��������� 
*��������� 
*����ֵ  ��
*************************************************/
extern void Display_Version(void)
{
	char pstr_data[4][Display_Line_Width]={"SW:","HW:V","FW:","Exit"};
	uint8_t *version = NULL;
	uint8_t *fw_pstr = NULL;
	uint8_t hw_version;
	uint8_t dot = '.';
	
	version = _0x_to_char(sw);    			//��ȡsw�汾��  char��
	if(version != NULL)
		strncat(pstr_data[0], version, sw_version_len);    //��дsw�汾��
	
	hw_version = Get_HW();    //��ȡhw�汾��  hex��
	version = hw_0x_to_char(hw_version);     //ת��char��
	strncat(pstr_data[1], &version[0], 1); 
	strncat(pstr_data[1], &dot, 1); 
	strncat(pstr_data[1], &version[1], 1); 
	
	
	fw_pstr = Get_FW();//��ȡ5��hex��ֵ
	version = _0x_to_char(fw_pstr);    //��ȡfw�汾��  char��
	if(version != NULL)
		strncat(pstr_data[2], version, fw_version_len);    //��дfw�汾��             
	
	
	OLED_ShowString(0,1,pstr_data[0],12);
	OLED_ShowString(0,2,pstr_data[1],12);
	OLED_ShowString(0,3,pstr_data[2],12);
//	printf(" pstr_fw %s ",pstr_fw);      // 0x20 0x18 0x12 0x11 0xb3

}
/*************************************************
*�������ܣ� ��λ����  
*��������� 
*��������� 
*����ֵ  ��
*************************************************/
extern void Restart_Action(void)
{
	ReStart();       //iwdg.c
}
/*************************************************
*�������ܣ� �ָ��������� ����λ 
*��������� 
*��������� 
*����ֵ  ��
*************************************************/
extern void Restore_Action(void)
{
	Restore_Flash();
	ReStart();	
}
/*************************************************
*�������ܣ� ���¶���   ����
*��������� 
*��������� 
*����ֵ  ��
*************************************************/
extern void Update_Action(void)
{
	char pstr_data[2][Display_Line_Width]={"FACTORY ONLY","Exit"};
	OLED_ShowString(0,SYSTEM_Y,pstr_data[0],12);
}





