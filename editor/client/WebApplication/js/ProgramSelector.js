
var ProgramSelector = (function(){

	var container = null;
	var dialog = null;
	var onProgramSelectedCallback =  null;

	var programList = [
	    { desc: "window", id : 10},
	    { desc: "building", id : 11},
	    { desc: "billboard", id : 110},
	];

	function init()
	{
        container = document.createElement("div");
        dialog = document.createElement("div");
        container.appendChild(dialog);
        container.style.position = "absolute";
        container.style.backgroundColor = "rgba(0,0,0,0.5)";
        container.style.top = "0px";
        container.style.left = "0px";
        container.style.right = "0px";
        container.style.bottom = "0px";
        dialog.style.position = "absolute";
        dialog.style.width = "500px";
        dialog.style.height = "200px";
        dialog.style.top = "50%";
        dialog.style.left = "50%";
        dialog.style.marginTop = "-100px";
        dialog.style.marginLeft = "-250px";
        dialog.style.backgroundColor = "white";
        dialog.className = "program-selector-dialog";
        document.body.appendChild(container);
        loadContent();
        hide();
	}

	function updateProgramList(list)
	{
		programList =  list;
	}

	function loadContent()
	{
        var html = 
        "<label class='program-selector-title'>Load GL Program source</label><br>" +
        "<label class='program-selector-label'>Program ID:</label><input class='program-selector-input' placeholder='Load a spesific program by ID'><br>" + 
        "<label class='program-selector-label'>Desc:&nbsp;</label>[SELECTOR]<br>" + 
        "<button class='program-selector-button program-selector-button-ok' > OK </button><button class='program-selector-button' onclick='ProgramSelector.hide();' > CANCEL </button>";

        var selectHTML  = "<select id='program-selector-select'><option disabled selected value> Search by shader description </option>";
        for(var i = 0; i < programList.length; i++)
        {
            selectHTML += "<option value=" + programList[i].id + ">" + programList[i].desc + "<option>";
        }
        selectHTML += "</select><br>";
        html = html.replace("[SELECTOR]", selectHTML);
        dialog.innerHTML = html;
        
        $('#program-selector-select').select2();
        $("#program-selector-select").change(function(e){
            document.querySelector(".program-selector-input").value = document.querySelector("#program-selector-select").value;
        });
	}

	function hide()
	{
		container.style.zIndex = -1;
	}

    function show(onProgramSelected)
    {
    	loadContent();
        document.querySelector(".program-selector-button-ok").addEventListener("click", function(){
            hide();
            onProgramSelectedCallback(document.querySelector(".program-selector-input").value);
        });
    	onProgramSelectedCallback = onProgramSelected;
        container.style.zIndex = 10;
    }

	return { init : init ,
		     show : show,
		     hide : hide,
		     updateProgramList : updateProgramList };
})();
