// 

var m_strID = "";
var m_strPW = "";
var m_strUserKind = "";

function fncInit()
{
	var btnKihon = document.getElementById("btnKihon");
	var btnGest = document.getElementById("btnGest");
	var btnTableLayout = document.getElementById("btnTableLayout");
	var btnGestSit = document.getElementById("btnGestSit");
	var btnExit = document.getElementById("btnExit");
	var btnInit = document.getElementById("btnInit");

	m_strID = localStorage.getItem("BridalID");
	m_strPW = localStorage.getItem("BridalPW");
	m_strUserKind = localStorage.getItem("UserKind");

	btnKihon.onclick = fncOnClickKihon;
	btnGest.onclick = fncOnClickGest;
	btnTableLayout.onclick = fncOnClickTableLayout;
	btnGestSit.onclick = fncOnClickGestSit;
	btnExit.onclick = fncOnClickExit;

	if(m_strUserKind == "1"){
		btnInit.onclick = fncOnClickInit;
	}else{
		btnInit.style.display = 'none';
	}

 }
function fncOnClickKihon()
{
	var url = "";

	url = "02kihon.html";
	window.location.href = url;
}
function fncOnClickGest()
{
	var url = "";

	url = "03gest.html";
	window.location.href = url;
}
function fncOnClickTableLayout()
{
	var url = "";

	url = "04tablelayout.html";
	window.location.href = url;
}
function fncOnClickGestSit()
{
	var url = "";

	url = "05gestsit.html";
	window.location.href = url;
}
function fncOnClickExit()
{
	var url = "";

	url = "index.html";
	window.location.href = url;
}
function fncOnClickInit()
{
	var url = "";

	url = "util/index.html";
	window.location.href = url;
}
