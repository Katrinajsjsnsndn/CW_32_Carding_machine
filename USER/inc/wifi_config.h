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
#include "../inc/main.h"

/* WiFi网络配置 */
#define WIFI_SSID           "Jumao"        // 修改为您的WiFi名称
#define WIFI_PASSWORD       "jumaoyuzhou2025"    // 修改为您的WiFi密码

/* TCP服务器配置 */
#define TCP_SERVER_IP       "42f5d77ef1.st1.iotda-device.cn-north-4.myhuaweicloud.com"       // 修改为您的服务器IP
#define TCP_SERVER_PORT     "8883"                // 修改为您的服务器端口

/* UART通信配置 */
#define UART3_BAUDRATE      115200                // UART3波特率
#define WIFI_BUFFER_SIZE    256                   // WiFi接收缓冲区大小

/* MQTT服务器配置 */
#define MQTT_BROKER_HOST    "42f5d77ef1.st1.iotda-device.cn-north-4.myhuaweicloud.com"  // MQTT服务器域名/IP
#define MQTT_BROKER_PORT    "1883"               // MQTT服务器端口（8883=TLS，1883=明文）
#define MQTT_SSL_ENABLE     1                     // 1: 启用TLS, 0: 关闭TLS（按需修改）
#define MQTT_CLIENT_ID      "689ed67e32771f177b567fea_2004_0_0_2025081809"     // 客户端ID
#define MQTT_USERNAME       "689ed67e32771f177b567fea_2004"      // 用户名
#define MQTT_PASSWORD       "afb5b2c503fa01268db1cb87ffe4104c57435aeafebd9d557ccf2682edbdfaec"      // 密码

/* MQTT测试主题与载荷 */
#define MQTT_TEST_TOPIC     "test/topic"
#define MQTT_TEST_PAYLOAD   "hello"

/* 常用AT命令 */
#define AT_CMD_TEST         "AT"                  // 测试AT命令
#define AT_CMD_ECHO_OFF     "ATE0"               // 关闭回显
#define AT_CMD_MODE_STA     "AT+WMODE=1,0"        // 设置为Station模式
#define AT_CMD_MODE_AP      "AT+WMODE=2,0"        // 设置为AP模式
#define AT_CMD_MODE_APSTA   "AT+WMODE=3,0"        // 设置为AP+Station模式
#define AT_CMD_CONNECT      "AT+WJAP"           // 连接WiFi网络
#define AT_CMD_DISCONNECT   "AT+WQAP"           // 断开WiFi网络
#define AT_CMD_GET_IP       "AT+IFSR"           // 获取IP地址
#define AT_CMD_SINGLE_CONN  "AT+IPMUX=0"        // 设置单连接模式
#define AT_CMD_MULTI_CONN   "AT+IPMUX=1"        // 设置多连接模式
#define AT_CMD_TCP_CONN     "AT+IPSTART"        // 建立TCP连接
#define AT_CMD_CLOSE_CONN   "AT+IPCLOSE"        // 关闭连接
#define AT_CMD_SEND_DATA    "AT+IPSEND"         // 发送数据
#define AT_CMD_GET_STATUS   "AT+IPSTATUS"       // 获取连接状态

/* MQTT相关AT命令（依赖模组支持）*/
#define AT_CMD_MQTT_CFG_HOST   "AT+MQTT=1"      // 设置MQTT服务器地址
#define AT_CMD_MQTT_CFG_PORT   "AT+MQTT=2"      // 设置MQTT端口
#define AT_CMD_MQTT_CFG_SSL    "AT+MQTT=3"      // 设置是否TLS
#define AT_CMD_MQTT_CFG_CID    "AT+MQTT=4"      // 设置ClientID
#define AT_CMD_MQTT_CFG_USER   "AT+MQTT=5"      // 设置用户名
#define AT_CMD_MQTT_CFG_PASS   "AT+MQTT=6"      // 设置密码
#define AT_CMD_MQTT_CONNECT    "AT+MQTT"        // 发起连接
#define AT_CMD_MQTT_PUBLISH    "AT+MQTTPUB"     // 发布消息

/* 响应字符串 */
#define RESP_OK             "OK"                 // 成功响应
#define RESP_ERROR          "ERROR"              // 错误响应
#define RESP_READY          "ready"              // 模块就绪
#define RESP_WIFI_CONNECTED "WIFI CONNECTED"     // WiFi已连接
#define RESP_WIFI_DISCONN   "WIFI DISCONNECT"    // WiFi已断开
#define RESP_GOT_IP         "WIFI GOT IP"        // 已获取IP地址

/* 超时时间配置 */
#define WIFI_INIT_TIMEOUT   5000                 // WiFi初始化超时时间(ms)
#define WIFI_CONN_TIMEOUT   100000                // WiFi连接超时时间(ms)
#define TCP_CONN_TIMEOUT    100000                 // TCP连接超时时间(ms)
#define DATA_SEND_TIMEOUT   3000                 // 数据发送超时时间(ms)

/* WiFi状态类型定义 */
typedef enum {
    WIFI_STATUS_DISCONNECTED = 0,
    WIFI_STATUS_CONNECTING,
    WIFI_STATUS_CONNECTED,
    WIFI_STATUS_ERROR
} wifi_status_t;

/* 函数声明 */
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

/* MQTT函数声明（基于模组MQTT AT指令）*/
uint8_t mqtt_configure(void);
uint8_t mqtt_connect(void);
uint8_t mqtt_publish(const char* topic, const char* payload, uint8_t qos, uint8_t retain);
void mqtt_disconnect(void);

#ifdef __cplusplus
}
#endif

#endif /* __WIFI_CONFIG_H */







