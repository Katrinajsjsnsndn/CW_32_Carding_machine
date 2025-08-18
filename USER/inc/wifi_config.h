/******************************************************************************
 * @file    wifi_config.h
 * @brief   WiFiģ�������ļ�
 * @author  CW32
 * @date    2024
 ******************************************************************************/

#ifndef __WIFI_CONFIG_H
#define __WIFI_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
#include "../inc/main.h"

/* WiFi�������� */
#define WIFI_SSID           "Jumao"        // �޸�Ϊ����WiFi����
#define WIFI_PASSWORD       "jumaoyuzhou2025"    // �޸�Ϊ����WiFi����

/* TCP���������� */
#define TCP_SERVER_IP       "42f5d77ef1.st1.iotda-device.cn-north-4.myhuaweicloud.com"       // �޸�Ϊ���ķ�����IP
#define TCP_SERVER_PORT     "8883"                // �޸�Ϊ���ķ������˿�

/* UARTͨ������ */
#define UART3_BAUDRATE      115200                // UART3������
#define WIFI_BUFFER_SIZE    256                   // WiFi���ջ�������С

/* ����AT���� */
#define AT_CMD_TEST         "AT"                  // ����AT����
#define AT_CMD_ECHO_OFF     "ATE0"               // �رջ���
#define AT_CMD_MODE_STA     "AT+WMODE=1,0"        // ����ΪStationģʽ
#define AT_CMD_MODE_AP      "AT+WMODE=2,0"        // ����ΪAPģʽ
#define AT_CMD_MODE_APSTA   "AT+WMODE=3,0"        // ����ΪAP+Stationģʽ
#define AT_CMD_CONNECT      "AT+WJAP"           // ����WiFi����
#define AT_CMD_DISCONNECT   "AT+WQAP"           // �Ͽ�WiFi����
#define AT_CMD_GET_IP       "AT+IFSR"           // ��ȡIP��ַ
#define AT_CMD_SINGLE_CONN  "AT+IPMUX=0"        // ���õ�����ģʽ
#define AT_CMD_MULTI_CONN   "AT+IPMUX=1"        // ���ö�����ģʽ
#define AT_CMD_TCP_CONN     "AT+IPSTART"        // ����TCP����
#define AT_CMD_CLOSE_CONN   "AT+IPCLOSE"        // �ر�����
#define AT_CMD_SEND_DATA    "AT+IPSEND"         // ��������
#define AT_CMD_GET_STATUS   "AT+IPSTATUS"       // ��ȡ����״̬

/* ��Ӧ�ַ��� */
#define RESP_OK             "OK"                 // �ɹ���Ӧ
#define RESP_ERROR          "ERROR"              // ������Ӧ
#define RESP_READY          "ready"              // ģ�����
#define RESP_WIFI_CONNECTED "WIFI CONNECTED"     // WiFi������
#define RESP_WIFI_DISCONN   "WIFI DISCONNECT"    // WiFi�ѶϿ�
#define RESP_GOT_IP         "WIFI GOT IP"        // �ѻ�ȡIP��ַ

/* ��ʱʱ������ */
#define WIFI_INIT_TIMEOUT   5000                 // WiFi��ʼ����ʱʱ��(ms)
#define WIFI_CONN_TIMEOUT   100000                // WiFi���ӳ�ʱʱ��(ms)
#define TCP_CONN_TIMEOUT    100000                 // TCP���ӳ�ʱʱ��(ms)
#define DATA_SEND_TIMEOUT   3000                 // ���ݷ��ͳ�ʱʱ��(ms)

/* WiFi״̬���Ͷ��� */
typedef enum {
    WIFI_STATUS_DISCONNECTED = 0,
    WIFI_STATUS_CONNECTING,
    WIFI_STATUS_CONNECTED,
    WIFI_STATUS_ERROR
} wifi_status_t;

/* �������� */
void wifi_init(void);
uint8_t wifi_connect_network(const char* ssid, const char* password);
void wifi_disconnect_network(void);
void wifi_get_ip_address(void);
uint8_t wifi_tcp_connect(const char* ip, const char* port);
uint8_t wifi_tcp_send_data(const char* data);
void wifi_tcp_close(void);
wifi_status_t wifi_get_status(void);
uint8_t wifi_is_tcp_connected(void);
uint8_t wifi_send_at_command_wait_response(const char* command, const char* expected_response, uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* __WIFI_CONFIG_H */







