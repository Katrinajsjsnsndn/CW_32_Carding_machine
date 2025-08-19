/******************************************************************************
 * @file    project_config.h
 * @brief   ��Ŀ�����ļ�
 * @author  CW32
 * @date    2024
 ******************************************************************************/

#ifndef __PROJECT_CONFIG_H
#define __PROJECT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * ϵͳ����
 * ============================================================================ */

/* ϵͳʱ������ */
#define SYSTEM_CLOCK_FREQ       48000000    // ϵͳʱ��Ƶ�� (Hz)
#define UART_CLOCK_FREQ         48000000    // UARTʱ��Ƶ�� (Hz)

/* �������� */
#define DEBUG_ENABLE             1           // ���õ��Թ��� (1:����, 0:����)
#define DEBUG_UART               CW_UART3   // ����UART�˿�

/* ============================================================================
 * UART3����
 * ============================================================================ */

/* �������� */
#define UART3_BAUDRATE          115200      // ������
#define UART3_BUFFER_SIZE       256         // ���ջ�������С
#define UART3_TIMEOUT           1000        // ��ʱʱ�� (ms)

/* ͨ�Ų��� */
#define UART3_DATA_BITS         8           // ����λ
#define UART3_STOP_BITS         1           // ֹͣλ
#define UART3_PARITY            NONE        // У��λ (NONE, EVEN, ODD)
#define UART3_FLOW_CONTROL      NONE        // ������ (NONE, RTS, CTS, RTS_CTS)

/* �ж����� */
#define UART3_RX_IRQ_ENABLE     1           // ���ý����ж� (1:����, 0:����)
#define UART3_TX_IRQ_ENABLE     0           // ���÷����ж� (1:����, 0:����)
#define UART3_IRQ_PRIORITY      0           // �ж����ȼ� (0-3, 0Ϊ���)

/* ============================================================================
 * WiFi����
 * ============================================================================ */

/* �������� */
#define WIFI_SSID               "YourWiFiSSID"      // WiFi��������
#define WIFI_PASSWORD           "YourWiFiPassword"  // WiFi����
#define WIFI_SECURITY           WPA2                // ��ȫ���� (WPA, WPA2, WEP, OPEN)

/* ����ģʽ */
#define WIFI_MODE               STATION             // ����ģʽ (STATION, AP, AP_STATION)
#define WIFI_AUTO_CONNECT       1                   // �Զ����� (1:����, 0:����)

/* TCP���� */
#define TCP_SERVER_IP           "192.168.1.100"     // TCP������IP��ַ
#define TCP_SERVER_PORT         "8080"              // TCP�������˿�
#define TCP_CONNECTION_TYPE     TCP                 // �������� (TCP, UDP)
#define TCP_AUTO_RECONNECT      1                   // �Զ����� (1:����, 0:����)

/* ��ʱ���� */
#define WIFI_INIT_TIMEOUT       5000                // WiFi��ʼ����ʱ (ms)
#define WIFI_CONN_TIMEOUT       10000               // WiFi���ӳ�ʱ (ms)
#define TCP_CONN_TIMEOUT        5000                // TCP���ӳ�ʱ (ms)
#define DATA_SEND_TIMEOUT       3000                // ���ݷ��ͳ�ʱ (ms)

/* ============================================================================
 * GPIO����
 * ============================================================================ */

/* UART3�������� */
#define UART3_TX_PIN           GPIO_PIN_1          // TX���� (PA01)
#define UART3_RX_PIN           GPIO_PIN_0          // RX���� (PA00)
#define UART3_TX_PORT          CW_GPIOA            // TX�˿�
#define UART3_RX_PORT          CW_GPIOA            // RX�˿�

/* ״ָ̬ʾLED */
#define STATUS_LED_PIN          GPIO_PIN_3          // ״̬LED����
#define STATUS_LED_PORT         CW_GPIOB            // ״̬LED�˿�
#define STATUS_LED_ON_LEVEL     1                   // LED������ƽ (0��1)

/* ============================================================================
 * ���ܿ���
 * ============================================================================ */

/* ��Ҫ���� */
#define FEATURE_UART_TEST       1                   // UART���Թ��� (1:����, 0:����)
#define FEATURE_WIFI_COMM       1                   // WiFiͨ�Ź��� (1:����, 0:����)
#define FEATURE_TCP_CLIENT      1                   // TCP�ͻ��˹��� (1:����, 0:����)
#define FEATURE_AT_COMMAND      1                   // AT����֧�� (1:����, 0:����)

/* �������� */
#define FEATURE_LED_INDICATOR   1                   // LEDָʾ�� (1:����, 0:����)
#define FEATURE_DEBUG_OUTPUT    1                   // ������� (1:����, 0:����)
#define FEATURE_ERROR_HANDLING  1                   // ������ (1:����, 0:����)

/* ============================================================================
 * ��������
 * ============================================================================ */

/* �Ż�ѡ�� */
#define OPTIMIZE_SIZE           1                   // �Ż������С (1:����, 0:����)
#define OPTIMIZE_SPEED          0                   // �Ż�ִ���ٶ� (1:����, 0:����)

/* ����ѡ�� */
#define DEBUG_SYMBOLS           1                   // �������Է��� (1:����, 0:����)
#define ASSERT_ENABLE           1                   // ���ö��� (1:����, 0:����)

/* ============================================================================
 * �汾��Ϣ
 * ============================================================================ */

#define PROJECT_VERSION_MAJOR   1                   // ���汾��
#define PROJECT_VERSION_MINOR   0                   // �ΰ汾��
#define PROJECT_VERSION_PATCH   0                   // �޶��汾��
#define PROJECT_VERSION_STRING  "1.0.0"             // �汾�ַ���
#define PROJECT_BUILD_DATE      __DATE__            // ��������
#define PROJECT_BUILD_TIME      __TIME__            // ����ʱ��

/* ============================================================================
 * �궨��
 * ============================================================================ */

/* ��������� */
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

/* ʵ�ú� */
#define ARRAY_SIZE(x)          (sizeof(x) / sizeof((x)[0]))
#define MIN(a, b)              ((a) < (b) ? (a) : (b))
#define MAX(a, b)              ((a) > (b) ? (a) : (b))
#define ABS(x)                 ((x) < 0 ? -(x) : (x))

#ifdef __cplusplus
}
#endif

#endif /* __PROJECT_CONFIG_H */










