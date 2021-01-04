
function fnclibAlert(msg)
{
	alert(msg);
}

function fnclibMessageWindow(title, msg)
{
	if(confirm(msg)){
		return("OK");
	}else{
		return("Cancel");
	}
}
function fnclibZeroPadding(num,length)
{
    return ('0000000000' + num).slice(-length);
}
