#include "common.h"

//ip addr
#define STM32_IP_0  0xc0   //192
#define STM32_IP_1  0xa8   //168
#define STM32_IP_2  0
#define STM32_IP_3  0xbc    //188
//mac addr
#define STM32_MAC_B0  0x00
#define STM32_MAC_B1  0x80
#define STM32_MAC_B2  0xE1
#define STM32_MAC_B3  0x00
#define STM32_MAC_B4  0x00
#define STM32_MAC_B5  0x00

#define User_Arr_Len  256
#define FPGA_Arr_Len  256

extern uint8_t Display_Line[][Display_Line_Width];  

uint8_t send_to_eth[3]={0};

static uint8_t Update_Flash_Flag = 0;
static uint8_t Error_Status;           //fpga  no link : 1;     fpga link : 0
static uint8_t Status_Flag=2;     //Ĭ�� init ״̬

// En_Or_Dis 5 + 
uint8_t User_Arr[User_Arr_Len]={0};
uint8_t FPGA_Arr[FPGA_Arr_Len]={0};



uint8_t RESTORE[RESTORE_NUM]={0x01};
uint8_t default_RESTORE[RESTORE_NUM]={0x00};


extern MENU_STATUS System_Feature_Arr[En_Or_Dis_NUM];
extern MENU_STATUS Config_Format_Arr[CONFIG_FORMAT_NUM];
extern MENU_STATUS Config_Video_Input_Arr[InputNum];
extern MENU_STATUS Config_Video_Output_Arr[OutputNum];
extern unsigned char Ip_Arr[IP_NUM]; 
extern unsigned char Mac_Arr[MAC_ARR_NUM];  
extern char utc_offset_num;
extern MENU_STATUS Config_Src_Mode_Arr[SRC_MODE_NUM];
extern MENU_STATUS Config_Src_Sync_Type_Arr[SRC_SYNV_TYPE_NUM] ;
extern MENU_STATUS Config_Src_Clock_Type_Arr[SRC_CLOCK_TYPE_NUM];
extern MENU_STATUS Config_Src_Sync_Frame_Arr[SRC_SYNC_FRAME_NUM];
extern MENU_STATUS Config_Display_Mode_Arr[DISPLAY_MODE_NUM];
extern MENU_STATUS Config_Display_Brightness_Arr[DISPLAY_BRIGHTNESS_NUM];



MENU_STATUS Default_System_Feature_Arr[En_Or_Dis_NUM] = {MENU_SET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET};
MENU_STATUS Default_Config_Format_Arr[CONFIG_FORMAT_NUM] = {MENU_SET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET,
																										 MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, 
																										 MENU_RESET};
MENU_STATUS Default_Config_Video_Input_Arr[InputNum] = {MENU_SET, MENU_RESET, MENU_RESET};
MENU_STATUS Default_Config_Video_Output_Arr[OutputNum] = {MENU_SET, MENU_RESET};
unsigned char Default_Ip_Arr[IP_NUM] = {STM32_IP_0,STM32_IP_1,STM32_IP_2,STM32_IP_3}; 
unsigned char Default_Mac_Arr[MAC_ARR_NUM] = {STM32_MAC_B0,STM32_MAC_B1,STM32_MAC_B2,STM32_MAC_B3,STM32_MAC_B4,STM32_MAC_B5};  
 char Default_utc_offset_num = 8;
MENU_STATUS Default_Config_Src_Mode_Arr[SRC_MODE_NUM] = {MENU_SET, MENU_RESET};
MENU_STATUS Default_Config_Src_Sync_Type_Arr[SRC_SYNV_TYPE_NUM] = {MENU_SET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET,
																																	 MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET};
MENU_STATUS Default_Config_Src_Clock_Type_Arr[SRC_CLOCK_TYPE_NUM] = {MENU_SET, MENU_RESET, MENU_RESET};
MENU_STATUS Default_Config_Src_Sync_Frame_Arr[SRC_SYNC_FRAME_NUM] = {MENU_SET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET, MENU_RESET};
MENU_STATUS Default_Config_Display_Mode_Arr[DISPLAY_MODE_NUM] = {MENU_RESET, MENU_RESET, MENU_SET};
MENU_STATUS Default_Config_Display_Brightness_Arr[DISPLAY_BRIGHTNESS_NUM] = {MENU_SET, MENU_RESET};



/*************************************************************************
*�������ܣ� ������������Ҫ��ʾstatus״̬
*��������� 0 : GOOD ״̬   ----     1 : ERROR״̬   -----     2 : INIT״̬
*���������
*����ֵ  ��
*************************************************************************/
extern void Set_Status(uint8_t status)            //set status  ��������Ҫ��ʾstatus״̬
{
	if(status == 0){
		Status_Flag = 0;      //good
		User_Arr[0x11] = 0;
		UserLEDR(0) ;         //error led down
	}else if(status == 1){
		Status_Flag = 1;      //error            oled��ʾ
		User_Arr[0x11] = 1;
		UserLEDR(1) ;         //error led up
	}else if(status == 2){
		Status_Flag = 2;      //init
	//	UserLEDR(0) ;         //error led down
	}
}
/*************************************************************************
*�������ܣ� ��ȡ��������Ҫ��ʾstatus״̬
*��������� 0 : GOOD ״̬   ----     1 : ERROR״̬   -----     2 : INIT״̬
*���������
*����ֵ  ��
*************************************************************************/
extern uint8_t  Get_Status(void)
{
	return Status_Flag;
}	








/*************************************************************************
*�������ܣ� ��default ���� д��flash���´ο���ʱ����ָ���������
*��������� 
*���������
*����ֵ  ��
*************************************************************************/
extern void Restore_Flash(void)
{
	romflash_pages_write(RESTORE_NUM_ADDR, default_RESTORE, sizeof(default_RESTORE));  //д�� restore 0x00 ��־     
}






