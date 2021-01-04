
/////////////////////////////////////////////////////////////////////////////
// CFileVect

#include "stdafx.h"
#include "FileVect.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Diblib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Mojienv.h"
//}

#define	TATEGAKI	1

static TCHAR	strgheadold[] = _T("string-start");	//ds 文字列データ開始識別文字列
static TCHAR	strgendold[] = _T("string-end");	//ds 文字列データ終了識別文字列
static TCHAR	mojiheadold[] = _T("moji-start");	//ds 文字データ開始識別文字列
static TCHAR	mojiendold[] = _T("moji-end");		//ds 文字データ終了識別文字列
static TCHAR	vectheadold[] = _T("vect-start");		//ds ベクトルデータ開始識別文字列
static TCHAR	vectendold[] = _T("vect-end");			//ds ベクトルデータ終了識別文字列
static TCHAR	logomarkold[] = _T("logo-data");		//ds ロゴデータ開始識別文字列
static TCHAR	strghead[] = _T("string2start");	//ds 文字列データ開始識別文字列
static TCHAR	strgend[] = _T("string2end");		//ds 文字列データ終了識別文字列
static TCHAR	mojihead[] = _T("moji2start");		//ds 文字データ開始識別文字列
static TCHAR	mojiend[] = _T("moji2end");			//ds 文字データ終了識別文字列
static TCHAR	mmvcthead[] = _T("mmvct_start");	//ds ベクトルデータ開始識別文字列
static TCHAR	mmvctend[] = _T("mmvct_end");		//ds ベクトルデータ終了識別文字列
static TCHAR	logomark[] = _T("logo-data");		//ds ロゴデータ開始識別文字列
static TCHAR	mmlgmark[] = _T("mmlg-data");		//ds ロゴデータ開始識別文字列
static TCHAR	mmwakuhead[] = _T("mmwk_start");	//ds 枠データ開始識別文字列
static TCHAR	mmwakuend[] = _T("mmwk_end");		//ds 枠データ終了識別文字列
static TCHAR	mmrulehead[] = _T("mmrl_start");	//ds 罫線データ開始識別文字列
static TCHAR	mmruleend[] = _T("mmrl_end");		//ds 罫線データ終了識別文字列

CFileVect::CFileVect(CScrollView* pcview)
{
	m_pcView = (CTonView*)pcview;
	m_pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_nFirst = ON;
	m_pcESF0Font = m_pcDoc->m_pcESF0Font;
	m_pcESF1Font = m_pcDoc->m_pcESF1Font;
	m_pcFSF1Font = m_pcDoc->m_pcFSF1Font;
}

