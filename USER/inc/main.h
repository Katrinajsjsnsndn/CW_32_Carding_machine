/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 CW.
  * All rights reserved.</center></h2>
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* ���ڽ��ջ��������� */
#define WIFI_BUFFER_SIZE 256
#define UART_RX_TIMEOUT_MS 100  // ���ճ�ʱʱ��(ms)

/* Includes ------------------------------------------------------------------*/
#include "cw32l011.h"
#include "cw32l011_gpio.h"
#include "cw32l011_sysctrl.h"
#include "cw32l011_systick.h"
#include "interrupts_cw32l011.h"
#include "system_cw32l011.h"
#include "cw32l011_uart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */


/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* USER CODE END ET */


/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
/* USER CODE END EC */


/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/* USER CODE END EM */


/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */
// ���ڽ�����غ���
void uart_rx_init(void);
uint16_t uart_rx_get_data(uint8_t *buffer, uint16_t max_len);
void uart_rx_clear_buffer(void);
uint8_t uart_rx_is_data_ready(void);



// ��ʱ����
void delay_ms(uint32_t ms);
/* USER CODE END EFP */


/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
#define WIFI_BUFFER_SIZE 256

// ȫ�ֱ�������
extern volatile uint8_t wifi_rx_buffer[WIFI_BUFFER_SIZE];
extern volatile uint16_t wifi_rx_index;
extern volatile uint8_t wifi_rx_flag;
/* USER CODE END Private defines */


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT CW *****END OF FILE****/
