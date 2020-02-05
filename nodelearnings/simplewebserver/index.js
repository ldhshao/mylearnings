var http = require('http')
var fs = require('fs')
var NO_RESOURCE = '<h1>Cannot find the resource</h1>'
var HTTP_SERVER_PORT = 3000
var htmlregexp = /\.html$/i
var jsregexp = /\.js$/i
const apiregexp = /api\//i
const apiseparator = '/'
const apicontent = 'content'

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
  console.log(url);
  if (apiregexp.test(url)){
    var apisects = url.split(apiseparator);
    var data;
    if (apisects.length >= 4){
      console.log(apisects[2]);
      if(apicontent === apisects[2]){
        switch(parseInt(apisects[3], 10)){
          case 0:
            data = respondIndex();
            break;
          case 1:
            data = respondBooks();
            break;
          case 2:
            data = respondArticles();
            break;
        }
      }
    }
    console.log(data);
    res.end(data);
  }
  else{
    var filename = filePathForUrl(url);
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
        console.log(data);
        res.end(data);
      }
    });
  }
});

function respondIndex(){
  //return '{index:0, entity:\'index html\'}';
  var index = {index:0, entity:'index html'};
  return JSON.stringify(index);
}

function respondBooks(){
  var books = [
    {title:'coding with C', author:'dhliu'},
    {title:'How to learn English', author:'jinli'},
    {title:'How to make business', author:'jiaheliu'}
  ];
  var res = {index:1, entity: books};
  return JSON.stringify(res);
}

function respondArticles(){
  var books = [
    {title:'IOCP调试总结', author:'dhliu', link:'https://blog.csdn.net/ldhshao/article/details/103629302'},
    {title:'Test driven development with pytest', author:'dhliu', link:'https://blog.csdn.net/ldhshao/article/details/95049440'}
  ];
  var res = {index:1, entity: books};
  return JSON.stringify(res);
}

server.listen(HTTP_SERVER_PORT);

