# workmen #
Maintainable deferred work for **Node.js**.
```sh
npm i workmen --save
```
## API ##
```js
var Worker = require('workmen');
var worker = new Worker();
```

### _worker_.open(_workFunction_) ###
Creates an event loop filled with work.
```js
worker.open(function() {
  // ... Do something with this event loop.
});
```

### _worker_.close() ###
Stop all work on a worker.
```js
worker.close();
```
