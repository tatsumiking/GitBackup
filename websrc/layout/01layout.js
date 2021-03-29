
const INITEVENT = 11;
const DOWNEVENT = 12;
const UPEVENT = 13;
const MOVEEVENT = 14;
const UPMOVEEVENT=15;
const KEYINEVENT = 19;
const ENDEVENT = 99;

const CMDLOAD = 1;
const CMDSAVE = 2;
const CMDSAMPLESELECT = 3;
const CMDNEWIMAGE = 12;
const CMDNEWTEXT = 13;
const CMDNEWPATH = 14;
const CMDNEWFIGURE = 15;
const CMDSELECT = 20;
const CMDSELECTPARTS = 11;
const CMDSELECTIMAGE = 12;
const CMDSELECTTEXT = 13;
const CMDSELECTPATH = 14;
const CMDSELECTFIGURE = 15;

const OBJPART = 3;
const OBJIMAGE = 4;
const OBJTEXT = 5;
const OBJPATH = 6;

const ATRMOVE = 0;
const ATRLINE = 1;
const ATRCURVE = 2;

const YOUSIYOKO = 0;
const YOUSITATE = 1;

const YOKOFRAME = 0;
const TATEFRAME = 1;
const PARTS = 2;

const SIZEA4LONG = 297;
const SIZEA4SHORT = 210;

var m_cnvsSampleWidth;
var m_cnvsSampleHeight;

var m_aryTTFrameImage = new Array();
var m_aryYKFrameImage = new Array();
var m_aryPartsImage = new Array();
var m_arySampleImage;
var m_sSelectImage;

var m_divLayoutAreaWidth;
var m_divLayoutAreaHeight;

var m_cnvsLayoutWidth;
var m_cnvsLayoutHeight;


var m_nOrientation;
var m_nSelectKind;

var m_keyeventCrt;
var m_nCrtClrIdx;
var m_aryClrTable = new Array();
var m_aryShClrTable = new Array();

var m_aryObject = new Array();
var m_nVHKind;
var m_sFontFamily;
var m_nFontSize;
var m_divText;
var m_inputText;
var m_nInputBaseLeft;
var m_nInputBaseTop;
var m_lblInfo;

