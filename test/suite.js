var socket = require('socket.io');
var ioClient = require('socket.io-client');
var cpus = require('os').cpus().length;
var http = require('http');
var events = require('./events');
var Worker = require('../addon');
var Logger = require('./logger');
var logger = new Logger(process.argv.slice(2)[0] === '--verbose');
var host = '127.0.0.1';
var port = 3000;

function Test() {
  this.logging = logger.logging;
}
Test.prototype.log = logger.log;
Test.prototype.createEnvironment = function(testEnviornment) {
  var suite = this;
  var workers = [];
  for (var i = 0; i < cpus; i++) {
    workers[i] = new Worker();
    workers[i].id = i;
    workers[i].port = port;
    workers[i].host = host;
    workers[i].open(work);
    port += 10;
  }
  function work() {
    var worker = this;
    suite.log('Worker ' + worker.id + ' open.');
    var factory = http.createServer(function(req, res) {
      res.writeHead(200, {
        'Content-Type': 'text/plain'
      });
      res.end('Hello from Worker ' + worker.id);
    });
    factory.listen(worker.port, worker.host);
    suite.log('Worker ' + worker.id + ' listening on ' + worker.host + ':' + worker.port);
    var io = socket(factory);
    io.on('connection', function(socket) {
      suite.log('Worker ' + worker.id + ' server connection established');
      events(suite, worker, socket);
    });
    testEnviornment(workers, worker);
    suite.log('Worker ' + worker.id + ' closed.');
    return worker.close();
  }
};

Test.prototype.connectClient = function(worker, callback) {
  callback(ioClient.connect('http://0.0.0.0:' + worker.port, {
    transports: ['websocket'],
    'force new connection': true
  }));
};


module.exports = Test;
