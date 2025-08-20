#include "my_init.h"
#include "../inc/main.h"
#include "cw32l011_btim.h"

void my_gpio_init()
{
		GPIO_InitTypeDef GPIO_InitStruct = {0};

		__SYSCTRL_GPIOA_CLK_ENABLE();		//Open GPIOA Clk
		__SYSCTRL_GPIOB_CLK_ENABLE();
		__SYSCTRL_GPIOC_CLK_ENABLE();	
		//set PA00 / PA01 / PA02 / PA03 as output
		GPIO_InitStruct.Pins = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Init( CW_GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pins =	GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.IT	 = GPIO_IT_RISING;
		GPIO_Init( CW_GPIOB, &GPIO_InitStruct);
		GPIOB_INTFLAG_CLR(GPIO_PIN_15);

		GPIO_InitStruct.Pins = GPIO_PIN_1;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
	
		GPIO_InitStruct.Pins = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
		GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
		// PA11 ����Ϊģ����������ADC
		GPIO_InitStruct.Pins = GPIO_PIN_11;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.IT	 = GPIO_IT_NONE;
		GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pins = GPIO_PIN_13;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_Init(CW_GPIOC, &GPIO_InitStruct);
		
		//UART TX RX ����
		PA00_AFx_UART3RXD();
		PA01_AFx_UART3TXD();
}

void RCC_Configuration(void)
{
		//SYSCLK = HSI = 8MHz = HCLK = PCLK
		SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);

		//����ʱ��ʹ��
		SYSCTRL_AHBPeriphClk_Enable(SYSCTRL_AHB_PERIPH_GPIOA, ENABLE);
		SYSCTRL_APBPeriphClk_Enable1(SYSCTRL_APB1_PERIPH_UART3, ENABLE); // ����ΪUART3
}		 

void UART_Configuration(void)
{
		UART_InitTypeDef UART_InitStructure = {0};

		UART_InitStructure.UART_BaudRate = 115200;
		UART_InitStructure.UART_Over = UART_Over_16;
		UART_InitStructure.UART_Source = UART_Source_PCLK;
		UART_InitStructure.UART_UclkFreq = 8000000;
		UART_InitStructure.UART_StartBit = UART_StartBit_FE;
		UART_InitStructure.UART_StopBits = UART_StopBits_1;
		UART_InitStructure.UART_Parity = UART_Parity_No ;
		UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
		UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
		UART_Init(CW_UART3, &UART_InitStructure);
}

void NVIC_Configuration(void)
{
		NVIC_SetPriority(GPIOB_IRQn,1);
		//���ȼ��������ȼ�����
		NVIC_SetPriority(UART3_IRQn, 0);
		//UARTx�ж�ʹ��
		NVIC_EnableIRQ(UART3_IRQn);
		NVIC_EnableIRQ(GPIOB_IRQn);
}

void delay_ms(uint32_t ms)
{
		uint32_t i, j;
		for(i = 0; i < ms; i++)
		{
				for(j = 0; j < 1000; j++)
				{
						__NOP();
				}
		}
}

void uart_rx_init(void)
{
		// ��ս��ջ�����
		uart_rx_clear_buffer();
		
		// ����UART3�����ж�
		UART_ITConfig(CW_UART3, UART_IT_RC, ENABLE);  // ���ý����ж�
		
		// ����UART3�ж�
		NVIC_EnableIRQ(UART3_IRQn);
}

uint16_t uart_rx_get_data(uint8_t *buffer, uint16_t max_len)
{
		uint16_t len = 0;
		if(wifi_rx_flag && wifi_rx_index > 0)
		{
				len = (wifi_rx_index < max_len) ? wifi_rx_index : max_len;
				for(uint16_t i = 0; i < len; i++)
				{
						buffer[i] = wifi_rx_buffer[i];
				}
				wifi_rx_flag = 0;
		}
		return len;
}

void uart_rx_clear_buffer(void)
{
		wifi_rx_index = 0;
		wifi_rx_flag = 0;
		for(uint16_t i = 0; i < WIFI_BUFFER_SIZE; i++)
		{
				wifi_rx_buffer[i] = 0;
		}
}

uint8_t uart_rx_is_data_ready(void)
{
		return wifi_rx_flag;
}

