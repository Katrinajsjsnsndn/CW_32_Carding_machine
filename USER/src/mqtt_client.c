/******************************************************************************
 * @file    mqtt_client.c
 * @brief   MQTT over WiFi module via AT commands
 * @author  CW32
 * @date    2025
 ******************************************************************************/

#include "wifi_config.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

static uint8_t mqtt_send_and_wait_ok(const char* command, uint32_t timeout_ms)
{
	return wifi_send_at_command_wait_response(command, RESP_OK, timeout_ms);
}

uint8_t mqtt_configure(void)
{
	char command[200];
	uint8_t ok = 1;

	// 服务器地址
	sprintf(command, "%s,%s", AT_CMD_MQTT_CFG_HOST, MQTT_BROKER_HOST);
	ok &= mqtt_send_and_wait_ok(command, 5000);

	// 端口
	sprintf(command, "%s,%s", AT_CMD_MQTT_CFG_PORT, MQTT_BROKER_PORT);
	ok &= mqtt_send_and_wait_ok(command, 3000);

	// TLS开关
	sprintf(command, "%s,%d", AT_CMD_MQTT_CFG_SSL, MQTT_SSL_ENABLE ? 1 : 0);
	ok &= mqtt_send_and_wait_ok(command, 3000);

	// Client ID
	sprintf(command, "%s,%s", AT_CMD_MQTT_CFG_CID, MQTT_CLIENT_ID);
	ok &= mqtt_send_and_wait_ok(command, 3000);

	// Username
	sprintf(command, "%s,%s", AT_CMD_MQTT_CFG_USER, MQTT_USERNAME);
	ok &= mqtt_send_and_wait_ok(command, 3000);

	// Password
	sprintf(command, "%s,%s", AT_CMD_MQTT_CFG_PASS, MQTT_PASSWORD);
	ok &= mqtt_send_and_wait_ok(command, 3000);

	return ok;
}

uint8_t mqtt_connect(void)
{
	return mqtt_send_and_wait_ok(AT_CMD_MQTT_CONNECT, 100000);
}

uint8_t mqtt_publish(const char* topic, const char* payload, uint8_t qos, uint8_t retain)
{
	char command[256];
	// 大多数模组支持：AT+MQTTPUB="topic","payload",qos,retain
	sprintf(command, "%s=\"%s\",%d,%d,\"%s\"", AT_CMD_MQTT_PUBLISH, topic, qos, retain, payload);
	return mqtt_send_and_wait_ok(command, 5000);
}

void mqtt_disconnect(void)
{
	// 某些模组使用 AT+MQTT=0 断开；若不支持，可留空或根据实际指令适配
	mqtt_send_and_wait_ok("AT+MQTT=0", 5000);
}



