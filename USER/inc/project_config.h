/******************************************************************************
 * @file    project_config.h
 * @brief   项目配置文件
 * @author  CW32
 * @date    2024
 ******************************************************************************/

#ifndef __PROJECT_CONFIG_H
#define __PROJECT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 系统配置
 * ============================================================================ */

/* 系统时钟配置 */
#define SYSTEM_CLOCK_FREQ       48000000    // 系统时钟频率 (Hz)
#define UART_CLOCK_FREQ         48000000    // UART时钟频率 (Hz)

/* 调试配置 */
#define DEBUG_ENABLE             1           // 启用调试功能 (1:启用, 0:禁用)
#define DEBUG_UART               CW_UART3   // 调试UART端口

/* ============================================================================
 * UART3配置
 * ============================================================================ */

/* 基本参数 */
#define UART3_BAUDRATE          115200      // 波特率
#define UART3_BUFFER_SIZE       256         // 接收缓冲区大小
#define UART3_TIMEOUT           1000        // 超时时间 (ms)

/* 通信参数 */
#define UART3_DATA_BITS         8           // 数据位
#define UART3_STOP_BITS         1           // 停止位
#define UART3_PARITY            NONE        // 校验位 (NONE, EVEN, ODD)
#define UART3_FLOW_CONTROL      NONE        // 流控制 (NONE, RTS, CTS, RTS_CTS)

/* 中断配置 */
#define UART3_RX_IRQ_ENABLE     1           // 启用接收中断 (1:启用, 0:禁用)
#define UART3_TX_IRQ_ENABLE     0           // 启用发送中断 (1:启用, 0:禁用)
#define UART3_IRQ_PRIORITY      0           // 中断优先级 (0-3, 0为最高)

/* ============================================================================
 * WiFi配置
 * ============================================================================ */

/* 网络配置 */
#define WIFI_SSID               "YourWiFiSSID"      // WiFi网络名称
#define WIFI_PASSWORD           "YourWiFiPassword"  // WiFi密码
#define WIFI_SECURITY           WPA2                // 安全类型 (WPA, WPA2, WEP, OPEN)

/* 工作模式 */
#define WIFI_MODE               STATION             // 工作模式 (STATION, AP, AP_STATION)
#define WIFI_AUTO_CONNECT       1                   // 自动连接 (1:启用, 0:禁用)

/* TCP配置 */
#define TCP_SERVER_IP           "192.168.1.100"     // TCP服务器IP地址
#define TCP_SERVER_PORT         "8080"              // TCP服务器端口
#define TCP_CONNECTION_TYPE     TCP                 // 连接类型 (TCP, UDP)
#define TCP_AUTO_RECONNECT      1                   // 自动重连 (1:启用, 0:禁用)

/* 超时配置 */
#define WIFI_INIT_TIMEOUT       5000                // WiFi初始化超时 (ms)
#define WIFI_CONN_TIMEOUT       10000               // WiFi连接超时 (ms)
#define TCP_CONN_TIMEOUT        5000                // TCP连接超时 (ms)
#define DATA_SEND_TIMEOUT       3000                // 数据发送超时 (ms)

/* ============================================================================
 * GPIO配置
 * ============================================================================ */

/* UART3引脚配置 */
#define UART3_TX_PIN           GPIO_PIN_1          // TX引脚 (PA01)
#define UART3_RX_PIN           GPIO_PIN_0          // RX引脚 (PA00)
#define UART3_TX_PORT          CW_GPIOA            // TX端口
#define UART3_RX_PORT          CW_GPIOA            // RX端口

/* 状态指示LED */
#define STATUS_LED_PIN          GPIO_PIN_3          // 状态LED引脚
#define STATUS_LED_PORT         CW_GPIOB            // 状态LED端口
#define STATUS_LED_ON_LEVEL     1                   // LED点亮电平 (0或1)

/* ============================================================================
 * 功能开关
 * ============================================================================ */

/* 主要功能 */
#define FEATURE_UART_TEST       1                   // UART测试功能 (1:启用, 0:禁用)
#define FEATURE_WIFI_COMM       1                   // WiFi通信功能 (1:启用, 0:禁用)
#define FEATURE_TCP_CLIENT      1                   // TCP客户端功能 (1:启用, 0:禁用)
#define FEATURE_AT_COMMAND      1                   // AT命令支持 (1:启用, 0:禁用)

/* 辅助功能 */
#define FEATURE_LED_INDICATOR   1                   // LED指示灯 (1:启用, 0:禁用)
#define FEATURE_DEBUG_OUTPUT    1                   // 调试输出 (1:启用, 0:禁用)
#define FEATURE_ERROR_HANDLING  1                   // 错误处理 (1:启用, 0:禁用)

/* ============================================================================
 * 编译配置
 * ============================================================================ */

/* 优化选项 */
#define OPTIMIZE_SIZE           1                   // 优化代码大小 (1:启用, 0:禁用)
#define OPTIMIZE_SPEED          0                   // 优化执行速度 (1:启用, 0:禁用)

/* 调试选项 */
#define DEBUG_SYMBOLS           1                   // 包含调试符号 (1:启用, 0:禁用)
#define ASSERT_ENABLE           1                   // 启用断言 (1:启用, 0:禁用)

/* ============================================================================
 * 版本信息
 * ============================================================================ */

#define PROJECT_VERSION_MAJOR   1                   // 主版本号
#define PROJECT_VERSION_MINOR   0                   // 次版本号
#define PROJECT_VERSION_PATCH   0                   // 修订版本号
#define PROJECT_VERSION_STRING  "1.0.0"             // 版本字符串
#define PROJECT_BUILD_DATE      __DATE__            // 编译日期
#define PROJECT_BUILD_TIME      __TIME__            // 编译时间

/* ============================================================================
 * 宏定义
 * ============================================================================ */

/* 条件编译宏 */
#if FEATURE_UART_TEST
    #define UART_TEST_ENABLED
#endif

#if FEATURE_WIFI_COMM
    #define WIFI_COMM_ENABLED
#endif

#if FEATURE_TCP_CLIENT
    #define TCP_CLIENT_ENABLED
#endif

#if FEATURE_LED_INDICATOR
    #define LED_INDICATOR_ENABLED
#endif

#if DEBUG_ENABLE
    #define DEBUG_MODE_ENABLED
#endif

/* 实用宏 */
#define ARRAY_SIZE(x)          (sizeof(x) / sizeof((x)[0]))
#define MIN(a, b)              ((a) < (b) ? (a) : (b))
#define MAX(a, b)              ((a) > (b) ? (a) : (b))
#define ABS(x)                 ((x) < 0 ? -(x) : (x))

#ifdef __cplusplus
}
#endif

#endif /* __PROJECT_CONFIG_H */










