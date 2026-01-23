import WebSocket, { WebSocketServer } from "ws";

const PORT = process.env.PORT || 9000;
const websocket_server = new WebSocketServer({ port: PORT });
const clients = new Set();

websocket_server.on('connection', ws => {
    clients.add(ws);
    console.log("Client connected")

    ws.on('message', msg => {
        for (const client of clients)
        {
            if (client !== ws && client.readyState === WebSocket.OPEN) client.send(msg);
        }
    });
    ws.on('close', () => {
        clients.delete(ws);
        console.log("Client disconnected");
    });
});

console.log(`BUN WEBSOCKET SERVER RUNNINGS ON ${PORT}`);