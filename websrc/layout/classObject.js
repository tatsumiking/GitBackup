// Object class definition
const YOKO = 0;
const TATE = 1;

const LEFTTOP = 0;
const CENTER = 1;
const RIGHTBOTTOM = 2;
const EQUAL = 3;

class  CImageEx extends Image
{
	constructor(){
		super();
		this.basename = "";
		this.flag = 0; // 0:NoLoad 1:Loaded 2:Draw
		this.sx = 0;
		this.sy = 0;
		this.wd = 0;
		this.hi = 0;
	}
}
class CPoint
{
	constructor(){
		this.atr = 0;
		this.x = 0;
		this.y = 0;
	}
}

class CRect
{
	constructor(){
		this.left = 0;
		this.right = 0;
		this.top = 0;
		this.bottom = 0;
	}
}
class CAtrXY
{
	constructor(){
		this.atr = 0;
		this.x = 0;
		this.y = 0;
	}
}

class CMiniMax
{
	constructor(){
		this.sx = 0;
		this.sy = 0;
		this.ex = 0;
		this.ey = 0;
	}
}
class clsObject
{
	constructor(){
		// プロパティ
		this.kind = 0; // OBJTEXT,OBJPLGN,OBJIMAGE,OBJGROUP
		this.objno = 0; // ボックスのインデックス
		this.lock = false;
		this.aryAtrXY = new Array(0); // PLGN以外は左上座標、右下座標を保持
		this.pcnt = 0; // PLGN以外は2固定ectguGetMiniMax = function(mimx)
		this.mimx = new CMiniMax();
	}
	fncGetMiniMax()
	{
		var pp;
	
		var axy = this.fncGetAtrXY(0);
		this.mimx.sx = axy.x;
		this.mimx.sy = axy.y;
		this.mimx.ex = axy.x;
		this.mimx.ey = axy.y;
		for(pp = 1; pp < this.pcnt; pp++){
			axy = this.fncGetAtrXY(pp);
			if(axy.x < this.mimx.sx){
				this.mimx.sx = axy.x;
			}
			if(axy.y < this.mimx.sy){
				this.mimx.sy = axy.y;
			}
			if(this.mimx.ex < axy.x){
				this.mimx.ex = axy.x;
			}
			if(this.mimx.ey < axy.y){
				this.mimx.ey = axy.y;
			}
		}
		var retmimx = new CMiniMax();
		retmimx.sx = this.mimx.sx;
		retmimx.sy = this.mimx.sy;
		retmimx.ex = this.mimx.ex;
		retmimx.ey = this.mimx.ey;
		return(retmimx);
	}
	fncMoveTrns(movex, movey)
	{
		var pp;
		var axy;
	
		if(movex == 0 && movey == 0){
			return;
		}
		axy = this.fncGetAtrXY(0);
		for(pp = 0; pp < this.pcnt; pp++){
			axy = this.fncGetAtrXY(pp);
			axy.x = axy.x + movex;
			axy.y = axy.y + movey;
			this.fncSetAtrXY(pp, axy.atr, axy.x, axy.y);
		}
	}
	fncSizeTrns(cntrx, cntry, timex, timey)
	{
		var pp;
		var axy;
	
		if(timex == 1.0 && timey == 1.0){
			return;
		}
		axy = this.fncGetAtrXY(0);
		for(pp = 0; pp < this.pcnt; pp++){
			axy = this.fncGetAtrXY(pp);
			axy.x = (axy.x - cntrx) * timex + cntrx;
			axy.y = (axy.y - cntry) * timey + cntry;
			this.fncSetAtrXY(pp, axy.atr, axy.x, axy.y);
		}
	}
	fncSetAtrXY(pp, atr, x, y)
	{
		var axy;
		var sp, max;
	
		if(this.pcnt <= pp){
			if(this.aryAtrXY.length <= pp){
				max = pp+1;
				for(sp = this.aryAtrXY.length; sp < max; sp++){
					axy = new CAtrXY();
					axy.atr = -1; axy.x = -1; axy.y = -1;
					this.aryAtrXY.push(axy);
				}
			}
			this.pcnt = max;
		}
		this.aryAtrXY[pp].atr = atr;
		this.aryAtrXY[pp].x = x;
		this.aryAtrXY[pp].y = y;

		axy = new CAtrXY();
		axy.atr = atr;
		axy.x = x;
		axy.y = y;
	}
	fncGetAtrXY(pp)
	{
		var axy;
	
		if(this.pcnt <= pp){
			axy = new CAtrXY();
			return(axy);
		}else{
			axy = new CAtrXY();
			axy.atr = this.aryAtrXY[pp].atr;
			axy.x = this.aryAtrXY[pp].x;
			axy.y = this.aryAtrXY[pp].y;
			return(axy);
		}
	}
}
class clsObjFigure extends clsObject
{
	constructor(){
		super();
		this.thick = 1.0; // 線幅,サイズ単位mm 画像透過率　1.0～0.1
		this.clrStroke = "#000000"; // アウトラインの色
		this.clrFill = "#000000"; // 塗りつぶしの色
	}
}
class  clsObjText extends clsObjFigure
{
	constructor(){
		super();
		this.vhkind = YOKO; // 縦書き、横書き、OBJTEXTのみ使用
		this.mode = LEFTTOP; // 右寄せ(上寄せ)、左寄せ(下寄せ)、中央寄せ、均等
		this.space = 0; // 文字サイズに対する％
		this.sFontFamily = "ＭＳ ゴシック"; // 書体(OBJTEXTのみ使用)
		this.nFontSize = 20;
		this.text = "";	// 文字列を格納
	}
}
class clsObjImage extends clsObject
{
	constructor(){
		super();
		this.image = new Image();
	}
}
function ObjFigu()
{
	// プロパティ
	this.kind = 0; // OBJTEXT,OBJPLGN,OBJIMAGE,OBJGROUP
	this.aryObjFigu;
	this.idx = 0; // 入力ボックスのインデックス
	this.font = "ＭＳ ゴシック"; // 書体(OBJTEXTのみ使用)
	this.yktt = YOKO; // 縦書き、横書き、OBJTEXTのみ使用
	this.mode = LEFT; // 右寄せ、左寄せ、中央寄せ、均等、上寄せ、下寄せ
	this.type = 0; // 0：１入力 1：複数行入力 2:空白入力
	this.space = 0;
	this.def = "";	// OBJTEXTは文字列を格納 OBJIMAGEはファイル名を格納
	this.aryAtrXY = Array(0); // PLGN以外は左上座標、右下座標を保持
	this.pcnt = 0;　 // PLGN以外は2固定
	this.arcr = 0; // 角Ｒ値
	this.thick = 1.0; // 線幅,サイズ単位mm 画像透過率　1.0～0.1
	this.clrStroke = "#000000"; // アウトラインの色
	this.clrFill = "#000000"; // 塗りつぶしの色
}
ObjFigu.prototype.fncResetArea = function(dmimx)
{
	var smimx;
	var ssubx, ssuby, dsubx, dsuby;
	var scx, scy, dcx, dcy;
	var timex, timey;
	var max, idx, pp;
	var axy;

	smimx = new CMiniMax();
	if(this.kind == OBJIMAGE && this.pcnt == 4){
		axy = this.fncGetAtrXY(1);
		smimx.sx = axy.x;
		smimx.sy = axy.y;
		axy = this.fncGetAtrXY(2);
		smimx.ex = axy.x;
		smimx.ey = axy.y;
	}else{
		smimx = this.fncObjFiguGetMiniMax(smimx);
	}
	scx = smimx.sx;
	scy = smimx.sy;
	dcx = dmimx.sx;
	dcy = dmimx.sy;
	ssubx = smimx.ex - smimx.sx;
	dsubx = dmimx.ex - dmimx.sx;
	ssuby = smimx.ey - smimx.sy;
	dsuby = dmimx.ey - dmimx.sy;

	if(dsubx == 0 && ssubx == 0){
		timex = 1.0;
	}else{
		timex = parseFloat(dsubx) / parseFloat(ssubx);
	}

	if(dsuby == 0 && ssuby == 0){
		timey = 1.0;
	}else{
		timey = parseFloat(dsuby) / parseFloat(ssuby);
	}

	switch(this.kind){
	case OBJGROUP:
		max = this.aryObjFigu.length;
		for(idx = 0; idx < max; idx++){
			smimx = this.aryObjFigu[idx].fncObjFiguGetMiniMax(smimx);
			smimx.sx = parseInt(parseFloat((smimx.sx - scx) * timex) + dcx);
			smimx.sy = parseInt(parseFloat((smimx.sy - scy) * timey) + dcy);
			smimx.ex = parseInt(parseFloat((smimx.ex - scx) * timex) + dcx);
			smimx.ey = parseInt(parseFloat((smimx.ey - scy) * timey) + dcy);
			this.aryObjFigu[idx].fncResetArea(smimx);
		}
		this.fncObjFiguSetAtrXY(0, 0, dmimx.sx, dmimx.sy);
		this.fncObjFiguSetAtrXY(1, 0, dmimx.ex, dmimx.ey);
		break;
	case OBJPLGN:
		for(pp = 0; pp < this.pcnt; pp++){
			axy = this.fncObjFiguGetAtrXY(pp);
			axy.x = parseInt(parseFloat((axy.x - scx) * timex) + dcx);
			axy.y = parseInt(parseFloat((axy.y - scy) * timey) + dcy);
			this.fncObjFiguSetAtrXY(pp, axy.atr, axy.x, axy.y);
		}
		break;
	case OBJIMAGE:
		for(pp = 0; pp < this.pcnt; pp++){
			axy = this.fncObjFiguGetAtrXY(pp);
			axy.x = parseInt(parseFloat((axy.x - scx) * timex) + dcx);
			axy.y = parseInt(parseFloat((axy.y - scy) * timey) + dcy);
			this.fncObjFiguSetAtrXY(pp, axy.atr, axy.x, axy.y);
		}
		break;
	default:
		this.fncObjFiguSetAtrXY(0, 0, dmimx.sx, dmimx.sy);
		this.fncObjFiguSetAtrXY(1, 0, dmimx.ex, dmimx.ey);
		break;
	}
}

ObjFigu.prototype.fncObjFiguSetAtrXY = function(pp, atr, x, y)
{
	var axy;
	var sp, max;

	if(this.pcnt <= pp){
		if(this.aryAtrXY.length <= pp){
			max = pp+1;
			for(sp = this.aryAtrXY.length; sp < max; sp++){
				axy = new StAtrXY();
				axy.atr = -1; axy.x = -1; axy.y = -1;
				this.aryAtrXY.push(axy);
			}
		}else{
			axy = this.aryAtrXY[pp];
		}
		this.pcnt = pp + 1;
	}else{
		axy = this.aryAtrXY[pp];
	}		
	axy.atr = atr;
	axy.x = x;
	axy.y = y;
}

ObjFigu.prototype.fncObjFiguGetAtrXY = function(pp)
{
	var axy;

	if(this.pcnt <= pp){
		axy = new StAtrXY();
		axy.atr = -1;
		axy.x = -1;
		axy.y = -1;
		return(axy);
	}else{
		axy = this.aryAtrXY[pp];
		return(axy);
	}
}
