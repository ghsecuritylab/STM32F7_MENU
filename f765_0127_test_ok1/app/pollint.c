#include "common.h"

extern void audio_main(void);

typedef struct {
    uint16_t             msCountDown;        // ������ʱ������
    uint8_t             _msCount;
    uint8_t             _10msCount;
    uint8_t             _100msCount;
    Boolean             Pollintack;
} STR_POLLINT;

static STR_POLLINT r_pollint;


#ifdef EN_TIM3

static STR_POLLINT tim3_pollint;


TIM_HandleTypeDef htim3;

/* TIM3 init function */
extern void MX_TIM3_Init(void)
{
    /* Set TIMx instance */
    htim3.Instance = TIM3;

    /* Initialize TIMx peripheral as follows:
         + Period = 10 - 1
         + Prescaler = ((SystemCoreClock / 2)/10000) - 1
         + ClockDivision = 0
         + Counter direction = Up
    */
    htim3.Init.Period            = 2-1;//263us irq 25  1080          2
    htim3.Init.Prescaler         = 1080 - 1;////((1+TIM_Prescaler)/108M)*(1+TIM_Period)=((1+108))/108M)*(1+0)=0.000001s=1us
    htim3.Init.ClockDivision     = 0;
    htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim3.Init.RepetitionCounter = 0;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if(HAL_TIM_Base_Init(&htim3) != HAL_OK) {
        /* Initialization Error */
        Error_Handler();
    }

    /*##-2- Start the TIM Base generation in interrupt mode ####################*/
    /* Start Channel1 */
    if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) {
        /* Starting Error */
        Error_Handler();
    }

}

// ÿ������������ʱ��
static void ChangeTimeUs(void)
{
    tim3_pollint.Pollintack = 0;

    if(tim3_pollint.msCountDown > 0) {
        tim3_pollint.msCountDown--;
    }

    tim3_pollint._msCount = (tim3_pollint._msCount + 1) % 10;

    if(tim3_pollint._msCount == 0) {
        tim3_pollint._10msCount = (tim3_pollint._10msCount + 1) % 10;

        if(tim3_pollint._10msCount == 0) {
            tim3_pollint._100msCount = (tim3_pollint._100msCount + 1) % 10;
        }
    }
}

// ÿ������������ʱ��,�ɹ����ķ���1
static Boolean ChangeTimeUsCount_OK(void)
{
    __WFI();

    if(tim3_pollint.Pollintack) {
        ChangeTimeUs();
        return(TRUE);
    } else {
        return(FALSE);
    }
}



extern void Timer3_irq_fun(void)
{
	tim3_pollint.Pollintack = 1;
//	udp_TimerTick();
}


