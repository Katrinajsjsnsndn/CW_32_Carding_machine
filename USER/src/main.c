
/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2021-03-12  1.0  xiebin First version for Device Driver Library of Module.
 **
 ******************************************************************************/
/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "../inc/main.h"
#include "cw32l011_uart.h"
#include "interrupts_cw32l011.h"
#include "wifi_config.h"
#include "cw32l011_adc.h"
#include "cw32l011_rtc.h"
#include "cw32l011_sysctrl.h"
#include <string.h>

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
// 全局变量定义
volatile uint8_t wifi_rx_buffer[WIFI_BUFFER_SIZE];  // 接收缓冲区
volatile uint16_t wifi_rx_index = 0;               // 接收索引
volatile uint8_t wifi_rx_flag = 0;                 // 接收标志

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void my_gpio_init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void UART_Configuration(void);
void NVIC_Configuration(void);
void UART_SendString(UART_TypeDef *UARTx, char *String);
void wifi_test_communication(void);
void delay_ms(uint32_t ms);
void adc_pa11_init(void);
uint16_t adc_pa11_read_raw(void);
uint16_t ADC_Read(uint32_t Channel);
void rtc_init_lsi(void);
void rtc_read(RTC_TimeTypeDef* t, RTC_DateTypeDef* d);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample toggle GPIOA
 **
 ******************************************************************************/
extern uint8_t Status_Flag;
uint8_t Inside_flag;
volatile uint16_t g_adc_pa11_raw = 0;
volatile RTC_TimeTypeDef g_rtc_time;
volatile RTC_DateTypeDef g_rtc_date;
uint8_t hour,minute,second,month,year,day,week;
int32_t main(void)
{
    //配置RCC
    RCC_Configuration();
    my_gpio_init();
    UART_Configuration();
    NVIC_Configuration();
    
    // 初始化串口接收
    uart_rx_init();
    // 初始化PA11 ADC（CH13）
    adc_pa11_init();
    // 初始化RTC（LSI时钟）
    rtc_init_lsi();
    
    GPIO_WritePin( CW_GPIOB, GPIO_PIN_3|GPIO_PIN_4, GPIO_Pin_SET );
    UART_SendString(CW_UART3, "AT\r\n");
    // 等待系统稳定
    delay_ms(500);
    
    // 初始化WiFi模块
    wifi_init();
    
    // 连接WiFi网络
    if(wifi_connect_network(WIFI_SSID, WIFI_PASSWORD))
    {
        // 获取IP地址
        wifi_get_ip_address();
        
        // 配置并连接MQTT
        if(mqtt_configure())
        {
                    
            if(mqtt_connect())
            {
                                delay_ms(20000);

                // 发布一条测试消息
                mqtt_publish(MQTT_TEST_TOPIC, MQTT_TEST_PAYLOAD, 0, 0);
            }
        }
    }

    while(1)
    {
            // 读取RTC时间日期（BCD）
            rtc_read((RTC_TimeTypeDef*)&g_rtc_time, (RTC_DateTypeDef*)&g_rtc_date);
												
						// 假设 g_rtc_time / g_rtc_date 已由 rtc_read() 更新
						 hour   = RTC_BCDToBin(g_rtc_time.Hour);
						 minute = RTC_BCDToBin(g_rtc_time.Minute);
						 second = RTC_BCDToBin(g_rtc_time.Second);

						 month  = RTC_BCDToBin(g_rtc_date.Month);
						 day    = RTC_BCDToBin(g_rtc_date.Day);
						 year  = 2000 + RTC_BCDToBin(g_rtc_date.Year);  // 年份转成 20xx
						 week   = g_rtc_date.Week; // 周是数值 0~6（不需BCD转换）
            switch(Status_Flag)
            {
                case 0:
                    break;
                case 1:
                            g_adc_pa11_raw = ADC_Read(ADC_InputCH13);
                            if(g_adc_pa11_raw<500)
                            {
                                                    // 获取时间戳
                                                    // 进入事件
                                Inside_flag=1;
                            }
                            else{
                                Status_Flag=0;
                            }
                    break;
            
            
            }
            // 读取PA11平均ADC值（15次平均），通道13
            delay_ms(100);

    }
}


/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
 /**
   * @brief  Reports the name of the source file and the source line number
   *         where the assert_param error has occurred.
   * @param  file: pointer to the source file name
   * @param  line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
       /* USER CODE END 6 */
}
#endif

void my_gpio_init()
{
		GPIO_InitTypeDef GPIO_InitStruct = {0};

    __SYSCTRL_GPIOA_CLK_ENABLE();    //Open GPIOA Clk
		__SYSCTRL_GPIOB_CLK_ENABLE();
		__SYSCTRL_GPIOC_CLK_ENABLE();	
    //set PA00 / PA01 / PA02 / PA03 as output
    GPIO_InitStruct.Pins = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init( CW_GPIOB, &GPIO_InitStruct);
		
    GPIO_InitStruct.Pins =  GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.IT   = GPIO_IT_RISING;
    GPIO_Init( CW_GPIOB, &GPIO_InitStruct);
		GPIOB_INTFLAG_CLR(GPIO_PIN_15);

    GPIO_InitStruct.Pins = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
	
    GPIO_InitStruct.Pins = GPIO_PIN_0;

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
    // PA11 设置为模拟输入用于ADC
    GPIO_InitStruct.Pins = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.IT   = GPIO_IT_NONE;
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
		
    GPIO_InitStruct.Pins = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOC, &GPIO_InitStruct);
		
    //UART TX RX 复用
    PA00_AFx_UART3RXD();
    PA01_AFx_UART3TXD();
}

/**
 * @brief 配置RCC
 *
 */
