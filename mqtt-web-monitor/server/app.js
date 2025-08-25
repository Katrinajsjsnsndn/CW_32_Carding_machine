const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const cors = require('cors');
const path = require('path');
const fs = require('fs');

// load config
const config = {};
const envFile = path.join(__dirname, '../config.env');
if (fs.existsSync(envFile)) {
	const envContent = fs.readFileSync(envFile, 'utf8');
	envContent.split('\n').forEach(line => {
		const [key, value] = line.split('=');
		if (key && value && !key.startsWith('#')) {
			config[key.trim()] = value.trim();
		}
	});
}

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

// middleware
app.use(cors());
app.use(express.json({ limit: '1mb' }));

// static with utf-8 headers
app.use(express.static(path.join(__dirname, '../public'), {
	setHeaders: (res, filePath) => {
		if (filePath.endsWith('.html')) {
			res.setHeader('Content-Type', 'text/html; charset=utf-8');
		} else if (filePath.endsWith('.css')) {
			res.setHeader('Content-Type', 'text/css; charset=utf-8');
		} else if (filePath.endsWith('.js')) {
			res.setHeader('Content-Type', 'application/javascript; charset=utf-8');
		}
	}
}));

// connected websocket clients
const clients = new Set();
// store last events by type, e.g., { mqtt_status: {...} }
const lastEventsByType = {};

wss.on('connection', (ws) => {
	clients.add(ws);
	console.log('Web client connected. total clients:', clients.size);
	ws.send(JSON.stringify({ type: 'connection', message: 'connected', timestamp: new Date().toISOString() }));
	// push last mqtt status if available so UI shows correct state immediately
	if (lastEventsByType['mqtt_status']) {
		try { ws.send(JSON.stringify(lastEventsByType['mqtt_status'])); } catch {}
	}
	ws.on('close', () => {
		clients.delete(ws);
		console.log('Web client disconnected. total clients:', clients.size);
	});
	ws.on('error', (error) => {
		console.error('WebSocket error:', error);
		clients.delete(ws);
	});
});

function broadcastToClients(event) {
	try {
		if (event && typeof event === 'object') {
			if (!event.timestamp) event.timestamp = new Date().toISOString();
			if (event.type) lastEventsByType[event.type] = event;
		}
		const message = JSON.stringify(event);
		clients.forEach(client => {
			if (client.readyState === WebSocket.OPEN) {
				client.send(message);
			}
		});
	} catch (err) {
		console.error('broadcast error:', err);
	}
}

module.exports = { broadcastToClients };

// api
app.get('/api/status', (req, res) => {
	res.setHeader('Content-Type', 'application/json; charset=utf-8');
	res.json({
		status: 'running',
		connected_clients: clients.size,
		mqtt_config: {
			host: config.MQTT_HOST,
			port: config.MQTT_PORT,
			topics: config.MQTT_TOPICS ? config.MQTT_TOPICS.split(',') : []
		},
		timestamp: new Date().toISOString()
	});
});

// receive IoTDA HTTP forwarding
app.post('/api/iotda', (req, res) => {
	try {
		const payload = req.body;
		console.log('[IOTDA-HTTP] received:', JSON.stringify(payload));
		const event = {
			type: 'mqtt_message',
			topic: payload && payload.topic ? String(payload.topic) : 'iotda/http',
			message: payload,
			raw_message: JSON.stringify(payload)
		};
		broadcastToClients(event);
		console.log('[IOTDA-HTTP] broadcast to clients:', clients.size);
		res.json({ ok: true });
	} catch (e) {
		console.error('[IOTDA-HTTP] error:', e);
		res.status(500).json({ ok: false });
	}
});

app.get('/api/clear-data', (req, res) => {
	res.setHeader('Content-Type', 'application/json; charset=utf-8');
	res.json({ message: 'cleared' });
});

// start
const PORT = config.PORT || 3000;
server.listen(PORT, () => {
	console.log(`Server listening on port ${PORT}`);
	console.log(`Open: http://localhost:${PORT}`);
	console.log(`MQTT target: ${config.MQTT_HOST}:${config.MQTT_PORT}`);
	console.log(`Subscribe: ${config.MQTT_TOPICS}`);
});

require('./mqtt-client');
