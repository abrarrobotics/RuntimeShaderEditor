
var net = require("net");

var RuntimeShaderEditor = (function(){

    var socket = null;
    var isConnected = false;
    var footerLabel = null;
    var editor = null;
    var currentThemeIndex = 0;
    var connectButton = null;
    var loadButton = null;
    var sendButton = null;
    var restoreButton = null;
    var themes = [ "emelist" , "ambiance" , "chaos" ,
              "chrome" , "clouds" , "clouds_midnight" ,
              "cobalt" , "crimson_editor" , "dawn" ,
              "dreamweaver" , "eclipse" , "github" ,
              "idle_fingers" , "iplastic" , "katzenmilch" ,
              "kr_theme" , "kuroir" , "merbivore" ,
              "merbivore_soft" , "mono_industrial" , "monokai" ,
              "pastel_on_dark" ,"solarized_dark" , "solarized_light" ,
              "sqlserver" , "terminal" , "textmate" ,
              "tomorrow" , "tomorrow_night" , "tomorrow_night_blue" ,
              "tomorrow_night_bright" , "tomorrow_night_eighties" ,
              "twilight" , "vibrant_ink" , "xcode" ];

    function init()
    {
        footerLabel = document.querySelector(".footer-label");
        editor = ace.edit("code-editor");
        //editor.setTheme("ace/theme/monokai");
        editor.setTheme("ace/theme/xcode");
        editor.getSession().setMode("ace/mode/glsl");
        editor.setValue("#version 150\r\n \r\nout vec4 colorOut;\r\n \r\nvoid main()\r\n{\r\n    colorOut = vec4(1.0, 0.0, 0.0, 1.0);\r\n}");
        document.querySelector("#nextTheme").addEventListener("click", nextTheme);
        document.querySelector("#prevTheme").addEventListener("click", previousTheme);
        connectButton = document.querySelector("#btn-connect");
        loadButton = document.querySelector("#btn-load");
        sendButton = document.querySelector("#btn-send");
        restoreButton = document.querySelector("#btn-restore");
        connectButton.addEventListener("click", onConnectBtnClicked);
        loadButton.addEventListener("click", onLoadBtnClicked);
        document.querySelector("body").addEventListener("onscroll", onScroll);
        window.addEventListener("resize", onResize);
        onResize();
        updateButtonLabels();
        footerLabel.innerHTML = "Ready";
    }

    function onScroll(e)
    {
        console.log(e);
    }

    function onResize()
    {
        var rect = document.querySelector(".content").getBoundingClientRect();
        document.querySelector("#code-editor").style.height = rect.height + "px";
    }

    function nextTheme()
    {
    	currentThemeIndex++;
    	if(currentThemeIndex == themes.length)
    	{
    		currentThemeIndex = 0;
    	}
    	editor.setTheme("ace/theme/" + themes[currentThemeIndex]);
      footerLabel.innerHTML = "Current theme: " + themes[currentThemeIndex];
    }

    function previousTheme()
    {
    	currentThemeIndex--;
    	if(currentThemeIndex == -1)
    	{
    		currentThemeIndex = themes.length-1;
    	}
    	editor.setTheme("ace/theme/" + themes[currentThemeIndex]);
      footerLabel.innerHTML = "Current theme: " + themes[currentThemeIndex];
    }

    function onConnectBtnClicked(e)
    {
        if(!isConnected)
        {
            alertify
            .defaultValue("127.0.0.1:1111")
            .prompt("REMOTE TARGET:", function(val){
                connectButton.dataset.disabled = "true";
                footerLabel.innerHTML = "Connecting to " + val + "...";
                var sockaddr = val.split(":");
                socket = net.createConnection({host:sockaddr[0], port:sockaddr[1]}, function(){
                    // TODO: receive an ACK before considering the connection opened
                    onConnectionSucceed();
                });
                socket.on("error", onSocketError);
                socket.on("data", onSocketData);
                socket.on("close", onSocketDisconnected);
            });
        }
        else
        {
            socket.end();
            footerLabel.innerHTML = "Connection closed";
            onSocketDisconnected();
        }
        console.log(e);
    }

    function onLoadBtnClicked(e)
    {
        if (loadButton.dataset.disabled == "true")
        {
            return;
        }
        alertify
        .defaultValue("")
        .prompt("Load GL Program", function(val){
            socket.write("getShaderSource(" + val + ")\r\n");
        });
    }

    function onSocketData(data)
    {
        editor.setValue(data.toString());
    }

    function onSocketError()
    {
        footerLabel.innerHTML = "Connection failed";
        connectButton.dataset.disabled = "false";
    }

    function onSocketDisconnected()
    {
        connectButton.innerHTML = "Connect";
        isConnected = false;
        updateButtonLabels();
        connectButton.dataset.disabled = "false";
        loadButton.dataset.disabled = "true";
        sendButton.dataset.disabled = "true";
        restoreButton.dataset.disabled = "true";
    }

    function onConnectionSucceed()
    {
        connectButton.innerHTML = "Disconnect";
        updateButtonLabels();
        footerLabel.innerHTML = "Connection succeed";
        connectButton.dataset.disabled = "false";
        isConnected = true;
        loadButton.dataset.disabled = "false";
    }

    function onShaderLoaded()
    {

    }

    function updateButtonLabels()
    {
        var elems = document.querySelectorAll(".btnLabel");
        for(var i = 0; i < elems.length; i++)
        {
            // remove the forced width so the navigator will
            // calculate the default value
            elems[i].removeAttribute("style");
            // get the default width
            var rect = elems[i].getBoundingClientRect();  
            // force width
            elems[i].style.width = (rect.width + 5) + "px";
        }
    }

    return { init : init };

})();

RuntimeShaderEditor.init();
