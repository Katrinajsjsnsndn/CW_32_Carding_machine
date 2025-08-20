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
 ** ϵͳ��ʼ����غ�������
 **
 **   - 2024-01-01  1.0  Initial version for Device Driver Library of Module.
 **
 ******************************************************************************/
/*******************************************************************************
*
* ������ɺ�������Ϣ
* �人оԴ�뵼�����޹�˾������ʹ�����б�̴���ʾ���ķ�ר���İ�Ȩ��ɣ��������ɴ�
* ���ɸ��������ض���Ҫ�����Ƶ����ƹ��ܡ����ݲ��ܱ��ų����κη�����֤���人оԴ��
* �������޹�˾������򿪷��̺͹�Ӧ�̶Գ������֧�֣�����У����ṩ�κ���ʾ��
* ���ı�֤�������������������ڰ������й������ԡ�������ĳ���ض���;�ͷ���Ȩ�ı�֤
* ��������
* ���ۺ������Σ��人оԴ�뵼�����޹�˾������򿪷��̻�Ӧ�̾��������и����
* ��ʹ����֪�䷢���Ŀ�����ʱ��Ҳ����ˣ����ݵĶ�ʧ���𻵣�ֱ�ӵġ��ر�ġ�������
* ���ӵ��𺦣����κκ���Ծ����𺦣�������ҵ�����롢������Ԥ�ڿɽ�ʡ����
* ��ʧ��
* ĳЩ˾��Ͻ���������ֱ�ӵġ������Ļ����Ե������κε��ų������ƣ����ĳЩ��
* ȫ�������ų������ƿ��ܲ�������������
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
 * @brief ϵͳ��ʼ��������
 * @param None
 * @retval None
 */
void system_init(void);

/**
 * @brief WiFi�������ӳ�ʼ��
 * @param None
 * @retval uint8_t ����״̬ 1:�ɹ� 0:ʧ��
 */
uint8_t wifi_network_init(void);

/**
 * @brief ����RCCʱ��
 * @param None
 * @retval None
 */
void RCC_Configuration(void);

/**
 * @brief GPIO��ʼ��
 * @param None
 * @retval None
 */
void my_gpio_init(void);

/**
 * @brief UART����
 * @param None
 * @retval None
 */
void UART_Configuration(void);

/**
 * @brief NVIC�ж�����
 * @param None
 * @retval None
 */
void NVIC_Configuration(void);







//==================== ADC PA11 (CH13) ====================
/**
 * @brief ��ʼ��PA11 ADC��ͨ��13��
 * @param None
 * @retval None
 */
void adc_pa11_init(void);

/**
 * @brief ��ȡPA11 ADCԭʼֵ
 * @param None
 * @retval uint16_t ADCԭʼֵ
 */
uint16_t adc_pa11_read_raw(void);

/**
 * @brief ADC��ȡ������15��ƽ��������
 * @param Channel: ADCͨ��
 * @retval uint16_t ƽ��ADCֵ
 */
uint16_t ADC_Read(uint32_t Channel);

//==================== RTC init/read ====================
/**
 * @brief ��ʼ��RTC��ʹ��LSIʱ�ӣ�
 * @param None
 * @retval None
 */
void rtc_init_lsi(void);

/**
 * @brief ��ȡRTCʱ�������
 * @param t: ʱ��ṹ��ָ��
 * @param d: ���ڽṹ��ָ��
 * @retval None
 */
void rtc_read(RTC_TimeTypeDef* t, RTC_DateTypeDef* d);

//==================== BTIM 1�붨ʱ�� ====================
/**
 * @brief ��ʼ��1�붨ʱ��
 * @param None
 * @retval None
 */
void btim_1s_init(void);

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

#endif /* __MY_INIT_H */
