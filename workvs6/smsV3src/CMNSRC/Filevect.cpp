
/////////////////////////////////////////////////////////////////////////////
// CFileVect

#include "stdafx.h"
#include "math.h"

#include "filevect.h"

extern "C" {
	void diblibcreatebmpfile(LPSTR bmpfname);
	void diblibCmprimgToBmp(LPSTR dotfname, LPSTR bmpfname);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
	void mojienvgetflist(int idx, int *type, LPSTR sfname, LPSTR ffname);
}

#define	TATEGAKI	1

static char	strgheadold[] = "string-start";	//ds 文字列データ開始識別文字列
static char	strgendold[] = "string-end";	//ds 文字列データ終了識別文字列
static char	mojiheadold[] = "moji-start";	//ds 文字データ開始識別文字列
static char	mojiendold[] = "moji-end";		//ds 文字データ終了識別文字列
static char	vectheadold[] = "vect-start";		//ds ベクトルデータ開始識別文字列
static char	vectendold[] = "vect-end";			//ds ベクトルデータ終了識別文字列
static char	logomarkold[] = "logo-data";		//ds ロゴデータ開始識別文字列
static char	strghead[] = "string2start";	//ds 文字列データ開始識別文字列
static char	strgend[] = "string2end";		//ds 文字列データ終了識別文字列
static char	mojihead[] = "moji2start";		//ds 文字データ開始識別文字列
static char	mojiend[] = "moji2end";			//ds 文字データ終了識別文字列
static char	mmvcthead[] = "mmvct_start";	//ds ベクトルデータ開始識別文字列
static char	mmvctend[] = "mmvct_end";		//ds ベクトルデータ終了識別文字列
static char	logomark[] = "logo-data";		//ds ロゴデータ開始識別文字列
static char	mmlgmark[] = "mmlg-data";		//ds ロゴデータ開始識別文字列
static char	mmwakuhead[] = "mmwk_start";	//ds 枠データ開始識別文字列
static char	mmwakuend[] = "mmwk_end";		//ds 枠データ終了識別文字列
static char	mmrulehead[] = "mmrl_start";	//ds 罫線データ開始識別文字列
static char	mmruleend[] = "mmrl_end";		//ds 罫線データ終了識別文字列

CFileVect::CFileVect(CScrollView* pcview)
{
	m_pcView = (CTonView*)pcview;
	m_pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_nFirst = ON;
	m_pcESF0Font = m_pcDoc->m_pcESF0Font;
	m_pcESF1Font = m_pcDoc->m_pcESF1Font;
	m_pcFSF1Font = m_pcDoc->m_pcFSF1Font;
}

