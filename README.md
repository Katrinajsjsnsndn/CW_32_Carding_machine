# CW32L011 + AI_WB2 WiFi通信项目

## 项目概述

本项目实现了使用CW32L011单片机通过UART3与AI_WB2 WiFi模块进行通信的功能。使用PA00和PA01引脚分别作为UART3的RXD和TXD引脚。

## 硬件连接

### 引脚连接
- **PA00** → AI_WB2的TXD引脚（UART3_RXD）
- **PA01** → AI_WB2的RXD引脚（UART3_TXD）
- **GND** → AI_WB2的GND引脚
- **3.3V** → AI_WB2的VCC引脚（如果AI_WB2需要外部供电）

### 连接示意图
```
CW32L011          AI_WB2
   PA00    ←→    TXD
   PA01    ←→    RXD
   GND     ←→    GND
   3.3V    ←→    VCC (可选)
```

## 软件配置

### 1. WiFi网络配置
在 `USER/inc/wifi_config.h` 文件中修改以下配置：

```c
/* WiFi网络配置 */
#define WIFI_SSID           "YourWiFiSSID"        // 修改为您的WiFi网络名称
#define WIFI_PASSWORD       "YourWiFiPassword"    // 修改为您的WiFi密码

/* TCP服务器配置 */
#define TCP_SERVER_IP       "192.168.1.100"       // 修改为您的TCP服务器IP地址
#define TCP_SERVER_PORT     "8080"                // 修改为您的TCP服务器端口
```

### 2. UART通信参数
```c
/* UART通信配置 */
#define UART3_BAUDRATE      115200                // UART3波特率
#define UART3_BUFFER_SIZE   256                   // 接收缓冲区大小
```

## 功能特性

### 1. 基本WiFi功能
- WiFi网络连接/断开
- 获取IP地址
- 设置WiFi工作模式（Station/AP/AP+Station）

### 2. TCP通信功能
- 建立TCP客户端连接
- 发送/接收TCP数据
- 支持单连接和多连接模式

### 3. AT命令支持
- 完整的ESP8266 AT命令集支持
- 自动响应处理
- 超时机制

## 使用方法

### 1. 编译和烧录
1. 使用Keil uVision5打开项目文件 `Carding_machine.uvprojx`
2. 根据您的硬件配置修改 `wifi_config.h` 文件
3. 编译项目
4. 将生成的hex文件烧录到CW32L011单片机

### 2. 运行流程
程序启动后会自动执行以下步骤：
1. 初始化系统时钟和GPIO
2. 配置UART3通信参数
3. 初始化WiFi模块
4. 连接到指定的WiFi网络
5. 获取IP地址
6. 建立TCP连接到指定服务器
7. 发送测试数据
8. 进入主循环，处理接收到的数据

### 3. 自定义功能
您可以在主循环中添加自己的数据处理逻辑：

```c
while(1)
{
    // 检查是否有接收到的数据
    if(uart3_rx_flag)
    {
        uart3_rx_flag = 0;
        
        // 在这里添加您的数据处理逻辑
        uint8_t data_buffer[256];
        uint16_t len = uart3_receive_data(data_buffer, 256);
        
        if(len > 0)
        {
            // 处理接收到的数据
            // 例如：解析AT命令响应、处理TCP数据等
        }
    }
    
    // 主循环延时
    delay_ms(100);
}
```

## 常用AT命令

### WiFi相关命令
- `AT` - 测试AT命令
- `AT+CWMODE=1` - 设置为Station模式
- `AT+CWJAP="SSID","PASSWORD"` - 连接WiFi网络
- `AT+CWQAP` - 断开WiFi网络
- `AT+CIFSR` - 获取IP地址

### TCP相关命令
- `AT+CIPMUX=0` - 设置单连接模式
- `AT+CIPSTART="TCP","IP","PORT"` - 建立TCP连接
- `AT+CIPSEND=LENGTH` - 发送数据
- `AT+CIPCLOSE` - 关闭连接
- `AT+CIPSTATUS` - 获取连接状态

## 注意事项

### 1. 硬件注意事项
- 确保PA00和PA01引脚正确连接到AI_WB2模块
- 检查电源电压是否稳定（3.3V）
- 确保地线连接正确

### 2. 软件注意事项
- 修改WiFi配置信息后需要重新编译和烧录
- 确保WiFi网络名称和密码正确
- TCP服务器需要能够接受来自WiFi模块的连接

### 3. 调试建议
- 使用串口调试工具监控UART3通信
- 检查WiFi模块的LED指示灯状态
- 使用AT命令手动测试模块功能

## 故障排除

### 1. 无法连接WiFi
- 检查SSID和密码是否正确
- 确认WiFi信号强度
- 检查WiFi模块是否正常工作

### 2. 无法建立TCP连接
- 检查服务器IP地址和端口是否正确
- 确认服务器是否正在运行
- 检查防火墙设置

### 3. 通信不稳定
- 检查电源供应是否稳定
- 确认波特率设置是否匹配
- 检查硬件连接是否牢固

## 技术支持

如果您在使用过程中遇到问题，请：
1. 检查硬件连接
2. 验证配置参数
3. 查看串口输出信息
4. 参考ESP8266 AT命令手册

## 版本信息

- 版本：1.0
- 日期：2024
- 支持芯片：CW32L011
- 支持WiFi模块：AI_WB2 (ESP8266)





