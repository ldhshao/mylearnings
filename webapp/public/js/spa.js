/*
 * spa.js
 * root namespace module
 */

/*jslint
 */

var spa = (function () {
    var initModule = function ( $container ) {
        //$container.html(
        //    '<h1 style="display:inline-block; margin:25px;">'
        //    +'hello world!'
        //    +'</h1>'
        //);
        spa.shell.initModule ($container);
    };

    return { initModule : initModule };
}());
