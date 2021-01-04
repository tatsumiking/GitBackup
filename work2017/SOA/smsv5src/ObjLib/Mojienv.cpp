/*
	Easy Scanner ÉVÉXÉeÉÄã§í É\Å[ÉX	1992.12.01	by stan
base		mojienv.c	ÉXÉNÉäÅ[Éìä÷åWä÷êî
	Copyright äîÅjÉnÉäÉ}
*/

#include	"stdafx.h"

#include	"Mojienv.h"
#include	"Senvlib.h"
#include	"Scrnlib.h"

TCHAR	szinputstring[RECMAX];
TCHAR	szfontpath[FNAMEMAX];
TCHAR	szfontname[64];
int		esseisuno, arceisuno;
int		ftategaki;
DBL		dmmfwidth, dmmfhight;
DBL		dmmmojispace, dmmlinespace;
DBL		dmmstringlen;
DBL		dmmstartx, dmmstarty;
int		nspacekind;
int		nyokomojiflag;
int		nstartkind;
int		nconerkind;
DBL		dmojicount;
int		fontmax;
FILE	*m_fpFList;
DBL		dtrnslimit;

int		ngyakukind;	
TCHAR	szsmpletext[QRECMAX];
int		narngkind;
int		nstrgorder;
DBL		dstrgslop;
DBL		dstrground;
int		fbold;
int		fitric;
int		funderl;
int		fmaxlegal;

DllExport void mojienvinit()
{
	fbold = OFF;
	fitric = OFF;
	funderl = OFF;
	fmaxlegal = 0;
	nspacekind = 1;
	nyokomojiflag = 0;
}

DllExport void mojienvend()
{
}

DllExport void mojienvsetmaxlegal(int flag)
{
	fmaxlegal = flag;
}

DllExport int mojienvcheckmaxlegal()
{
	return(fmaxlegal);
}

DllExport void mojienvsetbold(int flag)
{
	fbold = flag;
}

DllExport int mojienvcheckbold()
{
	return(fbold);
}

DllExport void mojienvsetitric(int flag)
{
	fitric = flag;
}

DllExport int mojienvcheckitric()
{
	return(fitric);
}

DllExport void mojienvsetunderl(int flag)
{
	funderl = flag;
}

DllExport int mojienvcheckunderl()
{
	return(funderl);
}

DllExport void mojienvsettrnslimit(DBL dlimit)
{
	dtrnslimit = dlimit;
}

DllExport void mojienvgettrnslimit(LPDBL dlimit)
{
	*dlimit = dtrnslimit;
}

DllExport void mojienvsetstring(LPTSTR instr)
{
	_tcscpy_s(szinputstring, instr);
}

DllExport void mojienvgetstring(LPTSTR instr)
{
	int len = _tcslen(szinputstring) + 1;
	_tcscpy_s(instr, len, szinputstring);
}

DllExport void mojienvgetfontpath(LPTSTR fontpath)
{
	int len = _tcslen(szfontpath) + 1;
	_tcscpy_s(fontpath, len, szfontpath);
}

DllExport void mojienvsetfontpath(LPTSTR fontpath)
{
	_tcscpy_s(szfontpath, fontpath);
}

DllExport void mojienvgetfontname(LPTSTR fontname)
{
	int len = _tcslen(szfontname) + 1;
	_tcscpy_s(fontname, len, szfontname);
}

DllExport void mojienvsetfontname(LPTSTR fontname)
{
	_tcscpy_s(szfontname, fontname);
}

DllExport void mojienvgetesseisuno(LPINT no)
{
	*no = esseisuno;
}

DllExport void mojienvsetesseisuno(int no)
{
	esseisuno = no;
}

DllExport void mojienvgetarceisuno(LPINT no)
{
	*no = arceisuno;
}

DllExport void mojienvsetarceisuno(int no)
{
	arceisuno = no;
}

DllExport void mojienvgettategakiflag(LPINT tateflg)
{
	*tateflg = ftategaki;
}

DllExport void mojienvsettategakiflag(int tateflg)
{
	ftategaki = tateflg;
}

DllExport void mojienvgetspacekind(LPINT spkflg)
{
	*spkflg = nspacekind;
}

DllExport void mojienvsetspacekind(int spkflg)
{
	nspacekind = spkflg;
}

DllExport void mojienvgetyokomojiflag(LPINT yokomoji)
{
	*yokomoji = nyokomojiflag;
}

DllExport void mojienvsetyokomojiflag(int yokomoji)
{
	nyokomojiflag = yokomoji;
}

