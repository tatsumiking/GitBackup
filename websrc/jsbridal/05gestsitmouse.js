

// マウスイベント定数
var INITEVENT=1;
var SETEVENT=2;
var UPSETEVENT=3;
var MOVEEVENT=4;
var UPMOVEEVENT=5;
var ENDEVENT=9;

var m_nMouseDown = 0;
var m_nMouseCrtX = 0;
var m_nMouseCrtY = 0;
var m_nMouseAdjustX = 0; // 微調整用データ
var m_nMouseAdjustY = -1;
var m_dScrnRltvSX = 0.0;
var m_dScrnRltvSY = 0.0;
var m_dScrnBaseX = 0.0;
var m_dScrnBaseY = 0.0;
var m_dLimit = 3.0;

var userAgent = window.navigator.userAgent.toLowerCase();

if(userAgent.indexOf('msie') != -1 && window.navigator.msPointerEnabled){
	document.addEventListener("MSPointerDown",fncMouseDown);
	document.addEventListener("MSPointerMove",fncMouseMove);
	document.addEventListener("MSPointerUp",fncMouseUp);
}else if(document.addEventListener){
	document.addEventListener("mousedown",fncMouseDown);
	document.addEventListener("mousemove",fncMouseMove);
	document.addEventListener("mouseup",fncMouseUp);
	//document.addEventListener("touchstart",fncTouchDown);
	//document.addEventListener("touchmove",fncTouchMove);
	//document.addEventListener("touchend",fncTouchUp);
}else if(document.attachEvent){
	document.attachEvent("onmousedown" , fncMouseDown);
	document.attachEvent("onmousemove" , fncMouseMove);
	document.attachEvent("onmouseup" , fncMouseUp);
}

function fncTouchDown(event)
{
	var x = 0;
	var y = 0;

	x = event.targetTouches[0].pageX-m_nScrnLeft;
	y = event.targetTouches[0].pageY-m_nScrnTop;
	if(fncCheckCanvasArea(x, y) == true){
		//event.preventDefault();
		fncMouseDownXY(x, y);
	}
}

function fncTouchMove(event)
{
	var x = 0;
	var y = 0;

	x = event.targetTouches[0].pageX-m_nScrnLeft;
	y = event.targetTouches[0].pageY-m_nScrnTop;
	if(fncCheckCanvasArea(x, y) == true){
		//event.preventDefault();
		m_nMouseCrtX = x;
		m_nMouseCrtY = y;
		fncMouseMoveXY(x, y);
	}
}

function fncTouchUp(event)
{
	var x = 0;
	var y = 0;

	x = m_nMouseCrtX;
	y = m_nMouseCrtY;
	if(fncCheckCanvasArea(x, y) == true){
		//event.preventDefault();
		fncMouseUpXY(x, y);
	}
}

function fncGetPointAdd()
{
	var pt = new Point();
	var area = document.body.parentElement

	if(area == null || area.scrollLeft == null){
		pt.x = -m_nScrnLeft-m_nMouseAdjustX;
	}else{
		pt.x = -m_nScrnLeft-m_nMouseAdjustX+area.scrollLeft;
	}
	if(area == null || area.scrollTop == null){
		pt.y = -m_nScrnTop-m_nMouseAdjustY;
	}else{
		pt.y = -m_nScrnTop-m_nMouseAdjustY+area.scrollTop;
	}
	return(pt);
}
function fncCheckCanvasArea(x, y)
{
	var area = document.getElementById("cnvsPaper");
	var sx = 0;
	var sy = 0;
	var ex = 0;
	var ey = 0;

	if(area == null){
		return(false);
	}
	sx = 0;
	sy = 0;
	ex = area.offsetWidth;
	ey = area.offsetHeight;
	if(sx < x && x < ex && sy < y && y < ey){
		return(true);
	}
	return(false);
}

function fncMouseDown(event)
{
	var pt = fncGetPointAdd();
	var x = 0;
	var y = 0;

	x = event.clientX + pt.x;
	y = event.clientY + pt.y;
	if(fncCheckCanvasArea(x, y) == true){
		fncMouseDownXY(x, y);
	}
	//m_txtInfo.textContent = "("+x+","+y+")";
}

function fncMouseUp(event)
{
	var pt = fncGetPointAdd();
	var x = 0;
	var y = 0;

	x = event.clientX + pt.x;
	y = event.clientY + pt.y;
	if(fncCheckCanvasArea(x, y) == true){
		fncMouseUpXY(x, y);
	}
	//m_txtInfo.textContent = "("+x+","+y+")";
}

function fncMouseMove(event)
{
	var pt = fncGetPointAdd();
	var x = 0;
	var y = 0;

	x = event.clientX + pt.x;
	y = event.clientY + pt.y;
	if(fncCheckCanvasArea(x, y) == true){
		fncMouseMoveXY(x, y);
	}
	//m_txtInfo.textContent = "("+x+","+y+")";
}

function fncMouseDownXY(x, y)
{
	var event = 0;
	var nx = 0;
	var ny = 0;

	event = SETEVENT;
	m_nMouseDown = 1;
	nx = fncScrnTrnsRMX(x);
	ny = fncScrnTrnsRMY(y);
	nx = parseInt(nx);
	ny = parseInt(ny);
	fncCmdExec(event, nx, ny);
}

function fncMouseUpXY(x, y)
{
	var event = 0;
	var nx = 0;
	var ny = 0;

	event = UPSETEVENT;
	m_nMouseDown = 0;
	nx = fncScrnTrnsRMX(x);
	ny = fncScrnTrnsRMY(y);
	nx = parseInt(nx);
	ny = parseInt(ny);
	fncCmdExec(event, nx, ny);
}

function fncMouseMoveXY(x, y)
{
	var event = 0;
	var nx = 0;
	var ny = 0;

	if(m_nMouseDown == 1){
		event = MOVEEVENT;
	}else{
		event = UPMOVEEVENT;
	}
	nx = fncScrnTrnsRMX(x);
	ny = fncScrnTrnsRMY(y);
	nx = parseInt(nx);
	ny = parseInt(ny);
	fncCmdExec(event, nx, ny);
}
function fncScrnTrnsRMX(rltvx)
{
	var absx = 0;

	absx = (rltvx - m_dScrnRltvSX);
	return(absx);
}
function fncScrnTrnsRMY(rltvy)
{
	var absy = 0;

	absy = (rltvy - m_dScrnRltvSY);
	return(absy);
}
function fncScrnTrnsMRX(absx)
{
	var rltvx = 0;

	rltvx = absx + m_dScrnRltvSX;
	return(rltvx);
}
function fncScrnTrnsMRY(absy)
{
	var rltvy = 0;

	rltvy = absy + m_dScrnRltvSY;
	return(rltvy);
}