void CFileVect::LoadVectFile(LPTSTR filename)
{
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	basename[FNAMEMAX];
	int		len;
	int		len1, len2, len3, len4, len5;
	int		len6, len7, len8, len9, lenA, lenB, lenC;

	CDataGrup*	pcDataGrup;
	CDataList*	pcDataList;
	CStrgVect*	pcStrgVect;
	int			strgidx;
	TCHAR		strg[RECMAX];
	CData*		pcData;

	pcDataGrup = (CDataGrup*)(m_pcDoc->NewData(DATAGRUPID));
	pcDataList = (CDataList*)(m_pcDoc->NewData(DATALISTID));
	pcStrgVect = (CStrgVect*)(m_pcDoc->NewData(STRGVECTID));
	pcStrgVect->SetTateYoko(YOKOGAKI);
	strgidx = 0;
	strg[0] = _T('\0');

	pcDataGrup->SetData(pcDataList);

	len = _tcslen(filename);
	_tcscpy_s(basename, FNAMEMAX, filename);
	basename[len-4] = _T('\0');

	len1 = _tcslen(logomark);
	len2 = _tcslen(mmlgmark);
	len3 = _tcslen(mmwakuhead);
	len4 = _tcslen(strghead);
	len5 = _tcslen(mojihead);
	len6 = _tcslen(mmrulehead);
	len7 = _tcslen(mmvcthead);
	len8 = _tcslen(strgheadold);
	len9 = _tcslen(strgendold);
	lenA = _tcslen(mojiheadold);
	lenB = _tcslen(mojiendold);
	lenC = _tcslen(vectheadold);
	FontFileOpen(basename);
	_tfopen_s(&fp, filename, _T("r"));
	if(fp == NULL){
		return;
	}
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(_tcsnccmp(logomark, buf, len1) == 0){
			if((pcData = LoadLogoData(fp, buf)) == NULL){
				break;
			}
			pcDataGrup->SetData(pcData);
		}
		else if(_tcsnccmp(mmlgmark, buf, len2) == 0){
			if((pcData = LoadLogoData(fp, buf)) == NULL){
				break;
			}
			pcDataGrup->SetData(pcData);
		}
		else if(_tcsnccmp(mmwakuhead, buf, len3) == 0){
			if((pcData = LoadWakuData(fp, buf)) == NULL){
				break;
			}
			pcDataGrup->SetData(pcData);
		}
		else if(_tcsnccmp(strghead, buf, len4) == 0){
			if((pcData = LoadStrgData(fp, buf)) == NULL){
				break;
			}
			pcDataGrup->SetData(pcData);
		}
		else if(_tcsnccmp(mojihead, buf, len5) == 0){
			if((pcData = LoadMojiData(fp, buf)) == NULL){
				break;
			}
			if(strgidx < RECMAX){
				strg[strgidx++] = ((((CMojiVect*)pcData)->m_nCode) >> 8) & 0x00ff;
				strg[strgidx++] = ((((CMojiVect*)pcData)->m_nCode)) & 0x00ff;
				strg[strgidx] = _T('\0');
			}
			pcStrgVect->SetMojiLink((CMojiVect*)pcData);
		}
		else if(_tcsnccmp(mmrulehead, buf, len6) == 0){
			// 
		}
		else if(_tcsnccmp(mmvcthead, buf, len7) == 0){
			if((pcData = LoadVectData(fp, buf)) == NULL){
				break;
			}
			pcDataList->SetData(pcData);
		}
		else if(_tcsnccmp(strgheadold, buf, len8) == 0){
			continue;
		}
		else if(_tcsnccmp(strgendold, buf, len9) == 0){
			continue;
		}
		else if(_tcsnccmp(mojiheadold, buf, lenA) == 0){
			continue;
		}
		else if(_tcsnccmp(mojiendold, buf, lenB) == 0){
			continue;
		}
		else if(_tcsnccmp(vectheadold, buf, lenC) == 0){
			//fvctoldvectdataload
			if((pcData = LoadOldVectData(fp, buf)) == NULL){
				break;
			}
			pcDataList->SetData(pcData);
		}
	}
	fclose(fp);
	FontFileClose();

	pcStrgVect->SetStringUnLoad(strg);
	if(pcStrgVect->m_pcMojiTop == NULL){
		delete(pcStrgVect);
	}
	else{
		pcStrgVect->RenewMiniMax();
		pcDataGrup->SetData(pcStrgVect);
	}
	m_pcDoc->DataAddTail((CData*)pcDataGrup);

}

void CFileVect::LoadLogoFile(LPTSTR filename)
{
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	basename[FNAMEMAX];
	TCHAR	dotfname[FNAMEMAX];
	TCHAR	bmpfname[FNAMEMAX];
	TCHAR	tmp[QRECMAX];
	CDataLogo*	pcDataLogo;
	CDibCData*	pcDibCData;
	CData*		pcData;
	int			len, len1;
	DBL			sx, sy;
	DBL			ex, ey;
	DBL			xsize, ysize;
	

	len = _tcslen(filename);
	len1 = _tcslen(_T(".vct"));
	_tcscpy_s(basename, len-len1, filename);
	basename[len-len1] = _T('\0');
	_stprintf_s(dotfname, _T("%s.dot"), basename);

	_tfopen_s(&fp, filename, _T("r"));
	if(fp == NULL){
		return;
	}

	FileFgets(buf, RECMAX, fp);
	_stscanf_s(buf, _T("%s %lf %lf %lf %lf\n"), tmp, QRECMAX, &sx, &ey, &ex, &sy);
	pcDataLogo = (CDataLogo*)(m_pcDoc->NewData(DATALOGOID));
	bmpfname[0] = _T('\0');
	diblibcreateimgfilename(bmpfname);
	DibTrnsLogoFile(dotfname, bmpfname);

	pcDibCData = (CDibCData*)(m_pcDoc->NewData(DIBCDATAID));
	pcDibCData->SetFileName(bmpfname);
	pcDibCData->InitElement();
	pcDataLogo->SetDibCData(pcDibCData);
	xsize = ex - sx;
	ysize = ey - sy;
	pcDibCData->SetDstElement(sx, sy, xsize, ysize);


	len = _tcslen(mmvcthead);
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		else if(_tcsnccmp(mmvcthead, buf, len) == 0){
			if((pcData = LoadVectData(fp, buf)) == NULL){
				break;
			}
			pcDataLogo->SetData(pcData);
		}
	}
	m_pcDoc->DataAddTail((CData*)pcDataLogo);

	fclose(fp);
}

