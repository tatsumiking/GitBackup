
var m_szHotelDB;
var m_szKonreiTable;

function fncInit()
{
	fncInitDataBaseNameCombobox();

	m_szBridalDB = "bridal";
	m_szKonreiTable = "bridaluser";

	var cmbDatabaseName = document.getElementById("cmbDatabaseName");
	cmbDatabaseName.onchange = fncOnChangeDatabaseName;
	var btnCsvLoad = document.getElementById("btnCsvLoad");
	btnCsvLoad.onchange = fncOnChangeCsvLoad;
	var btnTestDataCreate = document.getElementById("btnTestDataCreate");
	btnTestDataCreate.onclick = fncOnClickInitTestDataCraete;
	var btnTableList = document.getElementById("btnTableList");
	btnTableList.onclick = fncOnClickTableList;
	var btnReturn = document.getElementById("btnReturn");
	btnReturn.onclick = fncOnClickReturn;
}
function fncOnChangeDatabaseName()
{
	var cmbDatabaseName = document.getElementById("cmbDatabaseName");
	idx = cmbDatabaseName.selectedIndex;
	var szDatabaseName = cmbDatabaseName.options[idx].text;
	fncInitTableNameCombobox(szDatabaseName)
}
function fncOnChangeCsvLoad()
{
	if(this.files.length == 0){
		fnclibAlert("アップロードファイルが選択されていません");
		return;
	}
	txtKanriNo = document.getElementById("txtKanriNo");
	m_sKonreiNo = txtKanriNo.value;
	var fileObj = this.files[0];
	var fileReader = new FileReader();
	fileReader.onload = fncUploadOnCsvLoad; // asbridal.jsで定義
	fileReader.readAsDataURL(fileObj);
}
function fncOnClickInitTestDataCraete()
{
	fncTestDataCreate(); // asbridal.jsで定義
}
function fncOnClickTableList()
{
	var idx;
	var cmbDatabaseName = document.getElementById("cmbDatabaseName");
	idx = cmbDatabaseName.selectedIndex;
	var szDatabaseName = cmbDatabaseName.options[idx].text;
	var cmbTableName = document.getElementById("cmbTableName");
	idx = cmbTableName.selectedIndex;
	szTableName = cmbTableName.options[idx].text;
	fncListTableData(szDatabaseName, szTableName);
}
function fncOnClickReturn()
{
	var url = "index.html";
	window.location.href = url;
}
function fncInitDataBaseNameCombobox()
{
	data = "dbnm=&sql=SHOW DATABASES;";
	var fnc = fncInitDataBaseNameCallback;
	sendRequest("POST","getlist.php",data,false,fnc);
}
function fncInitDataBaseNameCallback(xmlhttp)
{
	var idx;
	var retstr = xmlhttp.responseText;
	var ary = retstr.split(",");
	if(ary[0] == "0"){
		alert("データベースリストを取得することが出来ませんでした");
		return;
	}
	var cmbDatabaseName = document.getElementById("cmbDatabaseName");
	cmbDatabaseName.options.length = 0;
	ary = retstr.split(";");
	var max = ary.length;
	for(idx = 0;idx < max; idx++){
		cmbDatabaseName.options[idx] = new Option(ary[idx]);
	}
	idx = cmbDatabaseName.selectedIndex;
	var szDatabaseName = cmbDatabaseName.options[idx].text;
	fncInitTableNameCombobox(szDatabaseName)

}
function fncInitTableNameCombobox(szDatabaseName)
{
	data = "dbnm=&sql=SHOW TABLES FROM "+szDatabaseName+";";
	var fnc = fncInitTableNameCallback;
	sendRequest("POST","getlist.php",data,false,fnc);
}
function fncInitTableNameCallback(xmlhttp)
{
	var idx;
	var retstr = xmlhttp.responseText;
	var ary = retstr.split(",");
	if(ary[0] == "0"){
		alert(szTable+"テーブルリストを取得することが出来ませんでした");
		return;
	}
	var cmbTableName = document.getElementById("cmbTableName");
	cmbTableName.options.length = 0;
	ary = retstr.split(";");
	var max = ary.length;
	for(idx = 0;idx < max; idx++){
		cmbTableName.options[idx] = new Option(ary[idx]);
	}
}
function fncListTableData(szDatabaseName, szTableName)
{
	data = "dbnm="+szDatabaseName;
	data = data+"&sql=SELECT * FROM "+szTableName+";";
	var fnc = fncListTableCallback;
	sendRequest("POST","getlist.php",data,false,fnc);
}
function fncListTableCallback(xmlhttp)
{
	var idx;
	var retstr = xmlhttp.responseText;
	var ary = retstr.split(",");
	if(ary[0] == "0"){
		alert(szTable+"テーブルリストを取得することが出来ませんでした");
		return;
	}
	var lstTableData = document.getElementById("lstTableData");
	lstTableData.options.length = 0;
	ary = retstr.split(";");
	var max = ary.length;
	for(idx = 0;idx < max; idx++){
		lstTableData.options[idx] = new Option(ary[idx]);
	}
}


