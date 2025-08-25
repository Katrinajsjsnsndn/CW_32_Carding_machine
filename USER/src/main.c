
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
#include "cw32l011_btim.h"
#include "my_init.h"
#include <string.h>

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define SENSOR_THRESHOLD 26

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
// 全局变量定义
volatile uint8_t wifi_rx_buffer[WIFI_BUFFER_SIZE];  // 接收缓冲区
volatile uint16_t wifi_rx_index = 0;               // 接收索引
volatile uint8_t wifi_rx_flag = 0;                 // 接收标志

// 传感/状态与时间变量
uint8_t PIR_pinif = 0;    // PIR引脚中断标志位
uint8_t PIR_timcl = 5;    // PIR引脚清除计数
extern uint8_t Status_Flag;
uint8_t Inside_flag;
volatile uint16_t g_adc_pa11_raw = 0;
volatile RTC_TimeTypeDef g_rtc_time;
volatile RTC_DateTypeDef g_rtc_date;
uint8_t hour,minute,second,month,year,day,week;
uint16_t insideDvar = 0;            // 扫描计数 防止传感器数据漂移
uint16_t calibration_value = 0;     // 传感器校准基准值
volatile uint8_t timer_1s_flag = 0; // 1秒定时器标志

// 猫事件记录（进入/离开时间）
#define CAT_EVENT_MAX 32
typedef struct {
	RTC_TimeTypeDef entry_time;
	RTC_DateTypeDef entry_date;
	RTC_TimeTypeDef exit_time;
	RTC_DateTypeDef exit_date;
	uint8_t in_use;
	uint8_t has_exit;
} CatEventRecord;

volatile uint16_t g_cat_event_count = 0;
volatile uint16_t g_cat_event_write_index = 0;
volatile int16_t g_cat_event_open_index = -1; // -1 表示当前无未闭合事件
CatEventRecord g_cat_events[CAT_EVENT_MAX];

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void UART_SendString(UART_TypeDef *UARTx, char *String);
void wifi_test_communication(void);
uint16_t adc_pa11_read_raw(void);
uint16_t ADC_Read(uint32_t Channel);
void rtc_read(RTC_TimeTypeDef* t, RTC_DateTypeDef* d);
static void cat_record_entry(void);
static void cat_record_exit(void);
static void cat_publish_closed_event(const CatEventRecord* e);
static void sync_rtc_time_from_network(void);
uint16_t current = 0;

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
int32_t main(void)
{
    //配置RCC
    RCC_Configuration();
    my_gpio_init();
    UART_Configuration();
    NVIC_Configuration();
    btim_1s_init();
    // 初始化串口接收
    uart_rx_init();
    // 初始化PA11 ADC（CH13）
    adc_pa11_init();
    // 初始化RTC（LSI时钟）
    rtc_init_lsi();
    // 初始一次ADC基准校准
    calibration_value = ADC_Read(ADC_InputCH13);
    
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
                
                // 同步网络时间到RTC
                sync_rtc_time_from_network();
            }
        }
    }

    while(1)
    {
						rtc_read((RTC_TimeTypeDef*)&g_rtc_time, (RTC_DateTypeDef*)&g_rtc_date);

            switch(Status_Flag)
            {
                case 0:
                    break;
                case 1:
                Status_Flag = 2;//PIR中断触发完成
                current = ADC_Read(ADC_InputCH13);
                if(current < (calibration_value - SENSOR_THRESHOLD) || current > (calibration_value + SENSOR_THRESHOLD))
                            {
                                                    // 获取时间戳，记录进入事件
																cat_record_entry();
                                Inside_flag=1;
															  BTIM_SetCounter(CW_BTIM1, 0);     // 设置计数器值为0（清零）
																BTIM_Cmd(CW_BTIM1, ENABLE);
                            }
                            else{
                                Status_Flag=0;
                            }
                    break;
            case 3:
                BTIM_Cmd(CW_BTIM1, DISABLE);  // 停止定时器
                BTIM_SetCounter(CW_BTIM1, 0);     // 设置计数器值为0（清零）
                Status_Flag=4;
                // 红外检测 1秒一次
                current = ADC_Read(ADC_InputCH13);
                if((current < (calibration_value - SENSOR_THRESHOLD / 2) || current > (calibration_value + SENSOR_THRESHOLD / 2) || PIR_pinif==1) && insideDvar <= 45)
                {
                    if(GPIO_ReadPin(CW_GPIOB,GPIO_PIN_7))
                    {
                        insideDvar = 0;

                    }
                    else
                    {

                        insideDvar++;
                    }
                }
                else if(Inside_flag==1 && insideDvar>45)
                {
                    insideDvar  =0;
                    Inside_flag =0;
                    // 记录离开事件
                    cat_record_exit();
                    BTIM_Cmd(CW_BTIM1, DISABLE);  // 停止定时器
                    BTIM_SetCounter(CW_BTIM1, 0);     // 设置计数器值为0（清零）
                    Status_Flag=0;
                }
								        BTIM_SetCounter(CW_BTIM1, 0);     // 设置计数器值为0（清零）
                        BTIM_Cmd(CW_BTIM1, ENABLE);
                break;
        }
				mqtt_publish(MQTT_TEST_TOPIC, MQTT_TEST_PAYLOAD, 0, 0);
        delay_ms(100);

			
		}
}


