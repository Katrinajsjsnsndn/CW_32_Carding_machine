/******************************************************************************
 * @file    uart_test.h
 * @brief   UART3ͨ�Ų���ͷ�ļ�
 * @author  CW32
 * @date    2024
 ******************************************************************************/

#ifndef __UART_TEST_H
#define __UART_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

/* ��������� */
#define TEST_CMD_ECHO        "ECHO"              // ���Բ���
#define TEST_CMD_LOOPBACK    "LOOPBACK"          // �ػ�����
#define TEST_CMD_STATUS      "STATUS"            // ״̬��ѯ
#define TEST_CMD_VERSION     "VERSION"           // �汾��ѯ
#define TEST_CMD_HELP        "HELP"              // ������Ϣ

/* ������Ӧ���� */
#define TEST_RESP_OK         "OK"                // ���Գɹ�
#define TEST_RESP_ERROR      "ERROR"             // ����ʧ��
#define TEST_RESP_UNKNOWN    "UNKNOWN_CMD"       // δ֪����

/* �������� */
void uart_test_init(void);
void uart_test_process_command(const char* command);
void uart_test_send_response(const char* response);
void uart_test_echo_test(void);
void uart_test_loopback_test(void);
void uart_test_status_test(void);
void uart_test_version_test(void);
void uart_test_help_test(void);

#ifdef __cplusplus
}
#endif

#endif /* __UART_TEST_H */

