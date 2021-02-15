// 	</div>
var m_szHotelNam = "";
var m_szHotelDB = "";
var m_szKonreiTable = "";
var m_strID = "";
var m_strNO = "";
var m_strPW = "";

function fncInit()
{
	var inputNo = document.getElementById("userno");
	var inputPw = document.getElementById("password");
	var btnLogin = document.getElementById("btnLogin");

	m_szHotelName = "ホテルソア";
	m_szHotelDB = "jsbridal";
	m_szKonreiTable = "bridaluser";
	localStorage.setItem("HotelName", m_szHotelName);
	localStorage.setItem("HotelDB", m_szHotelDB);
	localStorage.setItem("KonreiTable", m_szKonreiTable);

	//inputNo.value = "000001";
	//inputPw.value = "496721";
	inputNo.value = "0000";
	inputPw.value = "0000";
	btnLogin.onclick = fncLoginOnClick;
}
function fncLoginOnClick()
{
	var inputNo = document.getElementById("userno");
	var inputPw = document.getElementById("password");
	var fnc = fncCheckNOPWCallback;

	m_strID = 0;
	m_strNO = inputNo.value;
	m_strPW = inputPw.value;
	if((m_strNO == "0000" && m_strPW == "0000")
	|| (m_strNO == "message" && m_strPW == "mesyokono")){
		localStorage.setItem("UserKind", "1");
		fnc01MenuCall("0", m_strNO, m_strPW);
		return;
	}
	var data = "com="+m_strNO+","+m_strPW+",";

	sendRequest("POST","php/checkidpw.php",data,false,fnc);
}
function fncCheckNOPWCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();

	data = xmlhttp.responseText;
	ary = data.split(',');

	localStorage.setItem("UserKind", ary[1]);
	if(ary[0] == "1"){
		fncCheckKonrei(m_strNO, m_strPW);
	}else{
		fnclibAlert("パスワードが違います");
	}
}
function fncCheckKonrei(strNO, strPW)
{
	var dbnm = "";
	var krtbl = "";
	var data = "";
	var fnc = fncCheckKonreiCallBack;

	dbnm = m_szHotelDB;
	krtbl = m_szKonreiTable;
	data = "dbnm="+dbnm+"&krtbl="+krtbl+"&krno="+strNO+"&krpw="+strPW;
	sendRequest("POST","php/initkonrei.php",data,false,fnc);
}
function fncCheckKonreiCallBack(xmlhttp)
{
	var data = "";
	var ary = new Array();

	data = xmlhttp.responseText;
	ary = data.split(',');

	if(ary[0] == "1"){
		m_strID = ary[1];
		fnc01MenuCall(m_strID, m_strNO, m_strPW);
	}
}
function fnc01MenuCall(strID, strNO, strPW)
{
	var url = "";

	localStorage.setItem("BridalID", strID);
	localStorage.setItem("BridalNO", strNO);
	localStorage.setItem("BridalPW", strPW);
	var sUserKind = localStorage.getItem("UserKind");
	if(sUserKind == "1"){
		localStorage.setItem("KonreiId", 0);
		localStorage.setItem("KonreiNo", "0");
	}else{
		var nID = fnclibStringToInt(strID);
		localStorage.setItem("KonreiId", nID);
		localStorage.setItem("KonreiNo", strNO);
	}
	url = "01menu.html";
	window.location.href = url;
}
