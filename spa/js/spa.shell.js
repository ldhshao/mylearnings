/*js/spa.shell.js
 *shell module for spa
 */

/* global $, spa */
spa.shell = (function () {
    //---------------- BEGIN MODULE SCOPE VARIABLES
    var configMap = {
        anchor_schema_map : { chat : {opened:true, closed:true} },
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
        + '    <div class="spa-shell-modal"></div>'
    },
        stateMap = { anchor_map : {} },
        jqueryMap = {},
        setJqueryMap, setChatAnchor, initModule,
        copyAnchorMap, changeAnchorPart, onHashChange;
    //---------------- END MODULE SCOPE VARIABLES
    
    //---------------- BEGIN UTILITY SCOPE VARIABLES
    copyAnchorMap = function () {
        return $.extend(true, {}, stateMap.anchor_map);
    };
    //---------------- END UTILITY SCOPE VARIABLES
    
    //---------------- BEGIN DOM SCOPE VARIABLES
    setJqueryMap = function (){
        var $container = stateMap.$container;
        jqueryMap = { $container : $container};
    };
    changeAnchorPart = function (arg_map) {
        var anchor_map_revise = copyAnchorMap(),
            bool_return = true, 
            key_name, key_name_dep;
        KEYVAL:
        for (key_name in arg_map){
            if (arg_map.hasOwnProperty(key_name)) {
                if (key_name.indexOf('_') === 0) { continue KEYVAL;}
                anchor_map_revise[key_name] = arg_map[key_name];
                key_name_dep = '_' + key_name;
                if (arg_map[key_name_dep]){
                    anchor_map_revise[key_name_dep] = arg_map[key_name_dep];
                }
                else{
                    delete anchor_map_revise[key_name_dep];
                    delete anchor_map_revise['_s' + key_name_dep];
                }
            }
        }
        //begin attempt to update uri; revert if not successful
        try {
            $.uriAnchor.setAnchor(anchor_map_revise);
        }
        catch (error){
            $.uriAnchor.setAnchor(stateMap.anchor_map, null, true);
            bool_return = false;
        }

        return bool_return;
    };
    //---------------- END DOM SCOPE VARIABLES
    
    //---------------- BEGIN EVENT SCOPE VARIABLES
    onHashChange = function( event) {
        var anchor_map_previous = copyAnchorMap(),
            anchor_map_proposed, is_ok = true,
            _s_chat_previous, _s_chat_proposed, s_chat_proposed;
        try { anchor_map_proposed = $.uriAnchor.makeAnchorMap();}
        catch (error){
            $.uriAnchor.setAnchor(anchor_map_previous, null, true);
            return false;
        }

        stateMap.anchor_map = anchor_map_proposed;
        _s_chat_previous = anchor_map_previous._s_chat;
        _s_chat_proposed = anchor_map_proposed._s_chat;
        if (!anchor_map_previous 
            || _s_chat_previous != _s_chat_proposed) {
            s_chat_proposed = anchor_map_proposed.chat;
            switch (s_chat_proposed){
                case 'opened':
                    is_ok = spa.chat.setSliderPosition('opened');
                    break;
                case 'closed':
                    is_ok = spa.chat.setSliderPosition('closed');
                    break;
                default:
                    spa.chat.setSliderPosition('closed');
                    delete anchor_map_proposed.chat;
                    $.uriAnchor.setAnchor(anchor_map_proposed, null, true);
            }
        }
        if (!is_ok){
            delete anchor_map_proposed.chat;
            $.uriAnchor.setAnchor(anchor_map_previous, null, true);
            stateMap.anchor_map = anchor_map_previous;
        }

        return false;
    };
    //---------------- END EVENT SCOPE VARIABLES
    
    //---------------- BEGIN CALLBACK
    setChatAnchor = function (position_type) {
        return changeAnchorPart({chat : position_type});
    };
    //---------------- END CALLBACK

    //---------------- BEGIN PUBLIC SCOPE VARIABLES
    initModule = function ( $container ) {
        stateMap.$container = $container;
        $container.html( configMap.main_html );
        setJqueryMap();

        spa.chat.configModule({
            set_chat_anchor : setChatAnchor,
            chat_model      : spa.model.chat,
            people_model    : spa.model.people
        });
        spa.chat.initModule($container);

        //config uriAnchor to use our schema
        $.uriAnchor.configModule({
            schema_map : configMap.anchor_schema_map
        });
        $(window).bind('hashchange', onHashChange)
                 .trigger('hashchange');
    };

    return { initModule : initModule };
    //---------------- END PUBLIC SCOPE VARIABLES
}());