/*************************************************************************
*�������ܣ� ���� Config_Format  ģʽ
*��������� 
*���������
*����ֵ  �� "Auto"=1, "720p50Hz"=2, "720p59Hz"=3, "720p60Hz"=4, "1080i50Hz"=5, "1080i59Hz"=6, "1080i60Hz"=7, "1080p23Hz"=8,
						"1080p24Hz"=9, "1080p25Hz"=10, "1080p29Hz"=11, "1080p30Hz"=12, "1080p50Hz"=13, "1080p59Hz"=14, "1080p60Hz"=15,
						"1080pSF23Hz=16", "1080pSF24Hz"=17        
						""=0
*************************************************************************/
extern uint8_t Get_Config_Format(void)
{
	uint8_t i=0;
	for(i=0; i<CONFIG_FORMAT_NUM; i++){
		if(Config_Format_Arr[i] == MENU_SET)
			return (i+1);
	}
	return 0;
}
/*************************************************************************
*�������ܣ� ���� Config_Video_Input  ģʽ
*��������� 
*���������
*����ֵ  �� HDMI = 1, HDSDI = 2, TESTPAR =  3 ,  ""=0
*************************************************************************/
extern uint8_t Get_Config_Video_Input(void)
{
	uint8_t i=0;
	for(i=0; i<InputNum; i++){
		if(Config_Video_Input_Arr[i] == MENU_SET)
			return (i+1);
	}
	return 0;
}
/*************************************************************************
*�������ܣ� ���� Config_Video_Output ģʽ
*��������� 
*���������
*����ֵ  �� HDMI = 1, HDSDI = 2,  ""=0
*************************************************************************/
extern uint8_t Get_Config_Video_Output(void)
{
	uint8_t i=0;
	for(i=0; i<OutputNum; i++){ 
		if(Config_Video_Output_Arr[i] == MENU_SET)
			return (i+1);
	}
	return 0;
}

/*************************************************************************
*�������ܣ� ����utc�ʱ��  HEX
*��������� 
*���������
*����ֵ  �� 
*************************************************************************/
extern int8_t Get_UTCoffset_Num(void)
{
	return utc_offset_num;
}

/*************************************************************************
*�������ܣ� ����һ�ֽ�ip����ֵ  HEX
*��������� 
*���������
*����ֵ  �� 
*************************************************************************/
extern uint8_t Get_IP_Fragment(uint8_t fragment)
{
	return Ip_Arr[fragment]; 
}
//
/*************************************************************************
*�������ܣ� ����һ�ֽ�mac����ֵ  HEX
*��������� 
*���������
*����ֵ  �� 
*************************************************************************/
extern uint8_t Get_MAC_Fragment(uint8_t fragment)
{
	return Mac_Arr[fragment]; 
}
 //
/*************************************************************************
*�������ܣ� ��������ģʽ
*��������� 
*���������
*����ֵ  ��  1 : ����ģʽ  ----         0 : �ӻ�ģʽ
*************************************************************************/
extern uint8_t Get_SRC_MODE(void)   
{ 
	if(Config_Src_Mode_Arr[0] == MENU_SET)
		return 1;   //��ǰ����ģʽ
	else
		return 0;   //��ǰ�ӻ�ģʽ
}
/*************************************************************************
*�������ܣ� ����Config_SRC_synctype
*��������� 
*���������
*����ֵ  �� "Bot_Grag"=1, "Bot_Black"=2, "Bot_White"=3, "Top_Grag"=4, "Top_Black"=5, "Top_White"=6, "Four_corners"=7, "Obvious"=8
						"Hidden_1"=9, "Hidden_2"=10,  "Hidden_3"=11, "Hidden_4"=12  
						""=0
*************************************************************************/
extern uint8_t Get_Config_SRC_synctype(void)
{
	uint8_t i=0;
	for(i=0; i<SRC_SYNV_TYPE_NUM; i++){
		if(Config_Src_Sync_Type_Arr[i] == MENU_SET)
			return (i+1);
	}
	return 0;
}
/*************************************************************************
*�������ܣ� ����Config_SRC_clocktype
*��������� 
*���������
*����ֵ  �� "Wire_Mode"=1,"OSC_Mode"=2,"GPS_Mode"=3
						""=0
*************************************************************************/
extern uint8_t Get_Config_SRC_clocktype(void)
{
	uint8_t i=0;
	for(i=0; i<SRC_CLOCK_TYPE_NUM; i++){
		if(Config_Src_Clock_Type_Arr[i] == MENU_SET)
			return (i+1);
	}
	return 0;
}
/*************************************************************************
*�������ܣ� ����Config_SRC_syncframe
*��������� 
*���������
*����ֵ  �� "25_Frames"=1,"30_Frames"=2,"50_Frames"=3,"60_Frames"=4,"100_Frames"=5,"120_Frames"=6
						""=0
*************************************************************************/
extern uint8_t Get_Config_SRC_syncframe(void)
{
	uint8_t i=0;
	for(i=0; i<SRC_SYNC_FRAME_NUM; i++){
		if(Config_Src_Sync_Frame_Arr[i] == MENU_SET)
			return (i+1);
	}
	return 0;
}
/*************************************************************************
*�������ܣ� ����Config_Display_mode
*��������� 
*���������
*����ֵ  �� "Video_Delay"=1,"Audio_Delay"=2,"Detail"=3
						""=0
*************************************************************************/
extern uint8_t Get_Config_Display_mode(void)
{
	uint8_t i=0;
	for(i=0; i<DISPLAY_MODE_NUM; i++){
		if(Config_Display_Mode_Arr[i] == MENU_SET)
			return (i+1);
	}
	return 0;
}
/*************************************************************************
*�������ܣ� ����Config_Display_Brightness
*��������� 
*���������
*����ֵ  �� 1 : ����    ----     0 : ����
*************************************************************************/
extern uint8_t Get_Config_Display_Brightness(void)
{
	if(Config_Display_Brightness_Arr[0] == MENU_SET)
			return 1;  //��ǰ����
	else
			return 0;  //��ǰ����
}
//
/*************************************************************************
*�������ܣ� ����GPS_Display_Status GPS_Display_Status[0]������GPS������  
*��������� 
*���������
*����ֵ  �� 1:�����˵�����ʾGPS��utcʱ�䣬  0:����ʾ
*************************************************************************/
extern uint8_t Get_GPS_Display_Status(void)
{
	if(System_Feature_Arr[0] == MENU_SET){
	//	printf(" 1 ");
		return 1;
	}
	else{
	//	printf(" 0 ");
		return 0;
	}
}

