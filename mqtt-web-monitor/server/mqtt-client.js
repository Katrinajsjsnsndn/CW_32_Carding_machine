const mqtt = require('mqtt');
const { broadcastToClients } = require('./app');
const fs = require('fs');
const path = require('path');

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

function buildMqttOptions(port, useTLS) {
	return {
		host: config.MQTT_HOST || 'localhost',
		port: port,
		username: config.MQTT_USERNAME || '',
		password: config.MQTT_PASSWORD || '',
		clientId: config.MQTT_CLIENT_ID || `web_client_${Math.random().toString(16).substr(2, 8)}`,
		clean: true,
		reconnectPeriod: 5000,
		connectTimeout: 30000,
		rejectUnauthorized: false,
		keepalive: 60,
		protocolVersion: 4,
		protocol: useTLS ? 'mqtts' : 'mqtt'
	};
}

function shouldUseTLS(port) {
	if (String(config.MQTT_SSL || '').trim() === '1') return true;
	return Number(port) === 8883;
}

const topics = config.MQTT_TOPICS ? config.MQTT_TOPICS.split(',') : [ 'test/topic', 'cat/events' ];

function connectWith(port) {
	const useTLS = shouldUseTLS(port);
	const opts = buildMqttOptions(Number(port), useTLS);
	const url = `${useTLS ? 'mqtts' : 'mqtt'}://${opts.host}:${opts.port}`;

	console.log(`[MQTT] connecting: ${url}  clientId=${opts.clientId}`);
	const client = mqtt.connect(url, opts);

	client.on('connect', () => {
		console.log('[MQTT] connected');
		topics.forEach(topic => {
			client.subscribe(topic, (err) => {
				if (err) console.error(`[MQTT] subscribe failed: ${topic}`, err);
				else console.log(`[MQTT] subscribed: ${topic}`);
			});
		});
		broadcastToClients({ type: 'mqtt_status', status: 'connected', message: 'connected', topics, server: `${opts.host}:${opts.port}` });
	});

	client.on('message', (topic, message) => {
		try {
			const messageStr = message.toString();
			console.log(`[MQTT] msg topic=${topic} payload=${messageStr}`);
			let parsedData;
			let isCatEvent = false;
			if (topic.includes('/user/') && messageStr) {
				// attempt to parse cat event format first
				if (messageStr.includes('entry') && messageStr.includes('exit')) {
					parsedData = parseCatEventData(messageStr);
					isCatEvent = true;
				}
			}
			if (!parsedData) {
				try { parsedData = JSON.parse(messageStr); }
				catch { parsedData = { raw_data: messageStr, hex_data: message.toString('hex') }; }
			}
			broadcastToClients({ type: 'mqtt_message', topic, message: parsedData, raw_message: messageStr, is_cat_event: isCatEvent, timestamp: new Date().toISOString() });
		} catch (error) {
			console.error('[MQTT] handle message error:', error);
		}
	});

	client.on('error', (err) => {
		console.error('[MQTT] error:', err.message);
		broadcastToClients({ type: 'mqtt_status', status: 'disconnected', message: `error: ${err.message}` });
	});

	client.on('close', () => {
		console.log('[MQTT] closed');
		broadcastToClients({ type: 'mqtt_status', status: 'disconnected', message: 'closed' });
	});

	client.on('reconnect', () => {
		broadcastToClients({ type: 'mqtt_status', status: 'reconnecting', message: 'reconnecting...' });
	});

	return client;
}

function parseCatEventData(messageStr) {
	try {
		const entryMatch = messageStr.match(/entry:20(\d{2})-(\d{2})-(\d{2}) (\d{2}):(\d{2}):(\d{2})/);
		const exitMatch  = messageStr.match(/exit:20(\d{2})-(\d{2})-(\d{2}) (\d{2}):(\d{2}):(\d{2})/);
		if (entryMatch && exitMatch) {
			const entryTime = `20${entryMatch[1]}-${entryMatch[2]}-${entryMatch[3]} ${entryMatch[4]}:${entryMatch[5]}:${entryMatch[6]}`;
			const exitTime  = `20${exitMatch[1]}-${exitMatch[2]}-${exitMatch[3]} ${exitMatch[4]}:${exitMatch[5]}:${exitMatch[6]}`;
			const durationMinutes = Math.round((new Date(exitTime) - new Date(entryTime)) / 60000);
			return { entry_time: entryTime, exit_time: exitTime, duration_minutes: durationMinutes };
		}
	} catch (e) {
		console.error('parse cat event failed:', e);
	}
	return { raw_data: messageStr, parse_error: 'cannot parse cat event format' };
}

const initialPort = Number(config.MQTT_PORT) || 1883;
connectWith(initialPort);
