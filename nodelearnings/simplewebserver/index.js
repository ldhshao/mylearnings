var http = require('http')
var fs = require('fs')
var NO_RESOURCE = '<h1>Cannot find the resource</h1>'
var HTTP_SERVER_PORT = 3000
var htmlregexp = /\.html$/i
var jsregexp = /\.js$/i

var handleError = function(err, res) {
  res.writeHead(404);
  res.end(NO_RESOURCE);
}
function filePathForUrl(url) {
  const dir = 'app/';
  var fileName = 'index.html';
  if (url.length > 1) {
    fileName = url.substring(1);
  }

  return dir + fileName;
}
var server = http.createServer(function (req, res) {
  console.log('Responding to a request.');
  var url = req.url;
  var filename = filePathForUrl(url);
  console.log(url);
  console.log(filename);
  fs.readFile(filename, function(err, data){
    if (err){
      handleError(err, res);
    }
    else{
      if (htmlregexp.test(filename)){
        res.setHeader('Content-Type', 'text/html');
      }
      else if (jsregexp.test(filename)){
        res.setHeader('Content-Type', 'application/javascript');
      }
      res.end(data);
    }
  });
});

server.listen(HTTP_SERVER_PORT);

