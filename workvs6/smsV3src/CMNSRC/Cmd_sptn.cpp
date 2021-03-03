
/////////////////////////////////////////////////////////////////////////////
// CCmdPtn

#include "stdafx.h"
#include "stdio.h"
#include "math.h"

#include "cmd_base.h"
#include "cmd_sptn.h"
#include "dlgptngp.h"
#include "dlgptnsv.h"
#include "dlgptnld.h"
#include "dlgptndl.h"
#include "dlgtxtin.h"

#define	FREEARCLIMIT	10.0

extern "C" {
	DBL	curvedegree1get();
	void baseaddsafix(LPSTR FileName, LPSTR Safix);
	void senvgetexepath(LPSTR path);
	int	senvsetdrawmode(int mode);
	void ptnenvgetpaperelement();
	void ptnenvsetpaperelement();
	void ptnenvgetgrupname(LPSTR name);
	void ptnenvsetgrupname(LPSTR name);
	void ptnenvsetpapersize(DBL xsize, DBL ysize);
	void ptnenvgetpapersize(DBL *xsize, DBL *ysize);
	void ptnenvgetgruppath(LPSTR path);
	void ptnenvsetselectidx(int idx);
	void ptnenvgetselectidx(int *idx);
	void ptnenvlistaddname(int slctidx, LPSTR ptnfname);
	void ptnenvlistchkaddname(LPSTR ptnfname);
	void ptnenvlistdelname(int slctidx, LPSTR ptnfname);
	void ptnenvlistgetname(int slctidx, LPSTR ptnfname);
	void ptnenvsetmode(int nunit, int noder, int nset);
	void ptnenvgetmode(LPINT nunit, LPINT noder, LPINT nset);
	void scrngetmmsize(LPDBL dmmxsize, LPDBL dmmysize);
	void scrnsetmmsize(DBL dmmxsize, DBL dmmysize);
	void scrnsetmmworkstart(DBL m_dPaperSX, DBL m_dPaperSY);
	void scrnsetmmworksize(DBL m_dPaperWidth, DBL m_dPaperHight);
	DBL scrnsetxytime(DBL timex, DBL timey);
	void scrnsetrltvtop(DBL xTop, DBL yTop);
	void scrngetrltvtop(LPDBL xTop, LPDBL yTop);
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void scrnsetrltvelement();
	void scrnsetdisprltvmove(DBL xmove, DBL ymove);
	void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove);
	void scrnresetzoomtime();
	void scrndxymmtorltv(LPDBL x, LPDBL y);
	void scrnsetdelpen(HDC PaintDC);
	void scrnsetslctpen(HDC PaintDC);
	void scrnpenend(HDC PaintDC);
	void mojienvgetspace(LPDBL dmsp, LPDBL dlsp);
	void mojienvgetfontname(LPSTR fontname);
	void mojienvsetfontname(LPSTR fontname);
	int DispatchWindows(HWND hWin);
}

CCmdSPtn::CCmdSPtn(CScrollView* pcview)
:CCmdBase(pcview)
{
	char	exepath[FNAMEMAX];
	char	envfname[FNAMEMAX];
	char	szgrupname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];

	senvgetexepath(exepath);
	sprintf(envfname, "%s\\defenv\\cmn_ptn.env", exepath);
	if((fp = fopen(envfname, "r")) == NULL){
		return;
	}
	CrDelFgets(buf, HRECMAX, fp);
	sscanf(buf, "%s", szgrupname);
	fclose(fp);

	ptnenvsetgrupname(szgrupname);
	ptnenvgetpaperelement();
	m_nSlctidx = 0;
}

CCmdSPtn::~CCmdSPtn()
{
	char	exepath[FNAMEMAX];
	char	envfname[FNAMEMAX];
	char	szgrupname[FNAMEMAX];
	FILE	*fp;

	ptnenvgetgrupname(szgrupname);
	senvgetexepath(exepath);
	sprintf(envfname, "%s\\defenv\\cmn_ptn.env", exepath);
	if((fp = fopen(envfname, "w")) == NULL){
		return;
	}
	fprintf(fp, "%s\n", szgrupname);
	fclose(fp);
}

