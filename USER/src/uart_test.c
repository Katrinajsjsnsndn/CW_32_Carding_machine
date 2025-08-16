/******************************************************************************
 * @file    uart_test.c
 * @brief   UART3ͨ�Ų���Դ�ļ�
 * @author  CW32
 * @date    2024
 ******************************************************************************/

#include "uart_test.h"
#include "main.h"
#include <string.h>

/**
 * @brief ��ʼ��UART���Թ���
 */
void uart_test_init(void)
{
    // ����������Ϣ
    uart3_send_string("UART3 Test System Ready\r\n");
    uart3_send_string("Type 'HELP' for available commands\r\n");
}

/**
 * @brief ����UART��������
 * @param command: ���յ�������
 */
void uart_test_process_command(const char* command)
{
    if(strcmp(command, TEST_CMD_ECHO) == 0)
    {
        uart_test_echo_test();
    }
    else if(strcmp(command, TEST_CMD_LOOPBACK) == 0)
    {
        uart_test_loopback_test();
    }
    else if(strcmp(command, TEST_CMD_STATUS) == 0)
    {
        uart_test_status_test();
    }
    else if(strcmp(command, TEST_CMD_VERSION) == 0)
    {
        uart_test_version_test();
    }
    else if(strcmp(command, TEST_CMD_HELP) == 0)
    {
        uart_test_help_test();
    }
    else
    {
        // δ֪������ʹ�����Ӧ
        char response[64];
        snprintf(response, sizeof(response), "Unknown command: %s", command);
        uart3_send_string(response);
        uart3_send_string("\r\n");
    }
}

/**
 * @brief ����UART������Ӧ
 * @param response: ��Ӧ����
 */
void uart_test_send_response(const char* response)
{
    uart3_send_string(response);
    uart3_send_string("\r\n");
}

/**
 * @brief ECHO����
 */
void uart_test_echo_test(void)
{
    uart3_send_string("ECHO Test: ");
    uart3_send_string("Hello from CW32L011 UART3!\r\n");
}

/**
 * @brief �ػ�����
 */
void uart_test_loopback_test(void)
{
    uart3_send_string("LOOPBACK Test: ");
    uart3_send_string("This is a loopback test message\r\n");
}

/**
 * @brief ״̬��ѯ����
 */
void uart_test_status_test(void)
{
    uart3_send_string("STATUS: UART3 is working normally\r\n");
    uart3_send_string("Baudrate: 115200\r\n");
    uart3_send_string("Buffer Size: 256 bytes\r\n");
}

/**
 * @brief �汾��Ϣ����
 */
void uart_test_version_test(void)
{
    uart3_send_string("VERSION: CW32L011 UART3 Test v1.0\r\n");
    uart3_send_string("Date: 2024\r\n");
    uart3_send_string("Author: CW32\r\n");
}

/**
 * @brief ������Ϣ����
 */
void uart_test_help_test(void)
{
    uart3_send_string("Available Commands:\r\n");
    uart3_send_string("  ECHO     - Echo test\r\n");
    uart3_send_string("  LOOPBACK - Loopback test\r\n");
    uart3_send_string("  STATUS   - Status query\r\n");
    uart3_send_string("  VERSION  - Version info\r\n");
    uart3_send_string("  HELP     - This help message\r\n");
}

/**
 * @brief ������յ�������
 */
void uart_test_process_received_data(void)
{
    // ����Ƿ���������
    if(uart3_rx_flag)
    {
        // ������ձ�־
        uart3_rx_flag = 0;
        
        // ��ȡ���յ�������
        uint8_t test_buffer[256];
        uint16_t len = uart3_receive_data(test_buffer, sizeof(test_buffer));
        
        if(len > 0)
        {
            // �����յ�������ת��Ϊ�ַ���
            test_buffer[len] = '\0';
            
            // ȥ���س����з�
            char* newline = strchr((char*)test_buffer, '\r');
            if(newline) *newline = '\0';
            newline = strchr((char*)test_buffer, '\n');
            if(newline) *newline = '\0';
            
            // ��������
            uart_test_process_command((char*)test_buffer);
        }
    }
}