CData* CFileVect::LoadLogoData(FILE *fp, TCHAR buf[])
{
	TCHAR	tmp[QRECMAX];
	TCHAR	dotfname[FNAMEMAX];
	TCHAR	bmpfname[FNAMEMAX];
	int		len;
	CDataLogo*	pcDataLogo;
	CDibCData*	pcDibCData;
	CData*		pcData;
	CVect*		pcVect;
	DBL			sx, sy;
	DBL			ex, ey;
	DBL			xsize, ysize;

	_stscanf_s(buf, _T("%s %s\n"), tmp, QRECMAX, dotfname, FNAMEMAX);
	pcDataLogo = (CDataLogo*)(m_pcDoc->NewData(DATALOGOID));

	bmpfname[0] = _T('\0');
	diblibcreateimgfilename(bmpfname);
	DibTrnsLogoFile(dotfname, bmpfname);

	pcDibCData = (CDibCData*)(m_pcDoc->NewData(DIBCDATAID));
	pcDibCData->SetFileName(bmpfname);
	pcDibCData->InitElement();
	pcDataLogo->SetDibCData(pcDibCData);

	if(FileFgets(buf, RECMAX, fp) == NULL){
		delete(pcDataLogo);
		return(NULL);
	}
	len = _tcslen(mmvcthead);
	if(_tcsnccmp(mmvcthead, buf, len) == 0){
		if((pcData = LoadVectData(fp, buf)) == NULL){
			delete(pcDataLogo);
			return(NULL);
		}
		pcVect = (CVect*)pcData;
		pcVect->GetDXY(0, &sx, &sy);
		pcVect->GetDXY(2, &ex, &ey);
		xsize = ex - sx;
		ysize = ey - sy;
	}
	pcDibCData->SetDstElement(sx, sy, xsize, ysize);

	return((CData*)pcDataLogo);
}

CData* CFileVect::LoadStrgData(FILE *fp, TCHAR buf[])
{
	TCHAR	tmp[QRECMAX];
	TCHAR	strg[RECMAX];
	int		len1, len2;
	int		strgidx;
	UINT	sjiscode, hc, lc;
	int		flag;
	CStrgVect*	pcStrgVect;
	CData*	pcData;

	_stscanf_s(buf, _T("%s %x\n"), tmp, QRECMAX, &flag);
	pcStrgVect = (CStrgVect*)(m_pcDoc->NewData(STRGVECTID));
	if(flag == TATEGAKI){
		pcStrgVect->SetTateYoko(TATEGAKI);
	}
	else{
		pcStrgVect->SetTateYoko(YOKOGAKI);
	}
	strgidx = 0;
	flag = OFF;
	len1 = _tcslen(strgend);
	len2 = _tcslen(mojihead);
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(_tcsnccmp(strgend, buf, len1) == 0){
			break;
		}
		if(_tcsnccmp(mojihead, buf, len2) == 0){
			if((pcData = LoadMojiData(fp, buf)) == NULL){
				delete(pcStrgVect);
				return(NULL);
			}
			pcStrgVect->SetMojiLink((CMojiVect*)pcData);
			flag = ON;
			sjiscode = ((CMojiVect*)pcData)->m_nCode;
			hc = (sjiscode >> 8) & 0x00ff;	
			lc = (sjiscode) & 0x00ff;	
			strg[strgidx++] = hc;
			strg[strgidx++] = lc;
			strg[strgidx] = _T('\0');
		}
	}
	if(flag == OFF){
		delete(pcStrgVect);
		return(NULL);
	}
	pcStrgVect->SetStringUnLoad(strg);
	pcStrgVect->RenewMiniMax();
	/* // 文字列のサイズ 文字間隔 フォント名のセット
	pcMojiVect = pcStrgVect->m_pcMojiTop;
	pcMojiVect->GetFontSize(&xsize, &ysize);
	*/
	return((CData*)pcStrgVect);
}

