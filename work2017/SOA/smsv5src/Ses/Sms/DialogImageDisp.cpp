// DlgImgDsp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "CrctWaku.h"
#include "DialogImageDisp.h"
#include "DialogImageSize.h"
#include "DialogSetJpgElement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogImageDisp ダイアログ

//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Senvlib.h"
	#include "../../ObjLib/Scrnlib.h"
//}

CDialogImageDisp::CDialogImageDisp(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogImageDisp::IDD, pParent)
{
	LoadImageElement();
	m_nCrtTop = 0;
	m_nExitCall = 0;
	m_clrSlct = -1;
	m_szFont[0] = '\0';
	m_pcView = (CTonView*)pParent;
	m_pcDoc = m_pcView->GetDocument();
	//{{AFX_DATA_INIT(CDialogImageDisp)
	//}}AFX_DATA_INIT
}

CDialogImageDisp::~CDialogImageDisp()
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFName[FNAMEMAX];
	FILE	*fp;

	senvgetexepath(szExePath);
    _stprintf_s(szFName, _T("%s\\imgprnt.env"), szExePath);
	_tfopen_s(&fp, szFName, _T("w"));
	if (fp == NULL) {
		return;
	}
	_ftprintf_s(fp, _T("%d\n"), m_nNoWakuOut);
	_ftprintf_s(fp, _T("%1.1lf\n"), m_dPrntBlkXStart);
	_ftprintf_s(fp, _T("%1.1lf\n"), m_dPrntBlkYStart);
	_ftprintf_s(fp, _T("%1.1lf\n"), m_dPrntBlkXSize);
	_ftprintf_s(fp, _T("%1.1lf\n"), m_dPrntBlkYSize);
	_ftprintf_s(fp, _T("%1.1lf\n"), m_dPrntBlkXSpace);
	_ftprintf_s(fp, _T("%1.1lf\n"), m_dPrntBlkYSpace);
	fclose(fp);
}

void CDialogImageDisp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogImageDisp)
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_STIMGTOP, m_stImgTop);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogImageDisp, CDialog)
	//{{AFX_MSG_MAP(CDialogImageDisp)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTNEXIT, OnBtnexit)
	ON_BN_CLICKED(IDC_BTNPREPAGE, OnBtnprepage)
	ON_BN_CLICKED(IDC_BTNLISTDISP, OnBtnlistdisp)
	ON_BN_CLICKED(IDC_BTNPRINTOUT, OnBtnprintout)
	ON_BN_CLICKED(IDC_BTNNEXTPAGE, OnBtnnextpage)
	ON_BN_CLICKED(IDC_BTNJPGOUT, OnBtnjpgout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogImageDisp メッセージ ハンドラ

BOOL CDialogImageDisp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_szTitle[30] = '\0';
	m_szTitle[31] = '\0';
	m_szTitle[32] = '\0';
	InitDataMax();
	return TRUE;
}

