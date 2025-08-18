/******************************************************************************
 * @file    wifi_comm.c
 * @brief   WiFi模块通信实现
 * @author  CW32
 * @date    2024
 ******************************************************************************/

#include "wifi_config.h"
#include "main.h"
#include <string.h>

// 全局变量
static volatile wifi_status_t wifi_current_status = WIFI_STATUS_DISCONNECTED;
static volatile uint8_t tcp_connected = 0;

/**
 * @brief 发送AT命令并等待响应
 * @param command: AT命令
 * @param expected_response: 期望的响应
 * @param timeout_ms: 超时时间(毫秒)
 * @return 1: 成功, 0: 失败
 */
uint8_t wifi_send_at_command_wait_response(const char* command, const char* expected_response, uint32_t timeout_ms)
{
    uint32_t start_time = 0;
    uint8_t response_buffer[WIFI_BUFFER_SIZE];
    
    // 清空接收缓冲区
    uart_rx_clear_buffer();
    
    // 发送命令
    UART_SendString(CW_UART3, (char*)command);
    UART_SendString(CW_UART3, "\r\n");
    
    // 等待响应
    while(start_time < timeout_ms)
    {
        if(uart_rx_is_data_ready())
        {
            uint16_t len = uart_rx_get_data(response_buffer, WIFI_BUFFER_SIZE);
            
            // 检查是否包含期望的响应
            for(uint16_t i = 0; i <= len - strlen(expected_response); i++)
            {
                if(strncmp((char*)&response_buffer[i], expected_response, strlen(expected_response)) == 0)
                {
                    return 1; // 找到期望的响应
                }
            }
        }
        
        delay_ms(10);
        start_time += 10;
    }
    
    return 0; // 超时
}

/**
 * @brief WiFi模块初始化
 */
void wifi_init(void)
{
    // 等待模块启动
    delay_ms(2000);
    
    // 测试AT命令
    if(wifi_send_at_command_wait_response(AT_CMD_TEST, RESP_OK, 2000))
    {
        // 关闭回显
        wifi_send_at_command_wait_response(AT_CMD_ECHO_OFF, RESP_OK, 1000);
        
        // 设置为Station模式
        if(wifi_send_at_command_wait_response(AT_CMD_MODE_STA, RESP_OK, 2000))
        {
            wifi_current_status = WIFI_STATUS_DISCONNECTED;
        }
        else
        {
            wifi_current_status = WIFI_STATUS_ERROR;
        }
    }
    else
    {
        wifi_current_status = WIFI_STATUS_ERROR;
    }
}

/**
 * @brief 连接WiFi网络
 * @param ssid: WiFi名称
 * @param password: WiFi密码
 * @return 1: 成功, 0: 失败
 */
uint8_t wifi_connect_network(const char* ssid, const char* password)
{
    char command[128];
    
    // 构建连接命令
    sprintf(command, "%s=\"%s\",\"%s\"", AT_CMD_CONNECT, ssid, password);

	// 发送连接命令
    if(wifi_send_at_command_wait_response(command, RESP_OK, WIFI_CONN_TIMEOUT))
    {
        wifi_current_status = WIFI_STATUS_CONNECTED;
        return 1;
    }
    else
    {
        wifi_current_status = WIFI_STATUS_ERROR;
        return 0;
    }
}

/**
 * @brief 断开WiFi网络
 */
void wifi_disconnect_network(void)
{
    if(wifi_send_at_command_wait_response(AT_CMD_DISCONNECT, RESP_OK, 2000))
    {
        wifi_current_status = WIFI_STATUS_DISCONNECTED;
        tcp_connected = 0;
    }
}

/**
 * @brief 获取IP地址
 */
void wifi_get_ip_address(void)
{
    wifi_send_at_command_wait_response(AT_CMD_GET_IP, RESP_OK, 2000);
}

/**
 * @brief 建立TCP连接
 * @param ip: 服务器IP地址
 * @param port: 服务器端口
 * @return 1: 成功, 0: 失败
 */
uint8_t wifi_tcp_connect(const char* ip, const char* port)
{
    char command[128];
    
    // 设置单连接模式
    wifi_send_at_command_wait_response(AT_CMD_SINGLE_CONN, RESP_OK, 1000);
    
    // 构建TCP连接命令
    sprintf(command, "%s=\"TCP\",\"%s\",%s", AT_CMD_TCP_CONN, ip, port);
    
    // 发送连接命令
    if(wifi_send_at_command_wait_response(command, RESP_OK, TCP_CONN_TIMEOUT))
    {
        tcp_connected = 1;
        return 1;
    }
    else
    {
        tcp_connected = 0;
        return 0;
    }
}

/**
 * @brief 发送TCP数据
 * @param data: 要发送的数据
 * @return 1: 成功, 0: 失败
 */
uint8_t wifi_tcp_send_data(const char* data)
{
    char command[32];
    uint16_t data_len = strlen(data);
    
    if(!tcp_connected)
    {
        return 0;
    }
    
    // 构建发送命令
    sprintf(command, "%s=%d", AT_CMD_SEND_DATA, data_len);
    
    // 发送长度命令
    if(wifi_send_at_command_wait_response(command, ">", 2000))
    {
        // 发送数据
        UART_SendString(CW_UART3, (char*)data);
        
        // 等待发送完成
        if(wifi_send_at_command_wait_response("", RESP_OK, DATA_SEND_TIMEOUT))
        {
            return 1;
        }
    }
    
    return 0;
}

/**
 * @brief 关闭TCP连接
 */
void wifi_tcp_close(void)
{
    if(wifi_send_at_command_wait_response(AT_CMD_CLOSE_CONN, RESP_OK, 2000))
    {
        tcp_connected = 0;
    }
}

/**
 * @brief 获取WiFi状态
 * @return WiFi状态
 */
wifi_status_t wifi_get_status(void)
{
    return wifi_current_status;
}

/**
 * @brief 检查TCP连接状态
 * @return 1: 已连接, 0: 未连接
 */
uint8_t wifi_is_tcp_connected(void)
{
    return tcp_connected;
}
