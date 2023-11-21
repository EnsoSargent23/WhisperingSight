// First, install the required modules with the command: npm install express

// Import the Express module
const express = require('express');
const cors = require('cors');
const http = require('http');
const WebSocket = require('ws');

// Create an instance of Express
const app = express();
app.use(cors());

// Create an HTTP server
const server = http.createServer((req, res) => {
  res.writeHead(200, {'Content-Type': 'text/plain'});
  res.end('WebSocket Server is running');
});

// Create a WebSocket server bound to the HTTP server
const wss = new WebSocket.Server({ server });

// Event handler for incoming WebSocket connections
wss.on('connection', (ws) => {
  console.log('Client connected');

  // Event handler for incoming messages from the client
  ws.on('message', (message) => {
    // Send the received message to all connected clients
    wss.clients.forEach((client) => {
      if (client !== ws && client.readyState === WebSocket.OPEN) {
        client.send(message);
      }
    });
  });

  // Event handler for closing the WebSocket connection
  ws.on('close', () => {
    console.log('Client disconnected');
  });
});

// Define an endpoint for GET requests with the 'zeit' parameter
app.get('/result/:cardname', (req, res) => {
  // Read the 'zeit' parameter from the query
  let zeit = req.params.cardname;

  let result = `This card is the ${cardname}`;

  // Send the result as a response
  console.log(zeit);
  console.log(result);

  // Send data to all WebSocket clients
  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(result);
    }
  });

  res.send(result);
});

// Start the server on port 3000
const port = 3000;
app.listen(port, () => {
  console.log(`Server is running on http://localhost:${port}`);
});

// Start the server on port 3001
server.listen(3001, () => {
  console.log('Server listening on http://localhost:3001');
});