void CDialogImageDisp::OnPaint() 
{
	POSITION pos;
	CCrctWaku *pcCrctWaku;
	CFont	cFont;
	CFont	*pcOldFont;
	CRect	dlgrect;
	CRect	toprect;
	CRect	imgrect;
	int		textsize;
	int		xcnt, ycnt;
	int		sx, sy, ex, ey;
	CRect	rect;
	int		idx, no;
	CString	csStr;

	CPaintDC dc(this);
	senvsetdrawmode(PREVIEW);
	GetWindowRect(&dlgrect);
	m_stTop.GetWindowRect(&toprect);
	m_stImgTop.GetWindowRect(&imgrect);
	m_dDispTime = imgrect.Width() / 10.0;

	imgrect.left -= toprect.left;
	imgrect.top -= toprect.top;
	imgrect.right = dlgrect.Width() - imgrect.left;
	imgrect.bottom = dlgrect.Height() - imgrect.top;
	xcnt = (int)(imgrect.Width() / ((m_dDispBlkXSpace + m_dDispBlkXSize) * m_dDispTime));
	ycnt = (int)(imgrect.Height() / ((m_dDispBlkYSpace + m_dDispBlkYSize) * m_dDispTime));
	m_nOnePageCnt = xcnt * ycnt;

	textsize = (int)(m_dDispBlkYSpace * m_dDispTime * 0.7);
	if(textsize > (int)(5.0 * m_dDispTime)){
		textsize = (int)(5.0 * m_dDispTime);
	}
	cFont.CreateFont(textsize, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, _T("ＭＳ 明朝"));
	pcOldFont = dc.SelectObject(&cFont);

	idx = 0;
	no = 0;
	if (m_nInSort == 0) {
		m_pcDoc->m_pcListCW = &(m_pcDoc->m_lstCWSort);
	}
	else {
		m_pcDoc->m_pcListCW = &(m_pcDoc->m_lstCWRead);
	}
	pos = m_pcDoc->m_pcListCW->GetHeadPosition();
	while (pos != NULL)
	{
		pcCrctWaku = m_pcDoc->m_pcListCW->GetNext(pos);
		if(no == m_nCrtTop){
			break;
		}
		if(pcCrctWaku->m_csFlag == "1"){
			no++;
		}
		idx++;
	}

	sx = imgrect.left;
	sy = imgrect.top;
	if(m_szTitle[0] != _T('\0')){
		csStr = m_szTitle;
		dc.TextOut(sx, sy, csStr);
		sy += textsize;
	}
	InitChangeElement();

	pos = m_pcDoc->m_pcListCW->GetHeadPosition();
	while (pos != NULL)
	{
		pcCrctWaku = m_pcDoc->m_pcListCW->GetNext(pos);
		if (pcCrctWaku->m_csFlag == "1") {
			m_pcDoc->DataAllCls();
			m_pcDoc->WakuLoad(pcCrctWaku->m_csFileName.GetBuffer(FNAMEMAX));
			m_pcDoc->OnFlag(ACTIVEFLAG);
			SetChangeElement();
			m_pcView->AllWakuSetString(pcCrctWaku->m_csStrings.GetBuffer(FNAMEMAX));
			rect.left = sx;
			rect.top = sy;
			rect.right = rect.left + (int)(m_dDispBlkXSize * m_dDispTime);
			rect.bottom = rect.top + (int)(m_dDispBlkYSize * m_dDispTime);
			m_pcView->DrawWakuInRect(&dc, rect);
			csStr.Format(_T("No%d (%03d)"), no+1, idx+1);
			dc.TextOut(rect.left, rect.bottom, csStr);
			sx = rect.right + (int)(m_dDispBlkXSpace * m_dDispTime);
			ex = sx + (int)(m_dDispBlkXSize * m_dDispTime);
			if(ex > imgrect.right){
				sx = imgrect.left;
				sy = rect.bottom + (int)(m_dDispBlkYSpace * m_dDispTime);
				ey = sy + (int)(m_dDispBlkYSize * m_dDispTime);
				if(ey > imgrect.bottom){
					break;
				}
			}
			no++;
		}
		idx++;
	}		

	dc.SelectObject(pcOldFont);
	cFont.DeleteObject();
}

void CDialogImageDisp::LoadImageElement()
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFName[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];

	m_dDispBlkXSize = 50;
	m_dDispBlkYSize = 50;
	m_dDispBlkXSpace = 10;
	m_dDispBlkYSpace = 10;

	m_nNoWakuOut = 0;
	m_dPrntBlkXStart = 10;
	m_dPrntBlkYStart = 10;
	m_dPrntBlkXSize = 50;
	m_dPrntBlkYSize = 50;
	m_dPrntBlkXSpace = 10;
	m_dPrntBlkYSpace = 10;
    
	senvgetexepath(szExePath);
    _stprintf_s(szFName, _T("%s\\imgprnt.env"), szExePath);
	_tfopen_s(&fp, szFName, _T("r"));
	if (fp == NULL) {
		return;
	}
	baselibCrDelFgets(buf, RECMAX, fp);
	m_nNoWakuOut = _ttoi(buf);
	baselibCrDelFgets(buf, RECMAX, fp);
	m_dPrntBlkXStart = _ttoi(buf);
	baselibCrDelFgets(buf, RECMAX, fp);
	m_dPrntBlkYStart = _ttoi(buf);
	baselibCrDelFgets(buf, RECMAX, fp);
	m_dPrntBlkXSize = _ttoi(buf);
	baselibCrDelFgets(buf, RECMAX, fp);
	m_dPrntBlkYSize = _ttoi(buf);
	baselibCrDelFgets(buf, RECMAX, fp);
	m_dPrntBlkXSpace = _ttoi(buf);
	baselibCrDelFgets(buf, RECMAX, fp);
	m_dPrntBlkYSpace = _ttoi(buf);
	fclose(fp);
}
void CDialogImageDisp::OnBtnlistdisp() 
{
	m_nExitCall = 1;
	CDialog::OnOK();
}