extern void pollintTim3(void)
{
    if(ChangeTimeUsCount_OK()) {
		//pollint_spi_slave();
			
    }
}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
}
#endif
/*******************************************************************************
* Function Name  : TimerTickNproc
* Description    : ϵͳ1ms��ʱ������.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void TimerTickNproc(void)
{
    r_pollint.Pollintack = 1;
   // udp_TimerTick();
    //Audio_TimerTick();
   cpuclk_cnt_add();
  
}


// ����1-9ÿ1��ִ��һ��
static void Pollint1Sec(void)
{
    switch(r_pollint._100msCount) {
    case 1:
		//	printf(" %d \r\n",Get_SRC_MODE());
        break;

    case 2:
			pollint_Reg_0x83();
        break;

    case 3:
			pollint_Reg_0x8d();
        break;

    case 4:
			pollint_Reg_0x84();
        break;

    case 5:
			IWDG_Refresh();   //1s ιһ�ι�
        break;

    case 6:
		set_blink_flag();	  //һ����˸һ��
		
	//	if(get_remoteLwip_udp_link()){         //��� ���� ����
	//	}
        break;

    case 7:
			pollint_Reg_0x87();    //status ״̬��
        break;
    case 8:	
			
        break;
    case 9:
			pollint_Reg_0x82();   //error״̬��  ��  oled�������status��
        break;
    default:
        break;
    }
}

// ����1-9ÿ100����ִ��һ��
static void Pollint100mSec(void)
{
    switch(r_pollint._10msCount) {
    case 1:
			break;
    case 2: 
			
			Pollint_Display_Main_menu(0);
        break;

    case 3:
			
        break;

    case 4:
        break;

    case 5:
        break;

    case 6:
        break;

    case 7:
        break;

    case 8:
        break;

    case 9:
			if(Get_Update_Flash_Flag()){            //����flash
				Set_Update_Flash_Flag(0);
				Arr_Init();
				//Write_Flash(0,NULL,0);
				Write2Flash();
			}
        break;

    default:
        Pollint1Sec();
        break;
    }
}

// ����1-9ÿ10����ִ��һ��
static void Pollint10mSec(void)
{
    switch(r_pollint._msCount) {
    case 1:
		

        break;

    case 2:
        key_Scan();
        break;

    case 3:
		//	test_Adv_ReadReg();
        break;

    case 4:
        break;

    case 5:
			if(!Get_Status()){   //���״̬����0  �򲻶�
				if(Get_Config_SRC_clocktype() == 0x03){       //��gps_mode��  �����������źŸ�fpga
					Send_GPS_info();
				}
			}
        break;
    case 6:
				if(!Get_Status()){  //���״̬����0  �򲻶�fpga reg
					if( Get_Send_Dirft_Flag() ){
						Send_Dirft();                            //����Ʈֵ��fpga
					}
				}	
        break;
    case 7:
        break;

    case 8:
        break;

    case 9:
#ifdef  FPGA_LINK
			Power_On_Read_Reg_Once();
#endif
        break;

    default:
        Pollint100mSec();
        break;
    }
}

// ÿ������������ʱ��
static void ChangeTime1mSec(void)
{
    r_pollint.Pollintack = 0;

    if(r_pollint.msCountDown > 0) {
        r_pollint.msCountDown--;
    }

    r_pollint._msCount = (r_pollint._msCount + 1) % 10;

    if(r_pollint._msCount == 0) {
        r_pollint._10msCount = (r_pollint._10msCount + 1) % 10;

        if(r_pollint._10msCount == 0) {
            r_pollint._100msCount = (r_pollint._100msCount + 1) % 10;
        }
    }
}

// ÿ������������ʱ��,�ɹ����ķ���1
static Boolean ChangeTimeCount_OK(void)
{
    __WFI();

    if(r_pollint.Pollintack) {
        ChangeTime1mSec();
        return(TRUE);
    } else {
        return(FALSE);
    }
}



// ������ʱ,�������ʱ,�������κ���ʱ����ص��¼�,��׼ȷ,�����ڶ�ʱ��(���뼶��)����ʱ
extern void delayms_Lock(uint16_t value)
{
    r_pollint.msCountDown = value + 1;

    while(r_pollint.msCountDown) {
        ChangeTimeCount_OK();
    }
}

// ����    ��ʱ��ѯ����
extern void pollint(void)
{
    
	//Pollint_Mp2();
	//pollint_spi_slave();
	
	pollint_eth_send();
	pollint_spi_transmit();
    if(ChangeTimeCount_OK()) {
        // ÿ����ִ��һ�ε��¼�
			
        USART3_RxReaddy();
        USART3_CheckSerial();
				config_CheckStatus();
        Pollint10mSec();
	//	if(get_remoteLwip_udp_link()==NET_UNLINK)	
			MX_LWIP_Process();
		

			
    }
}

// ������ʱ,�������ʱ,��ʱ������ʱ����ص��¼�,��˲���̫׼ȷ,�����ڳ�ʱ��(�뼶��)����ʱ
extern void delay_NoLock(uint16_t value)
{
    r_pollint.msCountDown = value + 1;

    while(r_pollint.msCountDown) {
        pollint();
    }
}
