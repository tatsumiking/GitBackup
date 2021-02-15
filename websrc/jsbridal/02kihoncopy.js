
function fncKonreiCopy()
{
	var dbnm = "";
	var tble = "";
	var fild = "";
	var where = "";
	var data = "";
	var fnc = fncKCGetKonreiDataCallback;

	dbnm = "bridal";
	tble = "bridaluser";
	fild = "tablelayout";
	fild = fild + ",kyosiki,hirouen,kaijyou,mukotori";
	fild = fild + ",sinrozoku,sinroname1,sinroname2";
	fild = fild + ",sinpuzoku,sinpuname1,sinpuname2";
	fild = fild + ",sinrodish,sinpudish,sinrosub,sinpusub";
	fild = fild + ",paperlocate,papersize,ryoukekind,tablekind";
	fild = fild + ",textsize,takasagokind,nametype,flag1";

	where = "WHERE (username="+m_sKonreiNo+") LIMIT 1";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&where="+where;

	sendRequest("POST","php/getdata.php",data,false,fnc);
}
function fncKCGetKonreiDataCallback(xmlhttp)
{
	var data = "";
	var a = new Array();
	var dbnm = "";
	var sSql = "";
	var fnc = fncKCUpdateKonreiCallback;

	data = xmlhttp.responseText;
	a = data.split(',');
	if(a[0] == "0"){
		return;
	}
	dbnm = m_szHotelDB;
	sSql = "UPDATE "+m_szKonreiTable+" SET ";
	sSql = sSql+"tablelayout='"+a[0]+"'";
	sSql = sSql+",kyosiki='"+a[1]+"',hirouen='"+a[2]+"'";
	sSql = sSql+",kaijyou='"+a[3]+"',mukotori="+a[4];
	sSql = sSql+",sinrozoku='"+a[5]+"',sinroname1='"+a[6]+"',sinroname2='"+a[7]+"'";
	sSql = sSql+",sinpuzoku='"+a[8]+"',sinpuname1='"+a[9]+"',sinpuname2='"+a[10]+"'";
	sSql = sSql+",sinrodish='"+a[11]+"',sinpudish='"+a[12]+"'";
	sSql = sSql+",sinrosub='"+a[13]+"',sinpusub='"+a[14]+"'";
	sSql = sSql+",paperlocate='"+a[15]+"',papersize='"+a[16]+"'";
	sSql = sSql+",ryoukekind='"+a[17]+"',tablekind='"+a[18]+"'";
	sSql = sSql+",textsize='"+a[19]+"',takasagokind='"+a[20]+"'";
	sSql = sSql+",nametype='"+a[21]+"'";
	sSql = sSql+" WHERE (id="+m_nKonreiId+");";
	data = "dbnm="+dbnm+"&sql="+sSql;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncKCUpdateKonreiCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var dbnm = "";
	var tble = "";
	var fild = "";
	var fnc = fncKCGetGestListCallback;

	data = xmlhttp.responseText;
	ary = data.split(',');
	if(ary[0] == "0"){
		return;
	}
	dbnm = "bridal";
	tble = "ge"+m_sKonreiNo;
	fild = "id,tno,sno,name1,name2,sama,kind,skind,yomi,id";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&termsql=";
	sendRequest("POST","php/getlist.php",data,false,fnc);

}
function fncKCGetGestListCallback(xmlhttp)
{
	var data = "";
	var a = new Array();
	var aryRec = new Array();
	var datasql = "";
	var max = 0;
	var idx = 0;
	var dbnm = "";
	var sSql = "";
	var fnc = fncKCInsertGestListCallback;

	data = xmlhttp.responseText;
	a = data.split(',');
	if(a[0] == "0"){
		return;
	}
	datasql = "";
	aryRec = data.split(';');
	max = aryRec.length-1;
	for(idx = 0; idx < max; idx++){
		a = aryRec[idx].split(',');
		if(idx == 0){
			datasql = datasql + "(";
		}else{
			datasql = datasql + ",(";
		}
		datasql = datasql + a[1] + "," + a[2] + ",'" + a[3] + "','" + a[4] + "'";
		datasql = datasql + ",'" + a[5] + "','" + a[6] + "','" + a[7] + "','" + a[8] + "')";
	}
	dbnm = m_szHotelDB;
	sSql="INSERT INTO ge"+m_sKonreiNo+" (";
	sSql=sSql+"tno,sno";
	sSql=sSql+",name1,name2,sama";
	sSql=sSql+",kind,skind,yomi)";
	sSql=sSql+" VALUES "+datasql+";";
	data = "dbnm="+dbnm+"&sql="+sSql;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncKCInsertGestListCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();
	var dbnm = "";
	var tble = "";
	var fild = "";
	var fnc = fncKCGetLayoutListCallback;

	data = xmlhttp.responseText;
	ary = data.split(',');
	if(ary[0] == "0"){
		return;
	}

	dbnm = "bridal";
	tble = "tb"+m_sKonreiNo;
	fild = "dx,dy,id";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&termsql=";
	sendRequest("POST","php/getlist.php",data,false,fnc);
}
function fncKCGetLayoutListCallback(xmlhttp)
{
	var data = "";
	var a = new Array();
	var aryRec = new Array();
	var tableposition = "";
	var max = 0;
	var idx = 0;
	var dbnm = "";
	var sSql = "";
	var fnc = fncKCUpdateLayoutCallback;

	data = xmlhttp.responseText;
	a = data.split(',');
	if(a[0] == "0"){
		if(a[1] != "0"){
			fncKCGetString();
		}
		return;
	}
	tableposition = "";
	aryRec = data.split(';');
	max = aryRec.length;
	for(idx = 0; idx < max; idx++){
		a = aryRec[idx].split(',');
		if(a[0] == "0" && a[1] == "0")
		{
			break;
		}
		if(idx == 0){
			tableposition = tableposition + "";
		}else{
			tableposition = tableposition + ":";
		}
		tableposition = tableposition + a[0] + "x" + a[1];
	}
	dbnm = m_szHotelDB;
	sSql = "UPDATE "+m_szKonreiTable+" SET ";
	sSql = sSql+"tableposition='"+tableposition+"'";
	sSql = sSql+" WHERE (id="+m_nKonreiId+");";
	data = "dbnm="+dbnm+"&sql="+sSql;
	sendRequest("POST","php/execsql.php",data,false,fnc);

}
function fncKCUpdateLayoutCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();

	data = xmlhttp.responseText;
	ary = data.split(',');
	if(ary[0] == "0"){
		return;
	}
	fncKCGetString();
}
function fncKCGetString()
{
	var dbnm = "";
	var tble = "";
	var fild = "";
	var data = "";
	var fnc = fncKCGetStringCallback;

	dbnm = "bridal";
	tble = "ss"+m_sKonreiNo;
	fild = "id,leftstr,rightstr,id";
	data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&termsql=";
	sendRequest("POST","php/getlist.php",data,false,fnc);
}