void CDialogImageDisp::OnBtnnextpage() 
{
	m_nCrtTop += m_nOnePageCnt;
	if((m_nCrtTop + m_nOnePageCnt) > m_nDataMax){
		m_nCrtTop = m_nDataMax - m_nOnePageCnt;
	}
	Invalidate(TRUE);
}

void CDialogImageDisp::OnBtnprepage() 
{
	if(m_nCrtTop == 0){
		return;
	}
	m_nCrtTop -= m_nOnePageCnt;
	if(m_nCrtTop < 0){
		m_nCrtTop = 0;
	}
	Invalidate(TRUE);
}

void CDialogImageDisp::OnBtnprintout() 
{
	CDialogImageSize	*pcDialogImageSize;
	CPrintDialog		*pcPrintDialog;
	CDC		dcPrint;
	DOCINFO docInfo;
	int		ret;

	CTonApp *theApp = (CTonApp*)AfxGetApp();
	pcDialogImageSize = new CDialogImageSize(m_pcView);
	pcDialogImageSize->m_nNoWakuOut = m_nNoWakuOut;
	pcDialogImageSize->m_dPrntBlkXStart = m_dPrntBlkXStart;
	pcDialogImageSize->m_dPrntBlkYStart = m_dPrntBlkYStart;
	pcDialogImageSize->m_dPrntBlkXSize = m_dPrntBlkXSize;
	pcDialogImageSize->m_dPrntBlkYSize = m_dPrntBlkYSize;
	pcDialogImageSize->m_dPrntBlkXSpace = m_dPrntBlkXSpace;
	pcDialogImageSize->m_dPrntBlkYSpace = m_dPrntBlkYSpace;
	ret = pcDialogImageSize->DoModal();
	if(ret == IDCANCEL){
		return;
	}
	m_nNoWakuOut = pcDialogImageSize->m_nNoWakuOut;
	m_dPrntBlkXStart = pcDialogImageSize->m_dPrntBlkXStart;
	m_dPrntBlkYStart = pcDialogImageSize->m_dPrntBlkYStart;
	m_dPrntBlkXSize = pcDialogImageSize->m_dPrntBlkXSize;
	m_dPrntBlkYSize = pcDialogImageSize->m_dPrntBlkYSize;
	m_dPrntBlkXSpace = pcDialogImageSize->m_dPrntBlkXSpace;
	m_dPrntBlkYSpace = pcDialogImageSize->m_dPrntBlkYSpace;
	delete(pcDialogImageSize);

	pcPrintDialog = new CPrintDialog(FALSE);
	pcPrintDialog->m_pd.hDevMode = theApp->GetDevMode();
	pcPrintDialog->m_pd.hDevNames = theApp->GetDevNames();
	ret = pcPrintDialog->DoModal();
	if(ret == IDOK){
		dcPrint.Attach(pcPrintDialog->m_pd.hDC);
		memset(&docInfo, 0, sizeof(DOCINFO));
		docInfo.cbSize = sizeof(DOCINFO);
		docInfo.lpszDocName = m_szTitle;
		dcPrint.StartDoc(&docInfo);
		PrintOutMain(&dcPrint);		
		dcPrint.EndDoc();
	}
}

