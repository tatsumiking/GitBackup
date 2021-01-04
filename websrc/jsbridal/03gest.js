
var m_szHotelDB;
var m_szKonreiTable;
var m_nKonreiId;
var m_sKonreiNo;

var m_nGestId;

var m_strWhereSql;

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

	var btnNew = document.getElementById("btnNew");
	btnNew.onclick = fncOnClickNew;
	var btnUpdate = document.getElementById("btnUpdate");
	btnUpdate.onclick = fncOnClickUpdate;
	var btnDelete = document.getElementById("btnDelete");
	btnDelete.onclick = fncOnClickDelete;
	var btnFixed = document.getElementById("btnFixed");
	btnFixed.onclick = fncOnClickFixed;
	var aGaiji = document.getElementById("aGaiji");
	aGaiji.href = "list/EUDC.zip";
	var btnReturn = document.getElementById("btnReturn");
	btnReturn.onclick = fncOnClickReturn;

	var rdoAll = document.getElementById("rdoAll");
	rdoAll.onclick = fncOnClickRdoAll;
	var rdoSinro = document.getElementById("rdoSinro");
	rdoSinro.onclick = fncOnClickRdoSinro;
	var rdoSinpu = document.getElementById("rdoSinpu");
	rdoSinpu.onclick = fncOnClickRdoSinpu;

	var lstGest = document.getElementById("lstGest");
	lstGest.onchange = fncOnChangeGestList;

	m_strWhereSql = "";
	fncInitKonreiElement();
	fncInitSamaComboBox();	
	fncInitKubunComboBox();
	fncInitRemKubunComboBox();
	fncInitGestListBox(); // 
	m_nGestId = 0;
	txtGestId = document.getElementById("txtGestId");
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
	var nIdx = lstGest.selectedIndex;
	m_nGestId = lstGest.options[nIdx].value;
	txtGestId = document.getElementById("txtGestId");
	txtGestId.value = m_nGestId;

	fncGetGestData();
}
function fncOnClickNew()
{
	txtGestId = document.getElementById("txtGestId");
	m_nGestId = fnclibStringToInt(txtGestId.value);

	fncNewGestData();
}
function fncOnClickUpdate()
{
	txtGestId = document.getElementById("txtGestId");
	m_nGestId = fnclibStringToInt(txtGestId.value);

	fncUpdateGestData();	
}
function fncOnClickDelete()
{
	txtGestId = document.getElementById("txtGestId");
	m_nGestId = fnclibStringToInt(txtGestId.value);

	fncDeleteGestData();	
}
function fncOnClickFixed()
{
}
function fncOnClickReturn()
{
	var url = "01menu.html";
	window.location.href = url;
}
function fncNewGestData()
{
	if(m_nGestId != 0){
		fnclibAlert("現存の招待者が表示されています");
		return;
	}
	txtMyouji = document.getElementById("txtMyouji");
	var name1 = txtMyouji.value;
	txtNamae = document.getElementById("txtNamae");
	var name2 = txtNamae.value;

	var dbnm = m_szHotelDB;
	sSql = "INSERT INTO ge"+m_sKonreiNo+" ";
	sSql = sSql + "(name1,name2) VALUES ('"+name1+"','"+name2+"');"
	var data = "dbnm="+dbnm+"&sql="+sSql;
	var fnc = fncNewGestCallback;
	sendRequest("POST","php/insertsql.php",data,false,fnc);
}
function fncNewGestCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		return;
	}
	txtGestId = document.getElementById("txtGestId");
	txtGestId.value = ary[1];
	m_nGestId = fnclibStringToInt(ary[1]);
	fncUpdateGestData();
}
function fncUpdateGestData()
{
	if(m_nGestId == 0){
		fnclibAlert("招待者Idが設定されていません");
		return;
	}

	txtMyouji = document.getElementById("txtMyouji");
	var name1 = txtMyouji.value;
	txtNamae = document.getElementById("txtNamae");
	var name2 = txtNamae.value;

	cmbKeisyou = document.getElementById("cmbKeisyou");
	var sama = fnclibSelectedText(cmbKeisyou);
	cmbKubun = document.getElementById("cmbKubun");
	var kind = fnclibSelectedText(cmbKubun);
	cmbRenKubun = document.getElementById("cmbRenKubun");
	var skind = fnclibSelectedText(cmbRenKubun);
	txtKanayomi = document.getElementById("txtKanayomi");
	var yomi = txtKanayomi.value;
	txtTel = document.getElementById("txtTel");
	var telno = txtTel.value;
	txtPostNo = document.getElementById("txtPostNo");
	var adrsno = txtPostNo.value;
	txtAddress1 = document.getElementById("txtAddress1");
	var adrs1 = txtAddress1.value;
	txtAddress2 = document.getElementById("txtAddress2");
	var adrs2 = txtAddress2.value;
	txtKatagaki1 = document.getElementById("txtKatagaki1");
	var kt1 = txtKatagaki1.value;
	txtKatagaki2 = document.getElementById("txtKatagaki2");
	var kt2 = txtKatagaki2.value;
	txtKatagaki3 = document.getElementById("txtKatagaki3");
	var kt3 = txtKatagaki3.value;
	txtKatagaki4 = document.getElementById("txtKatagaki4");
	var kt4 = txtKatagaki4.value;
	txtGift = document.getElementById("txtGift");
	var gift = txtGift.value;
	txtDish = document.getElementById("txtDish");
	var dish = txtDish.value;
	txtBikou = document.getElementById("txtBikou");
	var sub1 = txtBikou.value;

	var dbnm = m_szHotelDB;
	sSql = "UPDATE ge"+m_sKonreiNo+" SET ";
	sSql = sSql+"name1='"+name1+"',name2='"+name2+"',yomi='"+yomi+"',sama='"+sama+"'";
	sSql = sSql+",kind='"+kind+"',skind='"+skind+"'";
	sSql = sSql+",adrsno='"+adrsno+"',adrs1='"+adrs1+"',adrs2='"+adrs2+"',telno='"+telno+"'";
	sSql = sSql+",kt1='"+kt1+"',kt2='"+kt2+"',kt3='"+kt3+"',kt4='"+kt4+"'";
	sSql = sSql+",gift='"+gift+"',dish='"+dish+"',sub1='"+sub1+"'";
	sSql = sSql+" WHERE (id="+m_nGestId+");";
	var data = "dbnm="+dbnm+"&sql="+sSql;
	var fnc = fncUpdateGestCallback;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncUpdateGestCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("ge"+m_sKonreiNo+"テーブルの招待者更新に失敗しました");
		return;
	}
	fncInitGestListBox();
}
function fncDeleteGestData()
{
	if(m_nGestId == 0){
		fnclibAlert("招待者Idが設定されていません");
		return;
	}
	var dbnm = m_szHotelDB;
	var sSql = "DELETE FROM ge"+m_sKonreiNo+" WHERE (id="+m_nGestId+") LIMIT 1;"
	var data = "dbnm="+dbnm+"&sql="+sSql;
	var fnc = fncDeleteGestCallback;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncDeleteGestCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("ge"+m_sKonreiNo+"テーブルの招待者削除に失敗しました");
		return;
	}
	txtGestId = document.getElementById("txtGestId");
	txtGestId.value = "";
	m_nGestId = 0;
	fncInitGestListBox();
}
