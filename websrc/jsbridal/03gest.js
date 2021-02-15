
var m_szHotelDB = "";
var m_szKonreiTable = "";
var m_nKonreiId = 0;
var m_sKonreiNo = "";

var m_nGestId = 0;

var m_strWhereSql = "";

function fncInit()
{
	var sId = "";
	var url = "";
	var btnNew = document.getElementById("btnNew");
	var btnUpdate = document.getElementById("btnUpdate");
	var btnDelete = document.getElementById("btnDelete");
	var btnFixed = document.getElementById("btnFixed");
	var aGaiji = document.getElementById("aGaiji");
	var btnReturn = document.getElementById("btnReturn");
	var rdoAll = document.getElementById("rdoAll");
	var rdoSinro = document.getElementById("rdoSinro");
	var rdoSinpu = document.getElementById("rdoSinpu");
	var lstGest = document.getElementById("lstGest");
	var txtGestId = document.getElementById("txtGestId");

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

	btnNew.onclick = fncOnClickNew;
	btnUpdate.onclick = fncOnClickUpdate;
	btnDelete.onclick = fncOnClickDelete;
	btnFixed.onclick = fncOnClickFixed;
	aGaiji.href = "list/EUDC.zip";
	btnReturn.onclick = fncOnClickReturn;

	rdoAll.onclick = fncOnClickRdoAll;
	rdoSinro.onclick = fncOnClickRdoSinro;
	rdoSinpu.onclick = fncOnClickRdoSinpu;

	lstGest.onchange = fncOnChangeGestList;

	m_strWhereSql = "";
	fncInitKonreiElement();
	fncInitSamaComboBox();	
	fncInitKubunComboBox();
	fncInitRemKubunComboBox();
	fncInitGestListBox(); // 
	m_nGestId = 0;
	txtGestId.value = "";
}
function fncOnClickRdoAll()
{
	m_strWhereSql = "";
	fncInitGestListBox();
}
function fncOnClickRdoSinro()
{
	m_strWhereSql = "WHERE (flag=1)";
	fncInitGestListBox();
}
function fncOnClickRdoSinpu()
{
	m_strWhereSql = "WHERE (flag=2)";
	fncInitGestListBox();
}
function fncOnChangeGestList()
{
	var lstGest = document.getElementById("lstGest");
	var nIdx = "";
	var txtGestId = document.getElementById("txtGestId");

	nIdx = lstGest.selectedIndex;
	m_nGestId = lstGest.options[nIdx].value;
	txtGestId.value = m_nGestId;
	fncGetGestData();
}
function fncOnClickNew()
{
	var txtGestId = document.getElementById("txtGestId");

	m_nGestId = fnclibStringToInt(txtGestId.value);
	fncNewGestData();
}
function fncOnClickUpdate()
{
	var txtGestId = document.getElementById("txtGestId");

	m_nGestId = fnclibStringToInt(txtGestId.value);
	fncUpdateGestData();	
}
function fncOnClickDelete()
{
	var txtGestId = document.getElementById("txtGestId");

	m_nGestId = fnclibStringToInt(txtGestId.value);
	fncDeleteGestData();	
}
function fncOnClickFixed()
{
}
function fncOnClickReturn()
{
	var url = "";

	url = "01menu.html";
	window.location.href = url;
}
function fncNewGestData()
{
	var txtMyouji = document.getElementById("txtMyouji");
	var txtNamae = document.getElementById("txtNamae");
	var name1 = "";
	var name2 = "";
	var dbnm = "";
	var sSql = "";
	var data = "";
	var fnc = fncNewGestCallback;

	if(m_nGestId != 0){
		fnclibAlert("現存の招待者が表示されています");
		return;
	}
	name1 = txtMyouji.value;
	name2 = txtNamae.value;

	dbnm = m_szHotelDB;
	sSql = "INSERT INTO ge"+m_sKonreiNo+" ";
	sSql = sSql + "(name1,name2) VALUES ('"+name1+"','"+name2+"');"
	data = "dbnm="+dbnm+"&sql="+sSql;
	sendRequest("POST","php/insertsql.php",data,false,fnc);
}
function fncNewGestCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var txtGestId = document.getElementById("txtGestId");

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		return;
	}
	txtGestId.value = ary[1];
	m_nGestId = fnclibStringToInt(ary[1]);
	fncUpdateGestData();
}
function fncUpdateGestData()
{
	var txtMyouji = document.getElementById("txtMyouji");
	var txtNamae = document.getElementById("txtNamae");
	var cmbKeisyou = document.getElementById("cmbKeisyou");
	var cmbKubun = document.getElementById("cmbKubun");
	var cmbRenKubun = document.getElementById("cmbRenKubun");
	var txtKanayomi = document.getElementById("txtKanayomi");
	var txtTel = document.getElementById("txtTel");
	var txtPostNo = document.getElementById("txtPostNo");
	var txtAddress1 = document.getElementById("txtAddress1");
	var txtAddress2 = document.getElementById("txtAddress2");
	var txtKatagaki1 = document.getElementById("txtKatagaki1");
	var txtKatagaki2 = document.getElementById("txtKatagaki2");
	var txtKatagaki3 = document.getElementById("txtKatagaki3");
	var txtKatagaki4 = document.getElementById("txtKatagaki4");
	var txtGift = document.getElementById("txtGift");
	var txtDish = document.getElementById("txtDish");
	var txtBikou = document.getElementById("txtBikou");
	var name1 = "";
	var name2 = "";
	var sama = "";
	var kind = "";
	var skind = "";
	var yomi = "";
	var telno = "";
	var adrsno = "";
	var adrs1 = "";
	var adrs2 = "";
	var kt1 = "";
	var kt2 = "";
	var kt3 = "";
	var kt4 = "";
	var gift = "";
	var dish = "";
	var sub1 = "";
	var dbnm = "";
	var sSql = "";
	var data = "";
	var fnc = fncUpdateGestCallback;

	if(m_nGestId == 0){
		fnclibAlert("招待者Idが設定されていません");
		return;
	}

	name1 = txtMyouji.value;
	name2 = txtNamae.value;

	sama = fnclibSelectedText(cmbKeisyou);
	kind = fnclibSelectedText(cmbKubun);
	skind = fnclibSelectedText(cmbRenKubun);
	yomi = txtKanayomi.value;
	telno = txtTel.value;
	adrsno = txtPostNo.value;
	adrs1 = txtAddress1.value;
	adrs2 = txtAddress2.value;
	kt1 = txtKatagaki1.value;
	kt2 = txtKatagaki2.value;
	kt3 = txtKatagaki3.value;
	kt4 = txtKatagaki4.value;
	gift = txtGift.value;
	dish = txtDish.value;
	sub1 = txtBikou.value;

	dbnm = m_szHotelDB;
	sSql = "UPDATE ge"+m_sKonreiNo+" SET ";
	sSql = sSql+"name1='"+name1+"',name2='"+name2+"',yomi='"+yomi+"',sama='"+sama+"'";
	sSql = sSql+",kind='"+kind+"',skind='"+skind+"'";
	sSql = sSql+",adrsno='"+adrsno+"',adrs1='"+adrs1+"',adrs2='"+adrs2+"',telno='"+telno+"'";
	sSql = sSql+",kt1='"+kt1+"',kt2='"+kt2+"',kt3='"+kt3+"',kt4='"+kt4+"'";
	sSql = sSql+",gift='"+gift+"',dish='"+dish+"',sub1='"+sub1+"'";
	sSql = sSql+" WHERE (id="+m_nGestId+");";
	data = "dbnm="+dbnm+"&sql="+sSql;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncUpdateGestCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("ge"+m_sKonreiNo+"テーブルの招待者更新に失敗しました");
		return;
	}
	fncInitGestListBox();
}
function fncDeleteGestData()
{
	var dbnm = "";
	var sSql = "";
	var data = "";
	var fnc = fncDeleteGestCallback;

	if(m_nGestId == 0){
		fnclibAlert("招待者Idが設定されていません");
		return;
	}
	dbnm = m_szHotelDB;
	sSql = "DELETE FROM ge"+m_sKonreiNo+" WHERE (id="+m_nGestId+") LIMIT 1;"
	data = "dbnm="+dbnm+"&sql="+sSql;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncDeleteGestCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var txtGestId = document.getElementById("txtGestId");

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("ge"+m_sKonreiNo+"テーブルの招待者削除に失敗しました");
		return;
	}
	txtGestId.value = "";
	m_nGestId = 0;
	fncInitGestListBox();
}
