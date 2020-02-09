'use strict';
var configRouter;

configRouter = function(app, server){
  app.get('/', function(request, response){
    response.send('Hello, Express');
  });
  app.all('/api/:objtype/*?', function(request, response, next){
    response.contentType('json');
    next();
  });
  app.get('/api/:objtype/list', function(request, response){
    response.send({title: request.params.objtype + ' list'});
  });
  app.post('/api/:objtype/create', function(request, response){
    response.send({title:request.params.objtype + ' created'});
  });
  app.get('/api/:objtype/read/:id([0-9]+)', function(request, response){
    response.send({title:request.params.objtype + ' with id ' + request.params.id + ' found'});
  });
  app.post('/api/:objtype/update/:id([0-9]+)', function(request, response){
    response.send({title:request.params.objtype + ' with id ' + request.params.id + ' updated'});
  });
  app.get('/api/:objtype/delete/:id([0-9]+)', function(request, response){
    response.send({title:request.params.objtype + ' with id ' + request.params.id + ' deleted'});
  });
};

module.exports = {configRouter: configRouter};
