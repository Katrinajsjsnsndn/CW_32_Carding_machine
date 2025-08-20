#ifndef __MY_INIT_H
#define __MY_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "cw32l011.h"
#include "cw32l011_gpio.h"
#include "cw32l011_sysctrl.h"
#include "cw32l011_uart.h"
#include "cw32l011_rtc.h"
#include "cw32l011_adc.h"
#include "interrupts_cw32l011.h"

void RCC_Configuration(void);
void my_gpio_init(void);
void UART_Configuration(void);
void NVIC_Configuration(void);
void delay_ms(uint32_t ms);
void uart_rx_init(void);
void adc_pa11_init(void);
void rtc_init_lsi(void);

#ifdef __cplusplus
}
#endif

#endif /* __MY_INIT_H */

/******************************************************************************/
/** \file my_init.h
 **
 ** 系统初始化相关函数声明
 **
 **   - 2024-01-01  1.0  Initial version for Device Driver Library of Module.
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
#ifndef __MY__INIT_H
#define __MY__INIT_H

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "cw32l011.h"
#include "cw32l011_uart.h"
#include "cw32l011_adc.h"
#include "cw32l011_rtc.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (module scope)
 ******************************************************************************/

/**
 * @brief 系统初始化主函数
 * @param None
 * @retval None
 */
void system_init(void);

/**
 * @brief WiFi网络连接初始化
 * @param None
 * @retval uint8_t 连接状态 1:成功 0:失败
 */
uint8_t wifi_network_init(void);

/**
 * @brief 配置RCC时钟
 * @param None
 * @retval None
 */
void RCC_Configuration(void);

/**
 * @brief GPIO初始化
 * @param None
 * @retval None
 */
void my_gpio_init(void);

/**
 * @brief UART配置
 * @param None
 * @retval None
 */
void UART_Configuration(void);

/**
 * @brief NVIC中断配置
 * @param None
 * @retval None
 */
void NVIC_Configuration(void);







//==================== ADC PA11 (CH13) ====================
/**
 * @brief 初始化PA11 ADC（通道13）
 * @param None
 * @retval None
 */
void adc_pa11_init(void);

/**
 * @brief 读取PA11 ADC原始值
 * @param None
 * @retval uint16_t ADC原始值
 */
uint16_t adc_pa11_read_raw(void);

/**
 * @brief ADC读取函数（15次平均采样）
 * @param Channel: ADC通道
 * @retval uint16_t 平均ADC值
 */
uint16_t ADC_Read(uint32_t Channel);

//==================== RTC init/read ====================
/**
 * @brief 初始化RTC（使用LSI时钟）
 * @param None
 * @retval None
 */
void rtc_init_lsi(void);

/**
 * @brief 读取RTC时间和日期
 * @param t: 时间结构体指针
 * @param d: 日期结构体指针
 * @retval None
 */
void rtc_read(RTC_TimeTypeDef* t, RTC_DateTypeDef* d);

//==================== BTIM 1秒定时器 ====================
/**
 * @brief 初始化1秒定时器
 * @param None
 * @retval None
 */
void btim_1s_init(void);

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

#endif /* __MY_INIT_H */
