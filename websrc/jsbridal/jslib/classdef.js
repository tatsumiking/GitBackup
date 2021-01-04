
function Point()
{
	this.atr;
	this.x;
	this.y;
}

function Rect()
{
	this.left;
	this.right;
	this.top;
	this.bottom;
}

function StAtrXY()
{
	this.atr;
	this.x;
	this.y;
}

function StMiniMax()
{
	this.sx;
	this.sy;
	this.ex;
	this.ey;
}

function clsTable()
{
	this.x = 0;
	this.y = 0;
	this.name = "";
	this.left = new Array(0,0,0,0,0,0,0);
	this.right = new Array(0,0,0,0,0,0,0);
}
function clsTableLine()
{
	this.tables = new Array();
}
function clsTableLayout()
{
	this.tlines = new Array(6);
	this.tlines[0] = new clsTableLine();
	this.tlines[1] = new clsTableLine();
	this.tlines[2] = new clsTableLine();
	this.tlines[3] = new clsTableLine();
	this.tlines[4] = new clsTableLine();
	this.tlines[5] = new clsTableLine();
}
function clsGestSit()
{
	this.id = 0;
	this.name = "";
	this.tno = 0;
	this.sno = 0;
}
function clsTableArea()
{
	this.nLine = 0;
	this.nClum = 0;
	this.sx = 0;
	this.sy = 0;
	this.ex = 0;
	this.ey = 0;
}
