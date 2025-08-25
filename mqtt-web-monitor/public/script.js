class MQTTWebDisplay {
    constructor() {
        this.ws = null;
        this.messageCount = 0;
        this.catEventCount = 0;
        this.messages = [];
        this.autoScroll = true;
        this.topicFilter = '';
        this.maxMessages = 100;
        this.showOnlyCatEvents = true;
        this.isConnected = false;
        
        this.init();
    }
    
    init() {
        this.connectWebSocket();
        this.setupEventListeners();
        this.updateDisplay();
    }
    
    connectWebSocket() {
        const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
        const wsUrl = `${protocol}//${window.location.host}`;
        
        console.log('连接WebSocket:', wsUrl);
        
        this.ws = new WebSocket(wsUrl);
        
        this.ws.onopen = () => {
            console.log('WebSocket连接成功');
            this.updateConnectionStatus('已连接', 'success');
        };
        
        this.ws.onmessage = (event) => {
            try {
                const data = JSON.parse(event.data);
                this.handleWebSocketMessage(data);
            } catch (error) {
                console.error('解析WebSocket消息失败:', error);
            }
        };
        
        this.ws.onclose = () => {
            console.log('WebSocket连接关闭');
            this.updateConnectionStatus('连接断开', 'danger');
            this.isConnected = false;
            
            // 自动重连
            setTimeout(() => {
                this.connectWebSocket();
            }, 3000);
        };
        
        this.ws.onerror = (error) => {
            console.error('WebSocket错误:', error);
            this.updateConnectionStatus('连接错误', 'danger');
        };
    }
    
    handleWebSocketMessage(data) {
        switch (data.type) {
            case 'connection':
                // WebSocket connected: set UI to connected
                this.updateMQTTStatus({ status: 'connected', message: 'websocket connected' });
                break;
                
            case 'mqtt_status':
                this.updateMQTTStatus(data);
                break;
                
            case 'mqtt_message':
                this.addMessage(data);
                break;
                
            case 'mqtt_error':
                console.error('MQTT错误:', data.error);
                this.showNotification('MQTT错误: ' + data.error, 'danger');
                break;
                
            default:
                console.log('未知消息类型:', data);
        }
    }
    
    updateMQTTStatus(data) {
        const statusElement = document.getElementById('mqtt-status');
        const detailElement = document.getElementById('mqtt-status-detail');
        const serverElement = document.getElementById('mqtt-server');
        
        // 更新状态显示
        statusElement.textContent = data.status === 'connected' ? '已连接' : 
                                  data.status === 'connecting' ? '连接中' :
                                  data.status === 'reconnecting' ? '重连中' : '未连接';
        
        statusElement.className = `badge ${this.getStatusClass(data.status)}`;
        
        if (data.message) detailElement.textContent = data.message;
        
        if (data.server) {
            serverElement.textContent = data.server;
        }
        
        // 更新主题列表
        if (data.topics) {
            this.updateSubscribedTopics(data.topics);
        }
        
        this.isConnected = data.status === 'connected';
    }
    
    getStatusClass(status) {
        switch (status) {
            case 'connected': return 'bg-success';
            case 'connecting': return 'bg-warning text-dark';
            case 'reconnecting': return 'bg-warning text-dark';
            case 'disconnected': return 'bg-danger';
            default: return 'bg-secondary';
        }
    }
    
    updateSubscribedTopics(topics) {
        const container = document.getElementById('subscribed-topics');
        container.innerHTML = topics.map(topic => 
            `<div class="badge bg-light text-dark me-1 mb-1">${topic}</div>`
        ).join('');
    }
    
    addMessage(data) {
        this.messageCount++;
        
        const message = {
            id: Date.now() + Math.random(),
            topic: data.topic,
            content: data.message,
            raw: data.raw_message,
            isCatEvent: data.is_cat_event || false,
            timestamp: data.timestamp,
            displayTime: new Date(data.timestamp).toLocaleString('zh-CN')
        };
        
        if (message.isCatEvent) {
            this.catEventCount++;
        }
        
        this.messages.unshift(message);
        
        // 限制消息数量
        if (this.messages.length > this.maxMessages) {
            this.messages = this.messages.slice(0, this.maxMessages);
        }
        
        this.updateDisplay();
        this.updateCounters();
    }
    
    updateDisplay() {
        const container = document.getElementById('messages-container');
        
        if (this.messages.length === 0) {
            container.innerHTML = `
                <div class="text-center text-muted py-5">
                    <i class="bi bi-hourglass-split fs-1"></i>
                    <p class="mt-2">等待MQTT数据...</p>
                </div>
            `;
            return;
        }
        
        // 应用过滤
        let filteredMessages = this.messages;
        
        // 主题过滤
        if (this.topicFilter) {
            filteredMessages = filteredMessages.filter(msg => 
                msg.topic.toLowerCase().includes(this.topicFilter.toLowerCase())
            );
        }
        
        // 仅显示猫咪事件
        if (this.showOnlyCatEvents) {
            filteredMessages = filteredMessages.filter(msg => msg.isCatEvent);
        }
        
        if (filteredMessages.length === 0) {
            container.innerHTML = `
                <div class="text-center text-muted py-5">
                    <i class="bi bi-search fs-1"></i>
                    <p class="mt-2">没有匹配的数据</p>
                </div>
            `;
            return;
        }
        
        const messagesHtml = filteredMessages.map(msg => this.createMessageHtml(msg)).join('');
        container.innerHTML = messagesHtml;
        
        // 自动滚动到底部
        if (this.autoScroll) {
            container.scrollTop = 0;
        }
    }
    
    createMessageHtml(message) {
        const contentStr = typeof message.content === 'object' ? 
            JSON.stringify(message.content, null, 2) : 
            String(message.content);
        
        const messageClass = message.isCatEvent ? 'message-item cat-event-item' : 'message-item';
        
        let detailsHtml = '';
        if (message.isCatEvent && message.content.entry_time) {
            detailsHtml = this.createCatEventDetailsHtml(message.content);
        }
        
        return `
            <div class="${messageClass}" data-message-id="${message.id}">
                <div class="message-header">
                    <span class="message-topic">${this.escapeHtml(message.topic)}</span>
                    <span class="message-time">${this.escapeHtml(message.displayTime)}</span>
                </div>
                <div class="message-content">${this.escapeHtml(contentStr)}</div>
                ${detailsHtml}
                <div class="message-raw">原始数据: ${this.escapeHtml(message.raw)}</div>
            </div>
        `;
    }
    
    createCatEventDetailsHtml(catData) {
        if (!catData.entry_time || !catData.exit_time) {
            return '';
        }
        
        return `
            <div class="cat-event-details">
                <h6><i class="bi bi-heart-fill text-danger"></i> 猫咪事件详情</h6>
                <div class="detail-row">
                    <span class="detail-label">进入时间:</span>
                    <span class="detail-value">${this.escapeHtml(catData.entry_time)}</span>
                </div>
                <div class="detail-row">
                    <span class="detail-label">离开时间:</span>
                    <span class="detail-value">${this.escapeHtml(catData.exit_time)}</span>
                </div>
                <div class="detail-row">
                    <span class="detail-label">使用时长:</span>
                    <span class="detail-value">${this.escapeHtml(String(catData.duration_minutes))} 分钟</span>
                </div>
                <div class="detail-row">
                    <span class="detail-label">日期:</span>
                    <span class="detail-value">${catData.entry_date ? this.escapeHtml(`${catData.entry_date.year}-${catData.entry_date.month}-${catData.entry_date.day}`) : '未知'}</span>
                </div>
            </div>
        `;
    }
    
    escapeHtml(text) {
        const div = document.createElement('div');
        div.textContent = text;
        return div.innerHTML;
    }
    
    updateCounters() {
        document.getElementById('message-count').textContent = this.messageCount;
        document.getElementById('cat-event-count').textContent = this.catEventCount;
    }
    
    updateConnectionStatus(status, type) {
        const statusElement = document.getElementById('client-count');
        statusElement.textContent = `客户端: 1`;
    }
    
    setupEventListeners() {
        // 过滤输入
        document.getElementById('topic-filter').addEventListener('input', (e) => {
            this.topicFilter = e.target.value;
            this.updateDisplay();
        });
        
        // 最大消息数选择
        document.getElementById('max-messages').addEventListener('change', (e) => {
            this.maxMessages = parseInt(e.target.value);
            this.updateDisplay();
        });
        
        // 仅显示猫咪事件复选框
        document.getElementById('show-only-cat-events').addEventListener('change', (e) => {
            this.showOnlyCatEvents = e.target.checked;
            this.updateDisplay();
        });
    }
    
    showNotification(message, type = 'info') {
        const notification = document.createElement('div');
        notification.className = `alert alert-${type} notification`;
        notification.innerHTML = `
            <button type="button" class="btn-close" onclick="this.parentElement.remove()"></button>
            ${message}
        `;
        
        document.body.appendChild(notification);
        
        // 5秒后自动移除
        setTimeout(() => {
            if (notification.parentElement) {
                notification.remove();
            }
        }, 5000);
    }
}

