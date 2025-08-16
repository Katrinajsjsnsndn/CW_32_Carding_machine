/******************************************************************************
 * @file    uart_test.c
 * @brief   UART3通信测试源文件
 * @author  CW32
 * @date    2024
 ******************************************************************************/

#include "uart_test.h"
#include "main.h"
#include <string.h>

/**
 * @brief 初始化UART测试功能
 */
void uart_test_init(void)
{
    // 发送启动消息
    uart3_send_string("UART3 Test System Ready\r\n");
    uart3_send_string("Type 'HELP' for available commands\r\n");
}

/**
 * @brief 处理UART测试命令
 * @param command: 接收到的命令
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
        // 未知命令，发送错误响应
        char response[64];
        snprintf(response, sizeof(response), "Unknown command: %s", command);
        uart3_send_string(response);
        uart3_send_string("\r\n");
    }
}

/**
 * @brief 发送UART测试响应
 * @param response: 响应内容
 */
void uart_test_send_response(const char* response)
{
    uart3_send_string(response);
    uart3_send_string("\r\n");
}

/**
 * @brief ECHO测试
 */
void uart_test_echo_test(void)
{
    uart3_send_string("ECHO Test: ");
    uart3_send_string("Hello from CW32L011 UART3!\r\n");
}

/**
 * @brief 回环测试
 */
void uart_test_loopback_test(void)
{
    uart3_send_string("LOOPBACK Test: ");
    uart3_send_string("This is a loopback test message\r\n");
}

/**
 * @brief 状态查询测试
 */
void uart_test_status_test(void)
{
    uart3_send_string("STATUS: UART3 is working normally\r\n");
    uart3_send_string("Baudrate: 115200\r\n");
    uart3_send_string("Buffer Size: 256 bytes\r\n");
}

/**
 * @brief 版本信息测试
 */
void uart_test_version_test(void)
{
    uart3_send_string("VERSION: CW32L011 UART3 Test v1.0\r\n");
    uart3_send_string("Date: 2024\r\n");
    uart3_send_string("Author: CW32\r\n");
}

/**
 * @brief 帮助信息测试
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
 * @brief 处理接收到的数据
 */
void uart_test_process_received_data(void)
{
    // 检查是否有新数据
    if(uart3_rx_flag)
    {
        // 清除接收标志
        uart3_rx_flag = 0;
        
        // 读取接收到的数据
        uint8_t test_buffer[256];
        uint16_t len = uart3_receive_data(test_buffer, sizeof(test_buffer));
        
        if(len > 0)
        {
            // 将接收到的数据转换为字符串
            test_buffer[len] = '\0';
            
            // 去除回车换行符
            char* newline = strchr((char*)test_buffer, '\r');
            if(newline) *newline = '\0';
            newline = strchr((char*)test_buffer, '\n');
            if(newline) *newline = '\0';
            
            // 处理命令
            uart_test_process_command((char*)test_buffer);
        }
    }
}

