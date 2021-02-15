
var m_szHotelDB = "";
var m_szKonreiTable = "";
var m_nKonreiId = 0;
var m_sKonreiNo = "";

var m_strSinroName1 = "";
var m_strSinroName2 = "";
var m_strSinpuName1 = "";
var m_strSinpuName2 = "";

var m_strBsyk1Kubun = "";
var m_strBsyk2Kubun = "";

var m_sPaperLocate = "";
var m_sPaperSize = "";
var m_sRyoukeKind = "";
var m_sTableKind = "";
var m_sTextSize = "";
var m_sTakasagoKind = "";
var m_sNameKind = "";
var m_sLeftText = "";
var m_sRightText = "";
var m_sTablePosition = "";

var m_aryClsTables = new Array();
var m_aryGestSit = new Array();

function fncInit()
{
	var url = "";
	var sId = "";

	m_sKonreiNo = localStorage.getItem("KonreiNo");
	if(m_sKonreiNo == "0"){
		fnclibAlert("婚礼が指定されていません");
		url = "02kihon.html";
		window.location.href = url;
		return;
	}
	m_szHotelDB = localStorage.getItem("HotelDB");
	m_szKonreiTable = localStorage.getItem("KonreiTable");
	sId = localStorage.getItem("KonreiId");
	m_nKonreiId = fnclibStringToInt(sId);

	fncInitKonreiElement();
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
	fild = fild + ",textsize,takasagokind,nametype";
	fild = fild + ",lefttext,righttext";
	fild = fild + ",tableposition";
	where = "WHERE (id="+m_nKonreiId+") LIMIT 1";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&where="+where;
	sendRequest("POST","php/getdata.php",data,false,fnc);
}
function fncGetKonreiDataCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var sStr = "";

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("婚礼データを取得することが出来ませんでした");
		return;
	}
	sStr = "婚礼管理番号("+ary[1]+")　"+ary[2]+"家　"+ary[3]+"家　"+ary[4];
	m_sPaperLocate = ary[5];
	m_sPaperSize = ary[6];
	m_sRyoukeKind = ary[7];
	m_sTableKind = ary[8];
	m_sTextSize = ary[9];
	m_sTakasagoKind = ary[10];
	m_sNameKind = ary[11];
	m_sLeftText = ary[12];
	m_sRightText = ary[13];
	m_sTablePosition = ary[14];

	fncInitAryTableName();
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
		m_aryTableName = Array("Ａ","Ｂ","Ｃ","Ｄ","Ｅ","Ｆ","Ｇ","Ｈ","Ｉ","Ｊ","Ｋ","Ｌ","Ｍ","Ｎ","Ｏ","Ｐ","Ｑ","Ｒ","Ｓ","Ｔ","Ｕ","Ｖ","Ｗ","Ｘ","Ｙ","Ｚ","ａ","ｂ","ｃ","ｄ","ｅ","ｆ","ｇ","ｈ","ｉ","ｊ","ｋ","ｌ","ｍ","ｎ","ｏ","ｐ","ｑ","ｒ","ｓ","ｔ","ｕ","ｖ","ｗ","ｘ","ｙ","ｚ");
	}else{
		m_aryTableName = data.split("\r\n");
	}
	fncInitTableLayout();
}
function fncInitTableLayout()
{
	// tblposs 166x250:500x250:833x250:166x750:500x750:833x750

	var arypos = new Array();
	var max = 0;
	var idx = 0;
	var tblidx = 0;
	var aryxy = new Array();
	var tbl = new clsTable();

	arypos = m_sTablePosition.split(":");
	max = arypos.length;
	tblidx = 0;
	for(idx = 0; idx < max; idx++){
		aryxy = arypos[tblidx].split("x");
		tbl = new clsTable();
		tbl.x = fnclibStringToInt(aryxy[0]);
		tbl.y = fnclibStringToInt(aryxy[1]);
		tbl.name = m_aryTableName[tblidx];
		m_aryClsTables.push(tbl);
		tblidx++;
	}
	fncInitGetSitArray();
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
	var lstidx = 0;
	var clsGS = new clsGestSit();
	var clsTbl = new clsTable();

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("招待者リストを取得することが出来ませんでした");
		return;
	}
	m_aryGestSit.length = 0;

	aryRec = data.split(";");
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
	
	fncInitPrintElement();
	fncDrawPaperCanvas();
}
function fncGetTable(tno)
{
	var max = 0;
	var idx = 0;
	var clsTbl = new clsTable();

	tblidx = 1;
	max = m_aryClsTables.length; // 6固定
	for(idx = 0; idx < max; idx++){
		if((idx+1) == tno){
			clsTbl = m_aryClsTables[idx]
			return(clsTbl);
		}
	}
	return(null);
}
