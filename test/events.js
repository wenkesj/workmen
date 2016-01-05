module.exports = function(suite, worker, socket) {
  socket.on('message', function(req) {
    suite.log('Worker ' + worker.id + ' recieved a message from ' + req.worker.id + ': ' + req.message);
  });
};
