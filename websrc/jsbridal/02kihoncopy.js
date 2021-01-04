function fncKonreiCopy()
{
	var dbnm = "bridal";
	var tble = "bridaluser";
	fild = "tablelayout";
	fild = fild + ",kyosiki,hirouen,kaijyou,mukotori";
	fild = fild + ",sinrozoku,sinroname1,sinroname2";
	fild = fild + ",sinpuzoku,sinpuname1,sinpuname2";
	fild = fild + ",sinrodish,sinpudish,sinrosub,sinpusub";
	fild = fild + ",paperlocate,papersize,ryoukekind,tablekind";
	fild = fild + ",textsize,takasagokind,nametype,flag1";

	var where = "WHERE (username="+m_sKonreiNo+") LIMIT 1";
	var data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&where="+where;
	var fnc = fncKCGetKonreiDataCallback;
	sendRequest("POST","php/getdata.php",data,false,fnc);
}
function fncKCGetKonreiDataCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var a = data.split(',');
	if(a[0] == "0"){
		return;
	}
	var dbnm = m_szHotelDB;
	var sSql = "UPDATE "+m_szKonreiTable+" SET ";
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
	var data = "dbnm="+dbnm+"&sql="+sSql;
	var fnc = fncKCUpdateKonreiCallback;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncKCUpdateKonreiCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(',');
	if(ary[0] == "0"){
		return;
	}
	var dbnm = "bridal";
	var tble = "ge"+m_sKonreiNo;
	var fild = "id,tno,sno,name1,name2,sama,kind,skind,yomi,id";
	var data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&termsql=";
	var fnc = fncKCGetGestListCallback;
	sendRequest("POST","php/getlist.php",data,false,fnc);

}
function fncKCGetGestListCallback(xmlhttp)
{
	var idx;

	var data = xmlhttp.responseText;
	var a = data.split(',');
	if(a[0] == "0"){
		return;
	}
	datasql = "";
	var aryRec = data.split(';');
	var max = aryRec.length-1;
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
	var sSql="INSERT INTO ge"+m_sKonreiNo+" (";
	sSql=sSql+"tno,sno";
	sSql=sSql+",name1,name2,sama";
	sSql=sSql+",kind,skind,yomi)";
	sSql=sSql+" VALUES "+datasql+";";
	var data = "dbnm="+m_szHotelDB+"&sql="+sSql;
	var fnc = fncKCInsertGestListCallback;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncKCInsertGestListCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(',');
	if(ary[0] == "0"){
		return;
	}

	var dbnm = "bridal";
	var tble = "tb"+m_sKonreiNo;
	var fild = "dx,dy,id";
	var data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&termsql=";
	var fnc = fncKCGetLayoutListCallback;
	sendRequest("POST","php/getlist.php",data,false,fnc);
}
function fncKCGetLayoutListCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var a = data.split(',');
	if(a[0] == "0"){
		if(a[1] != "0"){
			fncKCGetString();
		}
		return;
	}
	tableposition = "";
	var aryRec = data.split(';');
	var max = aryRec.length;
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
	var dbnm = m_szHotelDB;
	var sSql = "UPDATE "+m_szKonreiTable+" SET ";
	sSql = sSql+"tableposition='"+tableposition+"'";
	sSql = sSql+" WHERE (id="+m_nKonreiId+");";
	var data = "dbnm="+dbnm+"&sql="+sSql;
	var fnc = fncKCUpdateLayoutCallback;
	sendRequest("POST","php/execsql.php",data,false,fnc);

}
function fncKCUpdateLayoutCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(',');
	if(ary[0] == "0"){
		return;
	}
	fncKCGetString();
}
function fncKCGetString()
{
	var dbnm = "bridal";
	var tble = "ss"+m_sKonreiNo;
	var fild = "id,leftstr,rightstr,id";
	var data = "dbnm="+dbnm+"&tble="+tble+"&fild="+fild+"&termsql=";
	var fnc = fncKCGetStringCallback;
	sendRequest("POST","php/getlist.php",data,false,fnc);
}

function fncKCGetStringCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var a = data.split(',');
	if(a[0] == "0"){
		fnclibAlert("文字列以外の婚礼管理番号"+m_sKonreiNo+"の移行が完了しました");
		fncGetKonreiData();	
		if(m_strUserKind == "1")
		{
			fncInitKonreiListBox();	
		}
		return;
	}

	var dbnm = m_szHotelDB;
	var sSql = "UPDATE "+m_szKonreiTable+" SET ";
	sSql = sSql+"lefttext='"+a[1]+"',righttext='"+a[2]+"'";
	sSql = sSql+" WHERE (id="+m_nKonreiId+");";
	var data = "dbnm="+dbnm+"&sql="+sSql;
	var fnc = fncKCUpdateStringCallback;
	sendRequest("POST","php/execsql.php",data,false,fnc);
}
function fncKCGetStringCallback(xmlhttp)
{
	var data = xmlhttp.responseText;
	var ary = data.split(',');
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
