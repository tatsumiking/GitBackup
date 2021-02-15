

var m_clsLayout = new clsTableLayout();;
var m_szHotelDB = "";
var m_szKonreiTable = "";
var m_nKonreiId = 0;
var m_sKonreiNo = "";

var m_sPaperLocate = "";
var m_sPaperSize = "";
var m_sRyoukeKind = "";
var m_sTableKind = "";
var m_sTableLayout = "";
var m_sTablePosition = "";
var m_aryTableName = new Array();
var m_cnvsWidth = 0;
var m_cnvsHeight = 0;

function fncInit()
{
	var sId = "";
	var url = "";
	var btnAdd1 = document.getElementById("btnAdd1");
	var btnDel1 = document.getElementById("btnDel1");
	var btnAdd2 = document.getElementById("btnAdd2");
	var btnDel2 = document.getElementById("btnDel2");
	var btnAdd3 = document.getElementById("btnAdd3");
	var btnDel3 = document.getElementById("btnDel3");
	var btnAdd4 = document.getElementById("btnAdd4");
	var btnDel4 = document.getElementById("btnDel4");
	var btnAdd5 = document.getElementById("btnAdd5");
	var btnDel5 = document.getElementById("btnDel5");
	var btnAdd6 = document.getElementById("btnAdd6");
	var btnDel6 = document.getElementById("btnDel6");
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

	btnAdd1.onclick = fncOnClickBtnAdd1;
	btnDel1.onclick = fncOnClickBtnDel1;
	btnAdd2.onclick = fncOnClickBtnAdd2;
	btnDel2.onclick = fncOnClickBtnDel2;
	btnAdd3.onclick = fncOnClickBtnAdd3;
	btnDel3.onclick = fncOnClickBtnDel3;
	btnAdd4.onclick = fncOnClickBtnAdd4;
	btnDel4.onclick = fncOnClickBtnDel4;
	btnAdd5.onclick = fncOnClickBtnAdd5;
	btnDel5.onclick = fncOnClickBtnDel5;
	btnAdd6.onclick = fncOnClickBtnAdd6;
	btnDel6.onclick = fncOnClickBtnDel6;

	btnReturn.onclick = fncOnClickReturn;

	fncInitKonreiElement();
}
function fncOnClickBtnAdd1()
{
	var tbl = new clsTable();

	if(m_clsLayout.tlines[0].tables.length >= 6){
		return;
	}
	m_clsLayout.tlines[0].tables.push(tbl);
	fncResetTableCanvas();
}
function fncOnClickBtnDel1()
{
	if(m_clsLayout.tlines[0].tables.length <= 0){
		return;
	}
	m_clsLayout.tlines[0].tables.pop();
	fncResetTableCanvas();
}
function fncOnClickBtnAdd2()
{
	var tbl = new clsTable();

	if(m_clsLayout.tlines[1].tables.length >= 6){
		return;
	}
	m_clsLayout.tlines[1].tables.push(tbl);
	fncResetTableCanvas();
}
function fncOnClickBtnDel2()
{
	if(m_clsLayout.tlines[1].tables.length <= 0){
		return;
	}
	m_clsLayout.tlines[1].tables.pop();
	fncResetTableCanvas();
}
function fncOnClickBtnAdd3()
{
	var tbl = new clsTable();

	if(m_clsLayout.tlines[2].tables.length >= 6){
		return;
	}
	m_clsLayout.tlines[2].tables.push(tbl);
	fncResetTableCanvas();
}
function fncOnClickBtnDel3()
{
	if(m_clsLayout.tlines[2].tables.length <= 0){
		return;
	}
	m_clsLayout.tlines[2].tables.pop();
	fncResetTableCanvas();
}
function fncOnClickBtnAdd4()
{
	var tbl = new clsTable();

	if(m_clsLayout.tlines[3].tables.length >= 6){
		return;
	}
	m_clsLayout.tlines[3].tables.push(tbl);
	fncResetTableCanvas();
}
function fncOnClickBtnDel4()
{
	if(m_clsLayout.tlines[3].tables.length <= 0){
		return;
	}
	m_clsLayout.tlines[3].tables.pop();
	fncResetTableCanvas();
}
function fncOnClickBtnAdd5()
{
	var tbl = new clsTable();

	if(m_clsLayout.tlines[4].tables.length >= 6){
		return;
	}
	m_clsLayout.tlines[4].tables.push(tbl);
	fncResetTableCanvas();
}
function fncOnClickBtnDel5()
{
	if(m_clsLayout.tlines[4].tables.length <= 0){
		return;
	}
	m_clsLayout.tlines[4].tables.pop();
	fncResetTableCanvas();
}
function fncOnClickBtnAdd6()
{
	var tbl = new clsTable();

	if(m_clsLayout.tlines[5].tables.length >= 6){
		return;
	}
	m_clsLayout.tlines[5].tables.push(tbl);
	fncResetTableCanvas();
}
function fncOnClickBtnDel6()
{
	if(m_clsLayout.tlines[5].tables.length <= 0){
		return;
	}
	m_clsLayout.tlines[5].tables.pop();
	fncResetTableCanvas();
}
function fncOnClickReturn()
{
	fncUpdateTableLayoutElement();
}
function fncUpdateTableLayoutElement()
{
	var max = 0;
	var idx = 0;
	var cntmax = 0;
	var cnt = 0;
	var x = 0;
	var y = 0;
	var cmbPaperLocate = document.getElementById("cmbPaperLocate");
	var cmbPaperSize = document.getElementById("cmbPaperSize");
	var cmbRyoukeKind = document.getElementById("cmbRyoukeKind");
	var cmbTableKind = document.getElementById("cmbTableKind");
	var txtStr1 = document.getElementById("txtStr1");
	var txtStr2 = document.getElementById("txtStr2");
	var dbnm = "";
	var sSql = "";
	var data = "";
	var fnc = fncUpdateKonreiCallback;

	m_sTableLayout = "";
	m_sTablePosition = "";
	max = m_clsLayout.tlines.length; // 6固定
	for(idx = 0; idx < 6; idx++){
		cntmax = m_clsLayout.tlines[idx].tables.length;
		m_sTableLayout = m_sTableLayout + cntmax + ":";
		for(cnt = 0; cnt < cntmax; cnt++){
			if(m_sTablePosition != ""){
				m_sTablePosition = m_sTablePosition + ":";
			}
			x = parseInt(m_clsLayout.tlines[idx].tables[cnt].x);
			y = parseInt(m_clsLayout.tlines[idx].tables[cnt].y);
			m_sTablePosition = m_sTablePosition + x + "x" + y;
		}
	}
	m_sPaperLocate = fnclibSelectedText(cmbPaperLocate);
	m_sPaperSize = fnclibSelectedText(cmbPaperSize);
	m_sRyoukeKind = fnclibSelectedText(cmbRyoukeKind);
	m_sTableKind = fnclibSelectedText(cmbTableKind);
	sLeft = txtStr1.value;
	sRight = txtStr2.value;

	dbnm = m_szHotelDB;
	sSql = "UPDATE "+m_szKonreiTable+" SET ";
	sSql = sSql+"paperlocate='"+m_sPaperLocate+"',papersize='"+m_sPaperSize+"'";
	sSql = sSql+",ryoukekind='"+m_sRyoukeKind+"',tablekind='"+m_sTableKind+"'";
	sSql = sSql+",tablelayout='"+m_sTableLayout+"',tableposition='"+m_sTablePosition+"'";
	sSql = sSql+",lefttext='"+sLeft+"',righttext='"+sRight+"'";
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
	fild = fild + ",paperlocate,papersize,ryoukekind,tablekind";
	fild = fild + ",tablelayout,tableposition,lefttext,righttext";
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
	var txtStr1 = document.getElementById("txtStr1");
	var txtStr2 = document.getElementById("txtStr2");

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("婚礼データを取得することが出来ませんでした");
		return;
	}
	sStr = "婚礼管理番号("+ary[1]+")　"+ary[2]+"家　"+ary[3]+"家　"+ary[4];
	txtKonrei.innerText = sStr; // Microsoft
	txtKonrei.textContent = sStr; //W3C
	m_sPaperLocate = ary[5];
	m_sPaperSize = ary[6];
	m_sRyoukeKind = ary[7];
	m_sTableKind = ary[8];
	m_sTableLayout = ary[9];
	m_sTablePosition = ary[10];
	txtStr1.value = ary[11];
	txtStr2.value = ary[12];

	fncInitPaperLocateComboBox();
	fncInitPaperSizeComboBox();
	fncInitRyoukeKindComboBox();
	fncInitTableKindComboBox();
	fncInitAryTableName();
}
function fncInitPaperLocateComboBox()
{
	var data = "";
	var fnc = fncPaperLocateCallback;

	data = "file=../list/paperlocate.txt";
	sendRequest("POST","php/readcanmafile.php",data,false,fnc);
}
function fncPaperLocateCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var max = 0;
	var idx = 0;
	var optidx = 0;
	var cmbPaperLocate = document.getElementById("cmbPaperLocate");
	var opt = new Option("", 0);

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		cmbPaperLocate.options[0] = new Option("", 0);
		cmbPaperLocate.options[1] = new Option("横置き", 1);
		cmbPaperLocate.options[2] = new Option("縦置き", 2);
	}else{
		max = ary.length;
		cmbPaperLocate.options[0] = new Option("", 0);;
		for(idx = 0; idx < max; idx++){
			optidx = idx + 1;
			opt = new Option(ary[idx], optidx);
			cmbPaperLocate.options[optidx] = opt;
		}
	}
	fnclibSelectData(cmbPaperLocate, m_sPaperLocate);
}
function fncInitPaperSizeComboBox()
{
	var data = "";
	var fnc = fncPaperSizeCallback;

	data = "file=../list/papersize.txt";
	sendRequest("POST","php/readcanmafile.php",data,false,fnc);
}
function fncPaperSizeCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var max = 0;
	var idx = 0;
	var optidx = 0;
	var cmbPaperSize = document.getElementById("cmbPaperSize");
	var opt = new Option("", 0);

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		cmbPaperSize.options[0] = new Option("", 0);
		cmbPaperSize.options[1] = new Option("Ａ３", 1);
		cmbPaperSize.options[2] = new Option("Ｂ４", 2);
		cmbPaperSize.options[3] = new Option("Ａ４", 3);

	}else{
		max = ary.length;
		cmbPaperSize.options[0] = new Option("", 0);
		for(idx = 0; idx < max; idx++){
			optidx = idx+1;
			opt = new Option(ary[idx], optidx);
			cmbPaperSize.options[optidx] = opt;
		}
	}
	fnclibSelectData(cmbPaperSize, m_sPaperSize);
}
function fncInitRyoukeKindComboBox()
{
	var data = "";
	var fnc = fncRyoukeKindCallback;

	data = "file=../list/ryoukekind.txt";
	sendRequest("POST","php/readcanmafile.php",data,false,fnc);
}
function fncRyoukeKindCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var max = 0;
	var idx = 0;
	var optidx = 0;
	var cmbRyoukeKind = document.getElementById("cmbRyoukeKind");
	var opt = new Option("", 0);

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		cmbRyoukeKind.options[0] = new Option("", 0);
		cmbRyoukeKind.options[1] = new Option("なし", 1);
		cmbRyoukeKind.options[2] = new Option("結婚披露宴御席次表", 2);
		cmbRyoukeKind.options[3] = new Option("両家結婚披露宴御席次", 3);
		cmbRyoukeKind.options[4] = new Option("両家結婚披露宴御臨席表", 4);
		cmbRyoukeKind.options[5] = new Option("両家結婚披露宴御案内", 5);
		cmbRyoukeKind.options[6] = new Option("2行家", 6);
		cmbRyoukeKind.options[7] = new Option("1行家", 7);
		cmbRyoukeKind.options[8] = new Option("2行様", 8);
		cmbRyoukeKind.options[9] = new Option("1行様", 9);
	}else{
		max = ary.length;
		cmbRyoukeKind.options[0] = new Option("", 0);
		for(idx = 0; idx < max; idx++){
			optidx = idx+1;
			opt = new Option(ary[idx], optidx);
			cmbRyoukeKind.options[optidx] = opt;
		}
	}
	fnclibSelectData(cmbRyoukeKind, m_sRyoukeKind);	
}
function fncInitTableKindComboBox()
{
	var data = "";
	var fnc = fncTableKindCallback;

	data = "file=../list/tablekind.txt";
	sendRequest("POST","php/readcanmafile.php",data,false,fnc);
}
function fncTableKindCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var max = 0;
	var idx = 0;
	var optidx = 0;
	var cmbTableKind = document.getElementById("cmbTableKind");
	var opt = new Option("", 0);

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		cmbTableKind.options[0] = new Option("", 0);
		cmbTableKind.options[1] = new Option("丸テーブル", 1);
		cmbTableKind.options[2] = new Option("菱形テーブル", 2);
	}else{
		max = ary.length;
		cmbTableKind.options[0] = new Option("", 0);;
		for(idx = 0; idx < max; idx++){
			optidx = idx+1;
			opt = new Option(ary[idx], optidx);
			cmbTableKind.options[optidx] = opt;
		}
	}
	fnclibSelectData(cmbTableKind, m_sTableKind);
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
	var max = 0;
	var idx = 0;
	var tblidx = 0;
	var cntmax = 0;
	var cnt = 0;
	var aryxy = new Array();
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
	fncDrawPaperCanvas();
}
function fncResetTableCanvas()
{
	var tblmax = 0;
	var tblcnt = 0;
	var max = 0;
	var idx = 0;
	var lastline = 0;
	var spacex = 0;
	var spacey = 0;
	var cntmax = 0;
	var cnt = 0;
	var xpos = 0;
	var ypos = 0;
	var tblidx = 0;

	tblmax = 0;
	max = m_clsLayout.tlines.length; // 6固定
	for(idx = 0; idx < max; idx++){
		tblcnt = m_clsLayout.tlines[idx].tables.length;
		if(tblcnt != 0){
			lastline = idx+1;
		}
		if(tblcnt > tblmax){
			tblmax = tblcnt;
		}
	}
	tblidx = 0;
	spacex = 1000 / (tblmax * 2);
	spacey = 1000 / (lastline * 2);
	ypos = spacey;
	for(idx = 0; idx < max; idx++){
		cntmax = m_clsLayout.tlines[idx].tables.length;
		xpos = (tblmax - cntmax + 1) * spacex;
		for(cnt = 0; cnt < cntmax; cnt++){
			m_clsLayout.tlines[idx].tables[cnt].name = m_aryTableName[tblidx];
			m_clsLayout.tlines[idx].tables[cnt].x = xpos;
			m_clsLayout.tlines[idx].tables[cnt].y = ypos;
			xpos = xpos + spacex * 2;
			tblidx++;
		}
		ypos = ypos + spacey * 2;
	}
	fncDrawPaperCanvas();
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
	var cnvsPaper = document.getElementById("cnvsPaper");
	var ctxPaper = cnvsPaper.getContext('2d');

	m_cnvsWidth = cnvsPaper.offsetWidth;
	m_cnvsHeight = cnvsPaper.offsetHeight;
	cnvsPaper.width = m_cnvsWidth;
	cnvsPaper.height = m_cnvsHeight;
	ctxPaper.clearRect(0, 0, m_cnvsWidth, m_cnvsHeight);
	timex = m_cnvsWidth / 1000.0;
	timey = m_cnvsHeight / 1000.0;
	max = m_clsLayout.tlines.length; // 6固定
	for(idx = 0; idx < max; idx++){
		cntmax = m_clsLayout.tlines[idx].tables.length;
		for(cnt = 0; cnt < cntmax; cnt++){
			x = m_clsLayout.tlines[idx].tables[cnt].x * timex;
			y = m_clsLayout.tlines[idx].tables[cnt].y * timey;
			name = m_clsLayout.tlines[idx].tables[cnt].name;
			fncDrawTable(ctxPaper, x, y, name);
		}
	}
}
function fncDrawTable(ctx, x, y, name)
{
	var fsize = 1.0;

	fsize = parseInt(m_cnvsHeight / 12);
	ctx.beginPath();
	ctx.arc(x, y, fsize/4*3, 0, 2 * Math.PI);
	ctx.closePath();
	ctx.strokeStyle = "black";
	ctx.lineWidth = 2;
	ctx.stroke();

	x = x - fsize / 2;
	y = y + fsize / 2 - fsize / 6;
	ctx.fillStyle = "black";
	ctx.font = fsize+"px 'ＭＳ ゴシック'";
	ctx.fillText(name, x, y);
}