void CDialogImageDisp::OnBtnjpgout() 
{
	CDialogSetJpgElement *pcDialogSetJpgElement;

	pcDialogSetJpgElement = new CDialogSetJpgElement(this);
	pcDialogSetJpgElement->m_pcView = m_pcView;
	pcDialogSetJpgElement->m_pcDoc = m_pcDoc;
	pcDialogSetJpgElement->m_nInSort = m_nInSort;
	pcDialogSetJpgElement->DoModal();
	delete(pcDialogSetJpgElement);
}

void CDialogImageDisp::OnBtnexit() 
{
	m_nExitCall = 0;
	CDialog::OnOK();
}

void CDialogImageDisp::InitDataMax()
{
	POSITION pos;
	CCrctWaku *pcCrctWaku;

	m_nDataMax = 0;
	if (m_nInSort == 0) {
		m_pcDoc->m_pcListCW = &(m_pcDoc->m_lstCWSort);
	}
	else {
		m_pcDoc->m_pcListCW = &(m_pcDoc->m_lstCWRead);
	}
	pos = m_pcDoc->m_pcListCW->GetHeadPosition();
	while (pos != NULL)
	{
		pcCrctWaku = m_pcDoc->m_pcListCW->GetNext(pos);
		if(pcCrctWaku->m_csFlag == "1"){
			m_nDataMax++;
		}
	}
}

void CDialogImageDisp::PrintOutMain(CDC *pcDC)
{
	POSITION pos;
	CCrctWaku *pcCrctWaku;
	int		titlesize, textsize;
	CFont	cFont1, cFont2;
	CFont	*pcOldFont;
	int		sx, sy, ex, ey;
	CRect	rect;
	int		no, idx;
	CString	csStr;
	int		page;
	DBL		xsize, ysize;
	DBL		xtime, ytime, trnstime;
	int		nPageFirst;

	InitPrintEnv(pcDC);

	titlesize = (int)(5.0 * m_dPrntTime);
	textsize = (int)(m_dPrntBlkYSpace * m_dPrntTime * 0.7);
	if(textsize > (int)(5.0 * m_dPrntTime)){
		textsize = (int)(5.0 * m_dPrntTime);
	}
	cFont1.CreateFont(titlesize, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, _T("ＭＳ 明朝"));
	cFont2.CreateFont(textsize, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, _T("ＭＳ 明朝"));
	pcOldFont = pcDC->SelectObject(&cFont2);
	page = 1;
	nPageFirst = TRUE;
	no = 0;
	idx = 0;
	InitChangeElement();

	if (m_nInSort == 0) {
		m_pcDoc->m_pcListCW = &(m_pcDoc->m_lstCWSort);
	}
	else {
		m_pcDoc->m_pcListCW = &(m_pcDoc->m_lstCWRead);
	}
	pos = m_pcDoc->m_pcListCW->GetHeadPosition();
	while (pos != NULL)
	{
		pcCrctWaku = m_pcDoc->m_pcListCW->GetNext(pos);
		if(pcCrctWaku->m_csFlag == _T("1")){
			if(nPageFirst == TRUE){
				nPageFirst = FALSE;
				pcDC->StartPage();
				sx = (int)(m_dPrntBlkXStart * m_dPrntTime);
				sy = (int)(m_dPrntBlkYStart * m_dPrntTime);
				if(m_szTitle[0] != _T('\0')){
					pcDC->SelectObject(&cFont1);
					csStr.Format(_T("%s (Page%d)"), m_szTitle, page);
					pcDC->TextOut(sx, sy, csStr);
					pcDC->SelectObject(&cFont2);
					sy += titlesize;
				}
			}
			m_pcDoc->DataAllCls();
			m_pcDoc->WakuLoad(pcCrctWaku->m_csFileName.GetBuffer(FNAMEMAX));
			m_pcDoc->OnFlag(ACTIVEFLAG);
			SetChangeElement();
			m_pcView->AllWakuSetString(pcCrctWaku->m_csStrings.GetBuffer(FNAMEMAX));
			rect.left = sx;
			rect.top = sy;
			rect.right = rect.left + (int)(m_dPrntBlkXSize * m_dPrntTime);
			rect.bottom = rect.top + (int)(m_dPrntBlkYSize * m_dPrntTime);
			if(m_nNoWakuOut == 0){
				senvsetdrawmode(PREVIEW);
			}
			else{
				senvsetdrawmode(PRINTVIEW);
			}
			m_pcView->DrawWakuInRect(pcDC, rect);
			if(m_nNoWakuOut == 0){
				csStr.Format(_T("No%d (%03d)"), no+1, idx+1);
				pcDC->TextOut(rect.left, rect.bottom, csStr);
				scrngetmmsize(&xsize, &ysize);
				xtime = (m_dPrntBlkXSize / xsize);
				ytime = (m_dPrntBlkYSize / ysize);
				if(xtime < ytime){
					trnstime = xtime;
				}
				else{
					trnstime = ytime;
				}
				xsize *= trnstime;
				ysize *= trnstime;
				rect.right = rect.left + (int)(xsize * m_dPrntTime);
				rect.bottom = rect.top + (int)(ysize * m_dPrntTime);
				DrawBoxs(pcDC, rect);
			}
			sx = sx + (int)((m_dPrntBlkXSize + m_dPrntBlkXSpace) * m_dPrntTime);
			ex = sx + (int)(m_dPrntBlkXSize * m_dPrntTime);
			if(ex > m_nPrntXSize){
				sx = (int)(m_dPrntBlkXStart * m_dPrntTime);
				sy = sy + (int)((m_dPrntBlkYSize + m_dPrntBlkYSpace) * m_dPrntTime);
				ey = sy + (int)(m_dPrntBlkYSize * m_dPrntTime);
				if(ey > m_nPrntYSize){
					pcDC->EndPage();
					page++;
					nPageFirst = TRUE;
				}
			}
			no++;
		}
		idx++;
	}
	pcDC->SelectObject(pcOldFont);
	cFont1.DeleteObject();
	cFont2.DeleteObject();
	if(nPageFirst == FALSE){
		pcDC->EndPage();
	}
}

