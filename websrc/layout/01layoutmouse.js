

// マウスイベント定数
var m_nMouseDown = 0;
var m_nMouseCrtX = 0;
var m_nMouseCrtY = 0;
var m_nMouseAdjustX = 0; // 微調整用データ
var m_nMouseAdjustY = -1;
var m_nDivLayoutAreaLeft = 0;
var m_nDivLayoutAreaTop = 0;
var m_nCanvasLayoutAddLeft = 0;
var m_nCanvasLayoutAddTop = 0;
var m_dLimit = 3.0;

var m_evntfuc;

var userAgent = window.navigator.userAgent.toLowerCase();
const KEYCODE_DELETE=46;
const KEYCODE_BACKSPACE=8;
const KEYCODE_ENTER=13;

function fncMouseAddEventListener()
{
	if(userAgent.indexOf('msie') != -1 && window.navigator.msPointerEnabled){
		document.addEventListener("MSPointerDown",fncMouseDown);
		document.addEventListener("MSPointerMove",fncMouseMove);
		document.addEventListener("MSPointerUp",fncMouseUp);
	}else if(document.addEventListener){
		document.addEventListener("mousedown",fncMouseDown);
		document.addEventListener("mousemove",fncMouseMove);
		document.addEventListener("mouseup",fncMouseUp);
		//document.addEventListener("touchstart",fncMouseTouchDown);
		//document.addEventListener("touchmove",fncMopuseTouchMove);
		//document.addEventListener("touchend",fncMouseTouchUp);
	}
}
function fncMouseRemoveEventListener()
{
	if(userAgent.indexOf('msie') != -1 && window.navigator.msPointerEnabled){
		document.removeEventListener("MSPointerDown",fncMouseDown);
		document.removeEventListener("MSPointerMove",fncMouseMove);
		document.removeEventListener("MSPointerUp",fncMouseUp);
	}else if(document.addEventListener){
		document.removeEventListener("mousedown",fncMouseDown);
		document.removeEventListener("mousemove",fncMouseMove);
		document.removeEventListener("mouseup",fncMouseUp);
		//document.removeEventListener("touchstart",fncMouseTouchDown);
		//document.removeEventListener("touchmove",fncMopuseTouchMove);
		//document.removeEventListener("touchend",fncMouseTouchUp);
	}
}
function fncMouseDivLayoutOffset(offsetLeft, offsetTop)
{
	m_nDivLayoutAreaLeft = offsetLeft;
	m_nDivLayoutAreaTop = offsetTop;
}
function fncMouseCanvasLayoutOffset(offsetLeft, offsetTop)
{
	m_nCanvasLayoutAddLeft = offsetLeft;
	m_nCanvasLayoutAddTop = offsetTop;
}
function fncMouseKeyDown(event)
{
	m_keyeventCrt = event;
	if(m_keyeventCrt.keyCode == KEYCODE_DELETE
	|| m_keyeventCrt.keyCode == KEYCODE_BACKSPACE
	|| m_keyeventCrt.keyCode == KEYCODE_ENTER){
		fncCommandExec(KEYINEVENT, 0, 0);
	}
}
function fncMouseTouchDown(event)
{
	var x = 0;
	var y = 0;

	x = event.targetTouches[0].pageX;
	y = event.targetTouches[0].pageY;
	if(fncMouseCheckSampleArea(x, y) == true){
		fncToolSampleSelect(x, y);
	}
	if(fncMouseCheckLayoutArea(x, y) == true){
		//event.preventDefault();
		x = x - m_nDivLayoutAreaLeft - m_nCanvasLayoutAddLeft;
		y = y - m_nDivLayoutAreaTop - m_nCanvasLayoutAddTop;
		fncMouseDownXY(x, y);
	}
}

function fncMopuseTouchMove(event)
{
	var x = 0;
	var y = 0;

	x = event.targetTouches[0].pageX;
	y = event.targetTouches[0].pageY;
	if(fncMouseCheckLayoutArea(x, y) == true){
		//event.preventDefault();
		m_nMouseCrtX = x;
		m_nMouseCrtY = y;
		x = x - m_nDivLayoutAreaLeft - m_nCanvasLayoutAddLeft;
		y = y - m_nDivLayoutAreaTop - m_nCanvasLayoutAddTop;
		fncMouseMoveXY(x, y);
	}
}

function fncMouseTouchUp(event)
{
	var x = 0;
	var y = 0;

	x = m_nMouseCrtX;
	y = m_nMouseCrtY;
	if(fncMouseCheckLayoutArea(x, y) == true){
		//event.preventDefault();
		x = x - m_nDivLayoutAreaLeft - m_nCanvasLayoutAddLeft;
		y = y - m_nDivLayoutAreaTop - m_nCanvasLayoutAddTop;
		fncMouseUpXY(x, y);
	}
}

