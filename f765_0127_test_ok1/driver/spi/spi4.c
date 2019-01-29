#include "common.h"

#define Uart2_Send_Size 3

//��д ָ��
#define Write_Flag 0x01
#define Read_Flag 0x00
//reg ��ַ
#define config_r82_addr  0x82  
#define config_r83_addr  0x83  
#define config_r84_addr  0x84  
#define config_r87_addr  0x87  
#define config_r88_addr  0x88   
#define config_r89_addr  0x89
#define config_r8a_addr  0x8a
#define config_r8b_addr  0x8b
#define config_r8c_addr  0x8c
#define config_r8d_addr  0x8d
#define config_r8e_addr  0x8e
#define SRC_MODE_ADDR 0x04                  	//������ģʽreg��ַ
#define JIT_REG_ADDR_1 0x80                   //0x30; //      						���ӳ�reg��ַ0x80; 
#define JIT_REG_ADDR_2 0x81                   //0x31; //         					���ӳ�reg��ַ0x81; 

SPI_HandleTypeDef hspi4;

volatile uint8_t spi_lock=0;

//uint8_t none;

// spi���ͽ�������
uint8_t Write_Data[16]={0x00,0x5a,0x5a};
uint8_t Read_Data[16]={0};
uint8_t Null_arr[16] = {0};

//���·��ͱ�־
volatile uint8_t retry_flag=0;


uint8_t FW_REG_ADDR[5] = {0x94,0x95,0x96,0x97,0x98};   //   ��FW�汾��reg��ַ


//��reg��ֵ
uint8_t jit_reg_num_1 = 0x00;
uint8_t jit_reg_num_2 = 0x00;
uint8_t fw_reg_num[5] = {0};
uint8_t config_r82_num=0;
uint8_t config_r83_num=0;
uint8_t config_r84_num=0;
uint8_t config_r87_num=0;
uint8_t config_r88_num=0;
uint8_t config_r89_num=0;
uint8_t config_r8a_num=0;
uint8_t config_r8b_num=0;
uint8_t config_r8c_num=0;
uint8_t config_r8d_num=0;
uint8_t config_r8e_num=0;

//extern uint8_t SRC_MODE[];   //test

//
uint8_t i=0;




/* SPI4 init function */
extern void MX_SPI4_Init(void)
{

  /* SPI4 parameter configuration*/
  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;//SPI_NSS_HARD_OUTPUT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;  // div64 1.35M
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 7;
  hspi4.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi4.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* ------------------------------------------Set_NSS_Status--------------------------------------------*/

void Set_NSS_Status(uint8_t status)
{
	if(status)
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4,GPIO_PIN_SET) ;
	else
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4,GPIO_PIN_RESET) ;
}
/* ------------------------------------------Set_Retry_SPI4--------------------------------------------*/
void Set_Retry_SPI4(uint8_t num)
{
	if(num){
		retry_flag=1;
	}else{
		retry_flag=0;
	}
}


/*- ----------------------------------��ȡreg��Ϣ-------------------------------------*/
extern uint8_t Get_Jit_Reg_1(void)        //0x80
{
	uint8_t send_data[3] = {Read_Flag,JIT_REG_ADDR_1,0x00};
	while(spi_lock == 1){
		__NOP();__NOP();__NOP();

	}
	Send_To_Fpga(send_data,3);
	return jit_reg_num_1;
}
extern uint8_t Get_Jit_Reg_2(void)       //0x81
{
	uint8_t send_data[3] = {Read_Flag,JIT_REG_ADDR_2,0x00};
	while(spi_lock == 1){	
		__NOP();__NOP();__NOP();
	}
	Send_To_Fpga(send_data,3);
	return jit_reg_num_2;
}
extern uint8_t Get_Reg_0x82(void)
{
	uint8_t send_data[3] = {Read_Flag,config_r82_addr,0x00};
	while(spi_lock == 1){
		__NOP();__NOP();__NOP();

	}
	Send_To_Fpga(send_data,3);
	return config_r82_num;
}
extern uint8_t Get_Reg_0x83(void)
{
	uint8_t send_data[3] = {Read_Flag,config_r83_addr,0x00};
	while(spi_lock == 1){
		__NOP();__NOP();__NOP();

	}
	Send_To_Fpga(send_data,3);
	return config_r83_num;
}