function fncInit()
{
	m_nOrientation = YOUSIYOKO;
	m_nSelectKind = YOKOFRAME;
	
	m_arySampleImage = m_aryYKFrameImage;

	var btnUNDO = document.getElementById("btnUNDO");
	btnUNDO.onclick = onClickUndoLoad;
	var btnREDO = document.getElementById("btnREDO");
	btnREDO.onclick = onClickRedoLoad;
	var cmbFont = document.getElementById("cmbFont");
	var chkArch = document.getElementById("chkArch");
	chkArch.onclick = onClickChkArch;
	var chkShadow = document.getElementById("chkShadow");
	chkShadow.onclick = onClickChkShadoww;
	var divColorArea = document.getElementById("divColorArea");
	divColorArea.onclick = onClickColorArea;

	var btnLayoutLoad = document.getElementById("btnLayoutLoad");
	btnLayoutLoad.onclick = onClickLayoutLoad;
	var btnLayoutSave = document.getElementById("btnLayoutSave");
	btnLayoutSave.onclick = onClickLayoutSave;
	var btnImageLoad = document.getElementById("btnImageLoad");
	btnImageLoad.onclick = onClickImageLoad;
	var btnInputText = document.getElementById("btnInputText");
	btnInputText.onclick = onClickInputText;
	var btnSelect = document.getElementById("btnSelect");
	btnSelect.onclick = onClickSelect;
	var btnDelete = document.getElementById("btnDelete");
	btnDelete.onclick = onClickDelete;

	var btnExit = document.getElementById("btnExit");
	btnExit.onclick = onClickExit;

	m_lblInfo = document.getElementById("lblInfo");

	m_nOrientation = YOUSIYOKO;

	fncInitSampleTitle();
	fncSampleInitElement();
	fncLayoutInitElement();

	fncInitFontName();
	fncLoadClrTable();
	fncLoadTTFrameImage();
	fncLoadYKFrameImage();
	fncLoadPartsImage();

	fncMouseAddEventListener();
	document.addEventListener('keydown', fncMouseKeyDown);
}
function fncSampleInitElement()
{
	divSampleArea = document.getElementById("divSampleArea");
	divSampleArea.style.overflowY = "auto";
	divSample = document.getElementById("divSample");
	cnvsSample = document.getElementById("cnvsSample");
	m_cnvsSampleWidth = divSampleArea.offsetWidth;
	m_cnvsSampleHeight = divSampleArea.offsetHeight*2;
	divSample.width = m_cnvsSampleWidth;
	divSample.height = m_cnvsSampleHeight;
	cnvsSample.width = m_cnvsSampleWidth;
	cnvsSample.height = m_cnvsSampleHeight;
}
function fncLayoutInitElement()
{
	fncInitLayoutCanvas();
	fncInitInputArea();
}
function fncInitLayoutCanvas()
{
	var divLayoutArea = document.getElementById("divLayoutArea");
	var divLayout = document.getElementById("divLayout");
	var cnvsLayout_b = document.getElementById("cnvsLayout_b");
	var cnvsLayout_c = document.getElementById("cnvsLayout_c");
	var cnvsLayout_f = document.getElementById("cnvsLayout_f");
	var cnvsLayout_s = document.getElementById("cnvsLayout_s");
	m_divLayoutAreaWidth = divLayoutArea.offsetWidth;
	m_divLayoutAreaHeight = divLayoutArea.offsetHeight;
	m_dTrnsSX = 0;
	m_dTrnsSY = 0;
	divLayout.width = m_divLayoutAreaWidth;
	divLayout.height = m_divLayoutAreaHeight;
	if(m_nOrientation == YOUSIYOKO){
		m_cnvsLayoutWidth = m_divLayoutAreaWidth;
		m_cnvsLayoutHeight = m_divLayoutAreaHeight;
		cnvsLayout_b.left = 0;
		cnvsLayout_b.top = 0;
		cnvsLayout_b.width = m_cnvsLayoutWidth;
		cnvsLayout_b.height = m_cnvsLayoutHeight;
		cnvsLayout_c.left = 0;
		cnvsLayout_c.top = 0;
		cnvsLayout_c.width = m_cnvsLayoutWidth;
		cnvsLayout_c.height = m_cnvsLayoutHeight;
		cnvsLayout_f.left = 0;
		cnvsLayout_f.top = 0;
		cnvsLayout_f.width = m_cnvsLayoutWidth;
		cnvsLayout_f.height = m_cnvsLayoutHeight;
		cnvsLayout_s.left = 0;
		cnvsLayout_s.top = 0;
		cnvsLayout_s.width = m_cnvsLayoutWidth;
		cnvsLayout_s.height = m_cnvsLayoutHeight;
		m_dScrnBaseTime = m_cnvsLayoutHeight / SIZEA4SHORT;
	}else{ // YOUSITATE
		m_cnvsLayoutWidth = m_divLayoutAreaHeight * 0.7;
		m_cnvsLayoutHeight = m_divLayoutAreaHeight;
		cnvsLayout_b.left = (m_divLayoutAreaWidth - m_cnvsLayoutWidth) / 2;
		cnvsLayout_b.top = 0;
		cnvsLayout_b.width = m_cnvsLayoutWidth;
		cnvsLayout_b.height = m_cnvsLayoutHeight;
		cnvsLayout_c.left = (m_divLayoutAreaWidth - m_cnvsLayoutWidth) / 2;
		cnvsLayout_c.top = 0;
		cnvsLayout_c.width = m_cnvsLayoutWidth;
		cnvsLayout_c.height = m_cnvsLayoutHeight;
		cnvsLayout_f.left = (m_divLayoutAreaWidth - m_cnvsLayoutWidth) / 2;
		cnvsLayout_f.top = 0;
		cnvsLayout_f.width = m_cnvsLayoutWidth;
		cnvsLayout_f.height = m_cnvsLayoutHeight;
		cnvsLayout_s.left = (m_divLayoutAreaWidth - m_cnvsLayoutWidth) / 2;
		cnvsLayout_s.top = 0;
		cnvsLayout_s.width = m_cnvsLayoutWidth;
		cnvsLayout_s.height = m_cnvsLayoutHeight;
		m_dScrnBaseTime = m_divLayoutAreaHeight / SIZEA4LONG;
	}
	fncMouseDivLayoutOffset(divLayoutArea.offsetLeft, divLayoutArea.offsetTop);
	fncMouseCanvasLayoutOffset(cnvsLayout_b.left, cnvsLayout_b.top);
	m_ctxLayout_b = cnvsLayout_b.getContext('2d');
	m_ctxLayout_c = cnvsLayout_c.getContext('2d');
	m_ctxLayout_f = cnvsLayout_f.getContext('2d');
	m_ctxLayout_s = cnvsLayout_s.getContext('2d');
	m_nCrtClrIdx = 0;
}
function fncInitInputArea()
{
	var wd, hi;

	var divScrnArea = document.getElementById("divScrnArea");
	var divLayout_s = document.getElementById("divLayout_s");
	m_nInputBaseLeft = divLayout_s.offsetLeft;
	m_nInputBaseTop = divLayout_s.offsetTop;
	
	var fs = 20;
	wd = parseInt(m_cnvsLayoutWidth / 2)
	size = wd / fs * 2;
	m_inputText = document.createElement("textarea");
	m_inputText.style.left = m_nInputBaseLeft+"px";
	m_inputText.style.top = m_nInputBaseTop+"px";
	m_inputText.style.position = "absolute";
	m_inputText.style.font = fs + "px 'ゴシック'";
	m_inputText.style.imeMode = "active";
	m_inputText.style.visibility = "hidden";
	m_inputText.cols = 20;
	m_inputText.rows = 1;
	m_inputText.textContent = "";
	divScrnArea.appendChild(m_inputText);

}
function fncInitSampleTitle()
{
	var cmbSample = document.getElementById("cmbSample");
	cmbSample.options[0] = new Option("横フレーム");
	cmbSample.options[1] = new Option("縦フレーム");
	cmbSample.options[2] = new Option("パーツ");
	cmbSample.selectedIndex = 0;
	m_arySampleImage = m_aryYKFrameImage;
	cmbSample.onchange = onChangeSample;
}
function fncInitFontName()
{
	var cmbFont = document.getElementById("cmbFont");
	cmbFont.options[0] = new Option("ゴシック体", "sans-serif");
	cmbFont.options[1] = new Option("明朝体", "serif");
	cmbFont.options[2] = new Option("筆記体", "cursive");
	cmbFont.options[3] = new Option("装飾文字", "fantasy");
	cmbFont.selectedIndex = 0;
	m_sFontFamily = "sans-serif";
	cmbFont.onchange = onChangeFont;
}
function fncLoadClrTable()
{
	var fnc = fncLoadClrTableCallBack;
	var data = "file=../list/colortable.txt";
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncLoadClrTableCallBack(xmlhttp)
{
	var idx, max;

	var data = xmlhttp.responseText;
	var aryLine = data.split("\r\n");
	max = aryLine.length;
	for(idx = 1; idx < max; idx++){
		ary = aryLine[idx].split(",");
		m_aryClrTable.push(ary[0]);
		m_aryShClrTable.push(ary[1]);
	}
	m_nCrtClrIdx = 0;
	fncColorTableDraw();
}
function fncLoadTTFrameImage()
{
	var fnc = fncLoadTTFrameImageCallBack;
	var data = "file=../list/ttframe.txt";
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncLoadTTFrameImageCallBack(xmlhttp)
{
	var idx, max;
	var img;

	var data = xmlhttp.responseText;
	var aryLine = data.split("\r\n");
	max = aryLine.length;
	for(idx = 1; idx < max; idx++){
		if(aryLine[idx] == "" || aryLine[idx] == " "){
			break;
		}
		ary = aryLine[idx].split(",");
		img = new CImageEx();
		img.flag = 0;
		img.basename = ary[0];
		img.src = "ttframe/"+ary[0]+".gif";
		img.onload = fncCallBackloadImage;
		m_aryTTFrameImage.push(img);
	}
}
function fncLoadYKFrameImage()
{
	var fnc = fncLoadYKFrameImageCallBack;
	var data = "file=../list/ykframe.txt";
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncLoadYKFrameImageCallBack(xmlhttp)
{
	var idx, max;
	var img;

	var data = xmlhttp.responseText;
	var aryLine = data.split("\r\n");
	max = aryLine.length;
	for(idx = 1; idx < max; idx++){
		if(aryLine[idx] == "" || aryLine[idx] == " "){
			break;
		}
		ary = aryLine[idx].split(",");
		img = new CImageEx();
		img.flag = 0;
		img.basename = ary[0];
		img.src = "ykframe/"+ary[0]+".gif";
		img.onload = fncCallBackloadImage;
		m_aryYKFrameImage.push(img);
	}
}
function fncLoadPartsImage()
{
	var fnc = fncLoadPartsImageCallBack;
	var data = "file=../list/parts.txt";
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncLoadPartsImageCallBack(xmlhttp)
{
	var idx, max;
	var img;

	var data = xmlhttp.responseText;
	var aryLine = data.split("\r\n");
	max = aryLine.length;
	for(idx = 1; idx < max; idx++){
		if(aryLine[idx] == "" || aryLine[idx] == " "){
			break;
		}
		ary = aryLine[idx].split(",");
		img = new CImageEx();
		img.flag = 0;
		img.basename = ary[0];
		img.src = "parts/"+ary[0]+".png";
		img.onload = fncCallBackloadImage;
		m_aryPartsImage.push(img);
	}
}
function fncCallBackloadImage()
{
	this.flag = 1;

	max = m_aryTTFrameImage.length;
    for(idx = 0; idx < max; idx++){
        image = m_aryTTFrameImage[idx];
        if(image.flag == 0){
            return;
        }
    }
    max = m_aryYKFrameImage.length;
    for(idx = 0; idx < max; idx++){
        image = m_aryYKFrameImage[idx];
        if(image.flag == 0){
            return;
        }
    }
    max = m_aryPartsImage.length;
    for(idx = 0; idx < max; idx++){
        image = m_aryPartsImage[idx];
        if(image.flag == 0){
            return;
        }
    }
	fncToolSampleDraw();
	fncCommandSetCrt(CMDSELECT);
}
function onChangeSample()
{
	var cmbSample = document.getElementById("cmbSample");
	switch(cmbSample.selectedIndex){
	case 0:
		m_nOrientation = YOUSIYOKO;
		m_nSelectKind = YOKOFRAME;
		fncInitLayoutCanvas();
		m_arySampleImage = m_aryYKFrameImage;
		break;
	case 1:
		m_nOrientation = YOUSITATE;
		m_nSelectKind = TATEFRAME;
		fncInitLayoutCanvas();
		m_arySampleImage = m_aryTTFrameImage;
		break;
	case 2:
		m_arySampleImage = m_aryPartsImage;
		m_nSelectKind = PARTS;
		break;
	}
	fncToolSampleDraw();
	fncCommandSetCrt(CMDSELECT);
}
function onChangeFont()
{
	var cmbFont = document.getElementById("cmbFont");
	idx = cmbFont.selectedIndex;
	m_sFontFamily = options[idx].value;
}
function onClickUndoLoad()
{
	fncRedoSave();
	setSelectBoxs(0,0,0,0);
	m_aryObject.length = 0;
	fncUndoLoad();
	fncCommandSetCrt(CMDSELECT);
}
function onClickRedoLoad()
{
	m_aryObject.length = 0;
	fncRedoLoad();
	fncCommandSetCrt(CMDSELECT);
}
function onClickChkArch()
{
}
function onClickChkShadoww()
{
}
function onClickColorArea(e)
{

}
function onClickLayoutLoad()
{
	fncLayoutLoadData();
}
function onClickLayoutSave()
{
	fncLayoutSaveData();
}
function onClickImageLoad()
{

}
function onClickInputText()
{
	if(m_CrtObject == null || m_CrtObject.kind != OBJTEXT){
		fncCommandSetCrt(CMDNEWTEXT);
	}else{
		fncCommandSetCrt(CMDSELECTTEXT);	
	}
}
function onClickSelect()
{
	fncCommandSetCrt(CMDSELECT);
}
function onClickDelete()
{
	var max, idx;
	if(m_CrtObject != null){
		max = m_aryObject.length;
		for(idx = 0; idx < max; idx++){
			if(m_aryObject[idx] ==m_CrtObject){
				array.splice(idx, 1);
				m_CrtObject = null;
				break;
			}
		}
	}
	fncCommandSetCrt(CMDSELECT);
}
function onClickExit()
{
	m_aryObject.length = 0;
}