//
/*************************************************************************
*�������ܣ� stm32������ ,  ������� fpga regֵ  , ����  �ڲ˵���������ʱ
*��������� 
*���������
*����ֵ  �� 
************************************************************************/
extern void Set_FPGA_Reg_Config(uint32_t sector_addr,uint8_t reg_addr)
{
	uint8_t temp;	
	  //  spi -> fpga  ������fpga�ķ���ֵ  ����Ῠ��
	if(sector_addr == InputNum_ADDR || reg_addr == 0x01){   
		if(Config_Video_Input_Arr[2] == MENU_SET){                  
			Send_Test_Pattern(0x80);    //test_pattern
		}else{
			Send_Test_Pattern(0x00);
		}
	}else if(sector_addr == SRC_MODE_NUM_ADDR || reg_addr == 0x04){    
		if(Config_Src_Mode_Arr[0] == MENU_SET){               
			Send_Src_Mode(0x80);              //  	printf(" master ");
		}else if(Config_Src_Mode_Arr[1] == MENU_SET){
			Send_Src_Mode(0x00);              //		printf(" slave ");
		}
	}else if(sector_addr == SRC_CLOCK_TYPE_NUM_ADDR || reg_addr == 0x0D){	
		if(Config_Src_Clock_Type_Arr[0] == MENU_SET){
			temp = 0x00;
			Send_ClockSync_Mode(temp);   // 
		}else if(Config_Src_Clock_Type_Arr[1] == MENU_SET){
			temp = 0x40;
			Send_ClockSync_Mode(temp);   //  
		}else if(Config_Src_Clock_Type_Arr[2] == MENU_SET){
			temp = 0x80;
			Send_ClockSync_Mode(temp);   //  
		}
		Send_SyncType_Mode(0);    //ÿ������SRC_CLOCK_TYPEʱ�������SRC_SYNV_TYPE���0x00����wire_modeģʽ�£�Ĭ��ѡ��Bot_Greg��gps_mode/osc_modeĬ��ѡ��Obvious
	}else if(sector_addr == SRC_SYNV_TYPE_NUM_ADDR || reg_addr == 0x06){	
		if(Config_Src_Sync_Type_Arr[0] == MENU_SET){
			temp = 0x00;
			Send_SyncType_Mode(temp);   
		}else if(Config_Src_Sync_Type_Arr[1] == MENU_SET){
			temp = 0x10;
			Send_SyncType_Mode(temp);   //  
		}else if(Config_Src_Sync_Type_Arr[2] == MENU_SET){
			temp = 0x20;
			Send_SyncType_Mode(temp);   //  
		}else if(Config_Src_Sync_Type_Arr[3] == MENU_SET){
			temp = 0x30;
			Send_SyncType_Mode(temp);   //  
		}else if(Config_Src_Sync_Type_Arr[4] == MENU_SET){
			temp = 0x40;
			Send_SyncType_Mode(temp);   //  
		}else if(Config_Src_Sync_Type_Arr[5] == MENU_SET ){
			temp = 0x50;
			Send_SyncType_Mode(temp);   //  
		}else if(Config_Src_Sync_Type_Arr[6] == MENU_SET){
			temp = 0x60;
			Send_SyncType_Mode(temp);   //  
		}else if(Config_Src_Sync_Type_Arr[7] == MENU_SET){
			temp = 0x00;
			Send_SyncType_Mode(temp); 
		}else if(Config_Src_Sync_Type_Arr[8] == MENU_SET){
			temp = 0x70;
			Send_SyncType_Mode(temp); 
		}else if(Config_Src_Sync_Type_Arr[9] == MENU_SET){
			temp = 0x80;
			Send_SyncType_Mode(temp);   //  
		}else if(Config_Src_Sync_Type_Arr[10] == MENU_SET){
			temp = 0x90;
			Send_SyncType_Mode(temp);   //  
		}else if(Config_Src_Sync_Type_Arr[11] == MENU_SET){
			temp = 0xa0; 
			Send_SyncType_Mode(temp);   //  
		}	
	}else if(sector_addr == SRC_SYNC_FRAME_NUM_ADDR || reg_addr == 0x10){
		if(Config_Src_Sync_Frame_Arr[0] == MENU_SET){
			temp = 0x00;
			Send_SyncFrame_Mode(temp);   
		}else if(Config_Src_Sync_Frame_Arr[1] == MENU_SET){
			temp = 0x20;
			Send_SyncFrame_Mode(temp);   //  
		}else if(Config_Src_Sync_Frame_Arr[2] == MENU_SET){
			temp = 0x40;
			Send_SyncFrame_Mode(temp);   //  
		}else if(Config_Src_Sync_Frame_Arr[3] == MENU_SET){
			temp = 0x60;
			Send_SyncFrame_Mode(temp);   //  
		}else if(Config_Src_Sync_Frame_Arr[4] == MENU_SET){
			temp = 0x80;
			Send_SyncFrame_Mode(temp);   //  
		}else if(Config_Src_Sync_Frame_Arr[5] == MENU_SET){
			temp = 0xa0;
			Send_SyncFrame_Mode(temp);   //  
		}
	}else if(sector_addr == CONFIG_FORMAT_NUM_ADDR || reg_addr == 0x05){           //tpg
		if(Config_Format_Arr[0] == MENU_SET){         //auto  default 720p60
			temp = 0x38;
			Send_Format_2_0x05(temp);   
			temp = 0x00;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[1] == MENU_SET){ //720p50
			temp = 0x00;                 
			Send_Format_2_0x05(temp);    
			temp = 0x00;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[2] == MENU_SET){  //720p59
			temp = 0x38;
			Send_Format_2_0x05(temp);   
			temp = 0x80;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[3] == MENU_SET){  //720p60
			temp = 0x38;
			Send_Format_2_0x05(temp);   
			temp = 0x00;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[4] == MENU_SET){  //1080i50
			temp = 0x18;
			Send_Format_2_0x05(temp);     
			temp = 0x00;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[5] == MENU_SET){  //1080i59
			temp = 0x10;
			Send_Format_2_0x05(temp);     
			temp = 0x80;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[6] == MENU_SET){  //1080i60
			temp = 0x10;
			Send_Format_2_0x05(temp);     
			temp = 0x00;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[7] == MENU_SET){  //1080p23
			temp = 0x30;
			Send_Format_2_0x05(temp);     
			temp = 0x80;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[8] == MENU_SET){  //1080p24
			temp = 0x30;
			Send_Format_2_0x05(temp);     
			temp = 0x00;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[9] == MENU_SET){  //1080p25
			temp = 0x28;
			Send_Format_2_0x05(temp);     
			temp = 0x00;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[10] == MENU_SET){  //1080p29
			temp = 0x20;
			Send_Format_2_0x05(temp);     
			temp = 0x80;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[11] == MENU_SET){  //1080p30
			temp = 0x20;
			Send_Format_2_0x05(temp);     
			temp = 0x00;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[12] == MENU_SET){  //1080p50
			temp = 0x2c;
			Send_Format_2_0x05(temp);     
			temp = 0x00;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[13] == MENU_SET){  //1080p59
			temp = 0x24;
			Send_Format_2_0x05(temp);     
			temp = 0x80;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[14] == MENU_SET){  //1080p60
			temp = 0x24;
			Send_Format_2_0x05(temp);     
			temp = 0x00;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[15] == MENU_SET){  //1080pSF23
			temp = 0x08;
			Send_Format_2_0x05(temp);     
			temp = 0x80;
			Send_Format_2_0x11(temp);
		}else if(Config_Format_Arr[16] == MENU_SET){  //1080pSF24
			temp = 0x08;
			Send_Format_2_0x05(temp);     
			temp = 0x00;
			Send_Format_2_0x11(temp);
		}
	}
}
/*************************************************************************
*�������ܣ� ����ǰoled�������д��flash
*��������� 
*���������
*����ֵ  �� 
************************************************************************/
extern void Write2Flash(void)
{
	romflash_clean_pages(ADDR_FLASH_SECTOR_11);    //clear ��Ҫ800ms
	
	RESTORE[0] = 0x01;
	
	//��ȡflash   д��buf
	romflash_pages_write(En_Or_Dis_NUM_ADDR, System_Feature_Arr, sizeof(System_Feature_Arr));
	romflash_pages_write(CONFIG_FORMAT_NUM_ADDR, Config_Format_Arr, sizeof(Config_Format_Arr));
	romflash_pages_write(InputNum_ADDR, Config_Video_Input_Arr, sizeof(Config_Video_Input_Arr));
	romflash_pages_write(OutputNum_ADDR, Config_Video_Output_Arr, sizeof(Config_Video_Output_Arr));
	romflash_pages_write(IP_NUM_ADDR, Ip_Arr, sizeof(Ip_Arr));
	romflash_pages_write(MAC_ARR_NUM_ADDR, Mac_Arr, sizeof(Mac_Arr));
	romflash_pages_write(UTCOFFSET_NUM_ADDR, &utc_offset_num, 1);
	romflash_pages_write(SRC_MODE_NUM_ADDR, Config_Src_Mode_Arr, sizeof(Config_Src_Mode_Arr));
	romflash_pages_write(SRC_SYNV_TYPE_NUM_ADDR, Config_Src_Sync_Type_Arr, sizeof(Config_Src_Sync_Type_Arr));
	romflash_pages_write(SRC_CLOCK_TYPE_NUM_ADDR, Config_Src_Clock_Type_Arr, sizeof(Config_Src_Clock_Type_Arr));
	romflash_pages_write(SRC_SYNC_FRAME_NUM_ADDR, Config_Src_Sync_Frame_Arr, sizeof(Config_Src_Sync_Frame_Arr));
	romflash_pages_write(DISPLAY_MODE_NUM_ADDR, Config_Display_Mode_Arr, sizeof(Config_Display_Mode_Arr));
	romflash_pages_write(DISPLAY_BRIGHTNESS_NUM_ADDR, Config_Display_Brightness_Arr, sizeof(Config_Display_Brightness_Arr));

	romflash_pages_write(RESTORE_NUM_ADDR, RESTORE, sizeof(RESTORE));               ///0x00
	
	Update_User_Arr(0,0,1);        //test
}
/*************************************************************************
*�������ܣ� ��ȡflash���ip��mac��ַ
*��������� 
*���������
*����ֵ  �� 
************************************************************************/
extern void Read_Flash_IP_MAC(void)
{
	uint8_t flag=0;
	//�ȶ�һ��  �ж��Ƿ���Ҫ ʹ��Ĭ��ip
	romflash_pages_read(RESTORE_NUM_ADDR, RESTORE, sizeof(RESTORE));
	if(RESTORE[0] == 0x00){ 
		Ip_Arr[0]=STM32_IP_0;
		Ip_Arr[1]=STM32_IP_1;
		Ip_Arr[2]=STM32_IP_2;
		Ip_Arr[3]=STM32_IP_3;
	}else if(RESTORE[0] == 0x01){  
		romflash_pages_read(IP_NUM_ADDR, Ip_Arr, sizeof(Ip_Arr));
	}else{
		
	}
	romflash_pages_read(MAC_ARR_NUM_ADDR, Mac_Arr, sizeof(Mac_Arr));
}
/************************************************************************
*�������ܣ� ��ȡflash��ֵ������oled��������
*��������� 
*���������
*����ֵ  �� 
************************************************************************/
extern uint8_t ReadFlash2Arr(void)
{
	uint8_t flag=0;
	//�ȶ�һ��  �ж��Ƿ���Ҫ �ָ�Ĭ��ֵ
	
	romflash_pages_read(RESTORE_NUM_ADDR, RESTORE, sizeof(RESTORE));
	
	if(RESTORE[0] == 0x00){           //�ж��Ƿ���Ҫ 0x01�ָ�Ĭ��ֵ
		romflash_pages_read(En_Or_Dis_NUM_ADDR, System_Feature_Arr, sizeof(System_Feature_Arr));     //�ȶ�һ�� En_Or_Dis  �����ϴε�ֵ
		romflash_pages_read(MAC_ARR_NUM_ADDR, Mac_Arr, sizeof(Mac_Arr));          //�ȶ�һ�� MAC  �����ϴε�ֵ
		
		flag = romflash_clean_pages(ADDR_FLASH_SECTOR_11);	          //!!

		RESTORE[0] = 0x01;
		//дĬ��ֵ
		romflash_pages_write(En_Or_Dis_NUM_ADDR, System_Feature_Arr, sizeof(System_Feature_Arr));  //��дĬ��ֵ
		romflash_pages_write(CONFIG_FORMAT_NUM_ADDR, Default_Config_Format_Arr, sizeof(Config_Format_Arr));
		romflash_pages_write(InputNum_ADDR, Default_Config_Video_Input_Arr, sizeof(Config_Video_Input_Arr));
		romflash_pages_write(OutputNum_ADDR, Default_Config_Video_Output_Arr, sizeof(Config_Video_Output_Arr));
		romflash_pages_write(IP_NUM_ADDR, Default_Ip_Arr, sizeof(Ip_Arr));
		romflash_pages_write(MAC_ARR_NUM_ADDR, Default_Mac_Arr, sizeof(Mac_Arr));
		romflash_pages_write(UTCOFFSET_NUM_ADDR, &Default_utc_offset_num, 1);
		romflash_pages_write(SRC_MODE_NUM_ADDR, Default_Config_Src_Mode_Arr, sizeof(Config_Src_Mode_Arr));
		romflash_pages_write(SRC_SYNV_TYPE_NUM_ADDR, Default_Config_Src_Sync_Type_Arr, sizeof(Config_Src_Sync_Type_Arr));
		romflash_pages_write(SRC_CLOCK_TYPE_NUM_ADDR, Default_Config_Src_Clock_Type_Arr, sizeof(Config_Src_Clock_Type_Arr));
		romflash_pages_write(SRC_SYNC_FRAME_NUM_ADDR, Default_Config_Src_Sync_Frame_Arr, sizeof(Config_Src_Sync_Frame_Arr));
		romflash_pages_write(DISPLAY_MODE_NUM_ADDR, Default_Config_Display_Mode_Arr, sizeof(Config_Display_Mode_Arr));
		romflash_pages_write(DISPLAY_BRIGHTNESS_NUM_ADDR, Default_Config_Display_Brightness_Arr, sizeof(Config_Display_Brightness_Arr));
		
		romflash_pages_write(RESTORE_NUM_ADDR, RESTORE, sizeof(RESTORE));               ///0x00
			
		//�����ȡ  ���Ҵ��뵽��ǰbuf��   
		
		romflash_pages_read(En_Or_Dis_NUM_ADDR, System_Feature_Arr, sizeof(System_Feature_Arr));
		romflash_pages_read(CONFIG_FORMAT_NUM_ADDR, Config_Format_Arr, sizeof(Config_Format_Arr));
		romflash_pages_read(InputNum_ADDR, Config_Video_Input_Arr, sizeof(Config_Video_Input_Arr));
		romflash_pages_read(OutputNum_ADDR, Config_Video_Output_Arr, sizeof(Config_Video_Output_Arr));
		romflash_pages_read(IP_NUM_ADDR, Ip_Arr, sizeof(Ip_Arr));
		romflash_pages_read(MAC_ARR_NUM_ADDR, Mac_Arr, sizeof(Mac_Arr));
		romflash_pages_read(UTCOFFSET_NUM_ADDR, &utc_offset_num, 1);
		romflash_pages_read(SRC_MODE_NUM_ADDR, Config_Src_Mode_Arr, sizeof(Config_Src_Mode_Arr));
		romflash_pages_read(SRC_SYNV_TYPE_NUM_ADDR, Config_Src_Sync_Type_Arr, sizeof(Config_Src_Sync_Type_Arr));
		romflash_pages_read(SRC_CLOCK_TYPE_NUM_ADDR, Config_Src_Clock_Type_Arr, sizeof(Config_Src_Clock_Type_Arr));
		romflash_pages_read(SRC_SYNC_FRAME_NUM_ADDR, Config_Src_Sync_Frame_Arr, sizeof(Config_Src_Sync_Frame_Arr));
		romflash_pages_read(DISPLAY_MODE_NUM_ADDR, Config_Display_Mode_Arr, sizeof(Config_Display_Mode_Arr));
		romflash_pages_read(DISPLAY_BRIGHTNESS_NUM_ADDR, Config_Display_Brightness_Arr, sizeof(Config_Display_Brightness_Arr));
		
		
	}else if(RESTORE[0] == 0x01){  //����Ҫ�ָ�Ĭ��ֵ   �����ȡ  ���Ҵ��뵽��ǰbuf�� 
		romflash_pages_read(En_Or_Dis_NUM_ADDR, System_Feature_Arr, sizeof(System_Feature_Arr));
		romflash_pages_read(CONFIG_FORMAT_NUM_ADDR, Config_Format_Arr, sizeof(Config_Format_Arr));
		romflash_pages_read(InputNum_ADDR, Config_Video_Input_Arr, sizeof(Config_Video_Input_Arr));
		romflash_pages_read(OutputNum_ADDR, Config_Video_Output_Arr, sizeof(Config_Video_Output_Arr));
		romflash_pages_read(IP_NUM_ADDR, Ip_Arr, sizeof(Ip_Arr));
		romflash_pages_read(MAC_ARR_NUM_ADDR, Mac_Arr, sizeof(Mac_Arr));
		romflash_pages_read(UTCOFFSET_NUM_ADDR, &utc_offset_num, 1);
		romflash_pages_read(SRC_MODE_NUM_ADDR, Config_Src_Mode_Arr, sizeof(Config_Src_Mode_Arr));
		romflash_pages_read(SRC_SYNV_TYPE_NUM_ADDR, Config_Src_Sync_Type_Arr, sizeof(Config_Src_Sync_Type_Arr));
		romflash_pages_read(SRC_CLOCK_TYPE_NUM_ADDR, Config_Src_Clock_Type_Arr, sizeof(Config_Src_Clock_Type_Arr));
		romflash_pages_read(SRC_SYNC_FRAME_NUM_ADDR, Config_Src_Sync_Frame_Arr, sizeof(Config_Src_Sync_Frame_Arr));
		romflash_pages_read(DISPLAY_MODE_NUM_ADDR, Config_Display_Mode_Arr, sizeof(Config_Display_Mode_Arr));
		romflash_pages_read(DISPLAY_BRIGHTNESS_NUM_ADDR, Config_Display_Brightness_Arr, sizeof(Config_Display_Brightness_Arr));
	}
	
	
	Update_User_Arr(0,0,1);        //test
	
	return RESTORE[0];


	
}
/*
*�������ܣ� 
*��������� 
*���������
*����ֵ  �� 
*/
extern void Write2Flash_Send2FPGA(uint32_t sector_addr)
{
	uint8_t i;
	//ReSet_Change_Flag();	
	
	uint8_t temp;
	
#if 1
		if(sector_addr == InputNum_ADDR || sector_addr == OutputNum_ADDR){
			if(System_Feature_Arr[2] == 0x00){
				Config_Video_Input_Arr[0] = 0x02;           //����ѡ
				Config_Video_Output_Arr[0] = 0x02;
			}
			if(System_Feature_Arr[3] == 0x00){
				Config_Video_Input_Arr[1] = 0x02;
				Config_Video_Output_Arr[1] = 0x02;
			}
			if(System_Feature_Arr[4] == 0x00){
				Config_Video_Input_Arr[2] = 0x02;
			}
		}	
		
#endif		
		
	Write2Flash();	
		
#ifdef FPGA_LINK				
	Set_FPGA_Reg_Config(sector_addr,0);
#endif			
	
}

