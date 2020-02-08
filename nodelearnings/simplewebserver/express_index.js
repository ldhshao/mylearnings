'use strict';

var http = require('http');
var express = require('express');
var app, server;
const HTTP_SERVER_PORT = 3000

app = express();
server = http.createServer(app);

//router
app.get('/', function(request, respond){
  respond.send('Hello, Express');
});

server.listen(HTTP_SERVER_PORT);
console.log('Express server listening on port %d', HTTP_SERVER_PORT);