UINT CCmdSPtn::ExecGrupSet(UINT event, DBL dx, DBL dy)
{
	CDialogPtnGrup*	pDialogPtnGrup;
	char	grupname[HRECMAX];
	DBL		xsize, ysize;
	CTonApp*	theApp;
	CSysEnv*	pcSysEnv;

	if(event != INITEVENT){
		return(0);
	}
	pDialogPtnGrup = new CDialogPtnGrup(m_pcView);
	if(pDialogPtnGrup->DoModal() == IDOK){
		strcpy(grupname, pDialogPtnGrup->m_csGrupName.GetBuffer(RECMAX));
		xsize = pDialogPtnGrup->m_dPaperSizeX;
		ysize = pDialogPtnGrup->m_dPaperSizeY;
		ptnenvsetgrupname(grupname);
		ptnenvsetpapersize(xsize, ysize);
		ptnenvsetpaperelement();

		scrnsetmmsize(xsize, ysize);
		scrnsetmmworkstart(0.0, 0.0);
		scrnsetmmworksize(xsize, ysize);
		((CTonView*)m_pcView)->WorkSizeReset();
		theApp = (CTonApp*)AfxGetApp();
		pcSysEnv = theApp->m_pcSysEnv;
		pcSysEnv->ScrnResetCntrXY();

		scrnresetzoomtime();
		scrnsetrltvelement();
		((CTonView*)m_pcView)->InitScrollElement();
	}
	delete(pDialogPtnGrup);
	return(ENDEVENT);
}

UINT CCmdSPtn::ExecSave(UINT event, DBL dx, DBL dy)
{
	CDialogPtnSave *pDialogPtnSave;

	if(event != INITEVENT){
		return(0);
	}
	pDialogPtnSave = new CDialogPtnSave(m_pcView);
	pDialogPtnSave->m_nSlctidx = m_nSlctidx;
	if(pDialogPtnSave->DoModal() == IDOK){
		m_nSlctidx = pDialogPtnSave->m_nSlctidx;
		ptnenvsetselectidx(m_nSlctidx);	
		SaveCrtSlctSPtn();
	}
	
	return(ENDEVENT);
}

UINT CCmdSPtn::ExecLoad(UINT event, DBL dx, DBL dy)
{
	CDialogPtnLoad *pDialogPtnLoad;

	if(event != INITEVENT){
		return(0);
	}
	pDialogPtnLoad = new CDialogPtnLoad(m_pcView);
	pDialogPtnLoad->m_nSlctidx = 0;
	if(pDialogPtnLoad->DoModal() == IDOK){
		SaveUndo();
		m_nSlctidx = pDialogPtnLoad->m_nSlctidx;
		ptnenvsetselectidx(m_nSlctidx);

		LoadCrtSlctSPtn();
		
	}
	ScrnAllRedraw();
	
	return(ENDEVENT);
}

UINT CCmdSPtn::ExecDelete(UINT event, DBL dx, DBL dy)
{
	CDialogPtnDelete *pDialogPtnDelete;

	if(event != INITEVENT){
		return(0);
	}
	pDialogPtnDelete = new CDialogPtnDelete(m_pcView);
	pDialogPtnDelete->m_nSlctidx = 0;
	if(pDialogPtnDelete->DoModal() == IDOK){
		m_nSlctidx = pDialogPtnDelete->m_nSlctidx;
		ptnenvsetselectidx(m_nSlctidx);	
		DelCrtSlctSPtn();
	}
	
	return(ENDEVENT);
}

void CCmdSPtn::DataGrupSet()
{
	CTonDoc*	pcDoc;
	CData*		pcData;
	CDataGrup*	pcDataGrup;
	POSITION	pos;
	POSITION	npos;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());

	pcDataGrup = (CDataGrup*)(pcDoc->NewData(DATAGRUPID));

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		npos = pos;
		pcData = pcObjDataList->GetNext(npos);
		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			pcObjDataList->RemoveAt(pos);
			pcDataGrup->SetData(pcData);
		}
		if(npos == NULL){
			break;
		}
		pos = npos;
	}

	pcDoc->DataAddTail((CData*)pcDataGrup);
}

void CCmdSPtn::SaveCrtSlctSPtn()
{
	CDialogTextInput*	pDialogTextInput;
	char	gruppath[FNAMEMAX];
	char	ptnname[FNAMEMAX];
	char	ptnfilename[FNAMEMAX];
	char	bmpfilename[FNAMEMAX];
	int		ret;
	int		slctidx;
	int		len;

	ptnenvgetgruppath(gruppath);
	ptnenvgetselectidx(&slctidx);
	ptnenvlistgetname(slctidx, ptnname);
		
	pDialogTextInput = new CDialogTextInput(m_pcView);
	pDialogTextInput->m_nIMEMode = ON;
	pDialogTextInput->m_csText = _T(ptnname);
	ret = pDialogTextInput->DoModal();
	if(ret == IDOK){
		strcpy(ptnname, pDialogTextInput->m_csText.GetBuffer(RECMAX));
		baseaddsafix(ptnname, "ptn");
		sprintf(ptnfilename, "%s\\%s", gruppath, ptnname);
		SmdFileCreate(ptnfilename);

		strcpy(bmpfilename, ptnfilename);
		len = strlen(bmpfilename);
		bmpfilename[len-3] = '\0';
		strcat(bmpfilename, "bmp");
		SmallFileCreate(bmpfilename);

		ptnenvlistaddname(slctidx, ptnname);
	}
}

