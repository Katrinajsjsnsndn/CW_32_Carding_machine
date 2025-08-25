# MQTT数据实时监控网页 - 猫咪梳毛器项目

这是一个基于Node.js的MQTT数据实时监控网页应用，专门用于监控猫咪梳毛器项目的数据。

## 功能特性

- ? **实时MQTT连接** - 连接到华为云IoTDA MQTT服务器
- ? **数据实时显示** - 实时显示接收到的MQTT消息
- ? **猫咪事件解析** - 自动解析猫咪进入/离开事件数据
- ? **智能过滤** - 支持主题过滤和数据类型过滤
- ? **响应式设计** - 支持桌面和移动设备
- ? **数据导出** - 支持导出接收到的数据
- ? **自动重连** - MQTT连接断开时自动重连

## 项目结构

```
mqtt-web-monitor/
├── public/                 # 前端文件
│   ├── index.html         # 主页面
│   ├── style.css          # 样式文件
│   └── script.js          # 功能脚本
├── server/                 # 后端文件
│   ├── app.js             # 主服务器
│   └── mqtt-client.js     # MQTT客户端
├── config.env              # 配置文件
├── package.json            # 项目配置
└── README.md               # 说明文档
```

## 安装和运行

### 1. 安装依赖

```bash
cd mqtt-web-monitor
npm install
```

### 2. 配置MQTT连接

编辑 `config.env` 文件，确保MQTT连接参数正确：

```env
MQTT_HOST=42f5d77ef1.st1.iotda-device.cn-north-4.myhuaweicloud.com
MQTT_PORT=1883
MQTT_USERNAME=689ed67e32771f177b567fea_2004
MQTT_PASSWORD=57f23e26c88a5a97c95e69bc530db70aa7162e7c9717bf0819f470859fd0cf22
MQTT_CLIENT_ID=689ed67e32771f177b567fea_2004_0_0_2025082106
```

### 3. 启动应用

```bash
# 开发模式（自动重启）
npm run dev

# 生产模式
npm start
```

### 4. 访问网页

打开浏览器访问：http://localhost:3000

## 使用方法

### 基本操作

1. **查看连接状态** - 左侧面板显示MQTT连接状态和订阅主题
2. **实时数据监控** - 右侧显示接收到的所有MQTT消息
3. **数据过滤** - 使用左侧过滤面板筛选数据
4. **数据导出** - 点击"导出数据"按钮下载JSON格式数据

### 猫咪事件数据

应用会自动识别和解析猫咪事件数据，包括：
- 进入时间
- 离开时间  
- 使用时长
- 日期信息

### 过滤功能

- **主题过滤** - 输入关键词过滤特定主题的消息
- **数据类型过滤** - 选择是否只显示猫咪事件
- **数量限制** - 设置显示的最大消息数量

## 技术架构

- **后端**: Node.js + Express + WebSocket
- **前端**: HTML5 + CSS3 + JavaScript (ES6+)
- **MQTT客户端**: mqtt.js
- **实时通信**: WebSocket
- **UI框架**: Bootstrap 5

## 配置说明

### MQTT配置

- `MQTT_HOST`: MQTT服务器地址
- `MQTT_PORT`: MQTT服务器端口
- `MQTT_USERNAME`: 用户名
- `MQTT_PASSWORD`: 密码
- `MQTT_CLIENT_ID`: 客户端ID

### 服务器配置

- `PORT`: 网页服务器端口（默认3000）
- `NODE_ENV`: 运行环境（development/production）

## 故障排除

### 常见问题

1. **MQTT连接失败**
   - 检查网络连接
   - 验证MQTT服务器地址和端口
   - 确认用户名和密码正确

2. **网页无法访问**
   - 确认服务器已启动
   - 检查端口是否被占用
   - 查看控制台错误信息

3. **数据不显示**
   - 检查MQTT连接状态
   - 确认设备正在发送数据
   - 查看浏览器控制台错误

### 日志查看

服务器启动后会在控制台显示：
- MQTT连接状态
- 订阅的主题
- 接收到的消息
- 错误信息

## 开发说明

### 添加新功能

1. 在 `server/` 目录添加后端功能
2. 在 `public/` 目录添加前端功能
3. 更新 `package.json` 添加新依赖

### 自定义样式

编辑 `public/style.css` 文件自定义界面样式

### 扩展MQTT处理

在 `server/mqtt-client.js` 中添加新的消息处理逻辑

## 许可证

MIT License

## 联系方式

如有问题或建议，请联系项目维护者。
