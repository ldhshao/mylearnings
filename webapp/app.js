/*
 * app.js
 */

/*global */
var http = require( 'http' ),
    express = require('express'),
    app = express(),
    server = http.createServer( app),
    routes = require('./routes.js');

app.configure( function () {
    app.use( express.bodyParser() );
    app.use( express.methodOverride() );
    app.use( express.static(__dirname + '/public'));
    app.use( app.router );
});
app.configure( 'development', function () {
    app.use( express.logger() );
    app.use( express.errorHandler({
        dumpExceptions : true,
        showStack : true
    }) );
});
app.configure( 'production', function () {
    app.use( express.errorHandler() );
});

routes.initRoutes(app);

server.listen(3000);

console.log( 'Listening on port %d in %s mode', server.address().port, app.settings.env );
