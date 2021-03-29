

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
function fnclibStringToInt(str)
{
	var num = parseInt(str);
	if(isNaN(num)){
		num = 0;
	}
	return(num);
}
function fnclibStrnumToStrnum00(str)
{
	var num = parseInt(str);
	if(num <= 9){
		str = "0"+num;
	}
	return(str);
}
function fnclibNumToStrnum00(num)
{
	if(num <= 9){
		str = "0"+num;
	}else{
		str = num.ToString();
	}
	return(str);
}
function fncZeroPadding(num,length)
{
    return ('0000000000' + num).slice(-length);
}
function fnclibPriceToTenprice(str)
{
	while(str != (str = str.replace(/^(-?\d+)(\d{3})/, "$1,$2")));
	return(str);
}
function fnclibSelectData(cmb, key)
{
	var max = 0;
	var idx = 0;

	max = cmb.options.length;
	for(idx = 0; idx < max; idx++){
		if(cmb.options[idx].text == key){
			cmb.selectedIndex = idx;
			return;
		}
	}
	cmb.options[idx] = new Option(key, idx);
	cmb.selectedIndex = idx;
}
function fnclibSelectedText(cmb)
{
	var idx = 0;
	var sRetStr = "";

	idx = cmb.selectedIndex;
	sRetStr = cmb.options[idx].text;
	return(sRetStr);
}
function fnclibSelectedValue(cmb)
{
	var idx = 0;
	var sRetVal = "";

	idx = cmb.selectedIndex;
	sRetVal = cmb.options[idx].value;
	return(sRetVal);
}
function fnclinHanToZen(hanstr)
{
	var ch = "";
	var retStr = "";
	var idx = 0;
	var max = 0;
	var cidx = 0;
	var cmax = 0;
	var cc = "";
	var ret = "";

	retStr = "";
	han = " !\"#$%&'()*+,-./0123456789;:<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	zen = "　!＂＃＄％＆＇()＊＋,－.／０１２３４５６７８９：；＜＝＞？＠ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ［＼］＾＿｀ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ｛｜｝～";
	cmax = han.length;;
	max = hanstr.length;
	for(idx = 0; idx < max; idx++){
		ch = hanstr.substr(idx, 1);
		ret = "";
		for(cidx = 0; cidx < cmax; cidx++){
			cc = han.substr(cidx, 1);
			if(cc == ch){
				ret = zen.substr(cidx, 1);
				break;
			}
		}
		if(ret == ""){
			retStr = retStr + ch;
		}else{
			retStr = retStr + ret;
		}
	}
	return(retStr);
}
function fnclibCheckTelNo(tel)
{
	var ary = new Array();
	var max = 0;
	var idx = 0;
	var len = 0;
	var rettel = "";

	ary = tel.split('-');
	max = ary.length;
	rettel = "";
	for(idx = 0; idx < max; idx++){
		rettel = rettel + ary[idx];
	}
	len = rettel.length;
	if(len == 10 || len == 11){
		return(true);
	}
	return(false);
}
function fnclibCheckEmailAddress(mail)
{
	var ary = new Array();

	ary = mail.split('@');
	if(ary.length == 2){
		return(true);
	}
	return(false);
}
function fnclibCheckHankakuNum(str)
{
	var ret = "";

	ret = str.match(/^[0-9-]+$/);
	if(ret == null){
		return(false);
	}
	return(true);
}
function fnclibCheckHankaku(str)
{
	var ret = "";

	ret = str.match(/^[\x20-\x7E]+$/);
	if(ret == null){
		return(false);
	}
	return(true);
}
function fnclibCheckHankana(str)
{
	var ret = "";
	re = str.match(/[\uff61-\uff9f]/g);
	if(ret == null){
		return(false);
	}
	return(true);
}
function fnclibStrReplace(key1, key2, str)
{
	while (str.indexOf(key1,0) != -1 )
	{
		str=str.replace(key1,key2);
	}
	return(str);
}