extern uint8_t Get_Reg_0x84(void)
{
	uint8_t send_data[3] = {Read_Flag,config_r84_addr,0x00};
	while(spi_lock == 1){
		__NOP();__NOP();__NOP();

	}
	Send_To_Fpga(send_data,3);
	return config_r84_num;
}

extern uint8_t Get_Reg_0x87(void)
{
	uint8_t send_data[3] = {Read_Flag,config_r87_addr,0x00};
	while(spi_lock == 1){
		__NOP();__NOP();__NOP();

	}
	Send_To_Fpga(send_data,3);
	return config_r87_num;
}
extern uint8_t Get_Reg_0x88(void)
{
	uint8_t send_data[3] = {Read_Flag,config_r88_addr,0x00};
	
	while(spi_lock == 1){
		__NOP();__NOP();__NOP();
	}
	Send_To_Fpga(send_data,3);
	return config_r88_num;
}
extern uint8_t Get_Reg_0x89(void)
{
	uint8_t send_data[3] = {Read_Flag,config_r89_addr,0x00};
	while(spi_lock == 1){   //�ȴ�spi����
		__NOP();__NOP();__NOP();
	}
	Send_To_Fpga(send_data,3);
	return config_r89_num;
}
extern uint8_t Get_Reg_0x8a(void)
{
	uint8_t send_data[3] = {Read_Flag,config_r8a_addr,0x00};
	while(spi_lock == 1){
		__NOP();__NOP();__NOP();
	}
	Send_To_Fpga(send_data,3);
	return config_r8a_num;
}
extern uint8_t Get_Reg_0x8b(void)
{
	uint8_t send_data[3] = {Read_Flag,config_r8b_addr,0x00};
	while(spi_lock == 1){
		__NOP();__NOP();__NOP();
	}
	Send_To_Fpga(send_data,3);
	return config_r8b_num;
}
extern uint8_t Get_Reg_0x8c(void)
{
	uint8_t send_data[3] = {Read_Flag,config_r8c_addr,0x00};
	while(spi_lock == 1){
		__NOP();__NOP();__NOP();
	}
	Send_To_Fpga(send_data,3);
	return config_r8c_num;
}
extern uint8_t Get_Reg_0x8d(void)
{
	uint8_t send_data[3] = {Read_Flag,config_r8d_addr,0x00};
	while(spi_lock == 1){
		__NOP();__NOP();__NOP();
	}
	Send_To_Fpga(send_data,3);
	return config_r8d_num;
}
extern uint8_t Get_Reg_0x8e(void)
{
	uint8_t send_data[3] = {Read_Flag,config_r8e_addr,0x00};
	while(spi_lock == 1){
		__NOP();__NOP();__NOP();
	}
	Send_To_Fpga(send_data,3);
	return config_r8e_num;
}
//-----------------------------------------------------------------------------
extern uint8_t* Get_FW(void)     //Fw �汾��Ϣ
{
	if(fw_reg_num[0] != 0)
		return fw_reg_num;    //ȫ�ֱ���
	else
		return NULL;
}


