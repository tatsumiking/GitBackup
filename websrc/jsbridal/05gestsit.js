
var m_clsLayout = new clsTableLayout();
var m_szHotelDB = "";
var m_szKonreiTable = "";
var m_nKonreiId = 0;
var m_sKonreiNo = "";

var m_sTextSize = "";
var m_sTakasagoKind = "";
var m_sTableLayout = "";
var m_sTablePosition = "";
var m_aryGestSit = new Array();
var m_cnvsWidth = 0;
var m_cnvsHeight = 0;

var m_nSlctGestId = 0;

var m_strWhereSql = "";
var m_strOderSql = "";

var m_nScrnLeft = 0;
var m_nScrnTop = 0;

var m_aryTableArea = new Array();
var m_taLeft = null;
var m_taRight = null;
var m_cnvsPaper;
var m_ctxPaper;
var m_nFontSize = 1.0;


function fncInit()
{
	var sId = "";
	var url = "";
	var btnSave = document.getElementById("btnSave");
	var rdoAll = document.getElementById("rdoAll");
	var rdoSinro = document.getElementById("rdoSinro");
	var rdoSinpu = document.getElementById("rdoSinpu");
	var rdoInput = document.getElementById("rdoInput");
	var rdoKana = document.getElementById("rdoKana");
	var lstGest = document.getElementById("lstGest");
	var btnReturn = document.getElementById("btnReturn");

	m_szHotelDB = localStorage.getItem("HotelDB");
	m_szKonreiTable = localStorage.getItem("KonreiTable");
	m_strUserKind = localStorage.getItem("UserKind");
	sId = localStorage.getItem("KonreiId");
	m_nKonreiId = fnclibStringToInt(sId);
	m_sKonreiNo = localStorage.getItem("KonreiNo");
	if(m_sKonreiNo == "0"){
		fnclibAlert("婚礼が指定されていません");
		url = "02kihon.html";
		window.location.href = url;
		return;
	}

	btnSave.style.visibility = 'hidden';

	rdoAll.onclick = fncOnClickAll;
	rdoSinro.onclick = fncOnClickSinro;
	rdoSinpu.onclick = fncOnClickSinpu;

	rdoInput.onclick = fncOnClickInput;
	rdoKana.onclick = fncOnClickKana;

	lstGest.onchange = fncOnChangeGestList;

	btnReturn.onclick = fncOnClickReturn;

	fncTableTextSetFunction();

	fncInitKonreiElement();
}
function fncOnChangeGestList()
{
	var lstGest = document.getElementById("lstGest");
	var nIdx = 0;

	nIdx = lstGest.selectedIndex;
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
	var cmbTextSize = document.getElementById("cmbTextSize");
	var cmbTakasagoKind = document.getElementById("cmbTakasagoKind");
	var dbnm = "";
	var sSql = "";
	var data = "";
	var fnc = fncUpdateKonreiCallback;

	m_sTextSize = fnclibSelectedText(cmbTextSize);
	m_sTakasagoKind = fnclibSelectedText(cmbTakasagoKind);

	dbnm = m_szHotelDB;
	sSql = "UPDATE "+m_szKonreiTable+" SET ";
	sSql = sSql+"textsize='"+m_sTextSize+"',takasagokind='"+m_sTakasagoKind+"'";
	sSql = sSql+" WHERE (id="+m_nKonreiId+");";
	data = "dbnm="+dbnm+"&sql="+sSql;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncUpdateKonreiCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var url = "";

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert(m_szKonreiTable+"婚礼管理番号"+m_sKonreiNo+"の婚礼レコードの更新に失敗しました");
		return;
	}
	url = "01menu.html";
	window.location.href = url;
}
function fncInitKonreiElement()
{
	var dbnm = "";
	var tble = "";
	var fild = "";
	var where = "";
	var data = "";
	var fnc = fncGetKonreiDataCallback;

	dbnm = m_szHotelDB;
	tble = m_szKonreiTable;
	fild = "id,username,sinroname1,sinpuname1,kyosiki";
	fild = fild + ",textsize,takasagokind,tablelayout,tableposition";
	where = "WHERE (id="+m_nKonreiId+") LIMIT 1";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&where="+where;
	sendRequest("POST","php/getdata.php",data,false,fnc);
}
function fncGetKonreiDataCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var sStr = "";
	var txtKonrei = document.getElementById("txtKonrei");

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("婚礼データを取得することが出来ませんでした");
		return;
	}
	sStr = "婚礼管理番号("+ary[1]+")　"+ary[2]+"家　"+ary[3]+"家　"+ary[4];
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
	var data = "";
	var fnc = fncTextSizeCallback;

	data = "file=../list/textsize.txt";
	sendRequest("POST","php/readcanmafile.php",data,false,fnc);
}
function fncTextSizeCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var cmbTextSize = document.getElementById("cmbTextSize");
	var max = 0;
	var idx = 0;
	var optidx = 0;
	var opt = new Option("", 0);

	idx;
	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		cmbTextSize.options[0] = new Option("", 0);
		cmbTextSize.options[1] = new Option("小", 1);
		cmbTextSize.options[2] = new Option("中", 2);
		cmbTextSize.options[3] = new Option("大", 3);
	}else{
		max = ary.length;
		cmbTextSize.options[0] = new Option("", 0);
		for(idx = 0; idx < max; idx++){
			optidx = idx+1;
			opt = new Option(ary[idx], optidx);
			cmbTextSize.options[optidx] = opt;
		}
	}
	fnclibSelectData(cmbTextSize, m_sTextSize);	
}
function fncInitTakasagoKindComboBox()
{
	var data = "";
	var fnc = fncTakasagoKindCallback;

	data = "file=../list/takasagokind.txt";
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncTakasagoKindCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var aryLine = new Array();
	var cmbTakasagoKind = document.getElementById("cmbTakasagoKind");
	var max = 0;
	var idx = 0;
	var optidx = 0;
	var opt = new Option("", 0);

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		cmbTakasagoKind.options[0] = new Option("", 0);
		cmbTakasagoKind.options[1] = new Option("横書枠有", 1);
		cmbTakasagoKind.options[2] = new Option("横書高砂上(高砂名無し)", 2);
		cmbTakasagoKind.options[3] = new Option("横書高砂上(高砂名有)", 3);
		cmbTakasagoKind.options[4] = new Option("縦書高砂上(高砂名無し)", 4);
		cmbTakasagoKind.options[5] = new Option("縦書高砂上(高砂名有)", 5);
		cmbTakasagoKind.options[6] = new Option("横書媒酌有枠有", 6);
		cmbTakasagoKind.options[7] = new Option("横書媒酌有高砂上(高砂名無し)", 7);
		cmbTakasagoKind.options[8] = new Option("横書媒酌有高砂上(高砂名有)", 8);
		cmbTakasagoKind.options[9] = new Option("縦書媒酌有高砂上(高砂名無し)", 9);
		cmbTakasagoKind.options[10] = new Option("縦書媒酌有高砂上(高砂名有)", 10);
	}else{
		aryLine = data.split("\r\n");
		max = aryLine.length;
		cmbTakasagoKind.options[0] = new Option("", 0);
		for(idx = 0; idx < max; idx++){
			optidx = idx+1;
			opt = new Option(aryLine[idx], optidx);
			cmbTakasagoKind.options[optidx] = opt;
		}
	}
	fnclibSelectData(cmbTakasagoKind, m_sTakasagoKind);	
}
function fncInitAryTableName()
{
	var data = "";
	var fnc = fncTableNameCallback;

	data = "file=../list/tablename.txt";
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncTableNameCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		m_aryTableName = new Array("Ａ","Ｂ","Ｃ","Ｄ","Ｅ","Ｆ","Ｇ","Ｈ","Ｉ","Ｊ","Ｋ","Ｌ","Ｍ","Ｎ","Ｏ","Ｐ","Ｑ","Ｒ","Ｓ","Ｔ","Ｕ","Ｖ","Ｗ","Ｘ","Ｙ","Ｚ","ａ","ｂ","ｃ","ｄ","ｅ","ｆ","ｇ","ｈ","ｉ","ｊ","ｋ","ｌ","ｍ","ｎ","ｏ","ｐ","ｑ","ｒ","ｓ","ｔ","ｕ","ｖ","ｗ","ｘ","ｙ","ｚ");
	}else{
		m_aryTableName = data.split("\r\n");
	}
	fncInitTableLayout();
}
function fncInitTableLayout()
{
	// tblcnts 3:3:0:0:0:0:
	// tblposs 166x250:500x250:833x250:166x750:500x750:833x750
	var arycnt = new Array(); 
	var arypos = new Array();
	var tblidx = 0;
	var max = 0;
	var idx = 0;
	var cntmax = 0;
	var cnt = 0;
	var aryxy =  new Array();
	var tbl = new clsTable();

	arycnt = m_sTableLayout.split(":");
	arypos = m_sTablePosition.split(":");
	tblidx = 0;
	max = m_clsLayout.tlines.length; // 6固定
	for(idx = 0; idx < max; idx++){
		cntmax = fnclibStringToInt(arycnt[idx]);
		for(cnt = 0; cnt < cntmax; cnt++){
			aryxy = arypos[tblidx].split("x");
			tbl = new clsTable();
			tbl.x = fnclibStringToInt(aryxy[0]);
			tbl.y = fnclibStringToInt(aryxy[1]);
			tbl.name = m_aryTableName[tblidx];
			m_clsLayout.tlines[idx].tables.push(tbl);
			tblidx++;
		}
	}
	fncInitGetSitArray();
	fncDrawPaperCanvas();
}
function fncInitGetList()
{
	var dbnm = "";
	var tble = "";
	var fild = "";
	var trmsql = "";
	var data = "";
	var fnc = fncGestListCallback;

	dbnm = m_szHotelDB;
	tble = "ge"+m_sKonreiNo;
	fild = "id,name1,name2,kind";
	trmsql = m_strWhereSql + " " + m_strOderSql;
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&trmsql="+trmsql;
	sendRequest("POST","php/getlist.php",data,false,fnc);
}
function fncGestListCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var lstGest = document.getElementById("lstGest");
	var aryRec = new Array();
	var max = 0;
	var idx = 0;
	var lstidx = 0;
	var name = "";

	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("招待者リストを取得することが出来ませんでした");
		return;
	}
	lstGest.options.length = 0;
	aryRec = data.split(";");
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
	var max = 0;
	var idx = 0;
	var nId = 0;

	nId = fnclibStringToInt(sId);
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
	var dbnm = "";
	var tble = "";
	var fild = "";
	var trmsql = "";
	var data = "";
	var fnc = fncGestSitArrayCallback;

	dbnm = m_szHotelDB;
	tble = "ge"+m_sKonreiNo;
	fild = "id,name1,name2,kind,tno,sno";
	trmsql = "";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&trmsql="+trmsql;
	sendRequest("POST","php/getlist.php",data,false,fnc);
}
function fncGestSitArrayCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var aryRec = new Array();
	var max = 0;
	var idx = 0;
	var clsGS = new clsGestSit();
	var clsTbl = new clsTable();
	var sitidx = 0;

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("招待者リストを取得することが出来ませんでした");
		return;
	}
	m_aryGestSit.length = 0;
	aryRec = data.split(";");
	max = aryRec.length;
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
	var lstGest = document.getElementById("lstGest");
	var lstidx = 0;
	var max = 0;
	var idx = 0;
	var clsGS = new clsGestSit();

	lstGest.options.length = 0;
	max = m_aryGestSit.length;
	lstidx = 0;
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
	var tblidx = 0;
	var max = 0;
	var idx = 0;
	var cntmax = 0;
	var cnt = 0;
	var clsTbl = new clsTable();

	tblidx = 1;
	max = m_clsLayout.tlines.length; // 6固定
	for(idx = 0; idx < max; idx++){
		cntmax = m_clsLayout.tlines[idx].tables.length;
		for(cnt = 0; cnt < cntmax; cnt++){
			if(tblidx == tno){
				clsTbl = m_clsLayout.tlines[idx].tables[cnt];
				return(clsTbl);
			}
			tblidx++;
		}
	}
	return(null);
}
function fncGetTNo(clsTbl)
{
	var max = 0;
	var idx = 0;
	var cntmax = 0;
	var cnt = 0;
	var tno = 0;

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
	var timex = 1.0;
	var timey = 1.0;
	var max = 0;
	var idx = 0;
	var cntmax = 0;
	var cnt = 0;
	var x = 0.0;
	var y = 0.0;
	var name = "";
	var clsTA = new clsTableArea();
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