/*
*�������ܣ� ͨ�������Ͳ�����pc��
*��������� 
*���������
*����ֵ  �� 
*/
extern void Send_User_Data(uint8_t *Command_Arr,uint16_t Len)
{
	uint16_t pos=0;
	
	if(Command_Arr[0] == 0x02){             //��ָ��  0x02   
		pos = Command_Arr[1]; 
		send_to_eth[0] = Command_Arr[0];
		send_to_eth[1] = pos;
		send_to_eth[2] = User_Arr[pos];       //User_Arr[pos]
		
		Eth_Send_api(send_to_eth,3);
		
	}else if(Command_Arr[0] == 0x03){       //дָ��  0x03
		pos = Command_Arr[1]; 
		send_to_eth[0] = Command_Arr[0];
		send_to_eth[1] = pos;
		User_Arr[pos] = Command_Arr[2];       //test          //User_Arr[pos]
		
		Update_Flash_Arr(User_Arr,pos);       //��user_arr����  ����  �� ���Ӧ��flash������ 
		
		send_to_eth[2] = Command_Arr[2];       //test          //User_Arr[pos]
		
		Eth_Send_api(send_to_eth,3);
		
		Set_Update_Flash_Flag(1);    //Ӧ����eth txcallback��  �Ÿ���flash             pollint����ѯ���
		
	}
	
}