CData* CFileVect::LoadMojiData(FILE *fp, TCHAR buf[])
{
	TCHAR	tmp[QRECMAX];
	int		len1, len2;
	UINT	code;
	int		chngflag, fontno, datano;
	int		hFno;
	CMojiVect* pcMojiVect;
	CData*	pcData;
	CVect*	pcVect;
	CDataList* pcDataList;
	DBL		sx, sy, ex, ey;
	DBL		xsize, ysize;

	fontno = 0;
	_stscanf_s(buf, _T("%s %x %x %d %d\n"), tmp, QRECMAX, &chngflag, &code, &fontno, &datano);
	m_nFontNo = fontno;
	pcMojiVect = (CMojiVect*)(m_pcDoc->NewData(MOJIVECTID));
	pcMojiVect->m_nCode = code;
	pcMojiVect->m_nBaseCode = code;

	if(FileFgets(buf, RECMAX, fp) == NULL){
		delete(pcMojiVect);
		return(NULL);
	}
	len1 = _tcslen(mojiend);
	len2 = _tcslen(mmvcthead);
	if(_tcsnccmp(mojiend, buf, len1) == 0){
		delete(pcMojiVect);
		return(NULL);
	}
	if(_tcsnccmp(mmvcthead, buf, len2) == 0){
		if((pcData = LoadVectData(fp, buf)) == NULL){
			delete(pcMojiVect);
			return(NULL);
		}
	}
	else{
		delete(pcMojiVect);
		return(NULL);
	}
	pcVect = (CVect*)pcData;
	pcVect->GetDXY(0, &sx, &sy);
	pcVect->GetDXY(2, &ex, &ey);
	xsize = ex - sx;
	ysize = sy - ey;
	pcMojiVect->SetStartXY(sx, ey);
	pcMojiVect->SetFontSize(xsize, ysize);
	delete(pcVect);

	if((chngflag & 0x09) == 0){
		hFno = LoadFontData(datano);
		FileFgets(buf, RECMAX, fp);
		if(_tcsnccmp(mojiend, buf, len1) != 0){
			delete(pcMojiVect);
			return(NULL);
		}
		pcMojiVect->m_hFno = hFno;
		return(pcMojiVect);
	}
	//
	pcDataList = (CDataList*)(m_pcDoc->NewData(DATALISTID));
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(_tcsnccmp(mojiend, buf, len1) == 0){
			break;
		}
		if(_tcsnccmp(mmvcthead, buf, len2) == 0){
			if((pcData = LoadVectData(fp, buf)) == NULL){
				delete(pcDataList);
				delete(pcMojiVect);
				return(NULL);
			}
			pcDataList->SetData(pcData);
		}
	}
	if(pcDataList != NULL){
		pcMojiVect->m_pcDataList = pcDataList;
	}
	return((CData*)pcMojiVect);
}