function fncMouseGetPointAdd()
{
	var pt = new CPoint();
	var area = document.body.parentElement

	if(area == null || area.scrollLeft == null){
		pt.x = -m_nMouseAdjustX;
	}else{
		pt.x = -m_nMouseAdjustX+area.scrollLeft;
	}
	if(area == null || area.scrollTop == null){
		pt.y = -m_nMouseAdjustY;
	}else{
		pt.y = -m_nMouseAdjustY+area.scrollTop;
	}
	return(pt);
}
function fncMouseCheckLayoutArea(x, y)
{
	var area = document.getElementById("divLayoutArea");
	var sx = 0;
	var sy = 0;
	var ex = 0;
	var ey = 0;

	if(area == null){
		return(false);
	}
	sx = area.offsetLeft;
	sy = area.offsetTop;
	ex = sx + area.offsetWidth;
	ey = sy + area.offsetHeight;
	/*
	if(m_evntfuc == "fncMouseDown"){
		m_lblInfo.textContent = "sxy("+sx+","+sy+") exy("+ex+","+ey
			+") xy("+x+","+y+") divLayoutArea";
	}
	*/
	if(sx < x && x < ex && sy < y && y < ey){
		return(true);
	}
	return(false);
}
function fncMouseCheckSampleArea(x, y)
{
	var area = document.getElementById("divSample");
	var sx = 0;
	var sy = 0;
	var ex = 0;
	var ey = 0;

	if(area == null){
		return(false);
	}
	sx = area.offsetLeft;
	sy = area.offsetTop;
	ex = sx + area.offsetWidth;
	ey = sy + area.offsetHeight;
	/*
	if(m_evntfuc == "fncMouseDown"){
		m_lblInfo.textContent = "sxy("+sx+","+sy+") exy("+ex+","+ey
			+") xy("+x+","+y+") divSample";
	}
	*/
	if(sx < x && x < ex && sy < y && y < ey){
		return(true);
	}
	return(false);
}

function fncMouseDown(event)
{
	var pt = fncMouseGetPointAdd();
	var x = 0;
	var y = 0;

	m_evntfuc = "fncMouseDown";
	x = event.clientX + pt.x;
	y = event.clientY + pt.y;
	if(fncMouseCheckSampleArea(x, y) == true){
		fncToolSampleSelect(x, y); // 01layouttool
	}
	if(fncMouseCheckLayoutArea(x, y) == true){
		x = x - m_nDivLayoutAreaLeft - m_nCanvasLayoutAddLeft;
		y = y - m_nDivLayoutAreaTop - m_nCanvasLayoutAddTop;
		fncMouseDownXY(x, y);
	}
	//m_lblInfo.textContent = "("+x+","+y+")" + ret;
}

function fncMouseUp(event)
{
	var pt = fncMouseGetPointAdd();
	var x = 0;
	var y = 0;

	m_evntfuc = "fncMouseUp";
	x = event.clientX + pt.x;
	y = event.clientY + pt.y;
	if(fncMouseCheckLayoutArea(x, y) == true){
		x = x - m_nDivLayoutAreaLeft - m_nCanvasLayoutAddLeft;
		y = y - m_nDivLayoutAreaTop - m_nCanvasLayoutAddTop;
		fncMouseUpXY(x, y);
	}
	//m_txtInfo.textContent = "("+x+","+y+")";
}

function fncMouseMove(event)
{
	var pt = fncMouseGetPointAdd();
	var x = 0;
	var y = 0;

	m_evntfuc = "fncMouseMove";
	x = event.clientX + pt.x;
	y = event.clientY + pt.y;
	if(fncMouseCheckLayoutArea(x, y) == true){
		x = x - m_nDivLayoutAreaLeft - m_nCanvasLayoutAddLeft;
		y = y - m_nDivLayoutAreaTop - m_nCanvasLayoutAddTop;
		fncMouseMoveXY(x, y);
	}
	//m_txtInfo.textContent = "("+x+","+y+")";
}

function fncMouseDownXY(x, y)
{
	var event = 0;
	var nx = 0;
	var ny = 0;

	event = DOWNEVENT;
	m_nMouseDown = 1;
	nx = fncScrnTrnsRMX(x);
	ny = fncScrnTrnsRMY(y);
	nx = parseInt(nx);
	ny = parseInt(ny);
	fncCommandExec(event, nx, ny);
}

function fncMouseUpXY(x, y)
{
	var event = 0;
	var nx = 0;
	var ny = 0;

	event = UPEVENT;
	m_nMouseDown = 0;
	nx = fncScrnTrnsRMX(x);
	ny = fncScrnTrnsRMY(y);
	nx = parseInt(nx);
	ny = parseInt(ny);
	fncCommandExec(event, nx, ny);
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
	fncCommandExec(event, nx, ny);
}

