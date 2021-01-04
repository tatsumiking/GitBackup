
var m_clsLayout;
var m_szHotelDB;
var m_szKonreiTable;
var m_nKonreiId;
var m_sKonreiNo;

var m_sTextSize;
var m_sTakasagoKind;
var m_sTableLayout;
var m_sTablePosition;
var m_aryGestSit;
var m_cnvsWidth;
var m_cnvsHeight;

var m_nSlctGestId;

var m_strWhereSql;
var m_strOderSql;

var m_nScrnLeft;
var m_nScrnTop;

var m_aryTableArea;
var m_taLeft;
var m_taRight;
var m_cnvsPaper;
var m_ctxPaper;
var m_nFontSize;


function fncInit()
{
	m_nScrnLeft = 0;
	m_nScrnTop = 0;
	m_taLeft = null;
	m_taRight = null;
	m_nSlctGestId = 0;
	
	m_szHotelDB = localStorage.getItem("HotelDB");
	m_szKonreiTable = localStorage.getItem("KonreiTable");
	m_strUserKind = localStorage.getItem("UserKind");
	var sId = localStorage.getItem("KonreiId");
	m_nKonreiId = fnclibStringToInt(sId);
	m_sKonreiNo = localStorage.getItem("KonreiNo");
	if(m_sKonreiNo == "0"){
		fnclibAlert("婚礼が指定されていません");
		var url = "02kihon.html";
		window.location.href = url;
		return;
	}

	m_strWhereSql = "";
	m_strOderSql = "";
	m_clsLayout = new clsTableLayout();
	m_aryGestSit = new Array();

	var btnSave = document.getElementById("btnSave");
	btnSave.style.visibility = 'hidden';

	var rdoAll = document.getElementById("rdoAll");
	rdoAll.onclick = fncOnClickAll;
	var rdoSinro = document.getElementById("rdoSinro");
	rdoSinro.onclick = fncOnClickSinro;
	var rdoSinpu = document.getElementById("rdoSinpu");
	rdoSinpu.onclick = fncOnClickSinpu;

	var rdoInput = document.getElementById("rdoInput");
	rdoInput.onclick = fncOnClickInput;
	var rdoKana = document.getElementById("rdoKana");
	rdoKana.onclick = fncOnClickKana;

	var lstGest = document.getElementById("lstGest");
	lstGest.onchange = fncOnChangeGestList;

	var btnReturn = document.getElementById("btnReturn");
	btnReturn.onclick = fncOnClickReturn;

	fncTableTextSetFunction();

	fncInitKonreiElement();
}
function fncOnChangeGestList()
{
	var lstGest = document.getElementById("lstGest");
	var nIdx = lstGest.selectedIndex;
	m_nSlctGestId = lstGest.options[nIdx].value;
}
function fncOnClickAll()
{
	m_strWhereSql = "";
	fncInitGetList();
}
function fncOnClickSinro()
{
	m_strWhereSql = "WHERE (flag=1)";
	fncInitGetList();
}
function fncOnClickSinpu()
{
	m_strWhereSql = "WHERE (flag=2)";
	fncInitGetList();
}
function fncOnClickInput()
{
	m_strOderSql = "ORDER BY id ASC";
	fncInitGetList();
}
function fncOnClickKana()
{
	m_strOderSql = "ORDER BY yomi ASC";
	fncInitGetList();
}
function fncOnClickReturn()
{
	fncUpdateKonreiElement();
}
function fncUpdateKonreiElement()
{
	var max, idx;
	var cntmax, cnt;
	var x, y;

	var cmbTextSize = document.getElementById("cmbTextSize");
	m_sTextSize = fnclibSelectedText(cmbTextSize);
	var cmbTakasagoKind = document.getElementById("cmbTakasagoKind");
	m_sTakasagoKind = fnclibSelectedText(cmbTakasagoKind);

	var dbnm = m_szHotelDB;
	sSql = "UPDATE "+m_szKonreiTable+" SET ";
	sSql = sSql+"textsize='"+m_sTextSize+"',takasagokind='"+m_sTakasagoKind+"'";
	sSql = sSql+" WHERE (id="+m_nKonreiId+");";
	var data = "dbnm="+dbnm+"&sql="+sSql;
	var fnc = fncUpdateKonreiCallback;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncUpdateKonreiCallback(xmlhttp)
{
	var max, idx, setcnt;

	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert(m_szKonreiTable+"婚礼管理番号"+m_sKonreiNo+"の婚礼レコードの更新に失敗しました");
		return;
	}
	var url = "01menu.html";
	window.location.href = url;
}
function fncInitKonreiElement()
{
	var dbnm = m_szHotelDB;
	var tble = m_szKonreiTable;
	var fild = "id,username,sinroname1,sinpuname1,kyosiki";
	fild = fild + ",textsize,takasagokind,tablelayout,tableposition";
	var where = "WHERE (id="+m_nKonreiId+") LIMIT 1";
	var data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&where="+where;
	var fnc = fncGetKonreiDataCallback;
	sendRequest("POST","php/getdata.php",data,false,fnc);
}
function fncGetKonreiDataCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("婚礼データを取得することが出来ませんでした");
		return;
	}
	var sStr = "婚礼管理番号("+ary[1]+")　"+ary[2]+"家　"+ary[3]+"家　"+ary[4];
	var txtKonrei = document.getElementById("txtKonrei");
	txtKonrei.innerText = sStr; // Microsoft
	txtKonrei.textContent = sStr; //W3C
	m_sTextSize = ary[5];
	m_sTakasagoKind = ary[6];
	m_sTableLayout = ary[7];
	m_sTablePosition = ary[8];

	fncInitTextSizeComboBox();
	fncInitTakasagoKindComboBox();
	fncInitAryTableName();
}
function fncInitTextSizeComboBox()
{
	var data = "file=../list/textsize.txt";
	var fnc = fncTextSizeCallback;
	sendRequest("POST","php/readcanmafile.php",data,false,fnc);
}
function fncTextSizeCallback(xmlhttp)
{
	var idx;
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		return;
	}
	var max = ary.length;
	var cmbTextSize = document.getElementById("cmbTextSize");
	var opt = new Option("", 0);
	cmbTextSize.options[0] = opt;
	for(idx = 0; idx < max; idx++){
		opt = new Option(ary[idx], idx);
		cmbTextSize.options[idx] = opt;
	}
	fnclibSelectData(cmbTextSize, m_sTextSize);	
}
function fncInitTakasagoKindComboBox()
{
	var data = "file=../list/takasagokind.txt";
	var fnc = fncTakasagoKindCallback;
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncTakasagoKindCallback(xmlhttp)
{
	var idx;
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		return;
	}
	var aryLine = data.split("\r\n");
	var max = aryLine.length;
	var cmbTakasagoKind = document.getElementById("cmbTakasagoKind");
	var opt = new Option("", 0);
	cmbTakasagoKind.options[0] = opt;
	for(idx = 0; idx < max; idx++){
		opt = new Option(aryLine[idx], idx);
		cmbTakasagoKind.options[idx] = opt;
	}
	fnclibSelectData(cmbTakasagoKind, m_sTakasagoKind);	
}
function fncInitAryTableName()
{
	var data = "file=../list/tablename.txt";
	var fnc = fncTableNameCallback;
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncTableNameCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		m_aryTableName = Array("Ａ","Ｂ","Ｃ","Ｄ","Ｅ","Ｆ","Ｇ","Ｈ","Ｉ","Ｊ","Ｋ","Ｌ","Ｍ","Ｎ","Ｏ","Ｐ","Ｑ","Ｒ","Ｓ","Ｔ","Ｕ","Ｖ","Ｗ","Ｘ","Ｙ","Ｚ","ａ","ｂ","ｃ","ｄ","ｅ","ｆ","ｇ","ｈ","ｉ","ｊ","ｋ","ｌ","ｍ","ｎ","ｏ","ｐ","ｑ","ｒ","ｓ","ｔ","ｕ","ｖ","ｗ","ｘ","ｙ","ｚ");
	}else{
		m_aryTableName = data.split("\r\n");
	}
	fncInitTableLayout();
}
function fncInitTableLayout()
{
	// tblcnts 3:3:0:0:0:0:
	// tblposs 166x250:500x250:833x250:166x750:500x750:833x750

	var max, idx, tidx;
	var cnt, cntmax;
	var aryxy;

	var arycnt = m_sTableLayout.split(":");
	var arypos = m_sTablePosition.split(":");
	tidx = 0;
	max = m_clsLayout.tlines.length; // 6固定
	for(idx = 0; idx < max; idx++){
		cntmax = fnclibStringToInt(arycnt[idx]);
		for(cnt = 0; cnt < cntmax; cnt++){
			aryxy = arypos[tidx].split("x");
			var tbl = new clsTable();
			tbl.x = fnclibStringToInt(aryxy[0]);
			tbl.y = fnclibStringToInt(aryxy[1]);
			tbl.name = m_aryTableName[tidx];
			m_clsLayout.tlines[idx].tables.push(tbl);
			tidx++;
		}
	}
	fncInitGetSitArray();
	fncDrawPaperCanvas();
}
function fncInitGetList()
{
	var dbnm = m_szHotelDB;
	var tble = "ge"+m_sKonreiNo;
	var fild = "id,name1,name2,kind";
	var trmsql = m_strWhereSql + " " + m_strOderSql;
	var data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&trmsql="+trmsql;
	var fnc = fncGestListCallback;
	sendRequest("POST","php/getlist.php",data,false,fnc);
}
function fncGestListCallback(xmlhttp)
{
	var max, idx;
	var name;

	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("招待者リストを取得することが出来ませんでした");
		return;
	}
	var lstGest = document.getElementById("lstGest");
	lstGest.options.length = 0;
	var aryRec = data.split(";");
	max = aryRec.length;
	lstidx = 0;
	for(idx = 0; idx < max; idx++){
		ary = aryRec[idx].split(",");
		if(ary.length >= 6 && ary[1] != ""){
			if(checkArraGestSit(ary[0]) == 0){
				name = ary[1] + ary[2] + " " + ary[3];
				lstGest.options[lstidx] = new Option(name, ary[0]);
			}
		}
	}
}
function checkArraGestSit(sId)
{
	var max, idx;
	var nId = fnclibStringToInt(sId);
	max = m_aryGestSit.length;
	for(idx = 0; idx < max; idx++){
		if(m_aryGestSit[idx].id == nId){
			return(m_aryGestSit[idx].tno);
		}
	}
	return(1);
}
function fncInitGetSitArray()
{
	var dbnm = m_szHotelDB;
	var tble = "ge"+m_sKonreiNo;
	var fild = "id,name1,name2,kind,tno,sno";
	var trmsql = "";
	var data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&trmsql="+trmsql;
	var fnc = fncGestSitArrayCallback;
	sendRequest("POST","php/getlist.php",data,false,fnc);
}
function fncGestSitArrayCallback(xmlhttp)
{
	var max, idx;
	var lstidx, sitidx;
	var clsGS;
	var clsTbl;

	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("招待者リストを取得することが出来ませんでした");
		return;
	}
	m_aryGestSit.length = 0;

	var aryRec = data.split(";");
	max = aryRec.length;
	lstidx = 0;
	for(idx = 0; idx < max; idx++){
		ary = aryRec[idx].split(",");
		if(ary.length >= 6 && ary[1] != ""){
			clsGS = new clsGestSit();
			clsGS.id = fnclibStringToInt(ary[0]);
			clsGS.tno = fnclibStringToInt(ary[4]);
			clsGS.sno = fnclibStringToInt(ary[5]);
			clsGS.name = ary[1] + ary[2] + " " + ary[3];
			m_aryGestSit.push(clsGS);
			if(clsGS.tno != 0 && clsGS.sno != 0){
				clsTbl = fncGetTable(clsGS.tno);
				if(clsTbl != null){
					if(clsGS.sno <= 7){
						sitidx = clsGS.sno - 1;
						clsTbl.left[sitidx] = clsGS.id;
					}else{
						sitidx = 14 - clsGS.sno;
						clsTbl.right[sitidx] = clsGS.id;				
					}
				}
			}
		}
	}
	fncInitGestList();
	fncDispSitNameTable();
}
function fncInitGestList()
{
	var max, idx;

	var lstGest = document.getElementById("lstGest");
	lstGest.options.length = 0;
	max = m_aryGestSit.length;
	var lstidx = 0;
	for(idx = 0; idx < max; idx++){
		clsGS = m_aryGestSit[idx];
		if(clsGS.tno == 0 && clsGS.sno == 0){
			lstGest.options[lstidx] = new Option(clsGS.name, clsGS.id);
			lstidx++;
		}
	}
}
function fncGetTable(tno)
{
	var idx, max;
	var cnt, cntmax;
	var tidx;
	var clsTbl;

	tidx = 1;
	max = m_clsLayout.tlines.length; // 6固定
	for(idx = 0; idx < max; idx++){
		cntmax = m_clsLayout.tlines[idx].tables.length;
		for(cnt = 0; cnt < cntmax; cnt++){
			if(tidx == tno){
				clsTbl = m_clsLayout.tlines[idx].tables[cnt];
				return(clsTbl);
			}
			tidx++;
		}
	}
	return(null);
}
function fncGetTNo(clsTbl)
{
	var idx, max;
	var cnt, cntmax;
	var tno;

	tno = 1;
	max = m_clsLayout.tlines.length; // 6固定
	for(idx = 0; idx < max; idx++){
		cntmax = m_clsLayout.tlines[idx].tables.length;
		for(cnt = 0; cnt < cntmax; cnt++){
			if(clsTbl == m_clsLayout.tlines[idx].tables[cnt]){
				return(tno);
			}
			tno++;
		}
	}
	return(0);
}
function fncDrawPaperCanvas()
{
	var timex, timey;
	var idx, max;
	var cnt, cntmax;
	var x, y;
	var name;
	var clsTA;
	m_cnvsPaper = document.getElementById("cnvsPaper");
	m_ctxPaper = m_cnvsPaper.getContext('2d');

	m_cnvsWidth = m_cnvsPaper.offsetWidth;
	m_cnvsHeight = m_cnvsPaper.offsetHeight;
	m_nScrnLeft = m_cnvsPaper.offsetLeft;
	m_nScrnTop = m_cnvsPaper.offsetTop;
	m_cnvsPaper.width = m_cnvsWidth;
	m_cnvsPaper.height = m_cnvsHeight;
	m_nFontSize = parseInt(m_cnvsHeight / 12);

	m_ctxPaper.clearRect(0, 0, m_cnvsWidth, m_cnvsHeight);
	m_aryTableArea = new Array();
	timex = m_cnvsWidth / 1000.0;
	timey = m_cnvsHeight / 1000.0;
	max = m_clsLayout.tlines.length; // 6固定
	for(idx = 0; idx < max; idx++){
		cntmax = m_clsLayout.tlines[idx].tables.length;
		for(cnt = 0; cnt < cntmax; cnt++){
			x = m_clsLayout.tlines[idx].tables[cnt].x * timex;
			y = m_clsLayout.tlines[idx].tables[cnt].y * timey;
			clsTA = new clsTableArea();
			clsTA.nLine = idx;
			clsTA.nClum = cnt;
			clsTA.sx = x - m_nFontSize/4*3;
			clsTA.sy = y - m_nFontSize/4*3;
			clsTA.ex = x + m_nFontSize/4*3;
			clsTA.ey = y + m_nFontSize/4*3;
			m_aryTableArea.push(clsTA);			
			name = m_clsLayout.tlines[idx].tables[cnt].name;
			fncDrawTable(m_ctxPaper, x, y, name);
		}
	}
}
function fncDrawTable(ctx, x, y, name)
{
	ctx.beginPath();
	ctx.arc(x, y, m_nFontSize/4*3, 0, 2 * Math.PI);
	ctx.closePath();
	ctx.strokeStyle = "black";
	ctx.lineWidth = 2;
	ctx.stroke();

	x = x - m_nFontSize / 2;
	y = y + m_nFontSize / 2 - m_nFontSize / 6;
	ctx.fillStyle = "black";
	ctx.font = m_nFontSize+"px 'ＭＳ ゴシック'";
	ctx.fillText(name, x, y);
}
