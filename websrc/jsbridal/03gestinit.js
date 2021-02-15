
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
}
function fncInitSamaComboBox()
{
	var cmbKeisyou = document.getElementById("cmbKeisyou");

	cmbKeisyou.options[0] = new Option("", 0);
	cmbKeisyou.options[1] = new Option("様", 1);
	cmbKeisyou.options[2] = new Option("くん", 2);
	cmbKeisyou.options[3] = new Option("ちゃん", 3);
}
function fncInitKubunComboBox()
{
	var data = "";
	var fnc = fncKubunCallback;

	data = "file=../list/kubun.txt";
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncKubunCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var aryLine = new Array();
	var max = 0;
	var idx = 0;
	var opt = new Option("", 0);
	var cmbKubun = document.getElementById("cmbKubun");

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] != "0"){
		aryLine = data.split("\r\n");
		max = aryLine.length;
		cmbKubun.options[0] = opt;
		for(idx = 1; idx < max; idx++){
			ary = aryLine[idx].split(",");
			if(1 <= ary.length){
				opt = new Option(ary[0], idx);
				cmbKubun.options[idx] = opt;
			}
		}
	}
}
function fncInitRemKubunComboBox()
{
	var cmbRenKubun = document.getElementById("cmbRenKubun");

	cmbRenKubun.options[0] = new Option("", 0);
	cmbRenKubun.options[1] = new Option("本人名前", 1);
	cmbRenKubun.options[2] = new Option("令夫人", 2);
	cmbRenKubun.options[3] = new Option("御家族", 3);
	cmbRenKubun.options[4] = new Option("御一同", 4);
}
function fncInitGestListBox()
{
	var dbnm = "";
	var tble = "";
	var fild = "";
	var data = "";
	var fnc = fncGestListCallback;

	dbnm = m_szHotelDB;
	tble = "ge"+m_sKonreiNo;
	fild = "id,name1,name2,sama,kind";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&termsql="+m_strWhereSql;
	sendRequest("POST","php/getlist.php",data,false,fnc);
	
}
function fncGestListCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var aryRec = new Array();
	var max = 0;
	var idx = 0;
	var nGestCount = 0;
	var setidx = 0;
	var nId = 0;
	var sData = "";
	var lstGest = document.getElementById("lstGest");

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("招待者リストを取得することが出来ませんでした");
		return;
	}
	lstGest.options.length = 0;
	aryRec = data.split(";");
	max = aryRec.length;
	nGestCount = 0;
	for(idx = 0, setidx = 0;idx < max; idx++){
		ary = aryRec[idx].split(",");
		if(ary.length >= 5 && ary[1] != ""){
			nId = fnclibStringToInt(ary[0]);
			sData = fncZeroPadding(nId, 3);
			sData = sData + " " + ary[1] + " " + ary[2] + ary[3];
			sData = sData + " " + ary[4];
			lstGest.options[setidx] = new Option(sData, ary[0]);
			setidx++;
			nGestCount++;
		}
	}
}
function fncGetGestData()
{
	var dbnm = "";
	var tble = "";
	var fild = "";
	var where = "";
	var data = "";
	var fnc = fncGestDataCallback;

	if(m_nGestId == 0){
		return;
	}
	dbnm = m_szHotelDB;
	tble = "ge"+m_sKonreiNo;
	fild = "id,name1,name2,yomi,sama,kind,skind";
	fild = fild +",adrsno,adrs1,adrs2,telno";
	fild = fild +",kt1,kt2,kt3,kt4";
	fild = fild +",gift,dish,sub1";
	where = "WHERE (id="+m_nGestId+") LIMIT 1";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&where="+where;
	sendRequest("POST","php/getdata.php",data,false,fnc);
}
function fncGestDataCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var txtGestId = document.getElementById("txtGestId");
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

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fncSetDefGestData();
		return;
	}
	if(ary[1] == "" && ary[2] == "" ){
		fncSetDefGestData();
		return;
	}
	txtGestId.value = m_nGestId;
	txtMyouji.value = ary[1];
	txtNamae.value = ary[2];
	fnclibSelectData(cmbKeisyou, ary[4]);
	fnclibSelectData(cmbKubun, ary[5]);
	fnclibSelectData(cmbRenKubun, ary[6]);
	txtKanayomi.value = ary[3];
	txtTel.value = ary[10];
	txtPostNo.value = ary[7];
	txtAddress1.value = ary[8];
	txtAddress2.value = ary[9];
	txtKatagaki1.value = ary[11];
	txtKatagaki2.value = ary[12];
	txtKatagaki3.value = ary[13];
	txtKatagaki4.value = ary[14];
	txtGift.value = ary[15];
	txtDish.value = ary[16];
	txtBikou.value = ary[17];
}
function fncSetDefGestData()
{
	var txtGestId = document.getElementById("txtGestId");
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

	txtGestId.value = "";
	txtMyouji.value = "";
	txtNamae.value = "";
	fnclibSelectData(cmbKeisyou, "");
	fnclibSelectData(cmbKubun, "");
	fnclibSelectData(cmbRenKubun, "");
	txtKanayomi.value = "";
	txtTel.value = "";
	txtPostNo.value = "";
	txtAddress1.value = "";
	txtAddress2.value = "";
	txtKatagaki1.value = "";
	txtKatagaki2.value = "";
	txtKatagaki3.value = "";
	txtKatagaki4.value = "";
	txtGift.value = "";
	txtDish.value = "";
	txtBikou.value = "";
}
