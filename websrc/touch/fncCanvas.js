
var mlib_dFontSize;
var mlib_szFontName;

function fncCtxSetFont(ctx, fontsize, fontname)
{
	ctx.font = fontsize+"px"+"'"+fontname+"'";
	mlib_dFontSize = fontsize;
	mlib_szFontName = fontname;
}
function fncStroke(ctx, clr, thick)
{
	ctx.strokeStyle = clr;
	ctx.lineWidth = thick;
	ctx.stroke();
}
function fncFill(ctx, clr)
{
	ctx.fillStyle = clr;
	ctx.fill();
}
function fncCtxTategaki(ctx, sx, sy, str, size)
{
	var	x, y;
	var	i, max;
	var	sstr;

	x = sx;
	y = sy + size;
	max = str.length;
	for(i = 0; i < max; i++){
		sstr = str.substr(i, 1);
		ctx.fillText(sstr, x, y);
		y += size;
	}
}

function fncCtxEqualString(ctx, sx, sy, width, str)
{
	var size, space, tmp;
	var i, max;
	var sstr;
	var ey;

	ey = sy + mlib_dFontSize;
	max = str.length;
	size = ctx.measureText(str).width;
	if(max == 1){
		size = ctx.measureText(str).width;
		space = (width-size) / 2;
		ctx.fillText(str, sx+space, ey);
	} else if((size) < width){
		space = (width - size) / (max-1);
		for(i = 0; i < max; i++){
			sstr = str.substr(i, 1);
			ctx.fillText(sstr, sx, ey);
			size = ctx.measureText(sstr).width;
			sx = sx+size+space;
		}
	} else {
		tmp = mlib_dFontSize * width / size;
		ctx.font = tmp+"px'"+mlib_szFontName+"'";
		ctx.fillText(str, sx, ey);
		ctx.font = mlib_dFontSize+"px'"+mlib_szFontName+"'";
	}
}

function fncCtxLeftString(ctx, sx, sy, str)
{
	var ey;

	ey = sy + mlib_dFontSize;
	ctx.fillText(str, sx, ey);
}

function fncCtxRightString(ctx, ex, sy, str)
{
	var len, sx, ey;
	ey = sy + mlib_dFontSize;
	len = ctx.measureText(str).width;
	sx = ex - len;
	ctx.fillText(str, sx, ey);
}

function fncCtxRightEqualString(ctx, ex, sy, size, str)
{
	var len, sx, ey;
	var time;

	ctx.save();
	ey = sy + mlib_dFontSize;
	len = ctx.measureText(str).width;
	sx = ex - size;
	time = size / len;
	sx = sx / time;
	ctx.setTransform(time, 0, 0, 1, 0, 0);
	ctx.fillText(str, sx, ey);
	ctx.restore();
}

function fncCtxTopString(ctx, sx, sy, str)
{
	var size,ey;
	var i, max;
	var sstr;

	size = ctx.measureText("亜").width;
	max = str.length;
	ey = sy+mlib_dFontSize;
	for(i = 0; i < max; i++){
		sstr = str.substr(i, 1);
		ctx.fillText(sstr, sx, ey);
		ey = ey + mlib_dFontSize;
	}
}
function fncCtxCnterTextBoxs(ctx, cx, cy, str, clr)
{
	var	len;

	len = ctx.measureText(str).width;
	x = cx - len/2;
	y = cy + mlib_dFontSize/2;

	ctx.fillStyle = clr;
	ctx.fillText(str, x, y);
}
function fncCtxImage(ctx, img, sx, sy, wd, hi)
{
	ctx.drawImage(img, sx, sy, wd, hi);
}

function fncCtxRoundBoxs(ctx, sx, sy, ex, ey)
{
	var tsx, tsy, tex, tey;

	tsx = sx + 10;
	tsy = sy + 10;
	tex = ex - 10;
	tey = ey - 10;
	ctx.beginPath();
	ctx.moveTo(tsx,sy);
	ctx.lineTo(tex,sy);
	ctx.quadraticCurveTo(ex, sy, ex, tsy);
	ctx.lineTo(ex,tey);
	ctx.quadraticCurveTo(ex, ey, tex, ey);
	ctx.lineTo(tsx,ey);
	ctx.quadraticCurveTo(sx, ey, sx, tey);
	ctx.lineTo(sx,tsy);
	ctx.quadraticCurveTo(sx, sy, tsx, sy);
	ctx.closePath();
}

function fncCtxBoxs(ctx, sx, sy, ex, ey)
{
	ctx.beginPath();
	ctx.moveTo(sx,sy);
	ctx.lineTo(ex,sy);
	ctx.lineTo(ex,ey);
	ctx.lineTo(sx,ey);
	ctx.lineTo(sx,sy);
	ctx.closePath();
}

function fncCtxLine(ctx, sx, sy, ex, ey)
{
	ctx.beginPath();
	ctx.moveTo(sx,sy);
	ctx.lineTo(ex,ey);
	ctx.stroke();
}

