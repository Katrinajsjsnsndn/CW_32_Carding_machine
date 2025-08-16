/******************************************************************************
 * @file    wifi_config.h
 * @brief   WiFi模块配置文件
 * @author  CW32
 * @date    2024
 ******************************************************************************/

#ifndef __WIFI_CONFIG_H
#define __WIFI_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* WiFi网络配置 */
#define WIFI_SSID           "YourWiFiSSID"        // WiFi网络名称
#define WIFI_PASSWORD       "YourWiFiPassword"    // WiFi密码

/* TCP服务器配置 */
#define TCP_SERVER_IP       "192.168.1.100"       // TCP服务器IP地址
#define TCP_SERVER_PORT     "8080"                // TCP服务器端口

/* UART通信配置 */
#define UART3_BAUDRATE      115200                // UART3波特率
#define UART3_BUFFER_SIZE   256                   // 接收缓冲区大小

/* 常用AT命令 */
#define AT_CMD_TEST         "AT"                  // 测试AT命令
#define AT_CMD_ECHO_OFF     "ATE0"               // 关闭回显
#define AT_CMD_MODE_STA     "AT+CWMODE=1"        // 设置为Station模式
#define AT_CMD_MODE_AP      "AT+CWMODE=2"        // 设置为AP模式
#define AT_CMD_MODE_APSTA   "AT+CWMODE=3"        // 设置为AP+Station模式
#define AT_CMD_CONNECT      "AT+CWJAP"           // 连接WiFi网络
#define AT_CMD_DISCONNECT   "AT+CWQAP"           // 断开WiFi网络
#define AT_CMD_GET_IP       "AT+CIFSR"           // 获取IP地址
#define AT_CMD_SINGLE_CONN  "AT+CIPMUX=0"        // 设置单连接模式
#define AT_CMD_MULTI_CONN   "AT+CIPMUX=1"        // 设置多连接模式
#define AT_CMD_TCP_CONN     "AT+CIPSTART"        // 建立TCP连接
#define AT_CMD_CLOSE_CONN   "AT+CIPCLOSE"        // 关闭连接
#define AT_CMD_SEND_DATA    "AT+CIPSEND"         // 发送数据
#define AT_CMD_GET_STATUS   "AT+CIPSTATUS"       // 获取连接状态

/* 响应字符串 */
#define RESP_OK             "OK"                 // 成功响应
#define RESP_ERROR          "ERROR"              // 错误响应
#define RESP_READY          "ready"              // 模块就绪
#define RESP_WIFI_CONNECTED "WIFI CONNECTED"     // WiFi已连接
#define RESP_WIFI_DISCONN   "WIFI DISCONNECT"    // WiFi已断开
#define RESP_GOT_IP         "WIFI GOT IP"        // 已获取IP地址

/* 超时时间配置 */
#define WIFI_INIT_TIMEOUT   5000                 // WiFi初始化超时时间(ms)
#define WIFI_CONN_TIMEOUT   10000                // WiFi连接超时时间(ms)
#define TCP_CONN_TIMEOUT    5000                 // TCP连接超时时间(ms)
#define DATA_SEND_TIMEOUT   3000                 // 数据发送超时时间(ms)

/* 函数声明 */
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





