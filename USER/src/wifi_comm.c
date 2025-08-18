/******************************************************************************
 * @file    wifi_comm.c
 * @brief   WiFiģ��ͨ��ʵ��
 * @author  CW32
 * @date    2024
 ******************************************************************************/

#include "wifi_config.h"
#include "main.h"
#include <string.h>

// ȫ�ֱ���
static volatile wifi_status_t wifi_current_status = WIFI_STATUS_DISCONNECTED;
static volatile uint8_t tcp_connected = 0;

/**
 * @brief ����AT����ȴ���Ӧ
 * @param command: AT����
 * @param expected_response: ��������Ӧ
 * @param timeout_ms: ��ʱʱ��(����)
 * @return 1: �ɹ�, 0: ʧ��
 */
uint8_t wifi_send_at_command_wait_response(const char* command, const char* expected_response, uint32_t timeout_ms)
{
    uint32_t start_time = 0;
    uint8_t response_buffer[WIFI_BUFFER_SIZE];
    
    // ��ս��ջ�����
    uart_rx_clear_buffer();
    
    // ��������
    UART_SendString(CW_UART3, (char*)command);
    UART_SendString(CW_UART3, "\r\n");
    
    // �ȴ���Ӧ
    while(start_time < timeout_ms)
    {
        if(uart_rx_is_data_ready())
        {
            uint16_t len = uart_rx_get_data(response_buffer, WIFI_BUFFER_SIZE);
            
            // ����Ƿ������������Ӧ
            for(uint16_t i = 0; i <= len - strlen(expected_response); i++)
            {
                if(strncmp((char*)&response_buffer[i], expected_response, strlen(expected_response)) == 0)
                {
                    return 1; // �ҵ���������Ӧ
                }
            }
        }
        
        delay_ms(10);
        start_time += 10;
    }
    
    return 0; // ��ʱ
}

/**
 * @brief WiFiģ���ʼ��
 */
void wifi_init(void)
{
    // �ȴ�ģ������
    delay_ms(2000);
    
    // ����AT����
    if(wifi_send_at_command_wait_response(AT_CMD_TEST, RESP_OK, 2000))
    {
        // �رջ���
        wifi_send_at_command_wait_response(AT_CMD_ECHO_OFF, RESP_OK, 1000);
        
        // ����ΪStationģʽ
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
 * @brief ����WiFi����
 * @param ssid: WiFi����
 * @param password: WiFi����
 * @return 1: �ɹ�, 0: ʧ��
 */
uint8_t wifi_connect_network(const char* ssid, const char* password)
{
    char command[128];
    
    // ������������
    sprintf(command, "%s=\"%s\",\"%s\"", AT_CMD_CONNECT, ssid, password);

	// ������������
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
 * @brief �Ͽ�WiFi����
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
 * @brief ��ȡIP��ַ
 */
void wifi_get_ip_address(void)
{
    wifi_send_at_command_wait_response(AT_CMD_GET_IP, RESP_OK, 2000);
}

/**
 * @brief ����TCP����
 * @param ip: ������IP��ַ
 * @param port: �������˿�
 * @return 1: �ɹ�, 0: ʧ��
 */
uint8_t wifi_tcp_connect(const char* ip, const char* port)
{
    char command[128];
    
    // ���õ�����ģʽ
    wifi_send_at_command_wait_response(AT_CMD_SINGLE_CONN, RESP_OK, 1000);
    
    // ����TCP��������
    sprintf(command, "%s=\"TCP\",\"%s\",%s", AT_CMD_TCP_CONN, ip, port);
    
    // ������������
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
 * @brief ����TCP����
 * @param data: Ҫ���͵�����
 * @return 1: �ɹ�, 0: ʧ��
 */
uint8_t wifi_tcp_send_data(const char* data)
{
    char command[32];
    uint16_t data_len = strlen(data);
    
    if(!tcp_connected)
    {
        return 0;
    }
    
    // ������������
    sprintf(command, "%s=%d", AT_CMD_SEND_DATA, data_len);
    
    // ���ͳ�������
    if(wifi_send_at_command_wait_response(command, ">", 2000))
    {
        // ��������
        UART_SendString(CW_UART3, (char*)data);
        
        // �ȴ��������
        if(wifi_send_at_command_wait_response("", RESP_OK, DATA_SEND_TIMEOUT))
        {
            return 1;
        }
    }
    
    return 0;
}

/**
 * @brief �ر�TCP����
 */
void wifi_tcp_close(void)
{
    if(wifi_send_at_command_wait_response(AT_CMD_CLOSE_CONN, RESP_OK, 2000))
    {
        tcp_connected = 0;
    }
}

/**
 * @brief ��ȡWiFi״̬
 * @return WiFi״̬
 */
wifi_status_t wifi_get_status(void)
{
    return wifi_current_status;
}

/**
 * @brief ���TCP����״̬
 * @return 1: ������, 0: δ����
 */
uint8_t wifi_is_tcp_connected(void)
{
    return tcp_connected;
}
