function Logger(logging) {
  this.logging = logging;
}
Logger.prototype.log = function(message, override) {
  if (this.logging || override) {
    console.log(message);
  } else {
    void(0);
  }
};
module.exports = Logger;
