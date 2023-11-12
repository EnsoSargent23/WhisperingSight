// imports
const express = require('express');
const http = require('http');
const schedule = require('node-schedule');
const axios = require('axios');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const fs = require('fs');
const cors = require('cors');


// Start Express and define port
const app = express();
const serverPort = 3000;

// Set Cors Policy
app.use(cors());
app.use(express.json());
app.use(express.static('public'));

// Setting of Port of Arduino
const arduinoPort = new SerialPort({
  path: 'COM3',
  baudRate: 9600
});

// Server and Client information
const clientURL = 'http://localhost:80';

// fetching data from Serialport: Getting Card Information which is in the middle
function waitForData() {
  return new Promise((resolve) => {
    const parser = arduinoPort.pipe(new ReadlineParser({ delimiter: '\r\n' }));
    parser.once('data', (data) => {
      resolve(data);
    });
  });
}

// fetching data from Serialport: Getting Hand Information of the Player
function waitForData(playerid) {
  return "coming soon!"
}

// Get Request for Playerhand
app.get('/cardinmyhand/:playerid', async (req, res) => {
  try {
    const playerid = req.params.playerid;
    const data = await waitForData(playerid); // Warte auf Daten von der seriellen Schnittstelle
    res.send(data); // Sende die Daten an den Client
  } catch (error) {
    res.status(500).send('Fehler beim Lesen der Daten');
  }
});

// Get Request for Card in the middle
app.get('/cardinthemiddle', async (req, res) => {
  try {
    const data = await waitForData(); // Warte auf Daten von der seriellen Schnittstelle
    res.send(data); // Sende die Daten an den Client
  } catch (error) {
    res.status(500).send('Fehler beim Lesen der Daten');
  }
});




// // Shedule Format (* * * * *) (Sec(0-59), Min(0-59), Hours(0-23), Day (1-31), Month (0-11))
// const job = schedule.scheduleJob('*/15 * * * *', () => {
    

// axios.get(clientURL)
//   .then((response) => {
//   })
//   .catch((error) => {
//   });
//   });

// Port Definition
app.listen(serverPort, () => {
  console.log(`Server läuft auf http://localhost:${serverPort}`);
});
