
function fncLayoutLoadData(){
	var flaFilter:FileFilter;
	
	fncSetCrtCommand(CMDLOAD);
	flaFilter = new FileFilter("Layout file", "*.lyd");
	m_frLayoutLoad.browse([flaFilter]);
}

function onSelectedLayoutLoad(evt){
	m_frLayoutLoad.load();
	txtInfo.text = "A layout call is started.";
}

function onProgressLayoutLoad(evt:ProgressEvent){
	txtInfo.text = "Under a layout call";
}

function fncUndoLoad()
{
}

function onUndoLoadCallBack()
{
	fncLoadAnalyze(aryLine);
}

function fncRedoLoad()
{
}

function onRedoLoadCallBack()
{
	fncLoadAnalyze(aryLine);
}
function fncLoadAnalyze(str){
	var aryLine;
	var max, i;
	var aryCsv;
	var filename;
	var idx;
	var objsp;
	var bmpdata;
	var bmp;
	var size, clr, sclr;
	var par;
	
	txtInfo.text = "";
	aryLine = str.split("\r\n");
	max = aryLine.length;
	for(i = 0; i < max; i++){
		aryCsv = aryLine[i].split(",");
		if(aryCsv.length < 2){
			break;
		}
		if(aryCsv[0] == "TMPL"){
			filename = aryCsv[1];
			idx = fncSrchLYTemplateIdx(filename);
			if(filename != "" && idx != -1){
				fncSetTemplateBitmap(idx);
			}
			
		}else if(aryCsv[0] == "PHOT"){
			filename = aryCsv[1];
			m_nLoadPhotoSX = int(aryCsv[2]);
			m_nLoadPhotoSY = int(aryCsv[3]);
			m_dLoadPhotoScaloeX = Number(aryCsv[4]);
			m_dLoadPhotoScaloeY = Number(aryCsv[5]);
			if(filename != ""){
				fncLoadLYPhotoData(filename);
			}
		}else if(aryCsv[0] == "FIGU"){
			filename = aryCsv[1];
			idx = fncSrchLYFigureIdx(filename);
			bmpdata = m_aryLYFigureBmpData[idx];
			bmp = new Bitmap(bmpdata);
			objsp = new ObjSprite();
			objsp.m_nObjType = OBJFIGURE;
			objsp.alpha = 0.5;
			objsp.x = int(aryCsv[2]);
			objsp.y = int(aryCsv[3]);
			objsp.scaleX = Number(aryCsv[4]);
			objsp.scaleY = Number(aryCsv[5]);
			objsp.addChild(bmp);
			m_aryObject.push(objsp);
			m_sprEditArea.addChild(objsp);
		}else if(aryCsv[0] == "TEXT"){
			objsp = new ObjSprite();
			objsp.m_nObjType = OBJTEXT;
			objsp.x = int(aryCsv[2]);
			objsp.y = int(aryCsv[3]);
			size = int(aryCsv[4]);
			clr = int(aryCsv[6]);
			idx = fncSrchColorIdx(clr);
			sclr = m_aryShClrTable[idx];
			if(aryCsv[7] == "true"){
				objsp.m_bArch = true;
			}else{
				objsp.m_bArch = false;
			}
			if(aryCsv[8] == "true"){
				objsp.m_bShadow = true;
			}else{
				objsp.m_bShadow = false;
			}
			par = Number(aryCsv[9]);
			objsp.m_dRound = Number(aryCsv[10]);
			objsp.SetArchPar(par);
			objsp.CreateTextField(aryCsv[5], size, aryCsv[1], clr, sclr);
			m_aryObject.push(objsp);
			m_sprEditArea.addChild(objsp);
		}
	}
	m_nRectSX = 0; m_nRectSY = 0;
	m_nRectEX = 0; m_nRectEY = 0;
	m_spriteSelectRect.graphics.clear();
	m_nObjCrtIdx = -1;
	m_objspCrt = null;
	txtInfo.text = "";
}

function onCancelLayoutLoad(evt){
	txtInfo.text = "The layout call was canceled.";
}
function onIOErrorLayoutLoad(evt){
	txtInfo.text = "The layout call was interrupted for the error.";
}

function onSecurityErrorLayoutLoad(evt){
	txtInfo.text = "The layout call is forbidden. ";
}

