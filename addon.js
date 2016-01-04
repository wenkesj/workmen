var Baton = require('bindings')('addon').Baton;

Baton.prototype.pass = function(eventLoop) {
  var self = this;
  return new Promise(function(resolve, reject) {
    Baton.prototype.open.call(self, function() {
      Baton.prototype.defer.call(self);
      eventLoop();
      resolve();
    });
  });
};

module.exports = Baton;
