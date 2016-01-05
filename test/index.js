var Test = require('./suite');
var suite = new Test();
var total = 1000;

suite.createEnvironment(function(workers, worker) {
  suite.connectClient(worker, function(client) {
    suite.log('Worker ' + worker.id + ' client connection established');
    for (var i = 0; i < total; i++) {
      var message = 'Message ' + (i + 1) + ' of ' + total ;
      client.emit('message', {
        worker: worker,
        message: message
      });
    }
    suite.log('Worker ' + worker.id + ' done', true);
  });
});
