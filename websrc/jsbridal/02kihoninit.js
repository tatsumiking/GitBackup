
// 

function fncInitNengouComboBox()
{
	var data = "";
	var fnc = fncNengouCallback;

	data = "file=,,/list/nengou.txt";
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncNengouCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var aryLine = new Array();
	var max = 0;
	var idx = 0;
	var setidx = 0;
	var opt = new Option("","");
	var cmbKyosikiNengo = document.getElementById("cmbKyosikiNengo");
	var cmbHirouenNengo = document.getElementById("cmbHirouenNengo");

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		cmbKyosikiNengo.options[0] = new Option("西暦", "0");
		cmbKyosikiNengo.options[1] = new Option("令和", "2019");

		cmbHirouenNengo.options[0] = new Option("西暦", "0");
		cmbHirouenNengo.options[1] = new Option("令和", "2019");
	}else{
		aryLine = data.split("\r\n");
		max = aryLine.length;
		for(idx = 1, setidx = 0; idx < max; idx++){
			ary = aryLine[idx].split(",");
			if(2 <= ary.length){
				opt = new Option(ary[0], ary[1]);
				cmbKyosikiNengo.options[setidx] = opt;
				opt = new Option(ary[0], ary[1]);
				cmbHirouenNengo.options[setidx] = opt;
				setidx++;
			}
		}
	}
}
function fncNengouToFullYear(nGG, sNengou)
{
	var max = 0;
	var idx = 0;
	var cmbKyosikiNengo = document.getElementById("cmbKyosikiNengo");
	var nYear = 0;
	var sYear = "";

	max = cmbKyosikiNengo.length;
	// 	西暦をスキップ
	for(idx = 1; idx < max; idx++){
		if(cmbKyosikiNengo.options[idx].text == sNengou){
			nYear = nGG + fnclibStringToInt(cmbKyosikiNengo.options[idx].value) - 1;
			sYear = nYear.toString();
			return(sYear);
		}
	}
	sYear = nGG.toString(); // 西暦
	return(sYear);
}
function fncFullYearToNengou(nYear, sNengou)
{
	var max = 0;
	var idx = 0;
	var base = 0;
	var cmbKyosikiNengo = document.getElementById("cmbKyosikiNengo");
	var nGG = 0;
	var sGG = "";

	max = cmbKyosikiNengo.length;
	// 	西暦をスキップ
	for(idx = max - 1; idx > 0; idx--){
		base = fnclibStringToInt(cmbKyosikiNengo.options[idx].value);
		if(sNengou == "")
		{
			if(base <= nYear)
			{
				// 	年号は1から始まる
				nGG = nYear -  base + 1;
				sGG = fnclibStrnumToStrnum00(nGG);
				sNengou = cmbKyosikiNengo.options[idx].text;
				return[sGG, idx];
			}
		}else{
			if(cmbKyosikiNengo.options[idx].text == sNengou){
				// 	年号は1から始まる
				nGG = nYear -  base + 1;
				sGG = fnclibStrnumToStrnum00(nGG);
				return[sGG, idx];
			}
		}
	}
	sGG = nYear.toString(); // 西暦
	return[sGG, 0];
}
function fncInitSinroZokugaraComboBox()
{
	var data = "";
	var fnc = fncSinroZokugaraCallback;

	data = "file=../list/sinrozokugara.txt";
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncSinroZokugaraCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var aryLine = new Array();
	var max = 0;
	var idx = 0;
	var setidx = 0;
	var ary = new Array();
	var opt = new Option("","");
	var cmbSinroZokugara = document.getElementById("cmbSinroZokugara");

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		cmbSinroZokugara.options[0] = new Option("長男", "1");
		cmbSinroZokugara.options[1] = new Option("次男", "2");
		cmbSinroZokugara.options[2] = new Option("三男", "3");
		cmbSinroZokugara.options[3] = new Option("四男", "4");
		cmbSinroZokugara.options[4] = new Option("五男", "5");
	}else{
		aryLine = data.split("\r\n");
		max = aryLine.length;
		for(idx = 1, setidx = 0; idx < max; idx++){
			ary = aryLine[idx].split(",");
			if(2 <= ary.length){
				opt = new Option(ary[0], ary[1]);
				cmbSinroZokugara.options[setidx] = opt;
				setidx++;
			}
		}
	}
}
function fncInitSinpuZokugaraComboBox()
{
	var data = "";
	var fnc = fncSinpuZokugaraCallback;

	data = "file=../list/sinpuzokugara.txt";
	sendRequest("POST","php/readfile.php",data,false,fnc);
}
function fncSinpuZokugaraCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var aryLine = new Array();
	var max = 0;
	var idx = 0;
	var setidx = 0;
	var ary = new Array();
	var opt = new Option("","");
	var cmbSinpuZokugara = document.getElementById("cmbSinpuZokugara");

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		cmbSinpuZokugara.options[0] = new Option("長女", "1");
		cmbSinpuZokugara.options[1] = new Option("次女", "2");
		cmbSinpuZokugara.options[2] = new Option("三女", "3");
		cmbSinpuZokugara.options[3] = new Option("四女", "4");
		cmbSinpuZokugara.options[4] = new Option("五女", "5");
	}else{
		aryLine = data.split("\r\n");
		max = aryLine.length;
		for(idx = 1, setidx = 0; idx < max; idx++){
			ary = aryLine[idx].split(",");
			if(2 <= ary.length){
				opt = new Option(ary[0], ary[1]);
				cmbSinpuZokugara.options[setidx] = opt;
				setidx++;
			}
		}
	}
}
function fncInitHotelElement()
{
	var rdoNoAsc = document.getElementById("rdoNoAsc");

	fncInitKaijyouComboBox();
	rdoNoAsc.checked = true;
	fncInitKonreiListBox();
}
function fncInitKaijyouComboBox()
{
	var dbnm = "";
	var tble = "";
	var fild = "";
	var sort = "";
	var data = "";
	var fnc = fncKaijyouCallback;

	dbnm = m_szHotelDB;
	tble = "kaijyou";
	fild = "id,name";
	sort = "";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&trmsql"+sort;
	sendRequest("POST","php/getlist.php",data,false,fnc);
}
function fncKaijyouCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var aryRec = new Array();
	var max = 0;
	var idx = 0;
	var cmbKaijyou = document.getElementById("cmbKaijyou");

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert(szTable+"テーブルリストを取得することが出来ませんでした");
		return;
	}
	aryRec = data.split(";");
	max = aryRec.length;
	for(idx = 0;idx < max; idx++){
		ary = aryRec[idx].split(",");
		cmbKaijyou.options[idx] = new Option(ary[1]);
	}
}
function fncInitKonreiListBox()
{
	var dbnm = "";
	var tble = "";
	var fild = "";
	var data = "";
	var fnc = fncKonreiListCallback;

	dbnm = m_szHotelDB;
	tble = m_szKonreiTable;
	fild = "id,username,sinroname1,sinpuname1,kyosiki";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&trmsql="+m_strOderSql;
	sendRequest("POST","php/getlist.php",data,false,fnc);
}
function fncKonreiListCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var aryRec = new Array();
	var max = 0;
	var idx = 0;
	var setidx = 0;
	var lstKonrei = document.getElementById("lstKonrei");
	var cDate = new clsDate();
	var sData = "";

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		fnclibAlert("婚礼リストを取得することが出来ませんでした");
		return;
	}
	lstKonrei.options.length = 0;
	aryRec = data.split(";");
	max = aryRec.length;
	for(idx = 0, setidx = 0;idx < max; idx++){
		ary = aryRec[idx].split(",");
		if(ary.length >= 5){
			cDate.fncSetSimpleData(ary[4].toString());
			sData = ary[1] + " " + ary[2] + "家 " + ary[3] + "家 ";
			sData = sData + cDate.fncGetFormatData("西暦");
			lstKonrei.options[setidx] = new Option(sData, ary[0]);
			setidx++;
		}
	}
}
function fncGetKonreiData()
{
	var dbnm = "";
	var tble = "";
	var fild = "";
	var where = "";
	var data = "";
	var fnc = fncGetKonreiDataCallback;

	dbnm = m_szHotelDB;
	tble = m_szKonreiTable;
	fild = "id,username,password";
	fild = fild + ",kyosiki,hirouen,kaijyou,mukotori";
	fild = fild + ",sinrozoku,sinroname1,sinroname2";
	fild = fild + ",sinpuzoku,sinpuname1,sinpuname2";
	fild = fild + ",sinrodish,sinpudish,sinrosub,sinpusub";
	where = "WHERE (id="+m_nKonreiId+") LIMIT 1";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&where="+where;
	sendRequest("POST","php/getdata.php",data,false,fnc);
}
function fncGetKonreiDataCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var sYY = "";
	var aryNengou = "";

	var txtKonreiId = document.getElementById("txtKonreiId");
	var txtKanriNo = document.getElementById("txtKanriNo");
	var txtKanriPW = document.getElementById("txtKanriPW");

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

	var cmbKaijyou = document.getElementById("cmbKaijyou");
	var chkMukotori = document.getElementById("chkMukotori");

	var cmbSinroZokugara = document.getElementById("cmbSinroZokugara");
	var txtSinroMyouji = document.getElementById("txtSinroMyouji");
	var txtSinroNamae = document.getElementById("txtSinroNamae");

	var cmbSinpuZokugara = document.getElementById("cmbSinpuZokugara");
	var txtSinpuMyouji = document.getElementById("txtSinpuMyouji");
	var txtSinpuNamae = document.getElementById("txtSinpuNamae");

	var txtSinroRyouri = document.getElementById("txtSinroRyouri");
	var txtSinpuRyouri = document.getElementById("txtSinpuRyouri");
	var txtSinroBikou = document.getElementById("txtSinroBikou");
	var txtSinpuBikou = document.getElementById("txtSinpuBikou");

	data = xmlhttp.responseText;
	ary = data.split(",");
	if(ary[0] == "0"){
		txtKonreiId.textContent = m_nKonreiId;
		txtKanriNo.value = m_sKonreiNo;
		txtKanriPW.value = m_sKonreiPW;
		fncSetDefKonreiData();
		fncSaveCsvData();
		return;
	}
	if(ary[7] == "" && ary[11] == "" ){
		m_nKonreiId = ary[0];
		m_sKonreiNo = ary[1];
		m_sKonreiPW = ary[2];
		txtKonreiId.textContent = m_nKonreiId;
		txtKanriNo.value = m_sKonreiNo;
		txtKanriPW.value = m_sKonreiPW;
		fncSetDefKonreiData();
		fncSaveCsvData();
		return;
	}
	txtKonreiId.textContent = ary[0];
	m_nKonreiId = fnclibStringToInt(ary[0]);
	txtKanriNo.value = ary[1];
	m_sKonreiNo = ary[1];
	txtKanriPW.value = ary[2];
	m_sKonreiPW = ary[2];
	
	sYY = ary[3].substr(0,4);
	if(m_nDispNengo = 0)
	{
		cmbKyosikiNengo.selectedIndex = 0;
		txtKyosikiGG.value = sYY;
	}else{
		aryNengou = fncFullYearToNengou(fnclibStringToInt(sYY), "");
		cmbKyosikiNengo.selectedIndex = fnclibStringToInt(aryNengou[1]);
		txtKyosikiGG.value = aryNengou[0];
	}
	txtKyosikiMM.value = ary[3].substr(5,2);
	txtKyosikiDD.value = ary[3].substr(8,2);
	txtKyosikiHH.value = ary[3].substr(11,2);
	txtKyosikiMN.value = ary[3].substr(14,2);
	
	sYY = ary[4].substr(0,4);
	if(m_nDispNengo = 0)
	{
		cmbHirouenNengo.selectedIndex = 0;
		txtHirouenGG.value = sYY;
	}else{
		aryNengou = fncFullYearToNengou(fnclibStringToInt(sYY), "");
		cmbHirouenNengo.selectedIndex = fnclibStringToInt(aryNengou[1]);
		txtHirouenGG.value = aryNengou[0];
	}
	txtHirouenMM.value = ary[4].substr(5,2);
	txtHirouenDD.value = ary[4].substr(8,2);
	txtHirouenHH.value = ary[4].substr(11,2);
	txtHirouenMN.value = ary[4].substr(14,2);

	fnclibSelectData(cmbKaijyou, ary[5]);

	if(ary[6] == "0"){
		chkMukotori.checked = false;
	}else{
		chkMukotori.checked = true;
	}

	fnclibSelectData(cmbSinroZokugara, ary[7]);
	txtSinroMyouji.value = ary[8];
	txtSinroNamae.value = ary[9];
	
	fnclibSelectData(cmbSinpuZokugara, ary[10]);
	txtSinpuMyouji.value = ary[11];
	txtSinpuNamae.value = ary[12];
	
	txtSinroRyouri.value = ary[13];
	txtSinpuRyouri.value = ary[14];
	txtSinroBikou.value = ary[15];
	txtSinpuBikou.value = ary[16];

	fncSaveCsvData();
}
function fncSetDefKonreiData()
{
	var now = new Date();
	var sYY = "";
	var sMM = "";
	var sDD = "";
	var sHH = "";
	var aryNengou = "";

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

	var cmbKaijyou = document.getElementById("cmbKaijyou");
	var chkMukotori = document.getElementById("chkMukotori");

	var cmbSinroZokugara = document.getElementById("cmbSinroZokugara");
	var txtSinroMyouji = document.getElementById("txtSinroMyouji");
	var txtSinroNamae = document.getElementById("txtSinroNamae");

	var cmbSinpuZokugara = document.getElementById("cmbSinpuZokugara");
	var txtSinpuMyouji = document.getElementById("txtSinpuMyouji");
	var txtSinpuNamae = document.getElementById("txtSinpuNamae");

	var txtSinroRyouri = document.getElementById("txtSinroRyouri");
	var txtSinpuRyouri = document.getElementById("txtSinpuRyouri");
	var txtSinroBikou = document.getElementById("txtSinroBikou");
	var txtSinpuBikou = document.getElementById("txtSinpuBikou");

	sYY =fncZeroPadding(now.getFullYear(), 4); // 年
	sMM = fncZeroPadding(now.getMonth() + 1, 2); // 月
	sDD = fncZeroPadding(now.getDate(), 2); // 日
	sHH = "17";

	if(m_nDispNengo = 0)
	{
		cmbKyosikiNengo.selectedIndex = 0;
		txtKyosikiGG.value = sYY;
	}else{
		aryNengou = fncFullYearToNengou(fnclibStringToInt(sYY), "");
		cmbKyosikiNengo.selectedIndex = fnclibStringToInt(aryNengou[1]);
		txtKyosikiGG.value = aryNengou[0];
	}
	txtKyosikiMM.value = sMM;
	txtKyosikiDD.value = sDD;
	txtKyosikiHH.value = sHH;
	txtKyosikiMN.value = "00";
	
	if(m_nDispNengo = 0)
	{
		cmbHirouenNengo.selectedIndex = 0;
		txtHirouenGG.value = sYY;
	}else{
		aryNengou = fncFullYearToNengou(fnclibStringToInt(sYY), "");
		cmbHirouenNengo.selectedIndex = fnclibStringToInt(aryNengou[1]);
		txtHirouenGG.value = aryNengou[0];
	}
	txtHirouenMM.value = sMM;
	txtHirouenDD.value = sDD;
	txtHirouenHH.value = sHH;
	txtHirouenMN.value = "30";

	fnclibSelectData(cmbKaijyou, "");
	chkMukotori.checked = false;

	fnclibSelectData(cmbSinroZokugara, "");
	txtSinroMyouji.value = "";
	txtSinroNamae.value = "";
	
	fnclibSelectData(cmbSinpuZokugara, "");
	txtSinpuMyouji.value = "";
	txtSinpuNamae.value = "";
	
	txtSinroRyouri.value = "";
	txtSinpuRyouri.value = "";
	txtSinroBikou.value = "";
	txtSinpuBikou.value = "";
}
