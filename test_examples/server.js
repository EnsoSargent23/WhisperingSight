const http = require('http');
const WebSocket = require('ws');

// Erstelle einen HTTP-Server
const server = http.createServer((req, res) => {
  res.writeHead(200, {'Content-Type': 'text/plain'});
  res.end('WebSocket Server is running');
});

// Erstelle einen WebSocket-Server, der an den HTTP-Server gebunden ist
const wss = new WebSocket.Server({ server });

// Event-Handler für eingehende WebSocket-Verbindungen
wss.on('connection', (ws) => {
  console.log('Client connected');

  // Event-Handler für eingehende Nachrichten vom Client
  ws.on('message', (message) => {
    client.send(`${message}`);

    // Sende die empfangene Nachricht an alle verbundenen Clients
    wss.clients.forEach((client) => {
      if (client !== ws && client.readyState === WebSocket.OPEN) {
        client.send(message);
      }
    });
  });

  // Event-Handler für das Schließen der WebSocket-Verbindung
  ws.on('close', () => {
    console.log('Client disconnected');
  });
});

// Starte den Server auf Port 3000
server.listen(3000, () => {
  console.log('Server listening on http://localhost:3000');
});