void CFileVect::LoadVectFile(LPSTR filename)
{
	FILE	*fp;
	char	buf[RECMAX];
	char	basename[FNAMEMAX];
	int		len;
	CDataGrup*	pcDataGrup;
	CDataList*	pcDataList;
	CStrgVect*	pcStrgVect;
	int			strgidx;
	char		strg[RECMAX];
	CData*		pcData;

	pcDataGrup = (CDataGrup*)(m_pcDoc->NewData(DATAGRUPID));
	pcDataList = (CDataList*)(m_pcDoc->NewData(DATALISTID));
	pcStrgVect = (CStrgVect*)(m_pcDoc->NewData(STRGVECTID));
	pcStrgVect->SetTateYoko(YOKOGAKI);
	strgidx = 0;
	strg[0] = '\0';

	pcDataGrup->SetData(pcDataList);

	len = strlen(filename);
	strncpy(basename, filename, len-4);
	basename[len-4] = '\0';

	FontFileOpen(basename);
	if((fp = fopen(filename, "r")) == NULL){
		return;
	}
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(strncmp(logomark, buf, 9) == 0){
			if((pcData = LoadLogoData(fp, buf)) == NULL){
				break;
			}
			pcDataGrup->SetData(pcData);
		}
		else if(strncmp(mmlgmark, buf, 9) == 0){
			if((pcData = LoadLogoData(fp, buf)) == NULL){
				break;
			}
			pcDataGrup->SetData(pcData);
		}
		else if(strncmp(mmwakuhead, buf, 10) == 0){
			if((pcData = LoadWakuData(fp, buf)) == NULL){
				break;
			}
			pcDataGrup->SetData(pcData);
		}
		else if(strncmp(strghead, buf, 12) == 0){
			if((pcData = LoadStrgData(fp, buf)) == NULL){
				break;
			}
			pcDataGrup->SetData(pcData);
		}
		else if(strncmp(mojihead, buf, 10) == 0){
			if((pcData = LoadMojiData(fp, buf)) == NULL){
				break;
			}
			if(strgidx < RECMAX){
				strg[strgidx++] = ((((CMojiVect*)pcData)->m_nCode) >> 8) & 0x00ff;
				strg[strgidx++] = ((((CMojiVect*)pcData)->m_nCode)) & 0x00ff;
				strg[strgidx] = '\0';
			}
			pcStrgVect->SetMojiLink((CMojiVect*)pcData);
		}
		else if(strncmp(mmrulehead, buf, 10) == 0){
			if((pcData = LoadRuleData(fp, buf)) == NULL){
				break;
			}
			pcDataList->SetData(pcData);
		}
		else if(strncmp(mmvcthead, buf, 11) == 0){
			if((pcData = LoadVectData(fp, buf)) == NULL){
				break;
			}
			pcDataList->SetData(pcData);
		}
		else if(strncmp(strgheadold, buf, 12) == 0){
			continue;
		}
		else if(strncmp(strgendold, buf, 10) == 0){
			continue;
		}
		else if(strncmp(mojiheadold, buf, 10) == 0){
			continue;
		}
		else if(strncmp(mojiendold, buf, 8) == 0){
			continue;
		}
		else if(strncmp(vectheadold, buf, 10) == 0){
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

void CFileVect::LoadLogoFile(LPSTR filename)
{
	FILE	*fp;
	char	buf[RECMAX];
	char	basename[FNAMEMAX];
	char	dotfname[FNAMEMAX];
	char	bmpfname[FNAMEMAX];
	char	tmp[QRECMAX];
	CDataLogo*	pcDataLogo;
	CDibCData*	pcDibCData;
	CData*		pcData;
	int			len;
	DBL			sx, sy;
	DBL			ex, ey;
	DBL			xsize, ysize;
	

	len = strlen(filename);
	strncpy(basename, filename, len-4);
	basename[len-4] = '\0';
	sprintf(dotfname, "%s.dot", basename);

	if((fp = fopen(filename, "r")) == NULL){
		return;
	}

	FileFgets(buf, RECMAX, fp);
	sscanf(buf, "%s %lf %lf %lf %lf\n", tmp, &sx, &ey, &ex, &sy);
	pcDataLogo = (CDataLogo*)(m_pcDoc->NewData(DATALOGOID));

	diblibcreatebmpfile(bmpfname);
	DibTrnsLogoFile(dotfname, bmpfname);

	pcDibCData = (CDibCData*)(m_pcDoc->NewData(DIBCDATAID));
	pcDibCData->SetBmpFname(bmpfname);
	pcDibCData->DibLoad();
	pcDataLogo->SetDibCData(pcDibCData);
	xsize = ex - sx;
	ysize = ey - sy;
	pcDibCData->SetDstElement(sx, sy, xsize, ysize);

	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		else if(strncmp(mmvcthead, buf, 11) == 0){
			if((pcData = LoadVectData(fp, buf)) == NULL){
				break;
			}
			pcDataLogo->SetData(pcData);
		}
	}
	m_pcDoc->DataAddTail((CData*)pcDataLogo);

	fclose(fp);
}

CData* CFileVect::LoadLogoData(FILE *fp, char buf[])
{
	char	tmp[QRECMAX];
	char	dotfname[FNAMEMAX];
	char	bmpfname[FNAMEMAX];
	CDataLogo*	pcDataLogo;
	CDibCData*	pcDibCData;
	CData*		pcData;
	CVect*		pcVect;
	DBL			sx, sy;
	DBL			ex, ey;
	DBL			xsize, ysize;

	sscanf(buf, "%s %s\n", tmp, dotfname);
	pcDataLogo = (CDataLogo*)(m_pcDoc->NewData(DATALOGOID));

	diblibcreatebmpfile(bmpfname);
	DibTrnsLogoFile(dotfname, bmpfname);

	pcDibCData = (CDibCData*)(m_pcDoc->NewData(DIBCDATAID));
	pcDibCData->SetBmpFname(bmpfname);
	pcDibCData->DibLoad();
	pcDataLogo->SetDibCData(pcDibCData);

	if(FileFgets(buf, RECMAX, fp) == NULL){
		delete(pcDataLogo);
		return(NULL);
	}
	if(strncmp(mmvcthead, buf, 11) == 0){
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

CData* CFileVect::LoadStrgData(FILE *fp, char buf[])
{
	char	tmp[QRECMAX];
	char	strg[RECMAX];
	int		strgidx;
	UINT	sjiscode, hc, lc;
	int		flag;
	CStrgVect*	pcStrgVect;
	CData*	pcData;

	sscanf(buf, "%s %x\n", tmp, &flag);
	pcStrgVect = (CStrgVect*)(m_pcDoc->NewData(STRGVECTID));
	if(flag == TATEGAKI){
		pcStrgVect->SetTateYoko(TATEGAKI);
	}
	else{
		pcStrgVect->SetTateYoko(YOKOGAKI);
	}
	strgidx = 0;
	flag = OFF;
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(strncmp(strgend, buf, 10) == 0){
			break;
		}
		if(strncmp(mojihead, buf, 10) == 0){
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
			strg[strgidx] = '\0';
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

CData* CFileVect::LoadMojiData(FILE *fp, char buf[])
{
	char	tmp[QRECMAX];
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
	sscanf(buf, "%s %x %x %d %d\n", tmp, &chngflag, &code, &fontno, &datano);
	m_nFontNo = fontno;
	pcMojiVect = (CMojiVect*)(m_pcDoc->NewData(MOJIVECTID));
	pcMojiVect->m_nCode = code;
	pcMojiVect->m_nBaseCode = code;

	if(FileFgets(buf, RECMAX, fp) == NULL){
		delete(pcMojiVect);
		return(NULL);
	}
	if(strncmp(mojiend, buf, 8) == 0){
		delete(pcMojiVect);
		return(NULL);
	}

	if(strncmp(mmvcthead, buf, 11) == 0){
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
		if(strncmp(mojiend, buf, 8) != 0){
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
		if(strncmp(mojiend, buf, 8) == 0){
			break;
		}
		if(strncmp(mmvcthead, buf, 11) == 0){
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

CData* CFileVect::LoadVectData(FILE *fp, char buf[])
{
	char	tmp[QRECMAX];
	CVect*	pcVect;
	int		flag, pp;
	int		atr;
	DBL		x, y;

	sscanf(buf, "%s %d\n", tmp, &flag);
	pcVect =(CVect*)(m_pcDoc->NewData(BASEVECTID));
	pp = 0;
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(strncmp(mmvctend, buf, 9) == 0){
			break;
		}
		sscanf(buf, "%x %lf %lf\n", &atr, &x, &y);
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

CData* CFileVect::LoadOldVectData(FILE *fp, char buf[])
{
	char	tmp[QRECMAX];
	CVect*	pcVect;
	int		flag, pp;
	int		atr, nx, ny, xybit;
	double	dx, dy, time;

	sscanf(buf, "%s %d\n", tmp, &flag);
	pp = 0;
	pcVect =(CVect*)(m_pcDoc->NewData(BASEVECTID));
	time = 0.125;
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(strncmp(vectendold, buf, 8) == 0){
			break;
		}
		sscanf(buf, "%x %d %d %x\n", &atr, &nx, &ny, &xybit);
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

CData* CFileVect::LoadWakuData(FILE *fp, char buf[])
{
	char	tmp[QRECMAX];
	char	strg[RECMAX];
	int		idx;
	int		fonttype;
	char	fontname[QRECMAX];
	char	fontfullname[RECMAX];
	int		flag, space;
	CSWakVect*	pcSWakVect;
	CData*	pcData;
	CVect*	pcVect;
	DBL		sx, sy, ex, ey;
	DBL		xsize, ysize;

	sscanf(buf, "%s %x %d\n", tmp, &flag, &space);

	if(FileFgets(buf, RECMAX, fp) == NULL){
		return(NULL);
	}
	if(strncmp(mmvcthead, buf, 11) != 0){
		return(NULL);
	}
	pcData = LoadVectData(fp, buf);
	pcVect = (CVect*)pcData;
	pcVect->GetDXY(0, &sx, &sy);
	pcVect->GetDXY(2, &ex, &ey);
	xsize = ex - sx;
	ysize = sy - ey;
	delete(pcData);

	sscanf(buf, "%s %x\n", tmp, &flag);
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
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(strncmp(strghead, buf, 12) == 0){
			continue;
		}
		else if(strncmp(strgend, buf, 10) == 0){
			continue;
		}
		else if(strncmp(mmwakuend, buf, 8) == 0){
			break;
		}
		else if(strncmp(mojihead, buf, 10) == 0){
			if((pcData = LoadMojiData(fp, buf)) == NULL){
				delete(pcSWakVect);
				return(NULL);
			}
			pcSWakVect->SetMojiLink((CMojiVect*)pcData);
			strg[idx++] = ((((CMojiVect*)pcData)->m_nCode) >> 8) & 0x00ff;
			strg[idx++] = ((((CMojiVect*)pcData)->m_nCode)) & 0x00ff;
		}
	}
	strg[idx] = '\0';
	mojienvgetflist(m_nFontNo, &fonttype, fontname, fontfullname);
	pcSWakVect->SetStringUnLoad(strg);
	pcSWakVect->SetFontName(fontname);
	return((CData*)pcSWakVect);
}

CData* CFileVect::LoadRuleData(FILE *fp, char buf[])
{
	char	tmp[QRECMAX];
	DBL		rthickmm;
	int		flag;
	CRuleVect*	pcRuleVect;
	int		pp;
	int		atr;
	DBL		x, y;

	sscanf(buf, "%s %x %lf\n", tmp, &flag, &rthickmm);
	if(FileFgets(buf, RECMAX, fp) == NULL){
		return(NULL);
	}
	pcRuleVect = (CRuleVect*)(m_pcDoc->NewData(RULEVECTID));
	pcRuleVect->SetOutLineWidth(rthickmm);
	pp = 0;
	while(1){
		if(FileFgets(buf, RECMAX, fp) == NULL){
			return(NULL);
		}
		if(strncmp(mmruleend, buf, 8) == 0){
			break;
		}
		else if(strncmp(mmvcthead, buf, 11) == 0){
			continue;
		}
		else if(strncmp(mmvctend, buf, 9) == 0){
			continue;
		}
		sscanf(buf, "%x %lf %lf\n", &atr, &x, &y);
		pcRuleVect->SetAtrDXY(pp, atr, x, y);
		pp++;
	}
	pcRuleVect->SetPointCount(pp);
	pcRuleVect->RenewMiniMax();
	return((CData*)pcRuleVect);
}

void CFileVect::FontFileOpen(char basename[])
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

LPSTR CFileVect::FileFgets(char *buf, int bufmax, FILE *fp)
{
	while(1){
		if(fgets(buf, bufmax, fp) == NULL){
			return(NULL);
		}
		if(FileCommentChk(buf) == OFF){
			return(buf);
		}
		else{
			if(m_nFirst == ON){
				m_nFirst = OFF;
				m_pcDoc->m_csComent1 = _T(&(buf[4]));
			}
		}
	}
	CTonApp*	theApp;
	CMainFrame*	theWnd;

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	theWnd->HelpTextOut(0, buf);
}

int CFileVect::FileCommentChk(char *buf)
{
	if(strncmp("REM", buf, 3) == 0){
		return(ON);
	}
	else if(strncmp("rem", buf, 3) == 0){
		return(ON);
	}
	else if(strncmp("'", buf, 1) == 0){
		return(ON);
	}
	return(OFF);
}

void CFileVect::DibTrnsLogoFile(char *dotfname, char *bmpfname)
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
