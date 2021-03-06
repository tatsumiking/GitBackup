const CNTR_AREAOVER = 0;
const CNTR_MOVE = 1;
const CNTR_LEFT_TOP = 2;
const CNTR_CENTER_TOP = 3;
const CNTR_RIGHT_TOP = 4;
const CNTR_RIGHT_CENTER = 5;
const CNTR_RIGHT_BOTTOM = 6;
const CNTR_CENTER_BOTTOM = 7;
const CNTR_LEFT_BOTTOM = 8;
const CNTR_LEFT_CENTER = 9;

var m_nCrtCommand;
var m_CrtObject = null;
var m_dBaseRectSX, m_dBaseRectSY;
var m_dBaseRectEX, m_dBaseRectEY;
var m_dRectSX, m_dRectSY;
var m_dRectEX, m_dRectEY;
var m_nMouseCount;
var m_nCommandTimer;
var m_nCntrKind;
var m_dMouseBaseX, m_dMouseBaseY;
var m_dCntrX = 0, m_dCntrY = 0;
var m_dMoveX = 0, m_dMoveY = 0;
var m_dTimeX = 1.0, m_dTimeY = 1.0;

function fncCommandSetCrt(cmd)
{
	fncCommandExec(ENDEVENT, 0, 0);
	m_nCrtCommand = cmd;
	fncCommandExec(INITEVENT, 0, 0);
	fncDrawMain();
}
function fncCommandExec(event, nx, ny)
{
	var ret;

	switch(m_nCrtCommand){
	case CMDSELECT:
		ret = fncCommandSelect(event, nx, ny);
		break;
	case CMDNEWFIGURE:
		ret = fncCommandNewFigure(event, nx, ny)
		break;
	case CMDSELECTFIGURE:
		ret = fncCommandSelectFigure(event, nx, ny)
		break;
	case CMDTEXT:
		ret = fncCommandText(event, nx, ny)
		break;
	case CMDIMAGE:
		ret = fncCommandImage(event, nx, ny)
		break;
	}
	if(ret == ENDEVENT){
		
	}
}
function fncCommandSelect(event, nx, ny)
{
	var obj;

	switch(event){
		case INITEVENT:
			m_nMouseCount = 0;
			m_nCntrKind = CNTR_AREAOVER;
			fncDrawMain();
			break;
		case DOWNEVENT:
			if(m_nMouseCount == 0){
				if(m_CrtObject == null){
					//fncCommandInfoTextSet("m_CrtObject is NULL");
					obj = fncCommandSearchObject(nx, ny);
					fncCommandSetCrtObject(obj);
					fncDrawMain();
				}else{
					m_nCntrKind = fncCommandSearchCntrKind(nx, ny);
					//fncCommandInfoTextSet("m_nTrnsCommand " + m_nCntrKind);					
					if(m_nCntrKind == CNTR_AREAOVER){
						fncCommandSetCrtObject(null);
						fncDrawMain();
					}else{
						m_dMouseBaseX = nx;
						m_dMouseBaseY = ny;
						m_dMoveX = 0; m_dMoveY = 0;
						m_dTimeX = 1.0; m_dTimeY = 1.0;
						m_nMouseCount = 1;
						fncDrawMain();
					}
				}
			}
			break;
		case UPEVENT:
			if(m_nMouseCount == 1){
				fncCommandSetTrns(nx, ny);
				m_CrtObject.fncMoveTrns(m_dMoveX, m_dMoveY);
				m_CrtObject.fncSizeTrns(m_dCntrX, m_dCntrY, m_dTimeX, m_dTimeY);
				fncCommandInitSelectRect();
				m_dMoveX = 0; m_dMoveY = 0;
				m_dTimeX = 1.0; m_dTimeY = 1.0;
				fncDrawMain();
				fncCommandInfoTextSet("m_nTrnsCommand " + m_nCntrKind);	
				m_nMouseCount = 0;
			}
			break;
		case MOVEEVENT:
			if(m_nMouseCount == 1){
				fncCommandSetTrns(nx, ny);
				//fncCommandInfoTextSet("m_dTimeX" + m_dTimeX+",m_dTimeY"+m_dTimeY);
				fncDrawCrtObject();
			}
			break;
		case UPMOVEEVENT:
			if(m_nMouseCount == 0){
				if(m_CrtObject != null){
					if(m_CrtObject.lock == false){
						m_nCntrKind = fncCommandSearchCntrKind(nx, ny);
						fncDrawRectLayer();
					}
				}
			}
			break;
		case UPDATEEVENYT:
			if(m_nMouseCount == 0){
				if(m_CrtObject != null){
					if(m_CrtObject.kind == OBJTEXT){
						m_CrtObject.clrFill = m_nCrtFillClr;
						fncDrawCrtObject();
					}
				}
			}
			break;
		case ENDEVENT:
			break;
	}
	return(0);
}

