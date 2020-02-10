const fs = require('fs');
const gulp = require('gulp');
const htmlmin = require('gulp-htmlmin');
const cleancss = require('gulp-clean-css');
const uglify = require('gulp-uglify');
const gzip = require('gulp-gzip');
const inline = require('gulp-inline');
var inlinesource = require('gulp-inline-source');
const inlineImages = require('gulp-css-base64');
const log = require('fancy-log');

const dataFolder = 'src/data/';
const staticFolder = 'src/static/';

String.prototype.replaceAll = function(search, replacement) {
    var target = this;
    return target.split(search).join(replacement);
};

var toHeader = function(filename) {
    var source = dataFolder + filename;
    var destination = staticFolder + filename + '.h';
    var safename = filename.replaceAll('.', '_');

    var wstream = fs.createWriteStream(destination);
    wstream.on('error', function (err) {
        console.log(err);
    });

    var data = fs.readFileSync(source);

    wstream.write('#define ' + safename + '_len ' + data.length + '\n');
    wstream.write('const uint8_t ' + safename + '[] PROGMEM = {')

    for (i=0; i<data.length; i++) {
        if (i % 20 == 0) wstream.write("\n");
        wstream.write('0x' + ('00' + data[i].toString(16)).slice(-2));
        if (i<data.length-1) wstream.write(',');
    }

    wstream.write('\n};')
    wstream.end();
};

gulp.task('buildfs_inline', function() {
    return gulp.src('html/*.html')
        .pipe(inline({
            base: 'html/',
            js: uglify,
            css: [cleancss, inlineImages],
            disabledTypes: ['svg', 'img']
        }))
        .pipe(inlinesource())
        .pipe(htmlmin({
            collapseWhitespace: true,
            removeComments: true,
            minifyCSS: true,
            minifyJS: true
        }))
        .pipe(gzip())
        .pipe(gulp.dest(dataFolder));
});

var buildfs_embeded = gulp.series('buildfs_inline', 
async function() {
    toHeader('index.html.gz')
});


gulp.task('default', buildfs_embeded);
