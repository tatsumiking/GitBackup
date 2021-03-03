// DlgImgDsp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgImgDsp.h"
#include "DlgImgSize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogImageDisp ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	int	senvsetdrawmode(int mode);
	int	senvcheckdrawmode();
	int senvgetpennoaddname(int rval, int gval, int bval);
	void senvsetcrtfillpenno(int penno);
	void senvsetcrtlinepenno(int penno);
	void scrngetmmsize(DBL *pdmmxsize, DBL *pdmmysize);
}

CDialogImageDisp::CDialogImageDisp(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogImageDisp::IDD, pParent)
{
	LoadImageElement();
	m_nCrtTop = 0;
	m_nExitCall = 0;
	m_clrSlct = 0xffffffff;
	m_szFont[0] = '\0';
	m_pcView = (CTonView*)pParent;
	m_pcDoc = m_pcView->GetDocument();
	m_nNoWakuOut = 0;
	//{{AFX_DATA_INIT(CDialogImageDisp)
	//}}AFX_DATA_INIT
}

CDialogImageDisp::~CDialogImageDisp()
{
	char	szExePath[FNAMEMAX];
	char	szFName[FNAMEMAX];
	FILE	*fp;

	senvgetexepath(szExePath);
    sprintf(szFName, "%s\\imgprnt.env", szExePath);
	fp = fopen(szFName, "w");
	if(fp == NULL){
		return;
	}
	fprintf(fp, "%d\n", m_nNoWakuOut);
	fprintf(fp, "%1.1lf\n", m_dPrntBlkXStart);
	fprintf(fp, "%1.1lf\n", m_dPrntBlkYStart);
	fprintf(fp, "%1.1lf\n", m_dPrntBlkXSize);
	fprintf(fp, "%1.1lf\n", m_dPrntBlkYSize);
	fprintf(fp, "%1.1lf\n", m_dPrntBlkXSpace);
	fprintf(fp, "%1.1lf\n", m_dPrntBlkYSpace);
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
	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}

	textsize = (int)(m_dDispBlkYSpace * m_dDispTime * 0.7);
	if(textsize > (int)(5.0 * m_dDispTime)){
		textsize = (int)(5.0 * m_dDispTime);
	}
	cFont.CreateFont(textsize, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, "ＭＳ 明朝");
	pcOldFont = dc.SelectObject(&cFont);

	m_pcRdsOutCrct->MoveFirst();
	idx = 0;
	no = 0;
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(no == m_nCrtTop){
			break;
		}
		if(m_pcRdsOutCrct->m_column8 == "1"){
			no++;
		}
		idx++;
		m_pcRdsOutCrct->MoveNext();
	}

	sx = imgrect.left;
	sy = imgrect.top;
	if(m_szTitle[0] != '\0'){
		csStr = _T(m_szTitle);
		dc.TextOut(sx, sy, csStr);
		sy += textsize;
	}
	InitChangeElement();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(m_pcRdsOutCrct->m_column8 == "1"){
			m_pcDoc->DataAllCls();
			m_pcDoc->WakuLoad(m_pcRdsOutCrct->m_column4.GetBuffer(FNAMEMAX));
			m_pcDoc->OnFlag(ACTIVEFLAG);
			SetChangeElement();
			m_pcView->AllWakuSetString(m_pcRdsOutCrct->m_column5.GetBuffer(FNAMEMAX));
			rect.left = sx;
			rect.top = sy;
			rect.right = rect.left + (int)(m_dDispBlkXSize * m_dDispTime);
			rect.bottom = rect.top + (int)(m_dDispBlkYSize * m_dDispTime);
			m_pcView->DrawWakuInRect(&dc, rect);
			csStr.Format("No%d (%03d)", no+1, idx+1);
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
		m_pcRdsOutCrct->MoveNext();
		idx++;
	}		

	dc.SelectObject(pcOldFont);
	cFont.DeleteObject();

DBLOADEND:
	EndCrctDB();
}

