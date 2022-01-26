'use strict';
const url = 'mongodb://localhost:27017';
const dbName = 'expressapp';
var configRouter;

configRouter = function(app, server){
  app.get('/', function(request, response){
    //connectAppdb();
    response.send('Hello, Express');
  });
  app.all('/api/:objtype/*?', function(request, response, next){
    response.contentType('json');
    next();
  });
  app.get('/api/:objtype/list', function(request, response){
    const MongoClient = require('mongodb').MongoClient;
    var mongoClient = new MongoClient(url);
    mongoClient.connect(function(err, client) {
      const db = client.db(dbName);
	    if(err){
	    	console.error(err);
	    }else{
	    	console.log("Connected correctly to server");
        const collection = db.collection(request.params.objtype);
        //const collection = db.collection('books');
        collection.find({}).toArray(function(err, docs){
          if(err){
            console.error(err);
          }
          else{
            console.log('Found ' + request.params.objtype + ' list');
            console.log(docs);
            response.send(docs);
          }
          console.log('Disconnected database server');
          client.close();
        });
	    }
    });
    //response.send({title: request.params.objtype + ' list'});
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