//
/*
*�������ܣ� ��pc����޸�System_Feature_Arr�����������  �� �� Config_Video_Output_Arr �� Config_Video_Input_Arr ���� ��Ӱ��
*��������� 
*���������
*����ֵ  �� 
*/
extern void En_Or_Dis_UpData(void)
{	
	//TEST	
		if(System_Feature_Arr[2] == MENU_RESET){
			Config_Video_Input_Arr[0] = NON_OPERATE;           //����ѡ
			Config_Video_Output_Arr[0] = NON_OPERATE;
		}else{
			if(Config_Video_Input_Arr[0] == NON_OPERATE)       //��� ����ѡ�� ����         ������Ϊ'0'
				Config_Video_Input_Arr[0] = MENU_RESET;  
			if(Config_Video_Output_Arr[0] == NON_OPERATE)			
				Config_Video_Output_Arr[0] = MENU_RESET;
		}
		
		if(System_Feature_Arr[3] == MENU_RESET){
			Config_Video_Input_Arr[1] = NON_OPERATE;
			Config_Video_Output_Arr[1] = NON_OPERATE;
		}else{
			if(Config_Video_Input_Arr[1] == NON_OPERATE)       //��� ����ѡ�� ����         ������Ϊ'0'
				Config_Video_Input_Arr[1] = MENU_RESET;  
			if(Config_Video_Output_Arr[1] == NON_OPERATE)			
				Config_Video_Output_Arr[1] = MENU_RESET;
		}
		
		if(System_Feature_Arr[4] == MENU_RESET){
			Config_Video_Input_Arr[2] = NON_OPERATE;
		}else{
			if(Config_Video_Input_Arr[2] == NON_OPERATE)     //��� ����ѡ�� ����         ������Ϊ'0'
				Config_Video_Input_Arr[2] = MENU_RESET;         
		}	
}


