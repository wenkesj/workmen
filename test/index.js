var Worker = require('../addon');
var cpus = require("os").cpus().length;
var workers = [];
var a = 0;

function work() {
  for (var i = 0; i < 100000; i++) {
    a++;
  }
}

/* Spawn some worker batons. */
for (var i = 0; i < cpus; i++) {
  workers[i] = new Worker();
  workers[i].open(work);
}

setTimeout(function() {
  for (var i = 0; i < cpus; i++) {
    workers[i].defer(work);
  }
  console.log(a);
}, 1000);