void CDialogImageDisp::LoadImageElement()
{
	char	szExePath[FNAMEMAX];
	char	szFName[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];

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
    sprintf(szFName, "%s\\imgprnt.env", szExePath);
	fp = fopen(szFName, "r");
	if(fp == NULL){
		return;
	}
	fgets(buf, RECMAX, fp);
	m_nNoWakuOut = atoi(buf);
	fgets(buf, RECMAX, fp);
	m_dPrntBlkXStart = atof(buf);
	fgets(buf, RECMAX, fp);
	m_dPrntBlkYStart = atof(buf);
	fgets(buf, RECMAX, fp);
	m_dPrntBlkXSize = atof(buf);
	fgets(buf, RECMAX, fp);
	m_dPrntBlkYSize = atof(buf);
	fgets(buf, RECMAX, fp);
	m_dPrntBlkXSpace = atof(buf);
	fgets(buf, RECMAX, fp);
	m_dPrntBlkYSpace = atof(buf);
	fclose(fp);
}

void CDialogImageDisp::InitCrctDB()
{
	char	szExePath[FNAMEMAX];
	CString	csDBName;
	CString	csSQL;

	senvgetexepath(szExePath);
	csDBName.Format("%s\\outcrect.mdb", szExePath);
	if(m_nInSort == 0){
		csSQL.Format("select * from [ソートクエリー]");
	}
	else{
		csSQL.Format("select * from [出力テーブル]");
	}
	m_dbOutcrct.Open(csDBName, FALSE, FALSE);
	m_pcRdsOutCrct = new CRdsOutCrct(&m_dbOutcrct);
    m_pcRdsOutCrct->Open(dbOpenDynaset, csSQL, dbDenyWrite);
}

void CDialogImageDisp::EndCrctDB()
{
	m_pcRdsOutCrct->Close();
	delete(m_pcRdsOutCrct);
	m_dbOutcrct.Close();
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

void CDialogImageDisp::OnBtnexit() 
{
	m_nExitCall = 0;
	CDialog::OnOK();
}

void CDialogImageDisp::InitDataMax()
{
	InitCrctDB();
	m_nDataMax = 0;
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	m_pcRdsOutCrct->MoveFirst();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(m_pcRdsOutCrct->m_column8 == "1"){
			m_nDataMax++;
		}
		m_pcRdsOutCrct->MoveNext();
	}
DBLOADEND:
	EndCrctDB();
}

void CDialogImageDisp::PrintOutMain(CDC *pcDC)
{
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
	InitCrctDB();
	//m_nDataMax = 0;
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	titlesize = (int)(5.0 * m_dPrntTime);
	textsize = (int)(m_dPrntBlkYSpace * m_dPrntTime * 0.7);
	if(textsize > (int)(5.0 * m_dPrntTime)){
		textsize = (int)(5.0 * m_dPrntTime);
	}
	cFont1.CreateFont(titlesize, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, "ＭＳ 明朝");
	cFont2.CreateFont(textsize, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, "ＭＳ 明朝");
	pcOldFont = pcDC->SelectObject(&cFont2);
	m_pcRdsOutCrct->MoveFirst();
	page = 1;
	nPageFirst = TRUE;
	no = 0;
	idx = 0;
	InitChangeElement();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(m_pcRdsOutCrct->m_column8 == "1"){
			if(nPageFirst == TRUE){
				nPageFirst = FALSE;
				pcDC->StartPage();
				sx = (int)(m_dPrntBlkXStart * m_dPrntTime);
				sy = (int)(m_dPrntBlkYStart * m_dPrntTime);
				if(m_szTitle[0] != '\0'){
					pcDC->SelectObject(&cFont1);
					csStr.Format("%s (Page%d)", m_szTitle, page);
					pcDC->TextOut(sx, sy, csStr);
					pcDC->SelectObject(&cFont2);
					sy += titlesize;
				}
			}
			m_pcDoc->DataAllCls();
			m_pcDoc->WakuLoad(m_pcRdsOutCrct->m_column4.GetBuffer(FNAMEMAX));
			m_pcDoc->OnFlag(ACTIVEFLAG);
			SetChangeElement();
			m_pcView->AllWakuSetString(m_pcRdsOutCrct->m_column5.GetBuffer(FNAMEMAX));
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
				csStr.Format("No%d (%03d)", no+1, idx+1);
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
		m_pcRdsOutCrct->MoveNext();
		idx++;
	}
	pcDC->SelectObject(pcOldFont);
	cFont1.DeleteObject();
	cFont2.DeleteObject();
	if(nPageFirst == FALSE){
		pcDC->EndPage();
	}
DBLOADEND:
	EndCrctDB();
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