//
/*
*�������ܣ� �Ƿ���Ҫ����FLASH
*��������� 1������flash   0��������flash
*���������
*����ֵ  �� 
*/
extern void Set_Update_Flash_Flag(uint8_t flag)
{
	if(flag)
		Update_Flash_Flag = 1;
	else
		Update_Flash_Flag = 0;
}
//
/*
*�������ܣ� ��ȡ����FLASH ��־
*��������� 
*���������
*����ֵ  �� 1������flash   0��������flash
*/
extern uint8_t Get_Update_Flash_Flag(void)
{
	return Update_Flash_Flag;
}
/*
*�������ܣ� ����Config_Format
*��������� 
*���������
*����ֵ  ��
*/
extern void Set_Config_Format(uint8_t pos)
{
	int8_t num,i;
	num = pos - 1;
	for(i=0; i<CONFIG_FORMAT_NUM; i++){
		if(i == num)
			Config_Format_Arr[i] = MENU_SET;
		else
			Config_Format_Arr[i] = MENU_RESET;
	}
}
extern void Set_Config_Video_Input(uint8_t pos)
{
	int8_t num,i;
	num = pos - 1;
	for(i=0; i<InputNum; i++){
		if(i == num)
			Config_Video_Input_Arr[i] = MENU_SET;
		else
			Config_Video_Input_Arr[i] = MENU_RESET;
	}
}
extern void Set_Config_Video_Output(uint8_t pos)
{
	int8_t num,i;
	num = pos - 1;
	for(i=0; i<OutputNum; i++){
		if(i == num)
			Config_Video_Output_Arr[i] = MENU_SET;
		else
			Config_Video_Output_Arr[i] = MENU_RESET;
	}
}

