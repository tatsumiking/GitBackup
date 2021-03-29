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
class clsObjClr extends clsObject
{
	constructor(){
		super();
		this.thick = 0.0; // 線幅,サイズ単位mm 画像透過率　1.0～0.1
		this.clrStroke = "#000000"; // アウトラインの色
		this.clrFill = "#000000"; // 塗りつぶしの色
	}
}
class  clsObjText extends clsObjClr
{
	constructor(){
		super();
		this.vhkind = YOKO; // 縦書き、横書き、OBJTEXTのみ使用
		this.mode = LEFTTOP; // 右寄せ(上寄せ)、左寄せ(下寄せ)、中央寄せ、均等
		this.space = 0; // 文字サイズに対する％
		this.sFontFamily = "ＭＳ ゴシック"; // 書体(OBJTEXTのみ使用)
		this.text = "";	// 文字列を格納
	}
}
class  clsObjFigu extends clsObjClr
{
	constructor(){
		super();
		this.aryPath = new Array(0)
	}
}
class clsObjImage extends clsObject
{
	constructor(){
		super();
		this.flag = 0;
		this.image = new Image();
	}
}
