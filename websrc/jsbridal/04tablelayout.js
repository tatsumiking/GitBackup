

var m_clsLayout;
var m_szHotelDB;
var m_szKonreiTable;
var m_nKonreiId;
var m_sKonreiNo;

var m_sPaperLocate;
var m_sPaperSize;
var m_sRyoukeKind;
var m_sTableKind;
var m_sTableLayout;
var m_sTablePosition;
var m_aryTableName;
var m_cnvsWidth;
var m_cnvsHeight;

function fncInit()
{
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

	m_clsLayout = new clsTableLayout();
	var btnAdd1 = document.getElementById("btnAdd1");
	btnAdd1.onclick = fncOnClickBtnAdd1;
	var btnDel1 = document.getElementById("btnDel1");
	btnDel1.onclick = fncOnClickBtnDel1;
	var btnAdd2 = document.getElementById("btnAdd2");
	btnAdd2.onclick = fncOnClickBtnAdd2;
	var btnDel2 = document.getElementById("btnDel2");
	btnDel2.onclick = fncOnClickBtnDel2;
	var btnAdd3 = document.getElementById("btnAdd3");
	btnAdd3.onclick = fncOnClickBtnAdd3;
	var btnDel3 = document.getElementById("btnDel3");
	btnDel3.onclick = fncOnClickBtnDel3;
	var btnAdd4 = document.getElementById("btnAdd4");
	btnAdd4.onclick = fncOnClickBtnAdd4;
	var btnDel4 = document.getElementById("btnDel4");
	btnDel4.onclick = fncOnClickBtnDel4;
	var btnAdd5 = document.getElementById("btnAdd5");
	btnAdd5.onclick = fncOnClickBtnAdd5;
	var btnDel5 = document.getElementById("btnDel5");
	btnDel5.onclick = fncOnClickBtnDel5;
	var btnAdd6 = document.getElementById("btnAdd6");
	btnAdd6.onclick = fncOnClickBtnAdd6;
	var btnDel6 = document.getElementById("btnDel6");
	btnDel6.onclick = fncOnClickBtnDel6;

	var btnReturn = document.getElementById("btnReturn");
	btnReturn.onclick = fncOnClickReturn;

	fncInitKonreiElement();
}
function fncOnClickBtnAdd1()
{
	if(m_clsLayout.tlines[0].tables.length >= 6){
		return;
	}
	var tbl = new clsTable();
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
	if(m_clsLayout.tlines[1].tables.length >= 6){
		return;
	}
	var tbl = new clsTable();
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
	if(m_clsLayout.tlines[2].tables.length >= 6){
		return;
	}
	var tbl = new clsTable();
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
	if(m_clsLayout.tlines[3].tables.length >= 6){
		return;
	}
	var tbl = new clsTable();
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
	if(m_clsLayout.tlines[4].tables.length >= 6){
		return;
	}
	var tbl = new clsTable();
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
	if(m_clsLayout.tlines[5].tables.length >= 6){
		return;
	}
	var tbl = new clsTable();
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
	var max, idx;
	var cntmax, cnt;
	var x, y;

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
	var cmbPaperLocate = document.getElementById("cmbPaperLocate");
	m_sPaperLocate = fnclibSelectedText(cmbPaperLocate);
	var cmbPaperSize = document.getElementById("cmbPaperSize");
	m_sPaperSize = fnclibSelectedText(cmbPaperSize);
	var cmbRyoukeKind = document.getElementById("cmbRyoukeKind");
	m_sRyoukeKind = fnclibSelectedText(cmbRyoukeKind);
	var cmbTableKind = document.getElementById("cmbTableKind");
	m_sTableKind = fnclibSelectedText(cmbTableKind);
	var txtStr1 = document.getElementById("txtStr1");
	sLeft = txtStr1.value;
	var txtStr2 = document.getElementById("txtStr2");
	sRight = txtStr2.value;

	var dbnm = m_szHotelDB;
	sSql = "UPDATE "+m_szKonreiTable+" SET ";
	sSql = sSql+"paperlocate='"+m_sPaperLocate+"',papersize='"+m_sPaperSize+"'";
	sSql = sSql+",ryoukekind='"+m_sRyoukeKind+"',tablekind='"+m_sTableKind+"'";
	sSql = sSql+",tablelayout='"+m_sTableLayout+"',tableposition='"+m_sTablePosition+"'";
	sSql = sSql+",lefttext='"+sLeft+"',righttext='"+sRight+"'";
	sSql = sSql+" WHERE (id="+m_nKonreiId+");";
	var data = "dbnm="+dbnm+"&sql="+sSql;
	var fnc = fncUpdateKonreiCallback;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncUpdateKonreiCallback(xmlhttp)
{
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
	fild = fild + ",paperlocate,papersize,ryoukekind,tablekind";
	fild = fild + ",tablelayout,tableposition,lefttext,righttext";
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
	m_sPaperLocate = ary[5];
	m_sPaperSize = ary[6];
	m_sRyoukeKind = ary[7];
	m_sTableKind = ary[8];
	m_sTableLayout = ary[9];
	m_sTablePosition = ary[10];
	var txtStr1 = document.getElementById("txtStr1");
	txtStr1.value = ary[11];
	var txtStr2 = document.getElementById("txtStr2");
	txtStr2.value = ary[12];

	fncInitPaperLocateComboBox();
	fncInitPaperSizeComboBox();
	fncInitRyoukeKindComboBox();
	fncInitTableKindComboBox();
	fncInitAryTableName();
}
function fncInitPaperLocateComboBox()
{
	var data = "file=../list/paperlocate.txt";
	var fnc = fncPaperLocateCallback;
	sendRequest("POST","php/readcanmafile.php",data,false,fnc);
}
function fncPaperLocateCallback(xmlhttp)
{
	var idx;
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		return;
	}
	var max = ary.length;
	var cmbPaperLocate = document.getElementById("cmbPaperLocate");
	var opt = new Option("", 0);
	cmbPaperLocate.options[0] = opt;
	for(idx = 0; idx < max; idx++){
		opt = new Option(ary[idx], idx);
		cmbPaperLocate.options[idx] = opt;
	}
	fnclibSelectData(cmbPaperLocate, m_sPaperLocate);
}
function fncInitPaperSizeComboBox()
{
	var data = "file=../list/papersize.txt";
	var fnc = fncPaperSizeCallback;
	sendRequest("POST","php/readcanmafile.php",data,false,fnc);
}
function fncPaperSizeCallback(xmlhttp)
{
	var idx;
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		return;
	}
	var max = ary.length;
	var cmbPaperSize = document.getElementById("cmbPaperSize");
	var opt = new Option("", 0);
	cmbPaperSize.options[0] = opt;
	for(idx = 0; idx < max; idx++){
		opt = new Option(ary[idx], idx);
		cmbPaperSize.options[idx] = opt;
	}
	fnclibSelectData(cmbPaperSize, m_sPaperSize);
}
function fncInitRyoukeKindComboBox()
{
	var data = "file=../list/ryoukekind.txt";
	var fnc = fncRyoukeKindCallback;
	sendRequest("POST","php/readcanmafile.php",data,false,fnc);
}
function fncRyoukeKindCallback(xmlhttp)
{
	var idx;
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		return;
	}
	var max = ary.length;
	var cmbRyoukeKind = document.getElementById("cmbRyoukeKind");
	var opt = new Option("", 0);
	cmbRyoukeKind.options[0] = opt;
	for(idx = 0; idx < max; idx++){
		opt = new Option(ary[idx], idx);
		cmbRyoukeKind.options[idx] = opt;
	}
	fnclibSelectData(cmbRyoukeKind, m_sRyoukeKind);	
}
function fncInitTableKindComboBox()
{
	var data = "file=../list/tablekind.txt";
	var fnc = fncTableKindCallback;
	sendRequest("POST","php/readcanmafile.php",data,false,fnc);
}
function fncTableKindCallback(xmlhttp)
{
	var idx;
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		return;
	}
	var max = ary.length;
	var cmbTableKind = document.getElementById("cmbTableKind");
	var opt = new Option("", 0);
	cmbTableKind.options[0] = opt;
	for(idx = 0; idx < max; idx++){
		opt = new Option(ary[idx], idx);
		cmbTableKind.options[idx] = opt;
	}
	fnclibSelectData(cmbTableKind, m_sTableKind);
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
	fncDrawPaperCanvas();
}
function fncResetTableCanvas()
{
	var max, idx;
	var tblmax, tblcnt;
	var lastline;
	var spacex, spacey;
	var cnt, cntmax;
	var xpos, ypos;
	var tidx;

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
	tidx = 0;
	spacex = 1000 / (tblmax * 2);
	spacey = 1000 / (lastline * 2);
	ypos = spacey;
	for(idx = 0; idx < max; idx++){
		cntmax = m_clsLayout.tlines[idx].tables.length;
		xpos = (tblmax - cntmax + 1) * spacex;
		for(cnt = 0; cnt < cntmax; cnt++){
			m_clsLayout.tlines[idx].tables[cnt].name = m_aryTableName[tidx];
			m_clsLayout.tlines[idx].tables[cnt].x = xpos;
			m_clsLayout.tlines[idx].tables[cnt].y = ypos;
			xpos = xpos + spacex * 2;
			tidx++;
		}
		ypos = ypos + spacey * 2;
	}
	fncDrawPaperCanvas();
}
function fncDrawPaperCanvas()
{
	var wd, hi;
	var timex, timey;
	var idx, max;
	var cnt, cntmax;
	var x, y;
	var name;
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
	var fsize = parseInt(m_cnvsHeight / 12);

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