extern void Set_UTCoffset_Num(int8_t num)
{
	utc_offset_num = num;
}
//��������ģʽ
extern void Set_SRC_MODE(uint8_t mode)
{
	if(mode == 1){
		Config_Src_Mode_Arr[0] = MENU_SET;
		Config_Src_Mode_Arr[1] = MENU_RESET;
	}
	else if(mode == 0){
		Config_Src_Mode_Arr[0] = MENU_RESET;
		Config_Src_Mode_Arr[1] = MENU_SET;
	}
}
extern void Set_Config_SRC_synctype(uint8_t pos)
{
	int8_t num,i;
	num = pos - 1;
	for(i=0; i<SRC_SYNV_TYPE_NUM; i++){
		if(i == num)
			Config_Src_Sync_Type_Arr[i] = MENU_SET;
		else
			Config_Src_Sync_Type_Arr[i] = MENU_RESET;
	}
}
extern void Set_Config_SRC_clocktype(uint8_t pos)
{
	int8_t num,i;
	num = pos - 1;
	for(i=0; i<SRC_CLOCK_TYPE_NUM; i++){
		if(i == num)
			Config_Src_Clock_Type_Arr[i] = MENU_SET;
		else
			Config_Src_Clock_Type_Arr[i] = MENU_RESET;
	}
}
extern void Set_Config_SRC_syncframe(uint8_t pos)
{
	int8_t num,i;
	num = pos - 1;
	for(i=0; i<SRC_SYNC_FRAME_NUM; i++){
		if(i == num)
			Config_Src_Sync_Frame_Arr[i] = MENU_SET;
		else
			Config_Src_Sync_Frame_Arr[i] = MENU_RESET;
	}
}
extern void Set_Config_Display_mode(uint8_t pos)
{
	int8_t num,i;
	num = pos - 1;
	for(i=0; i<DISPLAY_MODE_NUM; i++){
		if(i == num)
			Config_Display_Mode_Arr[i] = MENU_SET;
		else
			Config_Display_Mode_Arr[i] = MENU_RESET;
	}
}

