var child_process = require('child_process');
var EventEmitter = require('events').EventEmitter;
var path = require('path');

var EXECUTABLE = path.join(__dirname, 'build', 'LinuxAreaSelector');
var REPLACE_NEWLINES = new RegExp('\n', 'g');

exports.createSelector = function (x, y, width, height) {
    var emitter = new EventEmitter();

    var child = child_process.spawn(EXECUTABLE, [String(x), String(y), String(width), String(height)], {
        cwd: __dirname,
        stdio: [null, 'pipe', null]
    });
    child.stderr.setEncoding('utf8');
    child.stderr.on('data', function (str) {
        str = str.replace(REPLACE_NEWLINES, '');
        var split = str.split(':');
        switch (split[0]) {
            case 'blur':
                emitter.emit('blur');
                break;
            case 'move':
                if (split.length === 5) {
                    emitter.emit('move', {
                        x: +split[1],
                        y: +split[2],
                        width: +split[3],
                        height: +split[4]
                    });
                }
                break;
        }
    });
    child.stderr.on('end', function () {
        emitter.emit('end');
    });

    return emitter;
};
