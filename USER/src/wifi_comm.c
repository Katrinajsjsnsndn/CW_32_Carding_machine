/******************************************************************************
 * @file    wifi_comm.c
 * @brief   WiFiģ��ͨ��ʵ��
 * @author  CW32
 * @date    2024
 ******************************************************************************/

#include "wifi_config.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include "cw32l011_rtc.h"

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

/**
 * @brief ���ò�����SNTPʱ��ͬ��
 * @param timezone: ʱ��ƫ�ƣ�Сʱ������8��ʾUTC+8
 * @return 1: �ɹ�, 0: ʧ��
 */
uint8_t wifi_sntp_config(uint8_t timezone)
{
    char command[32];
    sprintf(command, "%s=1,%d", AT_CMD_SNTP_CONFIG, timezone);
    return wifi_send_at_command_wait_response(command, RESP_OK, 5000);
}

/**
 * @brief ����SNTPʱ����Ӧ
 * @param response: SNTP��Ӧ�ַ���
 * @param time: ���ʱ��ṹ��
 * @param date: ������ڽṹ��
 * @return 1: �ɹ�����, 0: ����ʧ��
 */
static uint8_t parse_sntp_time(const char* response, RTC_TimeTypeDef* time, RTC_DateTypeDef* date)
{
    // ��Ӧ��ʽ: +SNTPTIME:WedMay0310:49:412023
    if(strncmp(response, "+SNTPTIME:", 10) != 0) {
        return 0;
    }
    
    const char* time_str = response + 10;
    char week[4], month[4];
    uint8_t day, hour, minute, second;
    uint16_t year;
    
    // ����: WedMay0310:49:412023
    if(sscanf(time_str, "%3s%3s%2hhu%2hhu:%2hhu:%2hhu%4hu", 
              week, month, &day, &hour, &minute, &second, &year) != 7) {
        return 0;
    }
    
    // ת���·����Ƶ�����
    uint8_t month_num = 0;
    if(strcmp(month, "Jan") == 0) month_num = 1;
    else if(strcmp(month, "Feb") == 0) month_num = 2;
    else if(strcmp(month, "Mar") == 0) month_num = 3;
    else if(strcmp(month, "Apr") == 0) month_num = 4;
    else if(strcmp(month, "May") == 0) month_num = 5;
    else if(strcmp(month, "Jun") == 0) month_num = 6;
    else if(strcmp(month, "Jul") == 0) month_num = 7;
    else if(strcmp(month, "Aug") == 0) month_num = 8;
    else if(strcmp(month, "Sep") == 0) month_num = 9;
    else if(strcmp(month, "Oct") == 0) month_num = 10;
    else if(strcmp(month, "Nov") == 0) month_num = 11;
    else if(strcmp(month, "Dec") == 0) month_num = 12;
    else return 0;
    
    // ת���������Ƶ�����
    uint8_t week_num = 0;
    if(strcmp(week, "Mon") == 0) week_num = RTC_Weekday_Monday;
    else if(strcmp(week, "Tue") == 0) week_num = RTC_Weekday_Tuesday;
    else if(strcmp(week, "Wed") == 0) week_num = RTC_Weekday_Wednesday;
    else if(strcmp(week, "Thu") == 0) week_num = RTC_Weekday_Thursday;
    else if(strcmp(week, "Fri") == 0) week_num = RTC_Weekday_Friday;
    else if(strcmp(week, "Sat") == 0) week_num = RTC_Weekday_Saturday;
    else if(strcmp(week, "Sun") == 0) week_num = RTC_Weekday_Sunday;
    else return 0;
    
    // ����ʱ��ṹ��
    time->H24 = RTC_HOUR24;
    time->Hour = RTC_BinToBCD(hour);
    time->Minute = RTC_BinToBCD(minute);
    time->Second = RTC_BinToBCD(second);
    
    // �������ڽṹ��
    date->Year = RTC_BinToBCD(year % 100);  // ֻȡ��ݺ���λ
    date->Month = RTC_BinToBCD(month_num);
    date->Day = RTC_BinToBCD(day);
    date->Week = week_num;
    
    return 1;
}

/**
 * @brief ��ȡSNTPʱ�䲢ͬ����RTC
 * @return 1: �ɹ�, 0: ʧ��
 */
uint8_t wifi_sntp_sync_time(void)
{
    uint8_t response_buffer[WIFI_BUFFER_SIZE];
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    
    // ��ս��ջ�����
    uart_rx_clear_buffer();
    
    // ����SNTPʱ���ѯ����
    UART_SendString(CW_UART3, (char*)AT_CMD_SNTP_QUERY);
    UART_SendString(CW_UART3, "\r\n");
    
    // �ȴ���Ӧ
    uint32_t start_time = 0;
    while(start_time < 10000)  // 10�볬ʱ
    {
        if(uart_rx_is_data_ready())
        {
            uint16_t len = uart_rx_get_data(response_buffer, WIFI_BUFFER_SIZE);
            response_buffer[len] = '\0';  // ȷ���ַ�������
            
            // ����SNTPʱ����Ӧ
            char* sntp_response = strstr((char*)response_buffer, "+SNTPTIME:");
            if(sntp_response && parse_sntp_time(sntp_response, &time, &date))
            {
                // ����RTCʱ��
                RTC_SetDate(&date);
                RTC_SetTime(&time);
                return 1;
            }
        }
        
        delay_ms(100);
        start_time += 100;
    }
    
    return 0;
}
