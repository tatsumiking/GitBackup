//

function fncTestDataCreate()
{
	var data = "dbnm="+m_szBridalDB;
	var fnc = fncTestDataCreateCallback;
	sendRequest("POST","initdb.php",data,false,fnc);
}
function fncTestDataCreateCallback(xmlhttp)
{
	var retstr = xmlhttp.responseText;
	var ary = retstr.split(",");
	if(ary[0] == "0"){
		alert("データベース"+m_szBridalDB+"の初期化に失敗しました");
		return;
	}
	var data = "dbnm="+m_szBridalDB;
	data = data + "&tble="+m_szKonreiTable;
	fild = "flag,userno";
	type = "0,0";
	fild = fild + ",username,password,dbname,tablelayout";
	type = type + ",6,32,16,16";
	fild = fild + ",kihonlockflag,gestlockflag,sitlockflag";
	type = type + ",0,0,0";
	fild = fild + ",kyosiki,hirouen,kaijyou,mukotori";
	type = type + ",1,1,96,0";
	fild = fild + ",sinrozoku,sinroname1,sinroname2";
	type = type + ",12,18,18";
	fild = fild + ",sinpuzoku,sinpuname1,sinpuname2";
	type = type + ",12,18,18";
	fild = fild + ",sinrodish,sinpudish,sinrosub,sinpusub";
	type = type + ",48,48,96,96";
	fild = fild + ",paperlocate,papersize,ryoukekind,tablekind";
	type = type + ",18,12,48,24";
	fild = fild + ",textsize,takasagokind,nametype";
	type = type + ",12,48,12";
	fild = fild + ",flag1,flag2";
	type = type + ",0,0";
	data = data + "&fild="+fild+"&type="+type;
	var fnc = fncCreateKihonTblCallback;
	sendRequest("POST","inittbl.php",data,false,fnc);
}
function fncCreateKihonTblCallback(xmlhttp)
{
	var retstr = xmlhttp.responseText;
	var ary = retstr.split(",");
	if(ary[0] == "0"){
		alert("婚礼テーブルの作成に失敗しました");
		return;
	}
	sqlstr = "INSERT INTO "+m_szKonreiTable;
	sqlstr = sqlstr+" (flag,username,password) VALUES";
	sqlstr = sqlstr+" (1,'0000','0000');";
	var data = "dbnm="+m_szBridalDB;
	data = data + "&sql="+sqlstr;
	var fnc = fncSetKanriDataCallback;
	sendRequest("POST","execsql.php",data,false,fnc);
}
function fncSetKanriDataCallback(xmlhttp)
{
	var retstr = xmlhttp.responseText;
	var ary = retstr.split(",");
	if(ary[0] == "0"){
		alert("管理者データの失敗しました");
		return;
	}
	alert("管理者データを作成しました");
	fncInitDataBaseNameCombobox();
}

function fncUploadOnCsvLoad(e)
{
	var base64img;
	
	base64img = e.target.result;

	var data = "file="+"temp/upload/ge"+m_sKonreiNo+".csv";
	data = data + "&data="+base64img;
	sendRequest("POST","uploadcsv.php",data,false,fncUploadCsvCallBack);
}
function fncUploadCsvCallBack(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(',');
	if(ary[0] == "0"){
		fnclibAlert("ERROR ファイルのUPLOADに失敗しました");
		return;
	}
	var dbnm = m_szBridalDB;
	var krtbl = m_szKonreiTable;
	var krno = m_sKonreiNo;
	var krpw = "";
	var data = "dbnm="+dbnm+"&tble="+krtbl+"&krno="+krno+"&krpw="+krpw;
	var fnc = fncCheckKonreiCallBack;
	sendRequest("POST","initkonrei.php",data,false,fnc);
}
function fncCheckKonreiCallBack(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(',');
	if(ary[0] == "0"){
		fnclibAlert("ERROR 婚礼関係のテーブル作成に失敗しました");
		return;
	}
	m_nKonreiId = fnclibStringToInt(ary[1]);
	var dbnm = m_szBridalDB;
	var krtbl = m_szKonreiTable;
	var recid = m_nKonreiId;
	var data = "dbnm="+dbnm+"&krtbl="+krtbl+"&recid="+recid+"&krno="+m_sKonreiNo;
	sendRequest("POST","loadcsv.php",data,false,fncLoadCsvCallBack);	
}
function fncLoadCsvCallBack(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(',');
	if(ary[0] == "0"){
		fnclibAlert("ERROR 婚礼情報を読み込みに失敗しました");
		return;
	}
	fnclibAlert("婚礼情報を読み込みました");
	fncInitDataBaseNameCombobox();
}
