/******************************************************************************
 * @file    uart_test.h
 * @brief   UART3通信测试头文件
 * @author  CW32
 * @date    2024
 ******************************************************************************/

#ifndef __UART_TEST_H
#define __UART_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

/* 测试命令定义 */
#define TEST_CMD_ECHO        "ECHO"              // 回显测试
#define TEST_CMD_LOOPBACK    "LOOPBACK"          // 回环测试
#define TEST_CMD_STATUS      "STATUS"            // 状态查询
#define TEST_CMD_VERSION     "VERSION"           // 版本查询
#define TEST_CMD_HELP        "HELP"              // 帮助信息

/* 测试响应定义 */
#define TEST_RESP_OK         "OK"                // 测试成功
#define TEST_RESP_ERROR      "ERROR"             // 测试失败
#define TEST_RESP_UNKNOWN    "UNKNOWN_CMD"       // 未知命令

/* 函数声明 */
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

