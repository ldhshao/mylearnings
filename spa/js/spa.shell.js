/*js/spa.shell.js
 *shell module for spa
 */

/* global $, spa */
spa.shell = (function () {
    //---------------- BEGIN MODULE SCOPE VARIABLES
    var configMap = {
        main_html : String()
        + '    <div class="spa-shell-head">'
        + '        <div class="spa-shell-head-logo">logo</div>'
        + '        <div class="spa-shell-head-acct">acct</div>'
        + '        <div class="spa-shell-head-search">search</div>'
        + '    </div>'
        + '    <div class="spa-shell-main">'
        + '        <div class="spa-shell-main-nav">nav</div>'
        + '        <div class="spa-shell-main-content">content</div>'
        + '    </div>'
        + '    <div class="spa-shell-foot">'
        + '    </div>'
        + '    <div class="spa-shell-chat">'
        + '        <div class="spa-shell-modal"></div>'
        + '    </div>'
    },
        stateMap = { $container : null },
        jqueryMap = {},
        setJqueryMap, initModule;
    //---------------- END MODULE SCOPE VARIABLES
    
    //---------------- BEGIN UTILITY SCOPE VARIABLES
    //---------------- END UTILITY SCOPE VARIABLES
    
    //---------------- BEGIN DOM SCOPE VARIABLES
    setJqueryMap = function (){
        var $container = stateMap.$container;
        jqueryMap = { $container : $container }
    };
    //---------------- END DOM SCOPE VARIABLES
    
    //---------------- BEGIN EVENT SCOPE VARIABLES
    //---------------- END EVENT SCOPE VARIABLES
    
    //---------------- BEGIN PUBLIC SCOPE VARIABLES
    initModule = function ( $container ) {
        stateMap.$container = $container;
        $container.html( configMap.main_html );
        setJqueryMap();
    };

    return { initModule : initModule };
    //---------------- END PUBLIC SCOPE VARIABLES
}());
