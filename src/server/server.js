// imports
const express = require('express');
const http = require('http');
const schedule = require('node-schedule');
const axios = require('axios');
const app = express();

// Shedule Format (* * * * *) (Sec(0-59), Min(0-59), Hours(0-23), Day (1-31), Month (0-11))


const job = schedule.scheduleJob('*/15 * * * *', () => {
    const clientURL = 'http://localhost:80';

axios.get(clientURL)
  .then((response) => {
  })
  .catch((error) => {
  });
  });

// Port Definition
app.listen(3000, () => {
  console.log('Backend-Server running on port 3000...');
});
