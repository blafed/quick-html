const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 8126 });

wss.on('connection', (ws) => {
    console.log('Client connected.');

    ws.on('message', (message) => {
        console.log('Received:', message);
        // Broadcast the message to all connected clients
        wss.clients.forEach((client) => {
            if (client !== ws && client.readyState === WebSocket.OPEN) {

                client.send(message.toString('utf8'));
            }
        });
    });

    ws.on('close', () => {
        console.log('Client disconnected.');
    });

    ws.on('error', (error) => {
        console.error('WebSocket error:', error);
    });
});

console.log('WebSocket server is running on ws://localhost:8126');
