'use strict';

var http = require('http');
var express = require('express');
var router = require('./expressapp/router.js');
var app, server;
const HTTP_SERVER_PORT = 3000

app = express();
server = http.createServer(app);
router.configRouter(app, server);

server.listen(HTTP_SERVER_PORT);
console.log('Express server listening on port %d', HTTP_SERVER_PORT);
