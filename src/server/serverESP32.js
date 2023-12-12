// Installiere zuerst die erforderlichen Module mit dem Befehl: npm install express

// Importiere das Express-Modul
const express = require('express');
const cors = require('cors');
const http = require('http');
const WebSocket = require('ws');



// Erstelle eine Instanz von Express
const app = express();
app.use(cors());

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



// Definiere einen Endpunkt für GET-Requests mit dem Parameter 'zeit'
app.get('/ergebnis/:karte', (req, res) => {
  // Lies den Zeitparameter aus dem path
  let karte = req.params.karte;

  let ergebnis = `${karte}`;

    // Sende Daten an alle WebSocket-Clients
    wss.clients.forEach((client) => {
      if (client.readyState === WebSocket.OPEN) {
          client.send(ergebnis);
      }
  });

  res.send(ergebnis);
});



// Starte den Server auf dem Port 3000
const port = 3000;
app.listen(port, () => {
  console.log(`Server läuft auf http://localhost:${port}`);
});

// Starte den Server auf Port 3000
server.listen(3001, () => {
  console.log('Server listening on http://localhost:3001');
});
