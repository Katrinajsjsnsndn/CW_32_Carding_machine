# CW32L011 + AI_WB2 快速开始指南

## ? 5分钟快速上手

### 第一步：硬件连接
1. 将AI_WB2模块连接到CW32L011开发板：
   - **PA00** ←→ **AI_WB2 TXD**
   - **PA01** ←→ **AI_WB2 RXD**
   - **GND** ←→ **AI_WB2 GND**
   - **3.3V** ←→ **AI_WB2 VCC** (如果需要)

### 第二步：软件配置
1. 打开 `USER/inc/wifi_config.h` 文件
2. 修改WiFi配置：
   ```c
   #define WIFI_SSID           "您的WiFi名称"
   #define WIFI_PASSWORD       "您的WiFi密码"
   ```
3. 修改TCP服务器配置：
   ```c
   #define TCP_SERVER_IP       "您的服务器IP"
   #define TCP_SERVER_PORT     "您的服务器端口"
   ```

### 第三步：编译和烧录
1. 使用Keil uVision5打开 `Carding_machine.uvprojx`
2. 点击编译按钮 (F7)
3. 将生成的hex文件烧录到CW32L011

### 第四步：测试通信
1. 上电后，系统会自动初始化
2. 通过串口工具连接UART3 (115200波特率)
3. 发送测试命令：
   - `HELP` - 查看可用命令
   - `ECHO` - 回显测试
   - `STATUS` - 状态查询

## ? 测试命令列表

| 命令 | 功能 | 示例 |
|------|------|------|
| `HELP` | 显示帮助信息 | `HELP` |
| `ECHO` | 回显测试 | `ECHO` |
| `STATUS` | 查询系统状态 | `STATUS` |
| `VERSION` | 查询版本信息 | `VERSION` |
| `LOOPBACK` | 回环测试 | `LOOPBACK` |

## ? 常见问题解决

### Q1: 无法接收数据
**A:** 检查波特率是否设置为115200，确保串口工具配置正确。

### Q2: 命令无响应
**A:** 确保发送的命令以回车符结尾，或者使用串口工具的"发送新行"功能。

### Q3: 系统启动异常
**A:** 检查硬件连接，确保PA00和PA01引脚连接正确。

## ? WiFi功能启用

要启用WiFi功能，请：

1. 在 `main.c` 中取消注释WiFi相关代码
2. 确保 `wifi_config.h` 中的配置正确
3. 重新编译和烧录

```c
// 取消注释以下代码块
/*
// 初始化WiFi模块
wifi_init();

// 连接到WiFi网络
wifi_connect_network(WIFI_SSID, WIFI_PASSWORD);
// ... 其他WiFi代码
*/
```

## ? 下一步

- 查看 `README.md` 了解详细功能
- 参考 `USER/inc/project_config.h` 进行高级配置
- 根据需求修改和扩展功能

## ? 技术支持

如遇问题，请检查：
1. 硬件连接
2. 软件配置
3. 串口输出信息
4. 项目文档

---

**祝您使用愉快！** ?