// 全局函数
function clearData() {
    if (confirm('确定要清除所有数据吗？')) {
        mqttDisplay.messages = [];
        mqttDisplay.messageCount = 0;
        mqttDisplay.catEventCount = 0;
        mqttDisplay.updateDisplay();
        mqttDisplay.updateCounters();
        mqttDisplay.showNotification('数据已清除', 'success');
    }
}

function applyFilter() {
    mqttDisplay.updateDisplay();
    mqttDisplay.showNotification('过滤条件已应用', 'info');
}

function toggleAutoScroll() {
    mqttDisplay.autoScroll = !mqttDisplay.autoScroll;
    const button = event.target;
    if (mqttDisplay.autoScroll) {
        button.innerHTML = '<i class="bi bi-arrow-down-circle"></i> 自动滚动';
        button.className = 'btn btn-sm btn-outline-warning';
    } else {
        button.innerHTML = '<i class="bi bi-arrow-down-circle-fill"></i> 自动滚动';
        button.className = 'btn btn-sm btn-warning';
    }
    mqttDisplay.showNotification(
        mqttDisplay.autoScroll ? '已启用自动滚动' : '已禁用自动滚动', 
        'info'
    );
}

function exportData() {
    const data = {
        export_time: new Date().toISOString(),
        total_messages: mqttDisplay.messageCount,
        cat_events: mqttDisplay.catEventCount,
        messages: mqttDisplay.messages
    };
    
    const blob = new Blob([JSON.stringify(data, null, 2)], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `mqtt_data_${new Date().toISOString().slice(0, 19).replace(/:/g, '-')}.json`;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
    
    mqttDisplay.showNotification('数据导出成功', 'success');
}

// 页面加载完成后初始化
document.addEventListener('DOMContentLoaded', () => {
    mqttDisplay = new MQTTWebDisplay();
    
    // 每30秒自动刷新状态
    setInterval(() => {
        if (mqttDisplay.isConnected) {
            fetch('/api/status')
                .then(response => response.json())
                .then(data => {
                    console.log('服务器状态:', data);
                })
                .catch(error => {
                    console.error('获取状态失败:', error);
                });
        }
    }, 30000);
});
