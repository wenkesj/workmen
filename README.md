# defer-work #
Maintainable deferred work for **Node.js**.
```sh
npm i defer-work --save
```
## API ##
```js
var Worker = require('defer-work');
var worker = new Worker();
```

### _worker_.open(_workFunction_) ###
Creates an event loop filled with deferred work.
```js
worker.open(function() {
  // ... Do something with this event loop.
});
```

### _worker_.defer() ###
Stop all work on a worker.
```js
worker.defer();
```