/*
*�������ܣ� ����flash   
*��������� 
*���������
*����ֵ  ��
*/
//eth:0x03 + addr + data
extern void Update_Flash_Arr(uint8_t *user_arr,uint16_t pos)
{
	uint16_t i=0;
	uint8_t temp;
#if 1	
	if(pos<5){                          //������ ����En_Or_Dis��Ϣ
		//System_Feature_Arr[pos] = User_Arr[pos];
		if(User_Arr[pos] == 0x01)
			System_Feature_Arr[pos] = MENU_SET;
		else if(User_Arr[pos] == 0x00)
			System_Feature_Arr[pos] = MENU_RESET;
	}
	En_Or_Dis_UpData();             //������
#endif	
	if(pos == 0x0f){
		if( User_Arr[pos] == 0x01){
			Config_Display_Brightness_Arr[0] = MENU_SET;   //����
			Config_Display_Brightness_Arr[1] = MENU_RESET;   //
			OLED_WR_Byte(0x81,OLED_CMD); //--set contrast control register
			OLED_WR_Byte(0xff,OLED_CMD); // Set SEG Output Current Brightness 
		}else if( User_Arr[pos] == 0x00){
			Config_Display_Brightness_Arr[0] = MENU_RESET;   //����
			Config_Display_Brightness_Arr[1] = MENU_SET;   //
			OLED_WR_Byte(0x81,OLED_CMD); //--set contrast control register
			OLED_WR_Byte(0x0f,OLED_CMD); // Set SEG Output Current Brightness 
		}
	}
	
	if(pos > 0x11 && pos < 0x16){        //0x12 0x13 0x14 0x15   ���������ip��ַ
		i = pos-0x12;
		Ip_Arr[i] = User_Arr[pos];
	}else if(pos == 0x10){                   // set config_Format
		Set_Config_Format(User_Arr[pos]);
	}else	if(pos == 0x1c){                   // set video_input
		Set_Config_Video_Input(User_Arr[pos]);
	}else if(pos == 0x1d){                   // set video_output
		Set_Config_Video_Output(User_Arr[pos]);
	}else if(pos == 0x1e){                   // set UTC_Offset
		Set_UTCoffset_Num(User_Arr[pos]);
	}else if(pos == 0x1f){                   // set SRC_MODE
		Set_SRC_MODE(User_Arr[pos]);
	}else if(pos == 0x20){                   // set Config_SRC_synctype
		Set_Config_SRC_synctype(User_Arr[pos]);   
	}else if(pos == 0x21){                   // set Config_SRC_clocktype
		Set_Config_SRC_clocktype(User_Arr[pos]);
	}else if(pos == 0x22){                   // set Config_SRC_syncframe
		Set_Config_SRC_syncframe(User_Arr[pos]);
	}else if(pos == 0x23){                   // set Config_Display_mode
		Set_Config_Display_mode(User_Arr[pos]);
	}else if(pos == 0xff){                    //reset
		if(User_Arr[pos] == 0xff){
			ReStart();
		}
	}
	
#if 0	  //for test
	for(i=0; i<User_Arr_Len; i++){
		printf(" %d ",User_Arr[i]);
	}
#endif	
}
/* ---------------------------------------------------Update_User_Arr-----------------------------------------------------*/
//  eth:0x02 + addr + 0x00
/*
*�������ܣ� �����û�����   
*��������� 
*���������
*����ֵ  ��
*/
extern void Update_User_Arr(uint8_t start_pos,uint8_t len,uint8_t complete_update_flag)
{
	uint16_t i=0;
	uint16_t pos=0;
	
	uint8_t *sw_pstr=NULL;
	uint8_t *fw_pstr=NULL;
	
	uint8_t temp;
	
	sw_pstr = Get_SW();             
	fw_pstr = Get_FW();
	
	//��flash������ ���µ�User_Arr��
	if(1){       																//ȫ������    //complete_update_flag
		for(i=0; i<En_Or_Dis_NUM; i++,pos++){     //En_Or_Dis��flash ���� �� user_arr
			if(System_Feature_Arr[i] == MENU_SET)
				User_Arr[pos] = 1;
			else if(System_Feature_Arr[i] == MENU_RESET)
				User_Arr[pos] = 0;
		}		
		
		if(sw_pstr != NULL){
			pos = 5;   //0x05
			for(i=0; i<5; i++,pos++){               //sw �汾 ��flash ���� �� user_arr
				User_Arr[pos] = sw_pstr[i];
			}
		}
		
		if(fw_pstr != NULL){
			pos = 10; //0x0a
			for(i=0; i<5; i++,pos++){               //fw �汾 ��flash ���� �� user_arr
				User_Arr[pos] = fw_pstr[i];
			}
		}
		
		pos = 0x0f;
		User_Arr[pos] = Get_Config_Display_Brightness();
		
		pos++;   //0x10
		User_Arr[pos] = Get_Config_Format();    //get_config_format
		
		pos++;   //0x11
		User_Arr[pos] = Get_Status();//Get_Error_Status();
		
		pos = 0x16;
		for(i=0; i<MAC_ARR_NUM; i++,pos++){     //mac_Arr��flash ���� �� user_arr
			User_Arr[pos] = Get_MAC_Fragment(i);
		}
/*		
		for(i=0; i<MAC_ARR1_NUM; i++,pos++){     //mac_Arr_1��flash ���� �� user_arr
			User_Arr[pos] = mac_Arr_1[i];
		}
		pos = 0x19;
		for(i=0; i<MAC_ARR2_NUM; i++,pos++){     //mac_Arr_2��flash ���� �� user_arr
			User_Arr[pos] = mac_Arr_2[i];
		}
*/		
		
		
		pos = 0x1c;
		User_Arr[pos] = Get_Config_Video_Input();   //Video_Input
		
		pos++;//= 0x1d;
		User_Arr[pos] = Get_Config_Video_Output();      //Video_Output
		
		pos++;// = 0x1e;
		User_Arr[pos] = Get_UTCoffset_Num();         //utc
		
		pos++;// = 0x1f;
		User_Arr[pos] = Get_SRC_MODE();        //src mode   :  master 1 / slave 0
		
		pos++;// = 0x20;
		User_Arr[pos] = Get_Config_SRC_synctype();   //Config_SRC_synctype
		
		pos++;// = 0x21;
		User_Arr[pos] = Get_Config_SRC_clocktype();   //Config_SRC_clocktype
		
		pos++;// = 0x22;
		User_Arr[pos] = Get_Config_SRC_syncframe();    //Config_SRC_syncframe
		
		pos++;// = 0x23;
		User_Arr[pos] = Get_Config_Display_mode();    //Config_Display_mode
		
		pos++;// = 0x24;
		User_Arr[pos] = Get_HW();                    //Get_HW
		
	}else{                          //���²���
	
	}
#if 0	  //for test
	for(i=0; i<User_Arr_Len; i++){
		printf(" %d ",User_Arr[i]);
	}
#endif	
}























