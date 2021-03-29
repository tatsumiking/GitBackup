
var m_ctxLayout_b;
var m_ctxLayout_c;
var m_ctxLayout_f;
var m_ctxLayout_s;

var m_nLayer;

function fncDrawMain()
{
	var idx;
	m_nLayer = 0;
	m_ctxLayout_b.clearRect(0, 0, m_cnvsLayoutWidth, m_cnvsLayoutHeight);
	m_ctxLayout_c.clearRect(0, 0, m_cnvsLayoutWidth, m_cnvsLayoutHeight);
	m_ctxLayout_f.clearRect(0, 0, m_cnvsLayoutWidth, m_cnvsLayoutHeight);
	m_ctxLayout_s.clearRect(0, 0, m_cnvsLayoutWidth, m_cnvsLayoutHeight);
	var max = m_aryObject.length;
	for(idx = 0; idx < max; idx++){
		if(m_aryObject[idx] == m_CrtObject){
			m_nLayer = 1;
		}
		switch(m_aryObject[idx].kind){
		case OBJIMAGE:
			fncDrawImage(m_aryObject[idx]);
			break;
		case OBJTEXT:
			fncDrawText(m_aryObject[idx]);
			break;
		case OBJPATH:
			fncDrawPath(m_aryObject[idx]);
			break;
		}
		if(m_aryObject[idx] == m_CrtObject){
			m_nLayer = 2;
		}
	}
	fncDrawSelectRect();
}
function fncDrawCrtObject()
{
	m_nLayer = 1;
	m_ctxLayout_c.clearRect(0, 0, m_cnvsLayoutWidth, m_cnvsLayoutHeight);
	m_ctxLayout_s.clearRect(0, 0, m_cnvsLayoutWidth, m_cnvsLayoutHeight);
	switch(m_CrtObject.kind){
		case OBJIMAGE:
			fncDrawImage(m_CrtObject);
			break;
		case OBJTEXT:
			fncDrawText(m_CrtObject);
			break;
		case OBJPATH:
			fncDrawPath(m_CrtObject);
			break;
	}
	fncDrawSelectRect();
}
function fncDrawRectLayer()
{
	m_ctxLayout_s.clearRect(0, 0, m_cnvsLayoutWidth, m_cnvsLayoutHeight);
	fncDrawSelectRect();
}
function fncDrawImage(imageobj)
{
	var sxy, exy
	var ctx;
	var sx, sy;
	var ex, ey;
	var wd, hi;

	sxy = imageobj.fncGetAtrXY(0);
	exy = imageobj.fncGetAtrXY(2);
    if(m_nLayer == 0){
		ctx = m_ctxLayout_b;
	}else if(m_nLayer == 1){
		sxy = fncCommandTrnsXY(sxy);
		exy = fncCommandTrnsXY(exy);
		ctx = m_ctxLayout_c;
	}else if(m_nLayer == 2){
		ctx = m_ctxLayout_f;
	}
	sxy = fncDrawTrnsXYMMToRltv(sxy);
	exy = fncDrawTrnsXYMMToRltv(exy);
	sx = sxy.x; sy = sxy.y; 
	ex = exy.x; ey = exy.y;
	wd = ex - sx;
	hi = ey - sy;

	ctx.drawImage(imageobj.image, sx, sy, wd, hi);
}
function  fncDrawText(textobj)
{
	var sxy, exy;
	var sp;
	var ctx;
	var sx, sy;
	var ex, ey;
	var wd, hi;
	var max, idx;
	var sCh;

	sxy = textobj.fncGetAtrXY(0);
	exy = textobj.fncGetAtrXY(2);
    if(m_nLayer == 0){
		ctx = m_ctxLayout_b;
	}else if(m_nLayer == 1){
		sxy = fncCommandTrnsXY(sxy);
		exy = fncCommandTrnsXY(exy);
		ctx = m_ctxLayout_c;
	}else if(m_nLayer == 2){
		ctx = m_ctxLayout_f;
	}
	sp = fncDrawTrnsLenMMToRltv(textobj.space);
	sxy = fncDrawTrnsXYMMToRltv(sxy);
	exy = fncDrawTrnsXYMMToRltv(exy);
	sx = sxy.x; sy = sxy.y; 
	ex = exy.x; ey = exy.y;
	wd = ex - sx;
	hi = ey - sy;
	ctx.fillStyle = textobj.clrFill;
	ctx.strokeStyle = textobj.clrStroke;
	ctx.lineWidth = textobj.thick;
	if(textobj.yktt == YOKO){
		ctx.font = hi+"px"+"'"+textobj.font+"'";	
	}else{
		ctx.font = wd+"px"+"'"+textobj.font+"'";	
	}
	for(idx = 0; idx < max; idx++){
		sCh = textobj.text.substring(idx, 1);
		ctx.fillText(sCh, sx, sy);
		if(textobj.yktt == YOKO){
			sx = sx + hi + sp;
		}else{
			sy = sy + wd + sp;
		}
	}
}
function  fncDrawPath(pathobj)
{
	var ctx;
	var max, idx;
	var axy1, axy2, axy3;

    if(m_nLayer == 0){
		ctx = m_ctxLayout_b;
	}else if(m_nLayer == 1){
		ctx = m_ctxLayout_c;
	}else if(m_nLayer == 2){
		ctx = m_ctxLayout_f;
	}
	ctx.fillStyle = pathobj.clrFill;
	ctx.strokeStyle = pathobj.clrStroke;
	ctx.lineWidth = pathobj.thick;

	ctx.beginPath();
	max = pathobj.pcnt;
	idx = 0;
	while(true){
		if(max <= idx){
			break;
		}
		axy1 = pathobj.fncGetAtrXY(idx);
		if(m_nLayer == 1){
			axy1 = fncCommandTrnsXY(axy1);
		}
		axy1 = fncDrawTrnsXYMMToRltv(axy1);
		if(axy1.atr == ATRMOVE){
			ctx.moveTo(axy1.x, axy1.y);
			idx++;
		}else if(axy1.atr == ATRLINE){
			ctx.lineTo(axy1.x, axy1.y);
			idx++;
		}else if(axy1.atr == ATRCURVE){
			axy2 = pathobj.fncGetAtrXY(idx+1);
			axy3 = pathobj.fncGetAtrXY(idx+2);
			if(m_nLayer == 1){
				axy2 = fncCommandTrnsXY(axy2);
				axy3 = fncCommandTrnsXY(axy3);
			}
			axy2 = fncDrawTrnsXYMMToRltv(axy2);
			axy3 = fncDrawTrnsXYMMToRltv(axy3);
			ctx.bezierCurveTo(axy1.x, axy1.y, axy2.x, axy2.y, axy3.x, axy3.y);
			idx = idx + 3;
		}else{
			idx++;
		}
	}
	ctx.closePath();
	ctx.stroke();
	ctx.fill();
}
function fncDrawSelectRect()
{
	var ctx;
	var sxy, exy;
	var cx, cy;
	
	if (m_dRectSX == 0 && m_dRectSY == 0 && m_dRectEX == 0 && m_dRectEY == 0)
	{
		return;
	}
	ctx = m_ctxLayout_s;
	sxy = new CAtrXY();
	exy = new CAtrXY();
	sxy.x = m_dRectSX;
	sxy.y = m_dRectSY;
	exy.x = m_dRectEX;
	exy.y = m_dRectEY;
	sxy = fncCommandTrnsXY(sxy);
	exy = fncCommandTrnsXY(exy);
	sxy = fncDrawTrnsXYMMToRltv(sxy);
	exy = fncDrawTrnsXYMMToRltv(exy);
	cx = (sxy.x + exy.x) / 2;
	cy = (sxy.y + exy.y) / 2;

	ctx.fillStyle = "";
	ctx.strokeStyle = "#F00";
	ctx.lineWidth = 2;

	dr = 6;
	fncDrawBoxs(ctx, sxy.x, sxy.y, exy.x, exy.y);
	fncDrawCrcl(ctx, cx, cy, dr);
	fncDrawCrcl(ctx, sxy.x, sxy.y, dr);
	fncDrawCrcl(ctx, cx, sxy.y, dr);
	fncDrawCrcl(ctx, exy.x, sxy.y, dr);
	fncDrawCrcl(ctx, exy.x, cy, dr);
	fncDrawCrcl(ctx, exy.x, exy.y, dr);
	fncDrawCrcl(ctx, cx, exy.y, dr);
	fncDrawCrcl(ctx, sxy.x, exy.y, dr);
	fncDrawCrcl(ctx, sxy.x, cy, dr);

	dr = 10;
	switch(m_nCntrKind){
	case CNTR_MOVE:
		fncDrawCrcl(ctx, cx, cy, dr);
		break;
	case CNTR_LEFT_TOP:
		fncDrawCrcl(ctx, sxy.x, sxy.y, dr);
		break;
	case CNTR_CENTER_TOP:
		fncDrawCrcl(ctx, cx, sxy.y, dr);
		break;
	case CNTR_RIGHT_TOP:
		fncDrawCrcl(ctx, exy.x, sxy.y, dr);
		break;
	case CNTR_RIGHT_CENTER:
		fncDrawCrcl(ctx, exy.x, cy, dr);
		break;
	case CNTR_RIGHT_BOTTOM:
		fncDrawCrcl(ctx, exy.x, exy.y, dr);
		break;
	case CNTR_CENTER_BOTTOM:
		fncDrawCrcl(ctx, cx, exy.y, dr);
		break;
	case CNTR_LEFT_BOTTOM:
		fncDrawCrcl(ctx, sxy.x, exy.y, dr);
		break;
	case CNTR_LEFT_CENTER:
		fncDrawCrcl(ctx, sxy.x, cy, dr);
		break;
	}
}
function fncDrawBoxs(ctx, sx, sy, ex, ey)
{
	ctx.beginPath();
	ctx.moveTo(sx, sy);
	ctx.lineTo(sx, ey);
	ctx.lineTo(ex, ey);
	ctx.lineTo(ex, sy);
	ctx.lineTo(sx, sy);
	ctx.closePath();
	ctx.stroke();
}
function fncDrawCrcl(ctx, cx, cy, dr)
{
	ctx.beginPath();
	ctx.arc(cx, cy, dr, 0, Math.PI*2);
	ctx.stroke();
}
function fncDrawTrnsXYMMToRltv(inxy)
{
	var outxy = new CAtrXY();
    outxy.x  = fncScrnTrnsMRX(inxy.x);
    outxy.y  = fncScrnTrnsMRY(inxy.y);
	return(outxy);
}
function  fncDrawTrnsLenMMToRltv(len)
{
	var retlen;

    retlen = fncScrnTrnsMRLen(len);
	return(retlen);
}

