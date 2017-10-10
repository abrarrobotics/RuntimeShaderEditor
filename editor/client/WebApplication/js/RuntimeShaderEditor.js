
var net = require("net");

var RuntimeShaderEditor = (function(){

    var originalSource = [];
    var vertexBuffer = "";
    var fragmentBuffer = "";
    var socket = null;
    var connectionState = "disconnected";
    var footerLabel = null;
    var editor = null;
    var currentThemeIndex = 0;
    var connectButton = null;
    var loadButton = null;
    var sendButton = null;
    var restoreButton = null;
    var currentProgram = -1;
    var currentTab = "vertex";
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
        document.getElementById('code-editor').style.fontSize='16px';
        editor.setValue(vertexBuffer);
        editor.clearSelection();
        document.querySelector("#nextTheme").addEventListener("click", nextTheme);
        document.querySelector("#prevTheme").addEventListener("click", previousTheme);
        connectButton = document.querySelector("#btn-connect");
        loadButton = document.querySelector("#btn-load");
        sendButton = document.querySelector("#btn-send");
        restoreButton = document.querySelector("#btn-restore");
        connectButton.addEventListener("click", onConnectBtnClicked);
        loadButton.addEventListener("click", onLoadBtnClicked);
        sendButton.addEventListener("click", onSendBtnClicked);
        restoreButton.addEventListener("click", onRestoreBtnClicked);
        document.querySelector("body").addEventListener("onscroll", onScroll);
        window.addEventListener("resize", onResize);

        $('#vertex-fragment-switch').w2tabs({
            name: 'vertex-fragment-switch',
            active: 'vertex',
            tabs: [
                { id: 'vertex', caption: 'Vertex Shader' },
                { id: 'fragment', caption: 'Fragment Shader'},
            ],
            onClick: function (event) {
                if(event.target == "fragment" && currentTab == "vertex")
                {
                    vertexBuffer = editor.getValue();
                    editor.setValue(fragmentBuffer);
                    editor.clearSelection();
                    currentTab = "fragment";
                }
                else if(event.target == "vertex" && currentTab == "fragment")
                {
                    fragmentBuffer = editor.getValue();
                    editor.setValue(vertexBuffer);
                    editor.clearSelection();
                    currentTab = "vertex";
                }
            }
        });

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
        var rect2 = document.querySelector("#vertex-fragment-switch").getBoundingClientRect();
        document.querySelector("#code-editor").style.height = (rect.height - rect2.height) + "px";
        editor.resize();
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
        if(connectionState == "disconnected")
        {
            alertify
            .defaultValue("127.0.0.1:12345")
            .prompt("REMOTE TARGET:", function(val){
                connectButton.dataset.disabled = "true";
                footerLabel.innerHTML = "Connecting to " + val + "...";
                var sockaddr = val.split(":");
                socket = net.createConnection({host:sockaddr[0], port:sockaddr[1]}, function(){
                    connectionState = "handshaking";
                    setTimeout(function(){
                        if(connectionState == "handshaking")
                        {
                            onSocketError();
                            socket.end();
                        }
                    }, 3000);
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
            connectionState = "loadingProgram";
            socket.write("getShaderSource(" + val + ")\r\n");
            currentProgram = val;
        });
    }

    function onRestoreBtnClicked()
    {
        if(restoreButton.dataset.disabled == "true")
        {
            return;
        }
        if(currentTab == "vertex")
        {
            vertexBuffer = originalSource[currentProgram].vertex;
            editor.setValue(vertexBuffer);
        }
        if(currentTab == "fragment")
        {
            fragmentBuffer = originalSource[currentProgram].fragment;
            editor.setValue(fragmentBuffer);
        }
        editor.clearSelection();
        onSendBtnClicked();
    }

    function onSocketData(data)
    {
        if(connectionState == "handshaking")
        {
            if(data.toString().indexOf("RSE-ACK") >= 0)
            {
                connectionState = "connected";
                onConnectionSucceed();
            }
            else
            {
                socket.end();
                onSocketError();
            }
        }
        else if(connectionState == "loadingProgram")
        {
            var code = data.toString().split("[SEPARATOR]");
            vertexBuffer = code[0];
            fragmentBuffer = code[1];
            originalSource[currentProgram] = {vertex : vertexBuffer, fragment : fragmentBuffer};
            if(currentTab == "vertex")
            {
                editor.setValue(vertexBuffer);
            }
            else
            {
                editor.setValue(fragmentBuffer);
            }
            editor.clearSelection();
            connectionState = "programLoaded";
            onProgramLoaded();
        }
        else if(connectionState == "patchingShader")
        {
            connectionState = "programLoaded";
            if(data.toString().indexOf("done") >= 0 )
            {
                footerLabel.innerHTML = "Done";
            }
            else
            {
                footerLabel.innerHTML = "Compilation Failed";
            }
        }
    }

    function onSendBtnClicked()
    {
        if(sendButton.dataset.disabled == "true")
        {
            return;
        }

        connectionState = "patchingShader";

        if(currentTab == "vertex")
        {
            vertexBuffer = editor.getValue();
            socket.write("patchShader(" + currentProgram + ",0)" + vertexBuffer);
        }
        else
        {
            fragmentBuffer = editor.getValue();
            socket.write("patchShader(" + currentProgram + ",1)" + fragmentBuffer);
        }
    }

    function onSocketError()
    {
        footerLabel.innerHTML = "Connection failed";
        connectButton.dataset.disabled = "false";
    }

    function onSocketDisconnected()
    {
        connectButton.innerHTML = "Connect";
        connectionState = "disconnected";
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
        connectionState = "connected";
        loadButton.dataset.disabled = "false";
    }

    function onProgramLoaded()
    {
        sendButton.dataset.disabled = "false";
        restoreButton.dataset.disabled = "false";
        footerLabel.innerHTML = "Program " + currentProgram + " loaded";
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