DllExport void mojienvgetstart(LPDBL dstartx, LPDBL dstarty)
{
	*dstartx = dmmstartx;
	*dstarty = dmmstarty;
}

DllExport void mojienvsetstart(DBL dstartx, DBL dstarty)
{
	dmmstartx = dstartx;
	dmmstarty = dstarty;
}

DllExport void mojienvgetsize(LPDBL dfwidth, LPDBL dfhight)
{
	*dfwidth = dmmfwidth;
	*dfhight = dmmfhight;
}

DllExport void mojienvsetsize(DBL dfwidth, DBL dfhight)
{
	dmmfwidth = dfwidth;
	dmmfhight = dfhight;
}

DllExport void mojienvgetspace(LPDBL dmojispace, LPDBL dlinespace)
{
	*dmojispace = dmmmojispace;
	*dlinespace = dmmlinespace;
}

DllExport void mojienvsetspace(DBL dmojispace, DBL dlinespace)
{
	dmmmojispace = dmojispace;
	dmmlinespace = dlinespace;
}

DllExport void mojienvgetstrlen(LPDBL dstringlen)
{
	*dstringlen = dmmstringlen;
}

DllExport void mojienvsetstrlen(DBL dstringlen)
{
	dmmstringlen = dstringlen;
}

DllExport void mojienvsetstartkind(int nkind)
{
	nstartkind = nkind;
}

DllExport void mojienvgetstartkind(LPINT nkind)
{
	*nkind = nstartkind;
}

DllExport void mojienvsetconerkind(int nkind)
{
	nconerkind = nkind;
}

DllExport void mojienvgetconerkind(LPINT nkind)
{
	*nkind = nconerkind;
}

DllExport void mojienvsetmojicount(DBL dmjcnt)
{
	dmojicount = dmjcnt;
}

DllExport void mojienvgetmojicount(LPDBL dmjcnt)
{
	*dmjcnt= dmojicount;
}

DllExport void mojienvsetgyakukind(int nkind)
{
	ngyakukind = nkind; 

}

DllExport void mojienvgetgyakukind(LPINT nkind)
{
	*nkind = ngyakukind; 
}

DllExport void mojienvsetsmpltext(LPTSTR smpl)
{
	_tcscpy_s(szsmpletext, smpl); 

}

DllExport void mojienvgetsmpltext(LPTSTR smpl)
{
	int len = _tcslen(szsmpletext) + 1;
	_tcscpy_s(smpl, len, szsmpletext); 
}

DllExport void mojienvsetarngkind(int nkind)
{
	narngkind = nkind; 
}

DllExport void mojienvgetarngkind(LPINT nkind)
{
	*nkind = narngkind; 
}

DllExport void mojienvsetorder(int nkind)
{
	nstrgorder = nkind; 
}

DllExport void mojienvgetorder(LPINT nkind)
{
	*nkind = nstrgorder; 
}

DllExport void mojienvsetstrgslop(DBL dslop)
{
	dstrgslop = dslop; 

}

DllExport void mojienvgetstrgslop(LPDBL dslop)
{
	*dslop = dstrgslop; 
}

DllExport void mojienvsetmojiround(DBL dround)
{
	dstrground = dround; 
}

DllExport void mojienvgetmojiround(LPDBL dround)
{
	*dround = dstrground; 
}

DllExport void mojienvflistinit()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	flstfname[FNAMEMAX];
	FILE	*fp;

	fontmax = 0;
	senvgetexepath(exepath);
	_stprintf_s(flstfname, _T("%s\\fontlist.lst"), exepath);
	_tfopen_s(&fp, flstfname, _T("wb"));
	if(fp != NULL){
		fclose(fp);
	}
	m_fpFList = NULL;
}

DllExport void mojienvflistopen(LPTSTR lpMode)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	flstfname[FNAMEMAX];

	senvgetexepath(exepath);
	_stprintf_s(flstfname, _T("%s\\fontlist.lst"), exepath);
	_tfopen_s(&m_fpFList, flstfname, lpMode);
}

DllExport void mojienvflistclose()
{
	fclose(m_fpFList);
}

DllExport int mojienvgetflistmax()
{
	return(fontmax);
}