/**************** 猫事件记录实现 ****************/
static void cat_record_entry(void)
{
	if (g_cat_event_open_index != -1) {
		return; // 已有未闭合事件，避免重复
	}
	CatEventRecord *e = &g_cat_events[g_cat_event_write_index];
	rtc_read((RTC_TimeTypeDef*)&g_rtc_time, (RTC_DateTypeDef*)&g_rtc_date);
	e->entry_time = g_rtc_time;
	e->entry_date = g_rtc_date;
	e->has_exit = 0;
	e->in_use = 1;
	g_cat_event_open_index = (int16_t)g_cat_event_write_index;
}

static void cat_record_exit(void)
{
	if (g_cat_event_open_index == -1) {
		return; // 没有进行中的事件
	}
	CatEventRecord *e = &g_cat_events[g_cat_event_open_index];
	rtc_read((RTC_TimeTypeDef*)&g_rtc_time, (RTC_DateTypeDef*)&g_rtc_date);
	e->exit_time = g_rtc_time;
	e->exit_date = g_rtc_date;
	e->has_exit = 1;

	// 推进写指针与计数
	g_cat_event_write_index = (uint16_t)((g_cat_event_write_index + 1) % CAT_EVENT_MAX);
	if (g_cat_event_count < CAT_EVENT_MAX) {
		g_cat_event_count++;
	}
	g_cat_event_open_index = -1;

	// 发布到云端
	cat_publish_closed_event(e);
}

static void cat_publish_closed_event(const CatEventRecord* e)
{
	char payload[160];
	// 格式：{"entry":"YYYY-MM-DD HH:MM:SS","exit":"YYYY-MM-DD HH:MM:SS"}
	sprintf(payload,
			"{entry:20%02u-%02u-%02u %02u:%02u:%02u exit:20%02u-%02u-%02u %02u:%02u:%02u}",
			(unsigned)RTC_BCDToBin(e->entry_date.Year),
			(unsigned)RTC_BCDToBin(e->entry_date.Month),
			(unsigned)RTC_BCDToBin(e->entry_date.Day),
			(unsigned)RTC_BCDToBin(e->entry_time.Hour),
			(unsigned)RTC_BCDToBin(e->entry_time.Minute),
			(unsigned)RTC_BCDToBin(e->entry_time.Second),
			(unsigned)RTC_BCDToBin(e->exit_date.Year),
			(unsigned)RTC_BCDToBin(e->exit_date.Month),
			(unsigned)RTC_BCDToBin(e->exit_date.Day),
			(unsigned)RTC_BCDToBin(e->exit_time.Hour),
			(unsigned)RTC_BCDToBin(e->exit_time.Minute),
			(unsigned)RTC_BCDToBin(e->exit_time.Second));
	// 发布，QoS0, 不保留
	mqtt_publish(MQTT_CAT_EVENT_TOPIC, payload, 0, 0);
}

static void sync_rtc_time_from_network(void)
{
    // 配置SNTP（时区UTC+8，即北京时间）
    if(wifi_sntp_config(8))
    {
        delay_ms(2000);  // 等待SNTP配置生效
        
        // 获取并同步时间
        if(wifi_sntp_sync_time())
        {
            // 时间同步成功
            // 可以在这里添加成功指示，比如LED闪烁
        }
        else
        {
            // 时间同步失败，使用默认时间
            RTC_TimeTypeDef t = {0};
            RTC_DateTypeDef d = {0};
            
            t.H24 = RTC_HOUR24;
            t.Hour = RTC_BinToBCD(10);   // 10点
            t.Minute = RTC_BinToBCD(30); // 30分
            t.Second = RTC_BinToBCD(0);  // 0秒
            
            d.Year = RTC_BinToBCD(25);   // 2025年
            d.Month = RTC_BinToBCD(8);   // 8月
            d.Day = RTC_BinToBCD(18);    // 18日
            d.Week = RTC_Weekday_Monday; // 周一
            
            RTC_SetDate(&d);
            RTC_SetTime(&t);
        }
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

//void proc(char* word)
//{
//	uint8_t count=0;
//	uint8_t len=strlen(word);
//	char *copy_word;
//	for(int i=0;i<len;i++)
//	{
//		if(word[i]!='*')
//		{
//			//word[i]=word[i];
//		}
//		else
//		{
//			word[i]=word[i+1];
//			i--;
//			count++;
//		}
//	}
//	for(int j=0;j<count;j++)
//	{
//		copy_word[j]='*';
//	}
//	for(int k=0;k<(len-count);k++)
//	{
//		if(word[k])
//	
//	}
//	
//	
//}