function fncCommandNewFigure(event, nx, ny)
{
	switch(event){
		case INITEVENT:
			m_nMouseCount = 0;
			break;
		case DOWNEVENT:
			if(m_nMouseCount == 0){
			}
			break;
		case UPEVENT:
			if(m_nMouseCount == 1){
			}
			break;
		case MOVEEVENT:
			if(m_nMouseCount == 1){
			}
			break;
		case UPMOVEEVENT:
			if(m_nMouseCount == 0){
			}
			break;
		case ENDEVENT:
			break;
	}
	return(0);
}
function fncCommandSelectFigure(event, nx, ny)
{
	switch(event){
		case INITEVENT:
			m_nMouseCount = 0;
			break;
		case DOWNEVENT:
			if(m_nMouseCount == 0){
			}
			break;
		case UPEVENT:
			if(m_nMouseCount == 1){
			}
			break;
		case MOVEEVENT:
			if(m_nMouseCount == 1){
			}
			break;
		case UPMOVEEVENT:
			if(m_nMouseCount == 0){
			}
			break;
		case ENDEVENT:
			break;
	}
	return(0);
}
function fncCommandText(event, nx, ny)
{
	var obj;
	switch(event){
		case INITEVENT:
			m_nMouseCount = 0;
			if(m_CrtObject != null){
				fncMouseDisableLayoutArea();
				fncCommandGetCrtObjectElement();
				fncCommandShowInputText();
				m_nMouseCount = 1;
			}
			break;
		case DOWNEVENT:
			break;
		case UPEVENT:
			if(m_nMouseCount == 0){
				fncMouseDisableLayoutArea();
				obj = fncCommandSearchObject(nx, ny);
				if(obj == null || obj.kind != OBJTEXT){
					m_CrtObject = null;
					m_dRectSX = nx;
					m_dRectSY = ny;
					m_dRectEX = m_dRectSX + m_nFontSize * 10;
					m_dRectEY = m_dRectSY + m_nFontSize;
				}else{
					fncCommandSetCrtObject(obj);
					fncCommandGetCrtObjectElement();
				}
				fncCommandShowInputText();
				m_nMouseCount = 1;
			}
			break;
		case KEYINEVENT:
			// if(m_nMouseCount == 1){
				if(m_keyeventCrt.keyCode == KEYCODE_ENTER){
					fncCommandInfoTextSet("mousecount " + m_nMouseCount);
					m_sTextStr = m_inputText.value;
					if(m_CrtObject == null){
						obj = new clsObjText();
						obj.kind = OBJTEXT;
						m_aryObject.push(obj);
					}else{
						obj = m_CrtObject;
					}
					fncCommandSetTextObject(obj);
					fncCommandSetCrtObject(obj);
					fncCommandHideInputText();
					m_inputText.style.visibility = "hidden";
					fncDrawMain();
					fncMouseEnableLayoutArea();
					fncCommandSetCrt(CMDSELECT);
					m_nMouseCount = 0;
				}
			//}
			break;
		case MOVEEVENT:
			break;
		case UPMOVEEVENT:
			break;
		case UPDATEEVENYT:
			if(m_nMouseCount == 0){
				if(m_CrtObject != null){
					if(m_CrtObject.kind == OBJTEXT){
						m_CrtObject.clrFill = m_nCrtFillClr;
						fncDrawCrtObject();
					}
				}
			}else if(m_nMouseCount == 1){
				m_inputText.style.color = m_nCrtFillClr;
				if(m_nCrtFillClr == "#fff"){
					m_inputText.style.backgroundColor = "#888";
				}else{
					m_inputText.style.backgroundColor = "#fff";		
				}
			}
			break;
		case ENDEVENT:
			if(m_nMouseCount == 1){
				fncCommandHideInputText();
				fncMouseEnableLayoutArea();
			}
			break;
	}
	return(0);
}
function fncCommandNewImage(event, nx, ny)
{
	return(0);
}