function fncKCGetStringCallback(xmlhttp)
{
	var data = "";
	var a = new Array();
	var dbnm = "";
	var sSql = "";
	var fnc = fncKCUpdateStringCallback;

	data = xmlhttp.responseText;
	a = data.split(',');
	if(a[0] == "0"){
		fnclibAlert("文字列以外の婚礼管理番号"+m_sKonreiNo+"の移行が完了しました");
		fncGetKonreiData();	
		if(m_strUserKind == "1")
		{
			fncInitKonreiListBox();	
		}
		return;
	}

	dbnm = m_szHotelDB;
	sSql = "UPDATE "+m_szKonreiTable+" SET ";
	sSql = sSql+"lefttext='"+a[1]+"',righttext='"+a[2]+"'";
	sSql = sSql+" WHERE (id="+m_nKonreiId+");";
	data = "dbnm="+dbnm+"&sql="+sSql;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncKCUpdateStringCallback(xmlhttp)
{
	var data = "";
	var ary = new Array();

	data = xmlhttp.responseText;
	ary = data.split(',');
	if(ary[0] == "0"){
		return;
	}

	fnclibAlert("婚礼管理番号"+m_sKonreiNo+"の移行が完了しました");
	fncGetKonreiData();	
	if(m_strUserKind == "1")
	{
		fncInitKonreiListBox();	
	}
}