void RCC_Configuration(void)
{
    //SYSCLK = HSI = 8MHz = HCLK = PCLK
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);

    //外设时钟使能
    SYSCTRL_AHBPeriphClk_Enable(SYSCTRL_AHB_PERIPH_GPIOA, ENABLE);
    SYSCTRL_APBPeriphClk_Enable1(SYSCTRL_APB1_PERIPH_UART3, ENABLE); // 修正为UART3
}     

/**
 * @brief 配置UART
 *
 */
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
/**
 * @brief 配置NVIC
 *
 */
void NVIC_Configuration(void)
{
		NVIC_SetPriority(GPIOB_IRQn,1);
    //优先级，无优先级分组
    NVIC_SetPriority(UART3_IRQn, 0);
    //UARTx中断使能
    NVIC_EnableIRQ(UART3_IRQn);
		NVIC_EnableIRQ(GPIOB_IRQn);
}


/**
 * @brief 毫秒延时函数
 * @param ms: 延时时间(毫秒)
 */
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

/**
 * @brief 初始化串口接收
 */
void uart_rx_init(void)
{
    // 清空接收缓冲区
    uart_rx_clear_buffer();
    
    // 启用UART3接收中断
    UART_ITConfig(CW_UART3, UART_IT_RC, ENABLE);  // 启用接收中断
    
    // 启用UART3中断
    NVIC_EnableIRQ(UART3_IRQn);
}

/**
 * @brief 获取接收到的数据
 * @param buffer: 数据缓冲区
 * @param max_len: 最大长度
 * @return 实际接收到的数据长度
 */
uint16_t uart_rx_get_data(uint8_t *buffer, uint16_t max_len)
{
    uint16_t len = 0;

    if(wifi_rx_flag && wifi_rx_index > 0)
    {
        // 计算实际长度
        len = (wifi_rx_index < max_len) ? wifi_rx_index : max_len;

        // 复制数据
        for(uint16_t i = 0; i < len; i++)
        {
            buffer[i] = wifi_rx_buffer[i];
        }

        // 清空接收标志
        wifi_rx_flag = 0;
    }
		//uart_rx_clear_buffer();
    return len;
}

/**
 * @brief 清空接收缓冲区
 */
void uart_rx_clear_buffer(void)
{
    wifi_rx_index = 0;
    wifi_rx_flag = 0;
    
    // 清空缓冲区内容
    for(uint16_t i = 0; i < WIFI_BUFFER_SIZE; i++)
    {
        wifi_rx_buffer[i] = 0;
    }
}

/**
 * @brief 检查是否有数据可读
 * @return 1: 有数据, 0: 无数据
 */
uint8_t uart_rx_is_data_ready(void)
{
    return wifi_rx_flag;
}

//==================== ADC PA11 (CH13) ====================
void adc_pa11_init(void)
{
		// 配置 ADC: 单次转换，序列仅 SQR0，通道为 CH13 (PA11)
		ADC_InitTypeDef adc = {0};
		adc.ADC_ClkDiv       = ADC_Clk_Div8;
		adc.ADC_ConvertMode  = ADC_ConvertMode_Once;
		adc.ADC_BgrEn        = FALSE;
		adc.ADC_TempSensorEn = FALSE;
		adc.ADC_SQREns       = ADC_SqrEns0to0; // 只转换SQR0
		adc.ADC_IN0.ADC_InputChannel = ADC_InputCH13; // PA11
		adc.ADC_IN0.ADC_SampTime     = ADC_SampTime70Clk;
		ADC_Init(&adc);
		ADC_Enable();
}

uint16_t adc_pa11_read_raw(void)
{
		// 软件触发一次
		ADC_SoftwareStartConvCmd(ENABLE);
		// 轮询等待单次转换完成
		while (ADC_GetITStatus(ADC_IT_EOC) == RESET) { }
		uint16_t val = ADC_GetConversionValue(ADC_RESULT_0);
		// 清EOC标志（可选）
		ADC_ClearITPendingBit(ADC_IT_EOC);
		return val;
}

// 平均采样：参考你的示例，采样15次求平均
uint16_t ADC_Read(uint32_t Channel)
{
    uint32_t sum = 0;
		GPIO_WritePin(CW_GPIOC,GPIO_PIN_13,GPIO_Pin_SET);

    // 选择SQR0通道为指定通道；保持当前采样时间
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

//==================== RTC init/read ====================
void rtc_init_lsi(void)
{
    // 1) 先启动 LSI 作为 RTC 的时钟源基准
    SYSCTRL_LSI_Enable();

    // 2) 使能 RTC 外设时钟（APB2）
    SYSCTRL_APBPeriphClk_Enable2(SYSCTRL_APB2_PERIPH_RTC, ENABLE);

    // 3) 停止 RTC，配置时钟源为 LSI 后再启动（非POR场景也能生效）
    RTC_Cmd(DISABLE);
    RTC_SetClockSource(RTC_RTCCLK_FROM_LSI);
    RTC_Cmd(ENABLE);

    // 4) 设置初始时间/日期（BCD）
    RTC_TimeTypeDef t = {0};
    RTC_DateTypeDef d = {0};

    t.H24    = RTC_HOUR24;
    t.Hour   = RTC_BinToBCD(12);
    t.Minute = RTC_BinToBCD(0);
    t.Second = RTC_BinToBCD(0);

    d.Year   = RTC_BinToBCD(25);
    d.Month  = RTC_BinToBCD(1);
    d.Day    = RTC_BinToBCD(1);
    d.Week   = RTC_Weekday_Wednesday;

    RTC_SetDate(&d);
    RTC_SetTime(&t);
}

void rtc_read(RTC_TimeTypeDef* t, RTC_DateTypeDef* d)
{
    RTC_GetTime(t);
    RTC_GetDate(d);
}