void CDialogImageDisp::InitPrintEnv(CDC *pcDC)
{
	DBL		dPrnMMXSize, dPrnMMYSize;
	int		dpix, dpiy;

	dPrnMMXSize = pcDC->GetDeviceCaps(HORZSIZE);
	dPrnMMYSize = pcDC->GetDeviceCaps(VERTSIZE);
	dpix = pcDC->GetDeviceCaps(LOGPIXELSX);
	dpiy = pcDC->GetDeviceCaps(LOGPIXELSY);
	m_dPrntTime = dpix / 25.4;
	m_nPrntXSize = dPrnMMXSize * m_dPrntTime;
	m_nPrntYSize = dPrnMMYSize * m_dPrntTime;
}

void CDialogImageDisp::DrawBoxs(CDC *pcDC, CRect rect)
{
	CPen	chPen;
	CPen	*pcOldPen;
	int		sx, sy, ex, ey;

	sx = rect.left;
	sy = rect.top;
	ex = rect.right;
	ey = rect.bottom;
	chPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pcOldPen = pcDC->SelectObject(&chPen);
	pcDC->MoveTo(sx, sy);
	pcDC->LineTo(sx, ey);
	pcDC->LineTo(ex, ey);
	pcDC->LineTo(ex, sy);
	pcDC->LineTo(sx, sy);
	pcDC->SelectObject(pcOldPen);
	chPen.DeleteObject();
}

void CDialogImageDisp::SetChangeElement()
{
	if(m_szFont[0] != '\0'){
		m_pcView->ChangeFont(m_szFont);
	}
	if(m_clrSlct != -1){
		m_pcDoc->SetOutLineClrFlagOn(0);
		m_pcDoc->SetFillClrFlagOn(0);
	}
}

void CDialogImageDisp::InitChangeElement()
{
	int		r, g, b;
	int		penno;

	if(m_clrSlct != -1){
		r = GetRValue(m_clrSlct); 
		g = GetGValue(m_clrSlct); 
		b = GetBValue(m_clrSlct);
		penno = senvgetpennoaddname(r, g, b);
		senvsetcrtfillpenno(penno);
		senvsetcrtlinepenno(penno);
	}
}