void CCmdSPtn::LoadCrtSlctSPtn()
{
	CTonDoc*	pcDoc;
	char	gruppath[FNAMEMAX];
	char	ptnname[FNAMEMAX];
	char	ptnfilename[FNAMEMAX];
	int		slctidx;
	DBL		sx, sy, ex, ey;
	DBL		xmm, ymm;
	DBL		movex, movey;

	ptnenvgetgruppath(gruppath);
	ptnenvgetselectidx(&slctidx);
	ptnenvlistgetname(slctidx, ptnname);
		
	
	baseaddsafix(ptnname, "ptn");
	sprintf(ptnfilename, "%s\\%s", gruppath, ptnname);

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->OffFlag(ACTIVEFLAG);
	SmdFileLoad(ptnfilename);
	DataGrupSet();
	pcDoc->GetDMiniMax(ACTIVEFLAG, &sx, &sy, &ex, &ey);
	scrngetmmsize(&xmm, &ymm);
	//movex = -sx+5;
	//movey = ymm-ey-5;
	//movex = -sx;
	//movey = ymm-ey;
	movex = 0;
	movey = ymm-ey-2;
	pcDoc->MoveTrns(movex, movey);
	HlpMsgDsp(STATBARMODE, ptnname);
}

void CCmdSPtn::DelCrtSlctSPtn()
{
	char	gruppath[FNAMEMAX];
	char	ptnname[FNAMEMAX];
	char	ptnfilename[FNAMEMAX];
	char	bmpfilename[FNAMEMAX];
	int		slctidx;
	int		len;

	ptnenvgetgruppath(gruppath);
	ptnenvgetselectidx(&slctidx);
	ptnenvlistgetname(slctidx, ptnname);
		
	baseaddsafix(ptnname, "ptn");
	sprintf(ptnfilename, "%s\\%s", gruppath, ptnname);
	strcpy(bmpfilename, ptnfilename);
	len = strlen(bmpfilename);
	bmpfilename[len-3] = '\0';
	strcat(bmpfilename, "bmp");

	_unlink(ptnfilename);
	_unlink(bmpfilename);

	ptnenvlistdelname(slctidx, ptnname);
}

void CCmdSPtn::SmdFileCreate(LPSTR ptnfilename)
{
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	CFile file(ptnfilename,  CFile::modeCreate|CFile::modeWrite);
	CArchive ar(&file, CArchive::store);
	pcDoc->Serialize(ar);
	ar.Close();
	file.Close();
}

void CCmdSPtn::SmdFileLoad(LPSTR ptnfilename)
{
	CTonDoc*	pcDoc;

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	CFile file(ptnfilename, CFile::modeRead);
	CArchive ar(&file, CArchive::load);
	pcDoc->Serialize(ar);
	ar.Close();
	file.Close();
}

void CCmdSPtn::SmallFileCreate(LPSTR bmpfilename)
{
	SmallDrawAreaInit();
	SmallDrawMain(bmpfilename);
	SmallDrawAreaEnd();
}

void CCmdSPtn::SmallDrawAreaInit()
{
	DBL		areaxsize, areaysize;
	DBL		xtime, ytime, time;

	scrngetmmsize(&m_dBxmm, &m_dBymm);
	xtime = m_dBxmm / ONESPTNSIZE;
	ytime = m_dBymm / ONESPTNSIZE;
	if(xtime > ytime){
		time = xtime;
	}
	else{
		time = ytime;
	}
	areaxsize = m_dBxmm / time;
	areaysize = m_dBymm / time;
	m_dBtime = scrnsetxytime(time, time);
	scrngetrltvtop(&m_dBsx, &m_dBsy); 
	scrngetdisprltvmove(&m_dBmvx, &m_dBmvy);
	scrnsetrltvtop(0.0, 0.0);
	scrnsetrltvelement();
	scrnrltvareaset(0.0, 0.0, (DBL)ONESPTNSIZE, (DBL)ONESPTNSIZE);
	scrnsetdisprltvmove(0.0, 0.0);
}

