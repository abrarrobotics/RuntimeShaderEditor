

var RSEMessage =(function(){

	var buffer = "";

	function addData(data)
	{
		if(typeof stringValue != "string")
			data = data.toString();
        buffer += data;
	}

	function isComplete()
	{
        var array = buffer.split(" ");
        var length = parseInt(array[0]) + array[0].length + 1;
        return buffer.length == length;
	}

	function getMessage()
	{
		var array = buffer.split(" ");
		var headerLength = array[0].length + 1;
		return buffer.substring(headerLength);
	}

	function clear()
	{
		buffer = "";
	}

	function prepareMessage(message)
	{
        return message.length + " " + message;
	}

	return { addData : addData,
	         isComplete : isComplete,
	         getMessage : getMessage,
	         clear : clear,
	         prepareMessage : prepareMessage };

})();