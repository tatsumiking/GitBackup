// ファイル関係関数

function fncFileUndoSave()
{

}
function fncFileUndoLoad()
{
    
}
function fncFileSaveLayout(sFileName)
{
    var idx;
    var sObjData;
    var sSaveData;

    sObjData = "";
    sSaveData = "//,"+m_nOrientation+","+m_nSelectKind+",\r\n";
	var max = m_aryObject.length;
	for(idx = 0; idx < max; idx++){
        if(m_aryObject[idx].kind == OBJIMAGE){
            sObjData = fncFileSaveImage(m_aryObject[idx]);
            sSaveData = sSaveData + sObjData;
        }else if(m_aryObject[idx].kind == OBJTEXT){
            sObjData = fncFileSaveText(m_aryObject[idx]);
            sSaveData = sSaveData + sObjData;
        }else if(m_aryObject[idx].kind == OBJFIGU){
            sObjData = fncFileSaveFigu(m_aryObject[idx]);
            sSaveData = sSaveData + sObjData;
        }
    }
	data = "file="+sFileName+"&data="+sSaveData;
	sendRequest("POST","savefile.php",data,false,fncFileSaveLayoutCallBack);	
}
function fncFileSaveLayoutCallBack(xmlhttp)
{
    fnclibAlert("ファイルを保存しました。");
}
function fncFileSaveImage(imgobj)
{
    var sData;

    if(imgobj.lock == false){
        sData = "IMAGE,0";
    }else{
        sData = "IMAGE,1";        
    }
    var sxy = imgobj.fncGetAtrXY(0);
    var exy = imgobj.fncGetAtrXY(2);
    sData = sData + "," + Math.round(sxy.x) + "," + Math.round(sxy.y);
    sData = sData + "," + Math.round(exy.x) + "," + Math.round(exy.y);
    sData = sData + "," + imgobj.image.src + ",\r\n";
    return(sData);
}
function fncFileSaveText(textobj)
{
    var sData;

    if(textobj.lock == false){
        sData = "TEXT,0";
    }else{
        sData = "TEXT,1";        
    }
    var sxy = textobj.fncGetAtrXY(0);
    var exy = textobj.fncGetAtrXY(2);
    sData = sData + "," + Math.round(sxy.x) + "," + Math.round(sxy.y);
    sData = sData + "," + Math.round(exy.x) + "," + Math.round(exy.y);
    sData = sData + "," + textobj.clrFill + "," + textobj.clrStroke + "," + textobj.thick;
    sData = sData + "," + textobj.vhkind + "," + textobj.mode + "," + textobj.space;
    sData = sData + "," + textobj.sFontFamily + "," + textobj.text + ",\r\n" ;
    return(sData);
}
function fncFileSaveFigu(figuobj)
{
    var sData;
    var max, idx;
    var aryAXY;
    var pcnt, pp;

    if(figuobj.lock == false){
        sData = "FIGU,0";
    }else{
        sData = "FIGU,1";        
    }
    max = figuobj.aryPath.length;
    sData = sData + "," + max; 
    sData = sData + "," + figuobj.clrFill + "," + figuobj.clrStroke + "," + figuobj.thick + ",\r\n";
    for(idx = 0; idx < max; idx++){
        aryAXY = figuobj.aryPath[idx];
        pcnt = aryAXY.length;
        sData = sData + "FGMOVE,"+Math.round(aryAXY[0].x*10)/10+ "," + Math.round(aryAXY[0].y*10)/10+ ",\r\n";
        pp = 1;
        while(true){
            if(pcnt <= pp){
                break;
            }
            if(aryAXY[pp-1].atr == 1){
                sData = sData + "FGLINE,"+Math.round(aryAXY[pp].x*10)/10+ "," + Math.round(aryAXY[pp].y*10)/10+ ",\r\n";
                pp = pp + 1;
            }else if(aryAXY[pp-1].atr == 2){
                sData = sData + "FGBEZE,"+Math.round(aryAXY[pp].x*10)/10+ "," + Math.round(aryAXY[pp].y*10)/10;
                sData = sData + ","+Math.round(aryAXY[pp+1].x*10)/10+ "," + Math.round(aryAXY[pp+1].y*10)/10;
                sData = sData + ","+Math.round(aryAXY[pp+2].x*10)/10+ "," + Math.round(aryAXY[pp+2].y*10)/10+ ",\r\n";
                pp = pp + 3;
            }else if(aryAXY[pp-1].atr == 3){
                sData = sData + "FGCBEZE,"+Math.round(aryAXY[pp].x*10)/10+ "," + Math.round(aryAXY[pp].y*10)/10;
                sData = sData + ","+Math.round(aryAXY[pp+1].x*10)/10+ "," + Math.round(aryAXY[pp+1].y*10)/10;
                sData = sData + ","+Math.round(aryAXY[pp+2].x*10)/10+ "," + Math.round(aryAXY[pp+2].y*10)/10+ ",\r\n";
                pp = pp + 3;
            }else{
                pp = pp + 1;
            }
        }
    }
    sData = sData + "FGEND,\r\n";
    return(sData);
}
function fncFileLoadLayout(sFileName)
{
	data = "file="+sFileName;
	sendRequest("POST","readfile.php",data,false,fncFileLoadLayoutCallBack);	
}
function fncFileLoadLayoutCallBack(xmlhttp)
{
    var idx, max;

	var data = xmlhttp.responseText;
	var aryLine = data.split("\r\n");
    var ary = aryLine[0].split(",");
    if(ary.length <= 1){
        fnclibAlert("ファイルの読込に失敗しました。");
        return;
    }
    m_nOrientation = parseInt(ary[1]);
    m_nSelectKind = parseInt(ary[2]);
	max = aryLine.length;
    idx  = 1;
    m_aryObject.length = 0;
    while(true){
        if(max <= idx){
            break;
        }
        ary = aryLine[idx].split(",");
        if(ary[0] == "IMAGE"){
            fncFileLoadImage(ary);
            idx++;
        }else if(ary[0] == "TEXT"){
            fncFileLoadText(ary);
            idx++;
        }else if(ary[0] == "FIGU"){
            idx = fncFileLoadFigu(aryLine, idx);
        }else{
            idx++;
        }
    }
    fncFileLoadWaitMainDraw();
}
function fncFileLoadImage(ary)
{
    var sx, sy, ex, ey;

    var imgobj = new clsObjImage();
    imgobj.kind = OBJIMAGE;
    if(ary[1] == "1"){
        imgobj.lock = true;
    }
    sx = parseInt(ary[2]);
    sy = parseInt(ary[3]);
    ex = parseInt(ary[4]);
    ey = parseInt(ary[5]);
    imgobj.fncSetAtrXY(0, 0, sx, sy);
    imgobj.fncSetAtrXY(1, 1, ex, sy);
    imgobj.fncSetAtrXY(2, 1, ex, ey);
    imgobj.fncSetAtrXY(3, 1, sx, ey);
    m_aryObject.push(imgobj);
    imgobj.image.src = ary[6];
    imgobj.image.onload = function(){
		imgobj.flag = 1;
    }
}
function fncFileLoadText(ary)
{
    var sx, sy, ex, ey;
    
    var textobj = new clsObjText();
    textobj.kind = OBJTEXT;
    if(ary[1] == "1"){
        textobj.lock = true;
    }
    sx = parseInt(ary[2]);
    sy = parseInt(ary[3]);
    ex = parseInt(ary[4]);
    ey = parseInt(ary[5]);
    textobj.fncSetAtrXY(0, 0, sx, sy);
    textobj.fncSetAtrXY(1, 1, ex, sy);
    textobj.fncSetAtrXY(2, 1, ex, ey);
    textobj.fncSetAtrXY(3, 1, sx, ey);
    textobj.clrFill = ary[6]; 
    textobj.clrStroke = ary[7];
    textobj.thick = parseFloat(ary[8]);
    textobj.vhkind = parseInt(ary[9]); 
    textobj.mode = parseInt(ary[10]); 
    textobj.space = parseFloat(ary[11]);
    textobj.sFontFamily = ary[12];
    textobj.text = ary[13];
    m_aryObject.push(textobj);
}
function fncFileLoadFigu(aryLine, idx)
{
    var idx, max;
    var flag;
    var dCrtX, dCrtY;
    var axy;

    var ary = aryLine[idx].split(",");
    var figuobj = new clsObjFigu();
    figuobj.kind = OBJTEXT;
    if(ary[1] == "1"){
        figuobj.kind = true;
    }
    figuobj.clrFill = ary[3]; 
    figuobj.clrStroke = ary[4];
    figuobj.thick = parseFloat(ary[5]);
    idx++;
    max = aryLine.length;
    flag = false;
    while(true){
        if(max <= idx){
            break;
        }
        ary = aryLine[idx].split(",");
        if(ary[0] == "FGEND"){
            if(flag == true){
                axy = new CAtrXY();
                axy.atr = 1; axy.x = dCrtX; axy.y = dCrtY;
                aryAXY.push(axy);
            }
            idx++;
            break;
        }else if(ary[0] == "FGMOVE"){
            if(flag == true){
                axy = new CAtrXY();
                axy.atr = 1; axy.x = dCrtX; axy.y = dCrtY;
                aryAXY.push(axy);
            }
            aryAXY = new Array(0);
            figuobj.aryPath.push(aryAXY);
            flag = true;
            dCrtX = parseFloat(ary[1]);
            dCrtY = parseFloat(ary[2]);
        }else if(ary[0] == "FGLINE"){
            axy = new CAtrXY();
            axy.atr = 1; axy.x = dCrtX; axy.y = dCrtY;
            aryAXY.push(axy);
            dCrtX = parseFloat(ary[1]);
            dCrtY = parseFloat(ary[2]);
        }else if(ary[0] == "FGBEZE"){
            axy = new CAtrXY();
            axy.atr = 2; axy.x = dCrtX; axy.y = dCrtY;
            aryAXY.push(axy);
            axy = new CAtrXY();
            axy.atr = 4; axy.x = parseFloat(ary[1]); axy.y = parseFloat(ary[2]);
            aryAXY.push(axy);
            axy = new CAtrXY();
            axy.atr = 4; axy.x = parseFloat(ary[3]); axy.y = parseFloat(ary[4]);
            aryAXY.push(axy);

            dCrtX = parseFloat(ary[5]);
            dCrtY = parseFloat(ary[6]);
        }else if(ary[0] == "FGCBEZE"){
            axy = new CAtrXY();
            axy.atr = 3; axy.x = dCrtX; axy.y = dCrtY;
            aryAXY.push(axy);
            axy = new CAtrXY();
            axy.atr = 4; axy.x = parseFloat(ary[1]); axy.y = parseFloat(ary[2]);
            aryAXY.push(axy);
            axy = new CAtrXY();
            axy.atr = 4; axy.x = parseFloat(ary[3]); axy.y = parseFloat(ary[4]);
            aryAXY.push(axy);

            dCrtX = parseFloat(ary[5]);
            dCrtY = parseFloat(ary[6]);
        }
        idx++;
    }
    return(idx);
}
var m_nFileTimer;

function fncFileLoadWaitMainDraw()
{
    m_nFileTimer = setTimeout(fncFileTimer, 500);
}
function fncFileTimer()
{
    var idx, max;

    clearTimeout(m_nFileTimer);
	var max = m_aryObject.length;
	for(idx = 0; idx < max; idx++){
        if(m_aryObject[idx].kind == OBJIMAGE){
            if(m_aryObject[idx].flag == 0){
                fncFileLoadWaitMainDraw();
                return;
            }
        }
    }
    fncDrawMain();
}