/*- ----------------------------------SPI4_TxRxCpltCallback-------------------------------------*/
extern void SPI4_TxRxCpltCallback(void)
{
	uint8_t Addr=0;
	uint8_t Command=0;
	uint8_t Data=0;
	Set_NSS_Status(1);                                                     //����  spi��������  �ź�
	
	if(Write_Data[1] == Read_Data[1]){//ͨ�ųɹ�---------------------------------------------------------------------
		Command = Read_Data[0];   //  �� ���� д
		Addr = Read_Data[1];
		Data = Read_Data[2];   //!!!
		
		if(Addr == 0xff)  //��ȡ�����һ��reg
			Set_Status(0);    //good״̬  ת��good״̬
		
		if(Addr == SRC_MODE_ADDR  && Command == Write_Flag){    //����SRC_MODE  ����ģʽ  д
			/*
			Data = Write_Data[2];   //!!!
			if(Data == 1){
					Set_SRC_MODE(1);          //master enable
			}else if(Data == 0){
				Set_SRC_MODE(0);               //slave enable
			}
			*/
		}else if(Addr == SRC_MODE_ADDR  && Command == Read_Flag){  //SRC_MODE  �� ����ģʽ  
	//		printf(" Data %x \r\n",Data);
			/*
					if(Data == 0x80) {
						Set_SRC_MODE(1);   //master enable
					}
					else if(Data == 0x00) {
						Set_SRC_MODE(0);   //slave enable
					}
			*/
		}else if(Addr == JIT_REG_ADDR_1  && Command == Read_Flag){  //��ȡ �ӳ�ʱ��
					jit_reg_num_1 = Data;
		}else if(Addr == JIT_REG_ADDR_2  && Command == Read_Flag){   //��ȡ �ӳ�ʱ��
					jit_reg_num_2 = Data;
		}else if(Addr == FW_REG_ADDR[0]  && Command == Read_Flag){   //��ȡFW�汾��    //0x80�Ժ��reg ���� fpga��ֻ��reg
					fw_reg_num[0] = Data;
		}else if(Addr == FW_REG_ADDR[1]  && Command == Read_Flag){
					fw_reg_num[1] = Data;
		}else if(Addr == FW_REG_ADDR[2]  && Command == Read_Flag){
					fw_reg_num[2] = Data;
		}else if(Addr == FW_REG_ADDR[3]  && Command == Read_Flag){
					fw_reg_num[3] = Data;
		}else if(Addr == FW_REG_ADDR[4]  && Command == Read_Flag){   //end 
					fw_reg_num[4] = Data;
		}else if(Addr == config_r82_addr  && Command == Read_Flag){  
					config_r82_num = Data;
		}else if(Addr == config_r83_addr  && Command == Read_Flag){  
					config_r83_num = Data;
		}else if(Addr == config_r84_addr  && Command == Read_Flag){  
					config_r84_num = Data;
		}else if(Addr == config_r87_addr  && Command == Read_Flag){ 
					config_r87_num = Data;
		}else if(Addr == config_r88_addr  && Command == Read_Flag){   //��ȡgps_mode�µ��ӳ�ʱ��   ��������
					config_r88_num = Data;
		}else if(Addr == config_r89_addr  && Command == Read_Flag){
					config_r89_num = Data;
		}else if(Addr == config_r8a_addr  && Command == Read_Flag){
					config_r8a_num = Data;
		}else if(Addr == config_r8b_addr  && Command == Read_Flag){
					config_r8b_num = Data;
		}else if(Addr == config_r8c_addr  && Command == Read_Flag){  
					config_r8c_num = Data;
		}else if(Addr == config_r8d_addr  && Command == Read_Flag){    
					config_r8d_num = Data;
		}else if(Addr == config_r8e_addr  && Command == Read_Flag){   
					config_r8e_num = Data;
		}
		
		
		Set_Retry_SPI4(0);                                                   //�ر��ٴδ����ź�
		
#ifndef FPGA_LINK		
		USART2_Putbuf(Read_Data,Uart2_Send_Size);                            //stm32 wirte data   !!!    to  QT_uart
#else 
		//�ж������Ƿ���ͨ  ��ͨ���͸�pc   ���򲻷���pc
		if(Read_Eth_Link_Status() == 0x01){
			//if(Addr < 0x80)  //bug
 				Eth_Send_api(Read_Data,3);                                            //stm32 wirte data   !!!    to  QT_eth
			
		}
#endif		

		
	}else{					//ͨ��ʧ��---------------------------------------------------------------------------------------
		Set_Retry_SPI4(1);                                                   //�����ٴδ����ź�
		//spi_lock=1;
		//	printf(" Retry ");   //��ò�����log�ź�
	}
	
	
	spi_lock=0;  //spi����

}


