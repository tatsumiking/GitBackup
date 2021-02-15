
var m_szHotelDB = "";
var m_szKonreiTable = "";
var m_nKonreiId = 0;
var m_sKonreiNo = "";

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
	fild = "id,username,kyosiki";
	fild = fild + ",sinroname1,sinroname2,sinpuname1,sinpuname2";
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
	sStr = "婚礼管理番号("+ary[1]+")　"+ary[3]+"家　"+ary[5]+"家　"+ary[2];
	m_strSinroName1 = ary[3];
	m_strSinroName2 = ary[4];
	m_strSinpuName1 = ary[5];
	m_strSinpuName2 = ary[6];
	m_sPaperLocate = ary[7];
	m_sPaperSize = ary[8];
	m_sRyoukeKind = ary[9];
	m_sTableKind = ary[10];
	m_sTextSize = ary[11];
	m_sTakasagoKind = ary[12];
	m_sNameKind = ary[13];
	m_sLeftText = ary[14];
	m_sRightText = ary[15];
	m_sTablePosition = ary[16];
	fncInitBaisyakuKubun();
}
function fncInitBaisyakuKubun()
{
	var data = "";
	var fnc = fncBaisyakuKubunCallback;

	data = "file=../list/kubun.txt";
	sendRequest("POST","php/readfile.php",data,false,fnc);

}
function fncBaisyakuKubunCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var aryLine = new Array();

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		m_strBsyk1Kubun = "御媒妁人";
		m_strBsyk2Kubun = "御媒妁令夫人";
	}else{
		aryLine = data.split("\r\n");
		m_strBsyk1Kubun = aryLine[1];
		m_strBsyk2Kubun = aryLine[2];
	}
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
		aryxy = arypos[tidx].split("x");
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
	for(idx = 0; idx < max; idx++){
		ary = aryRec[idx].split(",");
		if(ary.length >= 6 && ary[1] != ""){
			clsGS = new clsGestSit();
			clsGS.id = fnclibStringToInt(ary[0]);
			clsGS.tno = fnclibStringToInt(ary[4]);
			clsGS.sno = fnclibStringToInt(ary[5]);
			clsGS.sama = ary[3];
			clsGS.name = fncFormatName(ary[1], ary[2]);
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
function fncFormatName(name1, name2)
{
	var len1 = 0;
	var len2 = 0;
	var name = "";

	len1 = name1.length;
	len2 = name2.length;
	if(m_sNameKind == "Type2"){
		if (len1 == 1) {
			name1 = name1 + "　　";
		} else if (len1 == 2) {
			name1 = name1.substr(0, 1)+"　"+name1.substr(1, 1);
		}
		if (len2 == 1) {
			name2 = "　　"+name2;
		} else if (len2 == 2) {
			name2 = name2.substr(0, 1)+"　"+name2.substr(1, 1);
		}
		name = name1+ "　"+name2;

	}else if(m_sNameKind == "Type3"){
		if (len1 == 1) {
			name1 = name1 + "　";
		}
		if (len2 == 1) {
			name2 = "　"+name2;
		}
		name = name1+name2;
	}else{
		if (len1 == 1) {
			name1 = name1 + "　";
		}
		if (len2 == 1) {
			name2 = "　"+name2;
		}
		if (len1 > 2 && len2 > 2) {
			name = name1+name2;
		} else {
			name = name1+ "　"+name2;
		}
	}
	return(name);
}