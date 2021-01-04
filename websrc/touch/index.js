
var m_txtInfo;
var m_cnvsEditArea;
var m_ctxEditArea;
var m_nScrnLeft;
var m_nScrnTop;
var m_cnvsWidth;
var m_cnvsHeight;
var m_nOneXSize, m_nOneYSize;

var m_aryTouchBox;
var m_arySortCS;
var m_nTxtQuestMax = 25;
var m_nCrtQuestIdx;
var m_nQuestMax;

var m_dLimit = 5;
var m_startDate;
var m_stopDate;

var m_sdSuccess;
var m_sdMiss;
var m_nSuccess = false;
var m_nMiss = false;


function fncInit()
{
    var fontsize;
    var fontname;
    var space;
    var x, y;
    var sx, sy, ex, ey;

    m_txtInfo = document.getElementById("txtInfo");
    m_cnvsEditArea = document.getElementById("cnvsEditArea");
	m_ctxEditArea = m_cnvsEditArea.getContext('2d');

	m_cnvsWidth = m_cnvsEditArea.offsetWidth;
	m_cnvsHeight = m_cnvsEditArea.offsetHeight;
	m_cnvsEditArea.width = m_cnvsWidth;
	m_cnvsEditArea.height = m_cnvsHeight;

	m_cnvsHeight = m_cnvsEditArea.offsetHeight;
	m_nScrnLeft = m_cnvsEditArea.offsetLeft;
    m_nScrnTop = m_cnvsEditArea.offsetTop;
 
    var btnStart = document.getElementById("btnStart");
	btnStart.onclick = fncOnClickStart;
 
    fontname = "ＭＳ ゴシック";
    space = 10;
    if(m_cnvsWidth < m_cnvsHeight){
        m_nOneXSize = (m_cnvsWidth - space * 4) / 5;
        m_nOneYSize = m_nOneXSize;
        fontsize = m_nOneXSize / 3;
    }else{
        m_nOneXSize = (m_cnvsWidth - space * 4) / 5;
        m_nOneYSize = (m_cnvsHeight - space * 4) / 5;
        fontsize = m_nOneYSize / 2;
    }
    fncCtxSetFont(m_ctxEditArea, fontsize, fontname);
    
    m_nCrtQestIdx = -1;
    m_txtInfo.value = "開始ボタンを押してください。";
    m_sdSuccess = new Audio();
    m_sdMiss = new Audio();
    m_sdSuccess.src = "success.mp3";
    m_sdMiss.src = "miss.mp3";
    m_nSuccess = false;
    m_nMiss = false;
    
    m_aryTouchBox = new Array();
     sx = 0; sy = 0;
    for(y = 0; y < 5; y++){
        sy = y * (m_nOneYSize + space);
        ey = sy + m_nOneYSize;
        for(x = 0; x < 5; x++){
            sx = x * (m_nOneXSize + space);
            ex = sx + m_nOneXSize;
            clsTB = new clsTouchBox();
            clsTB.sx = sx;
            clsTB.ex = ex;
            clsTB.sy = sy;
            clsTB.ey = ey;
            m_aryTouchBox.push(clsTB);
        }
    }
}
function fncOnClickStart()
{
    var idx, max;
    var s00;
    var txtQuest;
    var idxRnd;
    var nLest = 25;
    var clsCS;

    fncAllOffFlagAryTouchBox();
    m_arySortCS = new Array();
    m_nQuestMax = fncGetQuestLength();
    nLest = m_nQuestMax;
    for(idx = 0; idx < m_nQuestMax; idx++){
        s00 = fnclibZeroPadding((idx+1), 2);
        idxRnd = Math.floor(Math.random() * nLest);
        idxTB = fncSrchLestIdxTB(idxRnd);
        txtQuest = document.getElementById("txtQuest"+s00);
        txtQuest.style.background="#FFF";
        clsCS = new clsCheckStr();
        clsCS.str = txtQuest.value;
        clsCS.idx = idxTB;
        m_arySortCS.push(clsCS);
        nLest--;
    }
    fncAllOffFlagAryTouchBox();
    fncDrawAryTouchBox();
    m_nCrtQestIdx = 0;
    var txtUser = document.getElementById("txtUser");
    m_sUserName = txtUser.value;
    fncStartTimer();
}
function fncStartTimer()
{
	m_startDate = new Date();
}
function fncStopTimer()
{
    m_stopDate = new Date();
    termSS = (m_stopDate - m_startDate) / 1000;
    m_txtInfo.value = "今回は"+termSS+"秒でした。";
    sSaveMsg = fncGetDateString(m_startDate)+","+termSS;
    if(m_sUserName == ""){
        filename = "user/gest.txt";
    }else{
        filename = "user/"+m_sUserName+".txt";
    }
    m_nCrtQestIdx = -1;
	var data = "file="+filename+"&data="+sSaveMsg;
	var fnc = fncSaveFileCallback;
	sendRequest("POST","savefile.php",data,false,fnc);
}
function fncGetDateString(date)
{
    var sYY = date.getFullYear();
    var sMM = date.getMonth() + 1;
    var sDD = date.getDate();
    var sHH = date.getHours();
    var sMin = date.getMinutes();
    var sMM = fnclibZeroPadding(sMM, 2);
    var sDD = fnclibZeroPadding(sDD, 2);
    var sHH = fnclibZeroPadding(sHH, 2);
    var sMin = fnclibZeroPadding(sMin, 2);
    return(sYY+"/"+sMM+"/"+sDD+" "+sHH+":"+sMin)
}
function fncSaveFileCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(",");
    if(ary[0] == "0"){
        return;
    }
}
function fncCmdExec(event, dx, dy)
{
    if(m_nCrtQestIdx == -1){
        return;
    }
    var clsCS = m_arySortCS[m_nCrtQestIdx]
    var clsTB = m_aryTouchBox[clsCS.idx];
    var str = clsCS.str;
    if(event == SETEVENT){
        if(m_nMiss == true){
            m_sdMiss.pause();
            m_nMiss = false;
        }
        if(m_nSuccess == true){
            m_sdSuccess.pause();
            m_nSuccess = false;
        }
        fncCmdDebugPos(dx, dy);
        if(clsTB.sx <= dx && dx <= clsTB.ex
        && clsTB.sy <= dy && dy <= clsTB.ey){
            s00 = fnclibZeroPadding((m_nCrtQestIdx+1), 2);
            txtQuest = document.getElementById("txtQuest"+s00);
            txtQuest.style.background="#CFC";
            clsTB.flag = 1;
            fncDrawTouchBox(clsTB, str);
            //m_sdSuccess.src = "success.mp3";
            m_sdSuccess.play();
            m_nSuccess = true;
            m_nCrtQestIdx++;
            if(m_nQuestMax == m_nCrtQestIdx){
                fncStopTimer();
            }
        }else{
            //m_sdMiss.src = "miss.mp3"; 
			m_sdMiss.play();
            m_nMiss = true;
        }
    }
}
function fncGetQuestLength()
{
    var idx;
    var s00;
    var txtQuest;

    for(idx = 0; idx < m_nTxtQuestMax; idx++){
        s00 = fnclibZeroPadding((idx+1), 2);
        txtQuest = document.getElementById("txtQuest"+s00);
        if(txtQuest.value == ""){
            return(idx);
        }
    }
    return(m_nTxtQuestMax);
}
function fncDrawAryTouchBox()
{
    var idx, max;
    var clsCS;
    var clsTB;

    max = m_arySortCS.length;
    for(idx = 0; idx < max; idx++){
        clsCS = m_arySortCS[idx];
        clsTB = m_aryTouchBox[clsCS.idx];
        fncDrawTouchBox(clsTB, clsCS.str);
    }
}
function fncDrawTouchBox(clsTB, str)
{
    var cx, cy;

    cx = (clsTB.sx + clsTB.ex) / 2;
    cy = (clsTB.sy + clsTB.ey) / 2;
    fncCtxBoxs(m_ctxEditArea, clsTB.sx, clsTB.sy, clsTB.ex, clsTB.ey);
    if(clsTB.flag == 0){
        fncFill(m_ctxEditArea, "#FFF");
    }
    else{
        fncFill(m_ctxEditArea, "#CFC");        
    }
    fncStroke(m_ctxEditArea, "#000", 2);
    cx = (clsTB.sx + clsTB.ex) / 2;
    cy = (clsTB.sy + clsTB.ey) / 2;
    fncCtxCnterTextBoxs(m_ctxEditArea, cx, cy, str, "#000");
}
function fncSrchLestIdxTB(idxRnd)
{
    var max, idx, setidx;

    setidx = 0;
    max = m_aryTouchBox.length;
    for(idx = 0; idx < max; idx++){
        if(m_aryTouchBox[idx].flag == 0){
            if(setidx == idxRnd){
                m_aryTouchBox[idx].flag = 1;
                return(idx);
            }
            setidx++;
        }
    }
}
function fncAllOffFlagAryTouchBox()
{
    var max, idx;

    max = m_aryTouchBox.length;
    for(idx = 0; idx < max; idx++){
        m_aryTouchBox[idx].flag = 0;
    }
}
function fncCmdDebugPos(dx, dy)
{
	var sx, sy, ex, ey;

	sx = dx - m_dLimit;
	sy = dy - m_dLimit;
	ex = dx + m_dLimit;
	ey = dy + m_dLimit;
	sx = fncScrnTrnsMRX(sx);
	sy = fncScrnTrnsMRY(sy);
	ex = fncScrnTrnsMRX(ex);
	ey = fncScrnTrnsMRY(ey);
	fncStroke(m_ctxEditArea, "#000", 1);
	fncCtxLine(m_ctxEditArea, sx, sy, ex, ey);
	fncCtxLine(m_ctxEditArea, sx, ey, ex, sy);
}