/*- ----------------------------------Test_Send_To_Fpga-------------------------------------*/

extern void Test_Send_To_Fpga(void)
{
	uint8_t get_error;

	Set_NSS_Status(0);                                                               // ����  spi��ʼ���� �ź�
	while(spi_lock == 1){
	}
	spi_lock = 1;
	if( HAL_SPI_Transmit_IT(&hspi4, &Write_Data[0], Uart2_Send_Size) != HAL_OK){
				printf(" transmit data error\r\n");
		}
	get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],Uart2_Send_Size);	
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
}


/*- ----------------------------------Send_To_Fpga-------------------------------------*/

extern void Send_To_Fpga(uint8_t *Send_Data,uint16_t Send_Len)
{
	uint8_t get_error;
	Write_Data[0] = Send_Data[0]; 
	Write_Data[1] = Send_Data[1];   //!!!!�����ж��Ƿ��ͳɹ�  txrxcall
	Write_Data[2] = Send_Data[2]; 
	
	Set_NSS_Status(0);                                                               // ����  spi��ʼ���� �ź�
	while(spi_lock == 1){
	}
	spi_lock = 1;   //spi����
	if( HAL_SPI_Transmit_IT(&hspi4, Send_Data, Send_Len) != HAL_OK){
				printf(" transmit data error\r\n");
		}
	get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],3);	
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
}

/* ------------------------------------------Send_FPGA_Restart--------------------------------------------*/
extern void Send_FPGA_Restart(uint8_t ch)  //��λfpga  0x10           ����fpga  0x00
{
	uint8_t get_error;
	
	Write_Data[0] = Write_Flag; 
	Write_Data[1] = 0x00;   
	Write_Data[2] = ch; 
	
	while(spi_lock == 1){
//		printf(" Reset_FPGA ");
	}
	
	Set_NSS_Status(0);
	spi_lock = 1;
	
		if( HAL_SPI_Transmit_IT(&hspi4, &Write_Data[0], Uart2_Send_Size) != HAL_OK){
				printf(" transmit data error Reset_FPGA\r\n");
		}
			get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],Uart2_Send_Size);
		
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
}


//��Ϊ ÿ��ֻ��һ���Ĵ���������256��Ϊֹ   10msһ�μ��
extern void Power_On_Read_Reg_Once(void)   //����Ҫ��������   ֻ�� stm32��fpga֮��
{
	static uint16_t timeout = 0;
	static uint16_t addr = 0x00;  //  0x00~0x80  fpga��д reg             0x80 ��  0xB8 Ϊֻ���Ĵ���           0xB0�Ժ�  ����
	static uint8_t fpga_flag=0;
	Write_Data[0] = Read_Flag; 
	Write_Data[1] = addr;   //!!!!�����ж��Ƿ��ͳɹ�  txrxcall
	Write_Data[2] = 0x00; 
	
	
	if(addr <= 0xB8){
		if(timeout >= 500){                // 300 * 10ms = 3s  ��stm32����3s��ʼ�����ȡfpga reg
			if(addr <= 0x80 ){
				if(addr == 0x00){
					if(fpga_flag==0){
				//		Send_FPGA_Restart(0x10);     //restart 0x10
						fpga_flag=1;
					}else{
				//		Send_FPGA_Restart(0x00);    
						addr++;
					}
				}else{
#ifdef  FPGA_LINK					
				Set_FPGA_Reg_Config(0,addr);
#endif
					addr++;
				}
			}else{
#ifdef  FPGA_LINK				
					Send_To_Fpga(Write_Data,3);
				addr++;
#endif
			}
			//addr++;
			if(addr > 0xB8){
				Update_User_Arr(0,0,1);    // ��ȫ��  ��reg������ֵ  ȫ������  ��  user_arr
				Set_Status(0);    //good״̬  ת��good״̬
			}
		}else{
			timeout++;
			Set_Status(2);    //init  ״̬	
		}
	}
}

