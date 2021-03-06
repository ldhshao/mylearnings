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
        + '        <div class="spa-shell-head-acct">manage</div>'
        + '        <div class="spa-shell-head-search">search</div>'
        + '    </div>'
        + '    <div class="spa-shell-main">'
        + '        <div class="spa-shell-main-nav">nav</div>'
        + '        <div class="spa-shell-main-content">content</div>'
        + '    </div>'
        + '    <div class="spa-shell-foot">'
        + '    </div>'
        + '    <div class="spa-shell-manager">'
        + '        <div class="spa-shell-manager-title">Login</div>'
        + '        <div class="spa-shell-manager-user">User</div>'
        + '        <input class="spa-shell-manager-input-user" placeholder="input your user name"/>'
        + '        <div class="spa-shell-manager-password">Password</div>'
        + '        <input class="spa-shell-manager-input-password" placeholder="input your password"//>'
        + '        <button class="spa-shell-manager-button-login">Login</button>'
        + '    </div>'
        + '    <div id="msg" class="spa-shell-msg">'
        + '    </div>'
    },
        stateMap = { anchor_map : {} },
        jqueryMap = {},
        setJqueryMap, setChatAnchor, initModule,
        copyAnchorMap, changeAnchorPart, showMsg, 
        funCompleteString, numTo11String, generateToken, generateRand,
        onHashChange,
        onManager, onLogin;
    //---------------- END MODULE SCOPE VARIABLES
    
    //---------------- BEGIN UTILITY SCOPE VARIABLES
    copyAnchorMap = function () {
        return $.extend(true, {}, stateMap.anchor_map);
    };
    showMsg = function(msg){
        jqueryMap.$msg.text(msg);
    };
    funCompleteString = function(s, tlen, c){
        var stemp = '', len = tlen;
        while(len--) stemp +=c;
        if (s.length < tlen)
            return stemp.substr(0, tlen - s.length) + s;
        return c;
    };
    numTo11String = function(x){
        return funCompleteString(x.toString(),11,'0');
    };
    generateToken = function(psw, salt){
        return sjcl.hash.sha256.hash(psw+salt).map(x=>numTo11String(x)).join('');
    };
    generateRand = function(){
        return sjcl.random.randomWords(10).map(x=>numTo11String(x)).join('');
    };
    //---------------- END UTILITY SCOPE VARIABLES
    
    //---------------- BEGIN DOM SCOPE VARIABLES
    setJqueryMap = function (){
        var $container = stateMap.$container;
        jqueryMap = { $container : $container,
                      $managerBtn: $container.find('.spa-shell-head-acct'),
                      $managerWin: $container.find('.spa-shell-manager'),
                      $loginBtn  : $container.find('.spa-shell-manager-button-login'),
                      $msg       : $container.find('#msg')
        };
        //console.log(jqueryMap.$managerBtn);
        //console.log(jqueryMap.$managerWin);
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
    onManager = function(event){
        console.log('acct clicked');
        jqueryMap.$managerWin.css({'display':'block'});
        return false;
    };
    onLogin = function(){
        var $user = jqueryMap.$managerWin.find('.spa-shell-manager-input-user'),
            $psw = jqueryMap.$managerWin.find('.spa-shell-manager-input-password');
        var user = $user.val(),
            psw = $psw.val(),
            salt = 'salt';
        if(!user || user == ""){
            //showMsg("Please input user name");
            alert("Please input user name");
            $user.focus ();
            return false;
        }
        if(!psw || psw == ""){
            alert("Please input password");
            $psw.focus ();
            return false;
        }
        psw = generateToken(psw, '');
        salt = generateRand();
        psw = generateToken(psw, salt);
        $.post('user/verify/'+user, {'token':psw,'salt':salt}, function(data, status){
            if (0 === data.result){
                jqueryMap.$managerWin.css({'display':'none'});
                jqueryMap.$managerWin.find('.spa-shell-manager-input-user').val('');
                jqueryMap.$managerWin.find('.spa-shell-manager-input-password').val('');
                //load manage ui
                return true;
            }
        });
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
        jqueryMap.$managerBtn.click(onManager);
        jqueryMap.$loginBtn.click(onLogin);
        console.log('manager configed');

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

    return { initModule : initModule, onManager : onManager };
    //---------------- END PUBLIC SCOPE VARIABLES
}());
