
function fncInitKonreiElement()
{
	var dbnm = m_szHotelDB;
	var tble = m_szKonreiTable;
	var fild = "id,username,sinroname1,sinpuname1,kyosiki";
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
	var data = "file=../list/kubun.txt";
	var fnc = fncKubunCallback;
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncKubunCallback(xmlhttp)
{
	var idx;

	var data = xmlhttp.responseText;
	var aryLine = data.split("\r\n");
	var max = aryLine.length;
	var cmbKubun = document.getElementById("cmbKubun");
	var opt = new Option("", 0);
	cmbKubun.options[0] = opt;
	for(idx = 1; idx < max; idx++){
		ary = aryLine[idx].split(",");
		if(1 <= ary.length){
			opt = new Option(ary[0], idx);
			cmbKubun.options[idx] = opt;
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
	var dbnm = m_szHotelDB;
	var tble = "ge"+m_sKonreiNo;
	var fild = "id,name1,name2,sama,kind";
	var data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&termsql="+m_strWhereSql;
	var fnc = fncGestListCallback;
	sendRequest("POST","php/getlist.php",data,false,fnc);
	
}
function fncGestListCallback(xmlhttp)
{
	var setidx;
	var nId;
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("招待者リストを取得することが出来ませんでした");
		return;
	}
	var lstGest = document.getElementById("lstGest");
	lstGest.options.length = 0;
	var aryRec = data.split(";");
	var max = aryRec.length;
	var nGestCount = 0;
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
	if(m_nGestId == 0){
		return;
	}
	var dbnm = m_szHotelDB;
	var tble = "ge"+m_sKonreiNo;
	var fild = "id,name1,name2,yomi,sama,kind,skind";
	fild = fild +",adrsno,adrs1,adrs2,telno";
	fild = fild +",kt1,kt2,kt3,kt4";
	fild = fild +",gift,dish,sub1";
	var where = "WHERE (id="+m_nGestId+") LIMIT 1";
	var data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&where="+where;
	var fnc = fncGestDataCallback;
	sendRequest("POST","php/getdata.php",data,false,fnc);
}
function fncGestDataCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(",");
	if(ary[0] == "0"){
		fncSetDefGestData();
		return;
	}
	if(ary[1] == "" && ary[2] == "" ){
		fncSetDefGestData();
		return;
	}
	txtGestId = document.getElementById("txtGestId");
	txtGestId.value = m_nGestId;
	txtMyouji = document.getElementById("txtMyouji");
	txtMyouji.value = ary[1];
	txtNamae = document.getElementById("txtNamae");
	txtNamae.value = ary[2];

	cmbKeisyou = document.getElementById("cmbKeisyou");
	fnclibSelectData(cmbKeisyou, ary[4]);
	cmbKubun = document.getElementById("cmbKubun");
	fnclibSelectData(cmbKubun, ary[5]);
	cmbRenKubun = document.getElementById("cmbRenKubun");
	fnclibSelectData(cmbRenKubun, ary[6]);
	txtKanayomi = document.getElementById("txtKanayomi");
	txtKanayomi.value = ary[3];
	txtTel = document.getElementById("txtTel");
	txtTel.value = ary[10];
	txtPostNo = document.getElementById("txtPostNo");
	txtPostNo.value = ary[7];
	txtAddress1 = document.getElementById("txtAddress1");
	txtAddress1.value = ary[8];
	txtAddress2 = document.getElementById("txtAddress2");
	txtAddress2.value = ary[9];
	txtKatagaki1 = document.getElementById("txtKatagaki1");
	txtKatagaki1.value = ary[11];
	txtKatagaki2 = document.getElementById("txtKatagaki2");
	txtKatagaki2.value = ary[12];
	txtKatagaki3 = document.getElementById("txtKatagaki3");
	txtKatagaki3.value = ary[13];
	txtKatagaki4 = document.getElementById("txtKatagaki4");
	txtKatagaki4.value = ary[14];
	txtGift = document.getElementById("txtGift");
	txtGift.value = ary[15];
	txtDish = document.getElementById("txtDish");
	txtDish.value = ary[16];
	txtBikou = document.getElementById("txtBikou");
	txtBikou.value = ary[17];
}
function fncSetDefGestData()
{
	txtGestId = document.getElementById("txtGestId");
	txtGestId.value = "";
	txtMyouji = document.getElementById("txtMyouji");
	txtMyouji.value = "";
	txtNamae = document.getElementById("txtNamae");
	txtNamae.value = "";
	cmbKeisyou = document.getElementById("cmbKeisyou");
	fnclibSelectData(cmbKeisyou, "");
	cmbKubun = document.getElementById("cmbKubun");
	fnclibSelectData(cmbKeisyou, "");
	cmbRenKubun = document.getElementById("cmbRenKubun");
	fnclibSelectData(cmbKeisyou, "");
	txtKanayomi = document.getElementById("txtKanayomi");
	txtKanayomi.value = "";
	txtTel = document.getElementById("txtTel");
	txtTel.value = "";
	txtPostNo = document.getElementById("txtPostNo");
	txtPostNo.value = "";
	txtAddress1 = document.getElementById("txtAddress1");
	txtAddress1.value = "";
	txtAddress2 = document.getElementById("txtAddress2");
	txtAddress2.value = "";
	txtKatagaki1 = document.getElementById("txtKatagaki1");
	txtKatagaki1.value = "";
	txtKatagaki2 = document.getElementById("txtKatagaki2");
	txtKatagaki2.value = "";
	txtKatagaki3 = document.getElementById("txtKatagaki3");
	txtKatagaki3.value = "";
	txtKatagaki4 = document.getElementById("txtKatagaki4");
	txtKatagaki4.value = "";
	txtGift = document.getElementById("txtGift");
	txtGift.value = "";
	txtDish = document.getElementById("txtDish");
	txtDish.value = "";
	txtBikou = document.getElementById("txtBikou");
	txtBikou.value = "";
}