/*- ----------------------------------Send_Test_Pattern--------------------------test_pattern-----------*/
extern void Send_Test_Pattern(uint8_t ch)    //       
{
	uint8_t get_error;
	
	Write_Data[0] = Write_Flag; 
	Write_Data[1] = 0x01;   
	Write_Data[2] = ch; 
	
	while(spi_lock == 1){
//		printf(" Set_FPGA ");
	}
	
	Set_NSS_Status(0);
	spi_lock = 1;
	
		if( HAL_SPI_Transmit_IT(&hspi4, &Write_Data[0], Uart2_Send_Size) != HAL_OK){
				printf(" transmit data error Set_FPGA\r\n");
		}
			get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],Uart2_Send_Size);
		
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
		
}




/*- ----------------------------------Set_Src_Mode-------------------------------------*/
extern void Send_Src_Mode(uint8_t ch)
{
	uint8_t get_error;

	Write_Data[0] = Write_Flag;
	Write_Data[1] = SRC_MODE_ADDR;    //0x04
	Write_Data[2] = ch;

	Set_NSS_Status(0);
	while(spi_lock == 1){
	}
	spi_lock = 1;
	
	if( HAL_SPI_Transmit_IT(&hspi4, &Write_Data[0], 3) != HAL_OK){
				printf(" transmit data error\r\n");
		}

	get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],3);
		
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
}
/* ------------------------------------------Send_ClockSync_Mode--------------------------------------------*/
extern void Send_ClockSync_Mode(uint8_t ch)
{
	uint8_t get_error;

	Write_Data[0] = Write_Flag;
	Write_Data[1] = 0x0D;    
	Write_Data[2] = ch;
	
	Set_NSS_Status(0);
	while(spi_lock == 1){
	}
	spi_lock = 1;
	
	if( HAL_SPI_Transmit_IT(&hspi4, &Write_Data[0], Uart2_Send_Size) != HAL_OK){
				printf(" transmit data error\r\n");
		}

	get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],Uart2_Send_Size);
		
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
}
/* ------------------------------------------Send_SyncType_Mode--------------------------------------------*/
extern void Send_SyncType_Mode(uint8_t ch)
{
	uint8_t get_error;

	Write_Data[0] = Write_Flag;
	Write_Data[1] = 0x06;    
	Write_Data[2] = ch;

	Set_NSS_Status(0);
	while(spi_lock == 1){
	}
	spi_lock = 1;
	
	if( HAL_SPI_Transmit_IT(&hspi4, &Write_Data[0], Uart2_Send_Size) != HAL_OK){
				printf(" transmit data error\r\n");
		}

	get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],Uart2_Send_Size);
		
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
}
/* ------------------------------------------Send_SyncFrame_Mode--------------------------------------------*/
extern void Send_SyncFrame_Mode(uint8_t ch)
{
	uint8_t get_error;

	Write_Data[0] = Write_Flag;
	Write_Data[1] = 0x10;    
	Write_Data[2] = ch;

	Set_NSS_Status(0);
	while(spi_lock == 1){
	}
	spi_lock = 1;
	
	if( HAL_SPI_Transmit_IT(&hspi4, &Write_Data[0], Uart2_Send_Size) != HAL_OK){
				printf(" transmit data error\r\n");
		}

	get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],Uart2_Send_Size);
		
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
}
/* ------------------------------------------Send_GPS_Sec_Num--------------------------------------------*/
extern void Send_GPS_Sec_Num(uint8_t Sec)
{
	uint8_t get_error;
	
	Write_Data[0] = Write_Flag;
	Write_Data[1] = 0x11;         //gps �յ���ǰ����ʱ���fpga reg
	Write_Data[2] = Sec;
	
	
	while(spi_lock == 1){
	}
	
	Set_Send_Sec_Plus(0);
	
	Set_NSS_Status(0);
	spi_lock = 1;
	
	if( HAL_SPI_Transmit_IT(&hspi4, &Write_Data[0], Uart2_Send_Size) != HAL_OK){
				printf(" transmit data error\r\n");
		}

	get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],Uart2_Send_Size);
		
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
}

