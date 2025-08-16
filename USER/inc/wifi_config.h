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

/* WiFi�������� */
#define WIFI_SSID           "YourWiFiSSID"        // WiFi��������
#define WIFI_PASSWORD       "YourWiFiPassword"    // WiFi����

/* TCP���������� */
#define TCP_SERVER_IP       "192.168.1.100"       // TCP������IP��ַ
#define TCP_SERVER_PORT     "8080"                // TCP�������˿�

/* UARTͨ������ */
#define UART3_BAUDRATE      115200                // UART3������
#define UART3_BUFFER_SIZE   256                   // ���ջ�������С

/* ����AT���� */
#define AT_CMD_TEST         "AT"                  // ����AT����
#define AT_CMD_ECHO_OFF     "ATE0"               // �رջ���
#define AT_CMD_MODE_STA     "AT+CWMODE=1"        // ����ΪStationģʽ
#define AT_CMD_MODE_AP      "AT+CWMODE=2"        // ����ΪAPģʽ
#define AT_CMD_MODE_APSTA   "AT+CWMODE=3"        // ����ΪAP+Stationģʽ
#define AT_CMD_CONNECT      "AT+CWJAP"           // ����WiFi����
#define AT_CMD_DISCONNECT   "AT+CWQAP"           // �Ͽ�WiFi����
#define AT_CMD_GET_IP       "AT+CIFSR"           // ��ȡIP��ַ
#define AT_CMD_SINGLE_CONN  "AT+CIPMUX=0"        // ���õ�����ģʽ
#define AT_CMD_MULTI_CONN   "AT+CIPMUX=1"        // ���ö�����ģʽ
#define AT_CMD_TCP_CONN     "AT+CIPSTART"        // ����TCP����
#define AT_CMD_CLOSE_CONN   "AT+CIPCLOSE"        // �ر�����
#define AT_CMD_SEND_DATA    "AT+CIPSEND"         // ��������
#define AT_CMD_GET_STATUS   "AT+CIPSTATUS"       // ��ȡ����״̬

/* ��Ӧ�ַ��� */
#define RESP_OK             "OK"                 // �ɹ���Ӧ
#define RESP_ERROR          "ERROR"              // ������Ӧ
#define RESP_READY          "ready"              // ģ�����
#define RESP_WIFI_CONNECTED "WIFI CONNECTED"     // WiFi������
#define RESP_WIFI_DISCONN   "WIFI DISCONNECT"    // WiFi�ѶϿ�
#define RESP_GOT_IP         "WIFI GOT IP"        // �ѻ�ȡIP��ַ

/* ��ʱʱ������ */
#define WIFI_INIT_TIMEOUT   5000                 // WiFi��ʼ����ʱʱ��(ms)
#define WIFI_CONN_TIMEOUT   10000                // WiFi���ӳ�ʱʱ��(ms)
#define TCP_CONN_TIMEOUT    5000                 // TCP���ӳ�ʱʱ��(ms)
#define DATA_SEND_TIMEOUT   3000                 // ���ݷ��ͳ�ʱʱ��(ms)

/* �������� */
void wifi_config_init(void);
void wifi_connect_network(const char* ssid, const char* password);
void wifi_disconnect_network(void);
void wifi_get_ip_address(void);
void wifi_tcp_connect(const char* ip, const char* port);
void wifi_tcp_send_data(const char* data);
void wifi_tcp_close(void);

#ifdef __cplusplus
}
#endif

#endif /* __WIFI_CONFIG_H */