void adc_pa11_init(void)
{
		// ���� ADC: ����ת�������н� SQR0��ͨ��Ϊ CH13 (PA11)
		ADC_InitTypeDef adc = {0};
		adc.ADC_ClkDiv		 = ADC_Clk_Div8;
		adc.ADC_ConvertMode	 = ADC_ConvertMode_Once;
		adc.ADC_BgrEn		 = FALSE;
		adc.ADC_TempSensorEn = FALSE;
		adc.ADC_SQREns		 = ADC_SqrEns0to0; // ֻת��SQR0
		adc.ADC_IN0.ADC_InputChannel = ADC_InputCH13; // PA11
		adc.ADC_IN0.ADC_SampTime		 = ADC_SampTime70Clk;
		ADC_Init(&adc);
		ADC_Enable();
}

uint16_t adc_pa11_read_raw(void)
{
		ADC_SoftwareStartConvCmd(ENABLE);
		while (ADC_GetITStatus(ADC_IT_EOC) == RESET) { }
		uint16_t val = ADC_GetConversionValue(ADC_RESULT_0);
		ADC_ClearITPendingBit(ADC_IT_EOC);
		return val;
}

uint16_t ADC_Read(uint32_t Channel)
{
		uint32_t sum = 0;
		GPIO_WritePin(CW_GPIOC,GPIO_PIN_13,GPIO_Pin_SET);
		CW_ADC->SQRCFR_f.SQRCH0 = Channel;
		CW_ADC->SAMPLE_f.SQRCH0 = ADC_SampTime70Clk;
		for (uint8_t i = 0; i < 15; i++)
		{
				ADC_SoftwareStartConvCmd(ENABLE);
				while (ADC_GetITStatus(ADC_IT_EOC) == RESET) { }
				sum += ADC_GetConversionValue(ADC_RESULT_0);
				ADC_ClearITPendingBit(ADC_IT_EOC);
		}
		GPIO_WritePin(CW_GPIOC,GPIO_PIN_13,GPIO_Pin_RESET);
		return (uint16_t)(sum / 15);
}

void rtc_init_lsi(void)
{
		// 1) ������ LSI ��Ϊ RTC ��ʱ��Դ��׼
		SYSCTRL_LSI_Enable();

		// 2) ʹ�� RTC ����ʱ�ӣ�APB2��
		SYSCTRL_APBPeriphClk_Enable2(SYSCTRL_APB2_PERIPH_RTC, ENABLE);

		// 3) ֹͣ RTC������ʱ��ԴΪ LSI ������������POR����Ҳ����Ч��
		RTC_Cmd(DISABLE);
		RTC_SetClockSource(RTC_RTCCLK_FROM_LSI);
		RTC_Cmd(ENABLE);

		// 4) ���ó�ʼʱ��/���ڣ�BCD��
		RTC_TimeTypeDef t = {0};
		RTC_DateTypeDef d = {0};

		t.H24		= RTC_HOUR24;
		t.Hour	 = RTC_BinToBCD(12);
		t.Minute = RTC_BinToBCD(0);
		t.Second = RTC_BinToBCD(0);

		d.Year	 = RTC_BinToBCD(25);
		d.Month	 = RTC_BinToBCD(1);
		d.Day		= RTC_BinToBCD(1);
		d.Week	 = RTC_Weekday_Wednesday;

		RTC_SetDate(&d);
		RTC_SetTime(&t);
}

void rtc_read(RTC_TimeTypeDef* t, RTC_DateTypeDef* d)
{
		RTC_GetTime(t);
		RTC_GetDate(d);
}

void btim_1s_init(void)
{
		// ʹ��BTIMʱ�ӣ�APB2��
		SYSCTRL_APBPeriphClk_Enable2(SYSCTRL_APB2_PERIPH_BTIM123, ENABLE);

		// ����BTIM1Ϊ1���ж�
		BTIM_TimeBaseInitTypeDef tb = {0};
		// ����PCLK=8MHz: Ԥ��Ƶ8000-1������1000-1 => 1Hz
		tb.BTIM_Prescaler = 8000 - 1;
		tb.BTIM_Period = 1000 - 1;
		tb.BTIM_Mode = BTIM_MODE_TIMER;
		tb.BTIM_CountMode = BTIM_COUNT_MODE_REPETITIVE;
		BTIM_TimeBaseInit(CW_BTIM1, &tb);

		// ��������ж�
		BTIM_ITConfig(CW_BTIM1, BTIM_IT_UPDATE, ENABLE);

		// NVIC����
		NVIC_SetPriority(BTIM1_IRQn, 1);
		NVIC_EnableIRQ(BTIM1_IRQn);

		// ��ʼ����������״̬����Ҫ������
		BTIM_Cmd(CW_BTIM1, DISABLE);
}

