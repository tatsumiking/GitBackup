
var m_szHotelName = "";
var m_szHotelDB = "";
var m_szKonreiTable = "";
var m_strID = ""; // login value;
var m_strPW = ""; // login value;
var m_strUserKind = "";

var m_nKonreiId = 0; // 画面表示用一事保管
var m_sKonreiNo = ""; // 画面表示用一事保管
var m_sKonreiPW = "";

var m_strOderSql = "";
var m_nDispNengo = 0; // 0 西暦表示 1 年号表示

function fncInit()
{
	var sId = "";
	var btnKonreiCopy = document.getElementById("btnKonreiCopy");
	var btnPCCsvLoad = document.getElementById("btnPCCsvLoad");
	var btnNew = document.getElementById("btnNew");
	var btnUpdate = document.getElementById("btnUpdate");
	var btnDelete = document.getElementById("btnDelete");
	var divKonreiListArea = document.getElementById("divKonreiListArea");
	var rdoNoAsc = document.getElementById("rdoNoAsc");
	var rdoNoDesc = document.getElementById("rdoNoDesc");
	var rdoDateAsc = document.getElementById("rdoDateAsc");
	var rdoDateDesc = document.getElementById("rdoDateDesc");
	var lstKonrei = document.getElementById("lstKonrei");
	var btnReturn = document.getElementById("btnReturn");

	m_szHotelName = localStorage.getItem("HotelName");
	m_szHotelDB = localStorage.getItem("HotelDB");
	m_szKonreiTable = localStorage.getItem("KonreiTable");
	m_strID = localStorage.getItem("BridalID");
	m_strPW = localStorage.getItem("BridalPW");
	m_strUserKind = localStorage.getItem("UserKind");
	m_nDispNengo = 0;

	fncInitNengouComboBox();
	fncInitKaijyouComboBox();

	fncInitSinroZokugaraComboBox();
	fncInitSinpuZokugaraComboBox();

	btnKonreiCopy.onclick = fncOnClickKonreiCopy;
	btnPCCsvLoad.onchange = fncOnChangePCCsvLoad;
	btnNew.onclick = fncOnClickNew;
	btnUpdate.onclick = fncOnClickUpdate;

	if(m_strUserKind == "1")
	{
		btnNew.style.visibility = 'visible';
		btnDelete.style.visibility = 'visible';
		divKonreiListArea.style.visibility = 'visible';
		btnDelete.onclick = fncOnClickDelete;
		// var rdoItems = document.getElementsByName("sort");
		// if(rdoItems[idx].checked == true) rdoItems[idx].value;

		rdoNoAsc.onclick = fncOnClickRdoNoAsc;
		rdoNoDesc.onclick = fncOnClickRdoNoDesc;
		rdoDateAsc.onclick = fncOnClickRdoDateAsc;
		rdoDateDesc.onclick = fncOnClickRdoDateDesc;
		lstKonrei.onchange = fncOnChangeKonreiList;

		sId = localStorage.getItem("KonreiId");
		m_nKonreiId = fnclibStringToInt(sId);
		m_sKonreiNo = localStorage.getItem("KonreiNo");
		if(m_sKonreiNo != "0")
		{
			fncGetKonreiData();
		}
		fncOnClickRdoNoAsc(); // m_strOderSql = " ORDER BY userno ASC";
	}else{
		sId = localStorage.getItem("KonreiId");
		m_nKonreiId = fnclibStringToInt(sId);
		m_sKonreiNo = localStorage.getItem("KonreiNo");

		btnNew.style.visibility = 'hidden';
		btnDelete.style.visibility = 'hidden';
		divKonreiListArea.style.visibility = 'hidden';
		fncGetKonreiData();
	}
	btnReturn.onclick = fncOnClickReturn;
}
function fncOnChangeKonreiList()
{
	var lstKonrei = document.getElementById("lstKonrei");
	var idx = 0;

	idx = lstKonrei.selectedIndex;
	m_nKonreiId = lstKonrei.options[idx].value;
	fncGetKonreiData();
}
function fncOnClickKonreiCopy()
{
	var txtKonreiId = document.getElementById("txtKonreiId");
	var txtKanriNo = document.getElementById("txtKanriNo");

	m_nKonreiId = fnclibStringToInt(txtKonreiId.textContent);
	m_sKonreiNo = txtKanriNo.value;
	fncKonreiCopy(); // 02kihoncopy.jsで定義
}
function fncOnChangePCCsvLoad()
{
	var txtKonreiId = document.getElementById("txtKonreiId");
	var txtKanriNo = document.getElementById("txtKanriNo");
	var fileObj = new Array();
	var fileReader = new FileReader();

	if(this.files.length == 0){
		fnclibAlert("アップロードファイルが選択されていません");
		return;
	}

	m_nKonreiId = fnclibStringToInt(txtKonreiId.textContent);
	m_sKonreiNo = txtKanriNo.value;
	if(m_nKonreiId == 0){
		fnclibAlert("婚礼が選択されていません");
		return;
	}
	fileObj = this.files[0];
	fileReader.onload = fncUploadOnPCCsvLoad;
	fileReader.readAsDataURL(fileObj);
}
function fncUploadOnPCCsvLoad(e)
{
	var base64img = "";
	var data = "";
	
	base64img = e.target.result;
	data = "file="+"../list/ge"+m_sKonreiNo+"/ge"+m_sKonreiNo+".csv";
	data = data + "&data="+base64img;
	sendRequest("POST","php/uploadcsv.php",data,false,fncUploadPCCsvCallBack);
}
function fncUploadPCCsvCallBack(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var dbnm = "";
	var krtbl = "";
	var recid = "";

	data = xmlhttp.responseText;
	ary = data.split(',');
	if(ary[0] == "0"){
		return;
	}
	dbnm = m_szHotelDB;
	krtbl = m_szKonreiTable;
	recid = m_nKonreiId;
	data = "dbnm="+dbnm+"&krtbl="+krtbl+"&recid="+recid+"&krno="+m_sKonreiNo;
	sendRequest("POST","php/loadpccsv.php",data,false,fncLoadPCCsvCallBack);	
}
function fncLoadPCCsvCallBack(xmlhttp)
{
	var data = "";
	var ary = new Array();

	data = xmlhttp.responseText;
	ary = data.split(',');
	if(ary[0] == "0"){
		return;
	}
	fncSaveCsvData();
	fnclibAlert("PCから婚礼招待者情報を読み込みました");
}
function fncOnClickNew()
{
	var txtKonreiId = document.getElementById("txtKonreiId");
	var txtKanriNo = document.getElementById("txtKanriNo");
	var txtKanriPW = document.getElementById("txtKanriPW");

	m_nKonreiId = fnclibStringToInt(txtKonreiId.textContent);
	m_sKonreiNo = txtKanriNo.value;
	m_sKonreiPW = txtKanriPW.value;

	if(m_sKonreiNo == "")
	{
		fnclibAlert("婚礼管理番号が設定されていないため新規作成は出来ません");
		return;
	}
	if(m_sKonreiNo.length != 6)
	{
		fnclibAlert("婚礼管理番号は0を含む６桁で設定されていないため新規作成は出来ません");
		return;
	}
	if(m_sKonreiPW == "")
	{
		fnclibAlert("婚礼管理ＰＷが設定されていないため新規作成は出来ません");
		return;
	}
	if(m_sKonreiPW.length != 6)
	{
		fnclibAlert("婚礼管理ＰＷは0を含む６桁で設定されていないため新規作成は出来ません");
		return;
	}
	fncCheckNOPW();
}
function fncCheckNOPW()
{

	var data = "";
	var fnc = fncCheckNOPWCallback;

	data = "com="+m_sKonreiNo+","+m_sKonreiPW+",";
	sendRequest("POST","php/checkidpw.php",data,false,fnc);
}
function fncCheckNOPWCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var dbnm = "";
	var tble = "";
	var fild = "";
	var where = "";
	var fnc = fncExistsKonreiNoCallBack;

	data = xmlhttp.responseText;
	ary = data.split(',');
	if(ary[0] == "0"){
		fnclibAlert("パスワードが違います");
		return;
	}
	dbnm = m_szHotelDB;
	tble = m_szKonreiTable;
	fild = "id,username";
	where = "WHERE (username="+m_sKonreiNo+") LIMIT 1";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&where="+where;

	sendRequest("POST","php/getdata.php",data,false,fnc);
}
function fncExistsKonreiNoCallBack(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var txtKonreiId = document.getElementById("txtKonreiId");
	var dbnm = "";
	var krtbl = "";
	var krno = "";
	var krpw = "";
	var fnc = fncCheckKonreiCallBack;

	data = xmlhttp.responseText;
	ary = data.split(',');
	if(ary[0] != "0"){
		fnclibAlert("ご使用の婚礼管理番号はすでに使用されています");
		m_nKonreiId = ary[0];
		txtKonreiId.textContent = txtKonreiId;
		fncGetKonreiData();
		return;
	}

	dbnm = m_szHotelDB;
	krtbl = m_szKonreiTable;
	krno = m_sKonreiNo;
	krpw = m_sKonreiPW;
	data = "dbnm="+dbnm+"&krtbl="+krtbl+"&krno="+krno+"&krpw="+krpw;
	sendRequest("POST","php/initkonrei.php",data,false,fnc);
}
function fncCheckKonreiCallBack(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var txtKonreiId = document.getElementById("txtKonreiId");

	data = xmlhttp.responseText;
	ary = data.split(',');
	if(ary[0] == "0"){
		return;
	}
	txtKonreiId.textContent = ary[1];
	m_nKonreiId = fnclibStringToInt(txtKonreiId.textContent);
	fncUpdateKonreiData();
	if(m_strUserKind == "1")
	{
		fncInitKonreiListBox();	
	}
}
function fncOnClickUpdate()
{
	var txtKonreiId = document.getElementById("txtKonreiId");
	var txtKanriNo = document.getElementById("txtKanriNo");

	m_nKonreiId = fnclibStringToInt(txtKonreiId.textContent);
	m_sKonreiNo = txtKanriNo.value;
	
	fncUpdateKonreiData();
}
function fncUpdateKonreiData()
{
	var idx = 0;
	var sKyosiki = "";
	var sHirouen = "";
	var sKaijyou = "";
	var sMukotori = "";
	var sSinroZoku = "";
	var sSinroName1 = "";
	var sSinroName2 = "";
	var sSinpuZoku = "";
	var sSinpuName1 = "";
	var sSinpuName2 = "";
	var sSinroDish = "";
	var sSinpuDish = "";
	var sSinroSub = "";
	var sSinpuSub = "";
	var nGG = "";
	var idx = 0;
	var sNengo = "";
	var sYY = "";
	var sMM = "";
	var sDD = "";
	var sHH = "";
	var sMN = "";
	var cmbKyosikiNengo = document.getElementById("cmbKyosikiNengo");
	var txtKyosikiGG = document.getElementById("txtKyosikiGG");
	var txtKyosikiMM = document.getElementById("txtKyosikiMM");
	var txtKyosikiDD = document.getElementById("txtKyosikiDD");
	var txtKyosikiHH = document.getElementById("txtKyosikiHH");
	var txtKyosikiMN = document.getElementById("txtKyosikiMN");
	var cmbHirouenNengo = document.getElementById("cmbHirouenNengo");
	var txtHirouenGG = document.getElementById("txtHirouenGG");
	var txtHirouenMM = document.getElementById("txtHirouenMM");
	var txtHirouenDD = document.getElementById("txtHirouenDD");
	var txtHirouenHH = document.getElementById("txtHirouenHH");
	var txtHirouenMN = document.getElementById("txtHirouenMN");
	var dbnm = "";
	var sSql = "";
	var data = "";
	var fnc = fncUpdateKonreiCallback;

	cmbKaijyou = document.getElementById("cmbKaijyou");
	chkMukotori = document.getElementById("chkMukotori");
	cmbSinroZokugara = document.getElementById("cmbSinroZokugara");
	txtSinroMyouji = document.getElementById("txtSinroMyouji");
	txtSinroNamae = document.getElementById("txtSinroNamae");
	cmbSinpuZokugara = document.getElementById("cmbSinpuZokugara");
	txtSinpuMyouji = document.getElementById("txtSinpuMyouji");
	txtSinpuNamae = document.getElementById("txtSinpuNamae");
	txtSinroRyouri = document.getElementById("txtSinroRyouri");
	txtSinpuRyouri = document.getElementById("txtSinpuRyouri");
	txtSinroBikou = document.getElementById("txtSinroBikou");
	txtSinpuBikou = document.getElementById("txtSinpuBikou");

	nGG = fnclibStringToInt(txtKyosikiGG.value);
	idx = cmbKyosikiNengo.selectedIndex;
	sNengo = cmbKyosikiNengo.options[idx].text;
	sYY = fncNengouToFullYear(nGG, sNengo);
	sMM = txtKyosikiMM.value
	sDD = txtKyosikiDD.value;
	sHH = txtKyosikiHH.value;
	sMN = txtKyosikiMN.value;
	sKyosiki = sYY+"-"+sMM+"-"+sDD+" "+sHH+":"+sMN+":00";

	idx = cmbHirouenNengo.selectedIndex;
	sNengo = cmbHirouenNengo.options[idx].text;
	nGG = fnclibStringToInt(txtHirouenGG.value);
	sNengo = cmbHirouenNengo.text;
	sYY = fncNengouToFullYear(nGG, sNengo);
	sMM = txtHirouenMM.value
	sDD = txtHirouenDD.value;
	sHH = txtHirouenHH.value;
	sMN = txtHirouenMN.value;
	sHirouen = sYY+"-"+sMM+"-"+sDD+" "+sHH+":"+sMN+":00";

	sKaijyou = cmbKaijyou.value;
	if(chkMukotori.checked == true)
	{
		sMukotori = "1";
	}else{
		sMukotori = "0";
	}

	sSinroZoku = cmbSinroZokugara.value;
	sSinroName1 = txtSinroMyouji.value;
	sSinroName2 = txtSinroNamae.value;

	sSinpuZoku = cmbSinpuZokugara.value;
	sSinpuName1 = txtSinpuMyouji.value;
	sSinpuName2 = txtSinpuNamae.value;

	sSinroDish = txtSinroRyouri.value;
	sSinpuDish = txtSinpuRyouri.value;
	sSinroSub = txtSinroBikou.value;
	sSinpuSub = txtSinpuBikou.value;

	dbnm = m_szHotelDB;
	sSql = "UPDATE "+m_szKonreiTable+" SET ";
	sSql = sSql+"kyosiki='"+sKyosiki+"',hirouen='"+sHirouen+"'";
	sSql = sSql+",kaijyou='"+sKaijyou+"',mukotori="+sMukotori;
	sSql = sSql+",sinrozoku='"+sSinroZoku+"',sinroname1='"+sSinroName1+"',sinroname2='"+sSinroName2+"'";
	sSql = sSql+",sinpuzoku='"+sSinpuZoku+"',sinpuname1='"+sSinpuName1+"',sinpuname2='"+sSinpuName2+"'";
	sSql = sSql+",sinrodish='"+sSinroDish+"',sinpudish='"+sSinpuDish+"'";
	sSql = sSql+",sinrosub='"+sSinroSub+"',sinpusub='"+sSinpuSub+"'";
	sSql = sSql+" WHERE (id="+m_nKonreiId+");";
	data = "dbnm="+dbnm+"&sql="+sSql;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncUpdateKonreiCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert(m_szKonreiTable+"婚礼管理番号"+m_sKonreiNo+"の婚礼レコードの更新に失敗しました");
		return;
	}
	fncSaveCsvData();
}
function fncOnClickDelete()
{
	var txtKonreiId = document.getElementById("txtKonreiId");
	var txtKanriNo = document.getElementById("txtKanriNo");
	var ret = "";
	var sGestTableName = "";
	var sSql = "";
	var data = "";
	var fnc = fncDeleteGestTableCallback;

	m_nKonreiId = fnclibStringToInt(txtKonreiId.textContent);
	m_sKonreiNo = txtKanriNo.value;

	if(m_nKonreiId != 0){
		var ret = fnclibMessageWindow("削除確認","現在表示されている婚礼を削除してよろしいですか？");
		if(ret == "Cancel"){
			return;
		}
	}else{
		fnclibAlert("婚礼が選択されていません");
		return;
	}
	sGestTableName = "ge"+m_sKonreiNo;
	sSql = "DROP TABLE "+sGestTableName; // テーブルの削除
	data = "sql="+sSql;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncDeleteGestTableCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var sGestTableName = "";
	var sSql = "";
	var fnc = fncDeleteKonreiCallback;

	data = xmlhttp.responseText;
	ary = data.split(",");
	sGestTableName = "ge"+m_sKonreiNo;
	if(ary[0] == "0"){
		fnclibAlert("招待者テーブル"+sGestTableName+"の削除に失敗しました");
		return;
	}
	sSql = "DELETE FROM "+m_szKonreiTable+" WHERE (id="+m_nKonreiId+") LIMIT 1;"
	data = "sql="+sSql;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncDeleteKonreiCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var txtKonreiId = document.getElementById("txtKonreiId");

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert(m_szKonreiTable+"テーブルの婚礼管理番号"+m_sKonreiNo+"のレコードの削除に失敗しました");
		return;
	}
	if(m_strUserKind == "1")
	{
		fncInitKonreiListBox();	
	}
	txtKonreiId.textContent = "";
}
function fncOnClickReturn()
{
	var url = "";

	localStorage.setItem("KonreiId", m_nKonreiId);
	localStorage.setItem("KonreiNo", m_sKonreiNo);
	url = "01menu.html";
	window.location.href = url;
}
function fncOnClickRdoNoAsc()
{
	m_strOderSql = " ORDER BY userno ASC";
	if(m_strUserKind == "1")
	{
		fncInitKonreiListBox();	
	}
}
function fncOnClickRdoNoDesc()
{
	m_strOderSql = "ORDER BY userno DESC";
	if(m_strUserKind == "1")
	{
		fncInitKonreiListBox();	
	}
}
function fncOnClickRdoDateAsc()
{
	m_strOderSql = "ORDER BY kyosiki ASC";
	if(m_strUserKind == "1")
	{
		fncInitKonreiListBox();	
	}
}
function fncOnClickRdoDateDesc()
{
	m_strOderSql = "ORDER BY kyosiki DESC";
	if(m_strUserKind == "1")
	{
		fncInitKonreiListBox();	
	}
}
function fncSaveCsvData()
{
	var dbnm = "";
	var krtbl = "";
	var recid = 0;
	var krno = "";
	var data = "";
	var fnc = fncSaveCsvCallBack;

	dbnm = m_szHotelDB;
	krtbl = m_szKonreiTable;
	recid = m_nKonreiId;
	krno = m_sKonreiNo;

	if(m_nKonreiId == 0){
		return;
	}
	data = "dbnm="+dbnm+"&krtbl="+krtbl+"&recid="+recid+"&krno="+krno;
	sendRequest("POST","php/savecsv.php",data,false,fnc);
}
function fncSaveCsvCallBack(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var aCsvSave = document.getElementById("aCsvSave");

	data = xmlhttp.responseText;
	ary = data.split(',');
	if(m_strUserKind == "1")
	{
		fncInitKonreiListBox();	
	}
	if(ary[0] == "0"){
		return;
	}
	aCsvSave.href = "temp/download/ge"+m_sKonreiNo+".csv";
}
// 