CData* CFileVect::LoadVectData(FILE *fp, TCHAR buf[])
{
	TCHAR	tmp[QRECMAX];
	int		len;
	CVect*	pcVect;
	int		flag, pp;
	int		atr;
	DBL		x, y;

	_stscanf_s(buf, _T("%s %d\n"), tmp, QRECMAX, &flag);
	pcVect =(CVect*)(m_pcDoc->NewData(BASEVECTID));
	pp = 0;
	len = _tcslen(mmvctend);
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(_tcsnccmp(mmvctend, buf, len) == 0){
			break;
		}
		_stscanf_s(buf, _T("%x %lf %lf\n"), &atr, &x, &y);
		pcVect->SetAtrDXY(pp, atr, x, y);
		pp++;
	}
	if(flag & REJIONFLAG){
		pcVect->OnFlag(REJIONFLAG);	
	}
	else{
		pcVect->OffFlag(REJIONFLAG);	
	}
	pcVect->SetPointCount(pp);
	pcVect->RenewMiniMax();
	return((CData*)pcVect);
}

CData* CFileVect::LoadOldVectData(FILE *fp, TCHAR buf[])
{
	TCHAR	tmp[QRECMAX];
	int		len;
	CVect*	pcVect;
	int		flag, pp;
	int		atr, nx, ny, xybit;
	double	dx, dy, time;

	_stscanf_s(buf, _T("%s %d\n"), tmp, QRECMAX, &flag);
	pp = 0;
	pcVect =(CVect*)(m_pcDoc->NewData(BASEVECTID));
	time = 0.125;
	len = _tcslen(vectendold);
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(_tcsnccmp(vectendold, buf, 8) == 0){
			break;
		}
		_stscanf_s(buf, _T("%x %d %d %x\n"), &atr, &nx, &ny, &xybit);
		Trnsnxybittodxy(nx, ny, xybit, &dx, &dy);
		dx *= time;
		dy *= time;
		pcVect->SetAtrDXY(pp, atr, dx, dy);
		pp++;
	}
	pcVect->SetPointCount(pp);
	pcVect->RenewMiniMax();
	return((CData*)pcVect);
}

CData* CFileVect::LoadWakuData(FILE *fp, TCHAR buf[])
{
	TCHAR	tmp[QRECMAX];
	TCHAR	strg[RECMAX];
	int		len, len1, len2, len3, len4;
	int		idx;
	int		fonttype;
	TCHAR	fontname[QRECMAX];
	TCHAR	fontfullname[RECMAX];
	int		flag, space;
	CSWakVect*	pcSWakVect;
	CData*	pcData;
	CVect*	pcVect;
	DBL		sx, sy, ex, ey;
	DBL		xsize, ysize;

	_stscanf_s(buf, _T("%s %x %d\n"), tmp, QRECMAX, &flag, &space);

	if(FileFgets(buf, RECMAX, fp) == NULL){
		return(NULL);
	}
	len = _tcslen(mmvcthead);
	if(_tcsnccmp(mmvcthead, buf, len) != 0){
		return(NULL);
	}
	pcData = LoadVectData(fp, buf);
	pcVect = (CVect*)pcData;
	pcVect->GetDXY(0, &sx, &sy);
	pcVect->GetDXY(2, &ex, &ey);
	xsize = ex - sx;
	ysize = sy - ey;
	delete(pcData);

	_stscanf_s(buf, _T("%s %x\n"), tmp, QRECMAX, &flag);
	pcSWakVect = (CSWakVect*)(m_pcDoc->NewData(SWAKVECTID));
	if(flag & TATEGAKI){
		pcSWakVect->SetFontSize(xsize, xsize);
		pcSWakVect->SetStrLen(ysize);
		pcSWakVect->SetTateYoko(TATEGAKI);
	}
	else{
		pcSWakVect->SetFontSize(ysize, ysize);
		pcSWakVect->SetStrLen(xsize);
		pcSWakVect->SetTateYoko(YOKOGAKI);
	}
	pcSWakVect->SetStrStartXY(sx, sy);
	pcSWakVect->SetSpaceEnv((DBL)space, (DBL)space);
	idx = 0;
	len1 = _tcslen(strghead);
	len2 = _tcslen(strgend);
	len3 = _tcslen(mmwakuend);
	len4 = _tcslen(mojihead);
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(_tcsnccmp(strghead, buf, len1) == 0){
			continue;
		}
		else if(_tcsnccmp(strgend, buf, len2) == 0){
			continue;
		}
		else if(_tcsnccmp(mmwakuend, buf, len3) == 0){
			break;
		}
		else if(_tcsnccmp(mojihead, buf, len4) == 0){
			if((pcData = LoadMojiData(fp, buf)) == NULL){
				delete(pcSWakVect);
				return(NULL);
			}
			pcSWakVect->SetMojiLink((CMojiVect*)pcData);
			strg[idx++] = ((((CMojiVect*)pcData)->m_nCode) >> 8) & 0x00ff;
			strg[idx++] = ((((CMojiVect*)pcData)->m_nCode)) & 0x00ff;
		}
	}
	strg[idx] = _T('\0');
	mojienvflistopen(_T("rb"));
	mojienvgetflist(m_nFontNo, &fonttype, fontname, fontfullname);
	mojienvflistclose();
	pcSWakVect->SetStringUnLoad(strg);
	pcSWakVect->SetFontName(fontname);
	return((CData*)pcSWakVect);
}