DllExport void mojienvsetflist(int type, LPTSTR tSFName, LPTSTR tFFName)
{
	TCHAR	buf[HRECMAX+16];
	int		len;
	int		len1, len2;
	TCHAR	tFName[QRECMAX+16];

	if(m_fpFList == NULL){
		return;
	}
	while (1) {
		len = fread(buf, sizeof(TCHAR), HRECMAX, m_fpFList);
		if (len != HRECMAX) {
			break;
		}
		len1 = _tcslen(&(buf[3])) + 1;
		_tcscpy_s(tFName, len1, &(buf[3]));
		if (_tcscmp(tFName, tSFName) == 0) {
			return;
		}
	}
	fseek(m_fpFList, 0L, 2);
	fontmax++;
	memset(buf, 0, HRECMAX*sizeof(TCHAR));
	_stprintf_s(buf, _T("%02d"), type);
	len1 = _tcslen(tSFName)+1;
	len2 = _tcslen(tFFName)+1;
	_tcscpy_s(&(buf[3]), len1, tSFName);
	_tcscpy_s(&(buf[64]), len2, tFFName);
	buf[HRECMAX-2] = 0x0d;
	buf[HRECMAX-1] = 0x0a;
	fwrite(buf, sizeof(TCHAR), HRECMAX, m_fpFList);
	return;
}

DllExport void mojienvgetflist(int idx, int *type, LPTSTR tSFName, LPTSTR tFFName)
{
	TCHAR	buf[HRECMAX];
	long	seekp;
	int		len, len1, len2;

	if(m_fpFList == NULL){
		return;
	}
	tSFName[0] = _T('\0');
	tFFName[0] = _T('\0');
	if(idx >= fontmax){
		return;
	}
	seekp = idx*HRECMAX*sizeof(TCHAR);
	fseek(m_fpFList, seekp, 0);
	len = fread(buf, sizeof(TCHAR), HRECMAX, m_fpFList);
	if(len != HRECMAX){
		return;
	}
	buf[HRECMAX - 2] = 0;
	buf[HRECMAX - 1] = 0;
	_stscanf_s(buf, _T("%d"), type);
	len1 = _tcslen(&(buf[3])) + 1;
	len2 = _tcslen(&(buf[64])) + 1;
	_tcscpy_s(tSFName, len1, &(buf[3]));
	_tcscpy_s(tFFName, len2, &(buf[64]));
	return;
}

DllExport void mojienvgetfonttypename(LPTSTR lpSFName, LPINT retftype, LPTSTR lpFFName)
{
	TCHAR	buf[HRECMAX+16];
	int		len;
	int		num;
	int		type;
	TCHAR	tSFName[QRECMAX];
	TCHAR	tFFName[QRECMAX];

	len = _tcslen(_T("ÇdÇrâpêî"));
	if(_tcsnccmp(lpSFName, _T("ÇdÇrâpêî"), len) == 0){
		num = _ttoi(&(lpSFName[len]));
		*retftype = 10;
		len = _tcslen(_T("Çdâpêî00"))+1;
		_stprintf_s(lpFFName, len, _T("Çdâpêî%02d"), num);
		return;
	}
	len = _tcslen(_T("Ç`âpêî"));
	if(_tcsnccmp(lpSFName, _T("Ç`âpêî"), len) == 0){
		num = _ttoi(&(lpSFName[len]));
		*retftype = 10;
		len = _tcslen(_T("Ç`âp0000"))+1;
		_stprintf_s(lpFFName, len, _T("Ç`âp%04d"), num);
		return;
	}
	len = _tcslen(_T("Ç`Çqâp"));
	if(_tcsnccmp(lpSFName, _T("Ç`Çqâp"), len) == 0){
		num = _ttoi(&(lpSFName[len]));
		*retftype = 32;
		len = _tcslen(_T("âpêî000"))+1;
		_stprintf_s(lpFFName, len, _T("âpêî%03d"), num);
		return;
	}
	len = _tcslen(_T("ÉXÉ|Å[Éc"));
	if(_tcsnccmp(lpSFName, _T("ÉXÉ|Å[Éc"), len) == 0){
		num = _ttoi(&(lpSFName[len]));
		*retftype = 32;
		len = _tcslen(_T("SPOT000"))+1;
		_stprintf_s(lpFFName, len, _T("SPOT%03d"), num);
		return;
	}
	
	if(m_fpFList == NULL){
		*retftype = 21;
		len = _tcslen(_T("ÇlÇr ñæí©"))+1;
		_tcscpy_s(lpFFName, len, _T("ÇlÇr ñæí©"));
		return;
	}
	fseek(m_fpFList, 0L, 0);
	while(1){
		len = fread(buf, sizeof(TCHAR), HRECMAX, m_fpFList);
		if(len != HRECMAX){
			lpFFName[0] = _T('\0');
			*retftype = 0;
			break;
		}
		buf[HRECMAX - 2] = 0;
		buf[HRECMAX - 1] = 0;
		_stscanf_s(buf, _T("%d"), &type);
		_tcscpy_s(tSFName, &(buf[3]));
		_tcscpy_s(tFFName, &(buf[64]));
		if(_tcscmp(lpSFName, tSFName) == 0){
			*retftype = type;
			len = _tcslen(tFFName)+1;
			_tcscpy_s(lpFFName, len, tFFName);
			break;
		}
	}
}

