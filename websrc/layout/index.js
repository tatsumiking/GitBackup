function fncInit()
{
	localStorage.setItem("LayoutID", "");
	localStorage.setItem("LayoutPW", "");

	var url = "01layout.html";
	//var url = "01menu.html";
	window.location.href = url;

	var btnInitPage = document.getElementById("btnInitPage");
	btnInitPage.onclick = fncOnClickInitPage;
}
function fncOnClickInitPage()
{
	var url = "01layout.html";
	//var url = "01menu.html";
	window.location.href = url;
}
