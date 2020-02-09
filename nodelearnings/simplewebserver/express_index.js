'use strict';

var http = require('http');
var express = require('express');
var app, server;
const HTTP_SERVER_PORT = 3000

app = express();
server = http.createServer(app);

//router
app.get('/', function(request, response){
  response.send('Hello, Express');
});
app.all('/book/*?', function(request, response, next){
  response.contentType('json');
  next();
});
app.get('/book/list', function(request, response){
  response.send({title:'book list'});
});
app.post('/book/create', function(request, response){
  response.send({title:'book created'});
});
app.get('/book/read/:id([0-9]+)', function(request, response){
  response.send({title:'book with id ' + request.params.id + ' found'});
});
app.post('/book/update/:id([0-9]+)', function(request, response){
  response.send({title:'book with id ' + request.params.id + ' updated'});
});
app.get('/book/delete/:id([0-9]+)', function(request, response){
  response.send({title:'book with id ' + request.params.id + ' deleted'});
});

server.listen(HTTP_SERVER_PORT);
console.log('Express server listening on port %d', HTTP_SERVER_PORT);