DllExport int mojienvgetfontno(LPTSTR lpSFName)
{
	TCHAR	buf[HRECMAX+16];
	int		len;
	int		type;
	TCHAR	tSFName[QRECMAX];
	TCHAR	tFFName[QRECMAX];
	int		i;

	if(m_fpFList == NULL){
		return(0);
	}
	fseek(m_fpFList, 0L, 0);
	i = 0;
	while(1){
		len = fread(buf, sizeof(TCHAR), HRECMAX, m_fpFList);
		if(len != HRECMAX){
			break;
		}
		buf[HRECMAX - 2] = 0;
		buf[HRECMAX - 1] = 0;
		_stscanf_s(buf, _T("%d"), &type);
		_tcscpy_s(tSFName, &(buf[3]));
		_tcscpy_s(tFFName, &(buf[64]));
		if(_tcscmp(lpSFName, tSFName) == 0){
			break;
		}
		i++;
	}
	return(i);
}


DllExport int mojienvgetfonttype(LPTSTR lpFFName)
{
	TCHAR	buf[HRECMAX+16];
	int		len;
	int		type;
	TCHAR	tSFName[QRECMAX];
	TCHAR	tFFName[QRECMAX];

	if(m_fpFList == NULL){
		return(0);
	}
	fseek(m_fpFList, 0L, 0);
	while(1){
		len = fread(buf, sizeof(TCHAR), HRECMAX, m_fpFList);
		if(len != HRECMAX){
			break;
		}
		buf[HRECMAX - 2] = 0;
		buf[HRECMAX - 1] = 0;
		_stscanf_s(buf, _T("%d"), &type);
		_tcscpy_s(tSFName, &(buf[3]));
		_tcscpy_s(tFFName, &(buf[64]));
		if(_tcscmp(lpFFName, tSFName) == 0){
			return(type);
		}
		if(_tcscmp(lpFFName, tFFName) == 0){
			return(type);
		}
	}
	return(TT2FONT);
}

DllExport void mojienvgetfilename(LPTSTR lpSFName, LPTSTR lpFFName)
{
	TCHAR	buf[HRECMAX+16];
	int		len;
	int		type;
	TCHAR	tSFName[QRECMAX];
	TCHAR	tFFName[QRECMAX];

	lpFFName[0] = _T('\0');
	if(m_fpFList == NULL){
		return;
	}
	fseek(m_fpFList, 0L, 0);
	while(1){
		len = fread(buf, sizeof(TCHAR), HRECMAX, m_fpFList);
		if(len != HRECMAX){
			break;
		}
		buf[HRECMAX - 2] = 0;
		buf[HRECMAX - 1] = 0;
		_stscanf_s(buf, _T("%d"), &type);
		_tcscpy_s(tSFName, &(buf[3]));
		_tcscpy_s(tFFName, &(buf[64]));
		if(_tcscmp(lpSFName, tSFName) == 0){
			len = _tcslen(tFFName) + 1;
			_tcscpy_s(lpFFName, len, tFFName);
			break;
		}
	}
}

DllExport void mojienvgetsfilename(LPTSTR lpFFName, LPTSTR lpSFName)
{
	TCHAR	buf[HRECMAX+16];
	int		len;
	int		type;
	TCHAR	tSFName[QRECMAX];
	TCHAR	tFFName[QRECMAX];

	lpFFName[0] = _T('\0');
	if(m_fpFList == NULL){
		return;
	}
	fseek(m_fpFList, 0L, 0);
	while(1){
		len = fread(buf, sizeof(TCHAR), HRECMAX, m_fpFList);
		if(len != HRECMAX){
			break;
		}
		buf[HRECMAX - 2] = 0;
		buf[HRECMAX - 1] = 0;
		_stscanf_s(buf, _T("%d"), &type);
		_tcscpy_s(tSFName, &(buf[3]));
		_tcscpy_s(tFFName, &(buf[64]));
		if(_tcscmp(lpFFName, tFFName) == 0){
			len = _tcslen(tSFName) + 1;
			_tcscpy_s(lpSFName, len, tSFName);
			break;
		}
	}
}