void CCmdSPtn::SmallDrawMain(LPSTR bmpfilename)
{
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER	lpbi;
	HBITMAP		hbmpOld;
	long		dwFileSize;
	CTonDoc*	pcDoc;
	int			mode;

	m_pcDC = m_pcView->GetDC();

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = ONESPTNSIZE;
	bi.biHeight = ONESPTNSIZE;
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	m_hBMP = CreateDIBitmap(m_pcDC->m_hDC, &bi, 0L, NULL, NULL, 0);

	m_dwXByte = (((ONESPTNSIZE*3 + 3) / 4) * 4);
	dwFileSize = sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD) + m_dwXByte * ONESPTNSIZE;

	m_hDIB = GlobalAlloc(GHND, dwFileSize);
	lpbi = (LPBITMAPINFOHEADER) GlobalLock(m_hDIB);
	*lpbi = bi;
	GetDIBits(m_pcDC->m_hDC, m_hBMP, 0, ONESPTNSIZE, NULL, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
	GlobalUnlock(m_hDIB);

	m_pcMemDC = new CDC();
	m_pcMemDC->CreateCompatibleDC(m_pcDC);
	m_pcView->ReleaseDC(m_pcDC);

	pcDoc = (CTonDoc*)m_pcView->GetDocument();
	hbmpOld = (HBITMAP)SelectObject(m_pcMemDC->m_hDC, m_hBMP);
	DCAreaCls(m_pcMemDC->m_hDC, 0, 0, ONESPTNSIZE, ONESPTNSIZE);

	mode = senvsetdrawmode(PREVIEW);

	CTypedPtrList<CObList,CData*> *pcObjDataList = pcDoc->m_pcObjDataList;
	POSITION pos = pcObjDataList->GetHeadPosition();
	while (pos != NULL)
	{
		CData* pcData = pcObjDataList->GetNext(pos);

		if(pcData->CheckFlag(ACTIVEFLAG) == ON){
			pcData->OnDraw(m_pcMemDC);
		}
	}
	senvsetdrawmode(mode);


	BmpFileDataOut(bmpfilename);

	SelectObject(m_pcMemDC->m_hDC, hbmpOld);

	delete(m_pcMemDC);
	GlobalFree(m_hDIB);

}

void CCmdSPtn::SmallDrawAreaEnd()
{
	scrnsetxytime(m_dBtime, m_dBtime);
	scrnsetmmsize(m_dBxmm, m_dBymm);
	scrnsetrltvtop(m_dBsx, m_dBsy); 
	scrnsetrltvelement();
	scrnsetdisprltvmove(m_dBmvx, m_dBmvy);
	scrnrltvareaset(-INTMAX, -INTMAX, INTMAX, INTMAX);
}

void CCmdSPtn::BmpFileDataOut(LPSTR bmpfilename)
{
	FILE	*fp;
	BITMAPFILEHEADER	bmpfh;
	LPBITMAPINFOHEADER	lpbi;
	LPBYTE	lpbits;
	long	dwsize;

	if((fp = fopen(bmpfilename, "wb")) == NULL){
		return;
	}
	dwsize = (long)(sizeof(BITMAPFILEHEADER))
			 + (long)(sizeof(BITMAPINFOHEADER))
			 + (long)m_dwXByte*(long)ONESPTNSIZE;
	bmpfh.bfType = 0x4d42;
	bmpfh.bfSize = dwsize;
	bmpfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, fp);
	lpbi = (LPBITMAPINFOHEADER) GlobalLock(m_hDIB);
	lpbits = ((LPBYTE)lpbi) + (long)(sizeof(BITMAPINFOHEADER));
	fwrite(lpbi, sizeof(BITMAPINFOHEADER), 1, fp);
	dwsize = m_dwXByte * ONESPTNSIZE + (long)(sizeof(BITMAPINFOHEADER));
	GetDIBits(m_pcMemDC->m_hDC, m_hBMP, 0, ONESPTNSIZE, lpbits, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
	fwrite(lpbits, dwsize, 1, fp);

	GlobalUnlock(m_hDIB);
	fclose(fp);
}

void CCmdSPtn::DCAreaCls(HDC hDC, int sx, int sy, int ex, int ey)
{
	HPEN	hP, oldhP;
	HBRUSH	hB, oldhB;

	oldhP = (HPEN)SelectObject(hDC, CreatePen(PS_SOLID, 1, RGB(255, 255, 255)));
	oldhB = (HBRUSH)SelectObject(hDC, CreateSolidBrush(RGB(255, 255, 255)));
	Rectangle(hDC, sx, sy, ex, ey);
	hB = (HBRUSH)SelectObject(hDC, oldhB);
	hP = (HPEN)SelectObject(hDC, oldhP);
	DeleteObject(hB);
	DeleteObject(hP);
}