void CFileVect::FontFileOpen(TCHAR basename[])
{
	if(m_pcESF1Font->DFileReadOpen(basename) == FAIL){
		if(m_pcESF0Font->DFileReadOpen(basename) == FAIL){
			m_nFontType = 0;
			return;
		}
		m_nFontType = ES0FONT;
		return;
	}
	m_nFontType = ES1FONT;
}

void CFileVect::FontFileClose()
{
	if(m_nFontType == ES1FONT){
		m_pcESF1Font->FontClose();
	}
	else{
		m_pcESF0Font->FontClose();
	}
}

int CFileVect::LoadFontData(int datano)
{
	int		hFno;

	hFno = m_pcFSF1Font->Create();
	switch(m_nFontType){
	case ES0FONT:
		m_pcESF0Font->DFileLoadSF1Trns(datano);
		break;
	case ES1FONT:
		m_pcESF1Font->DFileLoadSF1Trns(datano);
		break;
	default:
		m_pcFSF1Font->m_nDataLen = 0;
	}
	m_pcFSF1Font->PointMiniMaxGet();
	m_pcFSF1Font->DataSave(hFno);
	return(hFno);
}

LPTSTR CFileVect::FileFgets(TCHAR *buf, int bufmax, FILE *fp)
{
	int len;

	len = _tcslen(_T("REM "));

	while(1){
		if(baselibCrDelFgets(buf, bufmax, fp) == NULL){
			return(NULL);
		}
		if(FileCommentChk(buf) == OFF){
			return(buf);
		}
		else{
			if(m_nFirst == ON){
				m_nFirst = OFF;
				m_pcDoc->m_csComent1 = &(buf[len]);
			}
		}
	}
	CTonApp*	theApp;
	CMainFrame*	theWnd;

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	theWnd->HelpTextOut(0, buf);
}

int CFileVect::FileCommentChk(TCHAR *buf)
{
	int		len1, len2, len3;

	len1 = _tcslen(_T("REM"));
	len2 = _tcslen(_T("rem"));
	len3 = _tcslen(_T("'"));
	if(_tcsnccmp(_T("REM"), buf, len1) == 0){
		return(ON);
	}
	else if(_tcsnccmp(_T("rem"), buf, len2) == 0){
		return(ON);
	}
	else if(_tcsnccmp(_T("'"), buf, len3) == 0){
		return(ON);
	}
	return(OFF);
}

void CFileVect::DibTrnsLogoFile(TCHAR *dotfname, TCHAR *bmpfname)
{
	diblibCmprimgToBmp(dotfname, bmpfname);
}

void CFileVect::Trnsnxybittodxy(int nx, int ny, int xybit, DBL *dx, DBL *dy)
{
	DBL		mmxsize, mmysize;
	DBL		xt, yt;
	int		x1, y1;

	scrngetmmsize(&mmxsize, &mmysize);
	x1 = (xybit >> 4) & 0x0f;
	y1 = (xybit) & 0x0f;
	xt = ((DBL)x1) / 16.0;
	yt = ((DBL)y1) / 16.0;
	*dx = ((DBL)(nx)) + xt;
	*dy = ((DBL)(ny)) + yt;
	*dy = mmysize - (*dy);
}
