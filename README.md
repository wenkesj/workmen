# defer-work #
Maintainable deferred work for **Node.js**.
```sh
npm i defer-work --save
```
## API ##
```js
var Baton = require('defer-work');
var baton = new Baton();
```
### _baton_.pass(_callback_) ###
Creates an event loop queue item filled with deferred work. Returns a Promise object after all deferred work from the baton is finished.
```js
baton.pass(function() {
  // ... Deferred work...
}).then(function() {
  // ... Finished.
});
```
### _baton_.open(_callback_) ###
```js
baton.open(function() {
  // ... Do something with this event loop forever.
});
```

### _baton_.defer() ###
```js
baton.open(function() {
  // ... Run this event loop once.
  this.defer();
});
```