function fncCommandSelectImage(event, nx, ny)
{
	return(0);
}
function fncCommandSetCrtObject(obj)
{
	m_CrtObject = obj;
	fncCommandInitSelectRect();
}
function fncCommandInitSelectRect()
{
	var mimx;

	if(m_CrtObject == null){
		m_dBaseRectSX = 0.0;
		m_dBaseRectSY = 0.0;
		m_dBaseRectEX = 0.0;
		m_dBaseRectEY = 0.0;
		m_dRectSX = 0.0;
		m_dRectSY = 0.0;
		m_dRectEX = 0.0;
		m_dRectEY = 0.0;
		return;
	}
	mimx = m_CrtObject.fncGetMiniMax();
	m_dBaseRectSX = mimx.sx;
	m_dBaseRectSY = mimx.sy;
	m_dBaseRectEX = mimx.ex;
	m_dBaseRectEY = mimx.ey;
	m_dRectSX = mimx.sx;
	m_dRectSY = mimx.sy;
	m_dRectEX = mimx.ex;
	m_dRectEY = mimx.ey;
}
function fncCommandSearchObject(x, y)
{
	var idx;
	var mimx;

	var max = m_aryObject.length;
	for(idx = max-1; idx >= 0; idx--){
		mimx = m_aryObject[idx].fncGetMiniMax();
		if(mimx.sx < x && x < mimx.ex && mimx.sy < y && y < mimx.ey){
			return(m_aryObject[idx]);
		}
	}
	return(null);
}
function fncCommandSearchCntrKind(x, y)
{
	var cx, cy;
	var len = 16;
	len = fncScrnTrnsRMLen(len);

	if(m_CrtObject.lock == true){
		return(CNTR_AREAOVER);
	}	
	m_dMouseBaseX = x;
	m_dMouseBaseY = y;
	cx = (m_dRectSX + m_dRectEX) / 2;
	cy = (m_dRectSY + m_dRectEY) / 2;
	m_dCntrX = cx;
	m_dCntrY = cy;
	if(x < (m_dRectSX-len) ||  (m_dRectEX+len) < x
	|| y < (m_dRectSY-len) ||  (m_dRectEY+len) < y){
		return(CNTR_AREAOVER);
	} else if(m_dRectEX-len < x && x < m_dRectEX+len
	&&  m_dRectEY-len < y && y < m_dRectEY+len){
		m_dCntrX = m_dRectSX;
		m_dCntrY = m_dRectSY;
		return(CNTR_RIGHT_BOTTOM);		
	} else if(m_dRectEX-len < x && x < m_dRectEX+len
	&& m_dRectSY-len < y && y < m_dRectSY+len){
		m_dCntrX = m_dRectSX;
		m_dCntrY = m_dRectEY;
		return(CNTR_RIGHT_TOP);		
	} else if(m_dRectSX-len < x && x < m_dRectSX+len
	&& m_dRectSY-len < y && y < m_dRectSY+len){
		m_dCntrX = m_dRectEX;
		m_dCntrY = m_dRectEY;
		return(CNTR_LEFT_TOP);		
	} else if(m_dRectSX-len < x && x < m_dRectSX+len
	&& m_dRectEY-len < y && y < m_dRectEY+len){
		m_dCntrX = m_dRectEX;
		m_dCntrY = m_dRectSY;
		return(CNTR_LEFT_BOTTOM);		
	} else if(m_dRectEX-len < x && x < m_dRectEX+len
	&& cy-len < y && y < cy+len){
		m_dCntrX = m_dRectSX;
		return(CNTR_RIGHT_CENTER);		
	} else if(cx-len < x && x < cx+len
	&& m_dRectSY-len < y && y < m_dRectSY+len){
		m_dCntrY = m_dRectEY;
		return(CNTR_CENTER_TOP);		
	} else if(m_dRectSX-len < x && x < m_dRectSX+len
	&& cy-len < y && y < cy+len){
		m_dCntrX = m_dRectEX;
		return(CNTR_LEFT_CENTER);		
	} else if(cx-len < x && x < cx+len
	&& m_dRectEY-len < y && y < m_dRectEY+len){
		m_dCntrY = m_dRectSY;
		return(CNTR_CENTER_BOTTOM);		
	}else{ // sx < x && x < ex && sy < y && y < ey
		return(CNTR_MOVE);
	}
}
function fncCommandSetTrns(x,y)
{
	var dAddX, dAddY;

	if(m_nCntrKind == CNTR_MOVE){
		m_dMoveX = x - m_dMouseBaseX;
		m_dMoveY = y - m_dMouseBaseY;
	}else if(m_nCntrKind == CNTR_LEFT_TOP){
		dAddX = x - m_dMouseBaseX;
		dAddY = y - m_dMouseBaseY;
		m_dTimeX = (m_dBaseRectEX - (m_dBaseRectSX + dAddX)) / (m_dBaseRectEX-m_dBaseRectSX);
		m_dTimeY = (m_dBaseRectEY - (m_dBaseRectSY + dAddY)) / (m_dBaseRectEY-m_dBaseRectSY);
	}else if(m_nCntrKind == CNTR_RIGHT_TOP){
		dAddX = x - m_dMouseBaseX;
		dAddY = y - m_dMouseBaseY;
		m_dTimeX = ((m_dBaseRectEX + dAddX) - m_dBaseRectSX) / (m_dBaseRectEX-m_dBaseRectSX);
		m_dTimeY = (m_dBaseRectEY - (m_dBaseRectSY + dAddY)) / (m_dBaseRectEY-m_dBaseRectSY);
	}else if(m_nCntrKind == CNTR_RIGHT_BOTTOM){
		dAddX = x - m_dMouseBaseX;
		dAddY = y - m_dMouseBaseY;
		m_dTimeX = ((m_dBaseRectEX + dAddX) - m_dBaseRectSX) / (m_dBaseRectEX-m_dBaseRectSX);
		m_dTimeY = ((m_dBaseRectEY + dAddY) - m_dBaseRectSY) / (m_dBaseRectEY-m_dBaseRectSY);
	}else if(m_nCntrKind == CNTR_LEFT_BOTTOM){
		dAddX = x - m_dMouseBaseX;
		dAddY = y - m_dMouseBaseY;
		m_dTimeX = (m_dBaseRectEX - (m_dBaseRectSX + dAddX)) / (m_dBaseRectEX-m_dBaseRectSX);
		m_dTimeY = ((m_dBaseRectEY + dAddY) - m_dBaseRectSY) / (m_dBaseRectEY-m_dBaseRectSY);
	}else if(m_nCntrKind == CNTR_LEFT_CENTER){
		dAddX = x - m_dMouseBaseX;
		m_dTimeX = (m_dBaseRectEX - (m_dBaseRectSX + dAddX)) / (m_dBaseRectEX-m_dBaseRectSX);
	}else if(m_nCntrKind == CNTR_RIGHT_CENTER){
		dAddX = x - m_dMouseBaseX;
		m_dTimeX = ((m_dBaseRectEX + dAddX) - m_dBaseRectSX) / (m_dBaseRectEX-m_dBaseRectSX);
	}else if(m_nCntrKind == CNTR_CENTER_TOP){
		dAddY = y - m_dMouseBaseY;
		m_dTimeY = (m_dBaseRectEY - (m_dBaseRectSY + dAddY)) / (m_dBaseRectEY-m_dBaseRectSY);
	}else if(m_nCntrKind == CNTR_CENTER_BOTTOM){
		dAddY = y - m_dMouseBaseY;
		m_dTimeY = ((m_dBaseRectEY + dAddY) - m_dBaseRectSY) / (m_dBaseRectEY-m_dBaseRectSY);
	}

}
function fncCommandTrnsXY(axy)
{
	if(m_dMoveX != 0){
		axy.x = axy.x + m_dMoveX;
	}
	if(m_dMoveY != 0){
		axy.y = axy.y + m_dMoveY;
	}
	if(m_dTimeX != 1.0){
		axy.x = (axy.x - m_dCntrX) * m_dTimeX + m_dCntrX;
	}
	if(m_dTimeY != 1.0){
		axy.y = (axy.y - m_dCntrY) * m_dTimeY + m_dCntrY;
	}
	return(axy);
}
function fncCommandGetCrtObjectElement()
{
	m_nCrtFillClr = m_CrtObject.clrFill;
	m_nVHKind = m_CrtObject.vhkind;
	if(m_nVHKind == YOKO){
		m_nFontSize = m_dRectEY - m_dRectSY;
	}else{
		m_nFontSize = m_dRectEX - m_dRectSX;		
	}
	m_sFontFamily = m_CrtObject.sFontFamily;
	m_sTextStr = m_CrtObject.text;
}
function fncCommandShowInputText()
{
	var fs;
	var sx, sy, ex, ey;

	fs = m_nFontSize;
	fs = fncScrnTrnsMRLen(fs);
	fs = parseInt(fs);
	if(fs < 15){
		fs = 15;
	}

	sx = m_dRectSX;
	sy = m_dRectSY;
	ex = m_dRectEX;
	ey = m_dRectEY;
	sx = fncScrnTrnsMRX(sx);
	sy = fncScrnTrnsMRX(sy);
	ex = fncScrnTrnsMRX(ex);
	ey = fncScrnTrnsMRX(ey);
	wd = ex - sx;
	hi = ey - sy;
	sx = sx + m_nInputBaseLeft;
	sy = sy + m_nInputBaseTop;
	divLayout = document.getElementById("divLayout");
	m_inputText.style.visibility = "visible";
	m_inputText.style.color = m_nCrtFillClr;
	if(m_nCrtFillClr == "#fff"){
		m_inputText.style.backgroundColor = "#888";
	}else{
		m_inputText.style.backgroundColor = "#fff";		
	}
	m_inputText.style.left = sx + "px";
	m_inputText.style.top = sy + "px";
	m_inputText.style.font = fs + "px '"+m_sFontFamily+"'";
	if(m_sTextStr == ""){
		m_inputText.cols =10;
		m_inputText.value = "";
	}else{
		m_inputText.cols = (m_sTextStr.length + 2) * 2;
		m_inputText.value = m_sTextStr;
	}
	m_nCommandTimer = setTimeout(fncCommandTimer, 500);
}
function fncCommandTimer()
{
	clearTimeout(m_nCommandTimer);
	m_inputText.focus();
	if(m_sTextStr != ""){
		//m_inputText.select();
	}
}
function fncCommandHideInputText()
{
	m_inputText.style.left = m_nInputBaseLeft + "px";
	m_inputText.style.top = m_nInputBaseTop + "px";
	m_inputText.style.visibility = "hidden";
}
function fncCommandSetTextObject(obj)
{
	var len = m_sTextStr.length;
	if(m_nVHKind == YOKO){
		m_dRectEX = m_dRectSX + m_nFontSize * len;
		m_dRectEY = m_dRectSY + m_nFontSize;
	}else{
		m_dRectEX = m_dRectSX + m_nFontSize;
		m_dRectEY = m_dRectSY + m_nFontSize * len;
	}
	obj.fncSetAtrXY(0, 0, m_dRectSX, m_dRectSY);
	obj.fncSetAtrXY(1, 1, m_dRectEX, m_dRectSY);
	obj.fncSetAtrXY(2, 1, m_dRectEX, m_dRectEY);
	obj.fncSetAtrXY(3, 1, m_dRectSX, m_dRectEY);
	obj.clrFill = m_nCrtFillClr;
	obj.vhkind = m_nVHKind;
	obj.sFontFamily = m_sFontFamily;
	obj.text = m_sTextStr;
	return(obj);
}
function fncCommandInfoTextSet(msg)
{
	m_lblInfo.textContent = msg;
}
function fncCmdDebugPos(dx, dy)
{
    var sx = 0.0;
    var sy = 0.0;
    var ex = 0.0;
    var ey = 0.0;

	sx = dx - m_dLimit;
	sy = dy - m_dLimit;
	ex = dx + m_dLimit;
	ey = dy + m_dLimit;
	sx = fncScrnTrnsMRX(sx);
	sy = fncScrnTrnsMRY(sy);
	ex = fncScrnTrnsMRX(ex);
	ey = fncScrnTrnsMRY(ey);
	fncCtxSetPen(m_ctxPaper, "#000000", 1);
	fncCtxLine(m_ctxPaper, sx, sy, ex, ey);
	fncCtxLine(m_ctxPaper, sx, ey, ex, sy);
}

function fncCtxSetPen(ctx, color, thick)
{
	ctx_sStrokeColor = color
	ctx_nStrokeThick = thick;
	ctx.strokeStyle = ctx_sStrokeColor;
	ctx.lineWidth = ctx_nStrokeThick;
}

function fncCtxLine(ctx, sx, sy, ex, ey)
{
	ctx.beginPath();
	ctx.moveTo(sx,sy);
	ctx.lineTo(ex,ey);
	ctx.stroke();
}