/* ------------------------------------------Send_GPS_Sec_Pulse--------------------------------------------*/
extern void Send_GPS_Sec_Pulse(uint8_t Pulse)
{
	uint8_t get_error;
	
	Write_Data[0] = Write_Flag;
	Write_Data[1] = 0x0f;         
	Write_Data[2] = Pulse;
	
	
	while(spi_lock == 1){
	}
	
	Set_NSS_Status(0);
	spi_lock = 1;
	
	if( HAL_SPI_Transmit_IT(&hspi4, &Write_Data[0], Uart2_Send_Size) != HAL_OK){
				printf(" transmit data error\r\n");
		}

	get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],Uart2_Send_Size);
		
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
}




/* ------------------------------------------Send_GPS_Dirft--------------------------------------------*/
extern void Send_GPS_Dirft(uint8_t addr,uint8_t data)
{
	uint8_t get_error;
	
	Write_Data[0] = Write_Flag;
	Write_Data[1] = addr;         //gps �յ���ǰ����ʱ���fpga reg
	Write_Data[2] = data;
	
	
	while(spi_lock == 1){
	}
	
	Set_NSS_Status(0);
	spi_lock = 1;
	
	if( HAL_SPI_Transmit_IT(&hspi4, &Write_Data[0], Uart2_Send_Size) != HAL_OK){
				printf(" transmit data error\r\n");
		}

	get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],Uart2_Send_Size);
		
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
}


/* ------------------------------------------Send_Format_2_0x05--------------------------------------------*/
extern void Send_Format_2_0x05(uint8_t ch)
{
	uint8_t get_error;
	
	Write_Data[0] = Write_Flag;
	Write_Data[1] = 0x05;        
	Write_Data[2] = ch;
	
	
	while(spi_lock == 1){
	}
	
	Set_NSS_Status(0);
	spi_lock = 1;
	
	if( HAL_SPI_Transmit_IT(&hspi4, &Write_Data[0], Uart2_Send_Size) != HAL_OK){
				printf(" transmit data error\r\n");
		}

	get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],Uart2_Send_Size);
		
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
}
/* ------------------------------------------Send_Format_2_0x17--------------------------------------------*/
extern void Send_Format_2_0x11(uint8_t ch)
{
	uint8_t get_error;
	
	Write_Data[0] = Write_Flag;
	Write_Data[1] = 0x11;        
	Write_Data[2] = ch;
	
	
	while(spi_lock == 1){
	}
	
	Set_NSS_Status(0);
	spi_lock = 1;
	
	if( HAL_SPI_Transmit_IT(&hspi4, &Write_Data[0], Uart2_Send_Size) != HAL_OK){
				printf(" transmit data error\r\n");
		}

	get_error = HAL_SPI_TransmitReceive_IT(&hspi4, Null_arr, &Read_Data[0],Uart2_Send_Size);
		
	if( get_error != HAL_OK ){
		printf(" RX data error\r\n");
	}
}
















/*- ----------------------------------���·��ͻ���-------------------------------------*/

extern void Retry_SPI4(void)
{
	Set_Retry_SPI4(0);
	//Test_Send_To_Fpga();
	while(spi_lock == 1){
	}
	Send_To_Fpga(Write_Data,3);
}

extern uint8_t Get_Retry_SPI4_Flag(void)
{
	return retry_flag;
}



