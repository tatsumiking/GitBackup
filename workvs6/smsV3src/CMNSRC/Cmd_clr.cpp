
/////////////////////////////////////////////////////////////////////////////
// CCmdClr

#include "stdafx.h"
#include "math.h"

#include "cmd_base.h"
#include "cmd_dibc.h"

extern "C" {
	void diblibcreatebmpfile(LPSTR bmpfilename);
	void senvgetexepath(LPSTR execpath);
	void senvsetcrtfillpenno(int penno);
	void senvsetcrtlinepenno(int penno);
	void senvsetcrtlinethick(DBL thick);
	void senvgetpenrgb(int pno, LPINT rval, LPINT gval, LPINT bval);
}

CCmdClr::CCmdClr(CScrollView* pcview)
:CCmdBase(pcview)
{
}

UINT CCmdClr::ExecClrSet(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		pcData = SrchData(ACTIVEFLAG, dx, dy);
		if(pcData != NULL){
			SaveUndo();
			pcData->SetDefFillClr();
			pcData->SetDefOutLineClr();
			ScrnDataRedraw(pcData);
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdClr::ExecClrBackSet(UINT event, DBL dx, DBL dy)
{
	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	CColorDialog *pcColorDialog = new CColorDialog(m_pcApp->m_clrBack, CC_FULLOPEN, NULL);
	if(pcColorDialog->DoModal() == IDOK){
		m_pcApp->m_clrBack = pcColorDialog->GetColor();
	}
	delete(pcColorDialog);
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdClr::ExecClrBackReset(UINT event, DBL dx, DBL dy)
{
	if(event != INITEVENT){
		return(0);
	}
	InitClass();
	m_pcApp->m_clrBack = -1;
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdClr::ExecClrPicup(UINT event, DBL dx, DBL dy)
{
	CData	*pcData;
	int		pen1, pen2;
	DBL		width;
	int		r1, g1, b1;
	int		r2, g2, b2;
	char	msg[RECMAX];

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		pcData = SrchData(ACTIVEFLAG, dx, dy);
		if(pcData != NULL){
			pen1 = pcData->GetFillPen();
			senvsetcrtfillpenno(pen1);
			pen2 = pcData->GetOutLinePen();
			senvsetcrtlinepenno(pen2);
			width = pcData->GetOutLineWidth();
			senvsetcrtlinethick(width);
			senvgetpenrgb(pen1, &r1, &g1, &b1);
			senvgetpenrgb(pen2, &r2, &g2, &b2);
			SetToolBarElement();
			sprintf(msg, "塗潰%03d(R%03d, G%03d, B%03d)　線%03d(R%03d, G%03d, B%03d) 線幅(%4.1lf)",
						pen1, r1, g1, b1, pen2, r2, g2, b2, width);
			HlpMsgDsp(msg);
		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdClr::ExecClrCutOnOff(UINT event, DBL dx, DBL dy)
{
	CVect	*pcVect;
	DBL		xx, yy;
	int		retpp;

	switch(event){
	case INITEVENT:
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		pcVect = (CVect*)SrchDataOnLinePoint(dx, dy, &xx, &yy, &retpp);
		if(pcVect != NULL && pcVect->CheckID() == BASEVECTID){
			if(pcVect->CheckFlag(VCUTFLAG) == ON){
				BeginDraw();
				DrawModeSET();
				pcVect->DelDraw(m_pcDC);
				EndDraw();
				pcVect->OffFlag(VCUTFLAG);
			}
			else{
				pcVect->OnFlag(VCUTFLAG);
			}
			BeginDraw();
			DrawModeSET();
			pcVect->AtntionDraw(m_pcDC);
			EndDraw();

		}
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case ENDEVENT:
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}

UINT CCmdClr::ExecClrSlctCutOn(UINT event, DBL dx, DBL dy)
{
	POSITION	pos;
	CData	*pcData;

	if(event != INITEVENT){
		return(0);
	}
	SaveUndo();
	SetDataListSelectID(BASEVECTID, ACTIVEFLAG);
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		pcData->OnFlag(VCUTFLAG);
	}
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdClr::ExecClrSlctCutOff(UINT event, DBL dx, DBL dy)
{
	POSITION	pos;
	CData	*pcData;

	if(event != INITEVENT){
		return(0);
	}
	SaveUndo();
	SetDataListSelectID(BASEVECTID, ACTIVEFLAG);
	pos = m_cPtrList.GetHeadPosition();
	while (pos != NULL)
	{
		pcData = m_cPtrList.GetNext(pos);
		pcData->OffFlag(VCUTFLAG);
	}
	ScrnAllRedraw();
	return(ENDEVENT);
}

UINT CCmdClr::ExecGradation(UINT event, DBL dx, DBL dy)
{
	switch(event){
	case INITEVENT:
		m_pcDialogGradation = new CDialogGradation(m_pcView);
		if(m_pcDialogGradation->Create(IDD_GRADATION, m_pcView)){
			LocateGradationDlg();
		}
		m_nMouseSetCount = 0;
		break;
	case SETEVENT:
		break;
	case MOVEEVENT:
	case UPMOVEEVENT:
		break;
	case REDRAWEVENT:
		break;
	case RESETEVENT:
		break;
	case DLGSETEVENT:
		SaveUndo();
		CreateGradationDib();
		DataFDibCreate();
		m_pcDialogGradation->DestroyWindow();
		m_nMouseSetCount = 0;
		ScrnDataRedraw((CData *)m_pcDataFDib);
		return(ENDEVENT);
	case ENDEVENT:
		m_pcDialogGradation->DestroyWindow();
		m_nMouseSetCount = 0;
		return(ENDEVENT);
	default:
		break;
	}
	return(0);
}


void CCmdClr::LocateGradationDlg()
{
	/*
	CTonApp*	theApp;
	CMainFrame*	theWnd;
	CRect	prect;
	CRect	crect;
	int		x, y, cx, cy;
	int		ret;

	m_pcView->GetWindowRect(&prect);
	m_pcDialogGradation->GetWindowRect(&crect);
	x = prect.right - crect.Width();
	y = prect.top;
	cx = crect.Width();
	cy = crect.Height();

	theApp = (CTonApp*)AfxGetApp();
	theWnd = (CMainFrame*)theApp->GetMainWnd();
	ret = m_pcDialogGradation->SetWindowPos(theWnd, x, y, cx, cy, 0);
	*/
	m_pcDialogGradation->ShowWindow(SW_SHOW);
}

void CCmdClr::CreateGradationDib()
{
	int		slct;
	int		bitcnt;
	HANDLE	hBarDib;
	char	exepath[FNAMEMAX];
	char	bmpfname[FNAMEMAX];
	CFile	cpinFile;
	CFile	cpoutFile;
	LPBYTE	lpDib;
	LPBITMAPFILEHEADER lpbmf;

	long	lFileSize;
	long	rgbtbltop, rgbtblsize;
	HANDLE	hFileImg;
	LPUCHAR	lpFileImg;

	slct = m_pcDialogGradation->m_nCrtSelect;
	bitcnt = m_pcDialogGradation->m_nBitCount[slct];
	hBarDib = m_pcDialogGradation->m_hDib;
	lpDib = (LPBYTE)GlobalLock(hBarDib);
	lpbmf = (LPBITMAPFILEHEADER)(lpDib);

	senvgetexepath(exepath);
	sprintf(bmpfname, "%s\\defenv\\Grabase%d.bmp", exepath, slct+1); 
	if((cpinFile.Open(bmpfname, CFile::modeRead)) == FALSE){
		m_pcView->MessageBox("ファイルが見つかりません", bmpfname, MB_OK);
		return;
	}

	diblibcreatebmpfile(m_szBmpFname);
	if((cpoutFile.Open(m_szBmpFname, CFile::modeCreate | CFile::modeWrite)) == FALSE){
		return;
	}
	lFileSize = cpinFile.GetLength();
	hFileImg = GlobalAlloc(GHND, lFileSize);
	lpFileImg = (LPUCHAR)GlobalLock(hFileImg);

	rgbtbltop = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	rgbtblsize = lpbmf->bfOffBits - rgbtbltop;

	cpinFile.ReadHuge(lpFileImg, lFileSize);
	if(bitcnt == 8){
		memcpy(&(lpFileImg[rgbtbltop]), &(lpDib[rgbtbltop]), rgbtblsize);
	}
	cpoutFile.WriteHuge(lpFileImg, lFileSize);

	cpoutFile.Close();
	cpinFile.Close();

	GlobalUnlock(hFileImg);
	GlobalFree(hFileImg);

	GlobalUnlock(hBarDib);
}

void CCmdClr::DataFDibCreate()
{
	CTonDoc*	pcDoc;
	CDibCData*	pDibCData;
	DBL	minx, miny, maxx, maxy;

	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	m_pcDataFDib = (CDataFDib*)(pcDoc->NewData(DATAFDIBID));
	if(m_pcDataFDib == NULL){
		return;
	}
	pDibCData = (CDibCData*)(pcDoc->NewData(DIBCDATAID));
	pDibCData->SetBmpFname(m_szBmpFname);
	pDibCData->DibLoad();

	pcDoc->OnFlagDataLink(ACTIVEFLAG, m_pcDataFDib);

	m_pcDataFDib->SetDibCData(pDibCData);
	m_pcDataFDib->GetDMiniMax(&minx, &miny, &maxx, &maxy);
	minx -= 1; miny -= 1;
	maxx += 1; maxy += 1;
	pDibCData->SetDstElement(minx, miny, maxx-minx, maxy-miny);
	pDibCData->RenewMiniMax();
	AddLinkDoc((CData*)m_pcDataFDib);
}

