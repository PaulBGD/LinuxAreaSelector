var LinuxAreaSelector = require('.');

var selector = LinuxAreaSelector.createSelector(100, 100, 500, 500);
selector.on('error', function (err) {
    console.error(err);
});
selector.on('blur', function () {
    console.log('blurred');
});
selector.on('move', function (event) {
    console.log('moved to', event.x, event.y, event.width, event.height);
});
selector.on('end', function () {
    console.log('selector closed');
});

