// DlgLstDsp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "CrctWaku.h"
#include "DialogListDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogListDisp ダイアログ

//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Senvlib.h"
//}

CDialogListDisp::CDialogListDisp(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogListDisp::IDD, pParent)
{
	int		i;

	m_nInSort = 1;
	m_szTitle[0] = '\0';
	for(i = 0; i < CNTTBLMAX; i++){
		m_nCntTbl[i] = 0;
	}
	//{{AFX_DATA_INIT(CDialogListDisp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CDialogListDisp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogListDisp)
	DDX_Control(pDX, IDC_LSTRECTEXT, m_lstRecText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogListDisp, CDialog)
	//{{AFX_MSG_MAP(CDialogListDisp)
	ON_BN_CLICKED(IDC_BTNIMAGEDISP, OnBtnimagedisp)
	ON_BN_CLICKED(IDC_BTNPRINTOUT, OnBtnprintout)
	ON_BN_CLICKED(IDC_BTNEXIT, OnBtnexit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogListDisp メッセージ ハンドラ

void CDialogListDisp::OnBtnimagedisp() 
{
	m_nExitCall = 1;
	CDialog::OnOK();
}

void CDialogListDisp::OnBtnprintout() 
{
	CPrintDialog		*pcPrintDialog;
	CDC		dcPrint;
	DOCINFO docInfo;
	int		ret;

	CTonApp *theApp = (CTonApp*)AfxGetApp();
	senvsetdrawmode(PREVIEW);
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

void CDialogListDisp::OnBtnexit() 
{
	m_nExitCall = 0;
	CDialog::OnOK();
}

BOOL CDialogListDisp::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitRecTextList();

	return TRUE;
}

void CDialogListDisp::InitRecTextList()
{
	POSITION pos;
	CCrctWaku *pcCrctWaku;
	int		i;
	TCHAR	szText[RECMAX8];
	TCHAR	szStr[RECMAX];
	LPTSTR	sp;
	int		len;

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
			_tcscpy_s(szText, pcCrctWaku->m_csStrings.GetBuffer(RECMAX8));
			sp = &(szText[1]);
			for(i = 0; i < CNTTBLMAX; i++){
				if(*sp == '\0'){
					break;
				}
				sp = basedivstrg(sp, szStr, '/');
				len = _tcslen(szStr);
				if(CNTMINI > len){
					len = CNTMINI;
				}
				if(m_nCntTbl[i] < len){
					m_nCntTbl[i] = len;
				}
			}
			m_lstRecText.AddString(szText);
		}
	}
}

void CDialogListDisp::PrintOutMain(CDC *pcDC)
{
	POSITION pos;
	CCrctWaku *pcCrctWaku;
	int		textsize;
	CFont	cFont;
	CFont	*pcOldFont;
	int		sx, sy, ex, ey;
	int		i;
	TCHAR	szText[RECMAX8];
	TCHAR	szStr[RECMAX];
	LPTSTR	sp;
	int		no;
	CString	csStr;
	int		page;
	int		nPageFirst;

	InitPrintEnv(pcDC);

	textsize = (int)(5.0 * m_dPrntTime);
	cFont.CreateFont(textsize, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, _T("ＭＳ 明朝"));
	pcOldFont = pcDC->SelectObject(&cFont);

	page = 1;
	nPageFirst = TRUE;
	no = 1;
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
		if(nPageFirst == TRUE){
			nPageFirst = FALSE;
			pcDC->StartPage();
			sx = 0;
			sy = 0;
			if(m_szTitle[0] != '\0'){
				csStr.Format(_T("%s (Page%d)"), m_szTitle, page);
				pcDC->TextOut(sx+2, sy+2, csStr);
				sy += textsize;
			}
			ey = sy + textsize;
			pcDC->MoveTo(sx, sy);
			pcDC->LineTo(sx, ey);
			csStr = _T(" No ");
			pcDC->TextOut(sx+2, sy+2, csStr);
			sx += (NOCLMSIZE * textsize);
			for(i = 0; i < CNTTBLMAX; i++){
				if(m_nCntTbl[i] == 0){
					break;
				}
				if(sx > m_nPrntXSize){
					break;
				}
				csStr.Format(_T(" 項目%d"), i+1);
				pcDC->TextOut(sx+2, sy+2, csStr);
				pcDC->MoveTo(sx, sy);
				pcDC->LineTo(sx, ey);
				sx += (m_nCntTbl[i]/2+1)*textsize;	
			}
			pcDC->MoveTo(sx, sy);
			pcDC->LineTo(sx, ey);
			ex = sx;
			sx = 0;
			pcDC->MoveTo(sx, sy);
			pcDC->LineTo(ex, sy);
			sy = ey;
		}
		if(pcCrctWaku->m_csFlag == _T("1")){
			sx = 0;
			ey = sy + textsize;
			pcDC->MoveTo(sx, sy);
			pcDC->LineTo(ex, sy);
			pcDC->MoveTo(sx, sy);
			pcDC->LineTo(sx, ey);
			csStr.Format(_T("%03d"), no);
			pcDC->TextOut(sx+2, sy+2, csStr);
			sx += (NOCLMSIZE * textsize);
			_tcscpy_s(szText, pcCrctWaku->m_csStrings.GetBuffer(RECMAX8));
			sp = &(szText[1]);
			for(i = 0; i < CNTTBLMAX; i++){
				if(m_nCntTbl[i] == 0){
					break;
				}
				if(sx > m_nPrntXSize){
					break;
				}
				pcDC->MoveTo(sx, sy);
				pcDC->LineTo(sx, ey);
				sp = basedivstrg(sp, szStr, _T('/'));
				csStr = szStr;
				pcDC->TextOut(sx+2, sy+2, csStr);
				sx += (m_nCntTbl[i]/2+1)*textsize;	
			}
			pcDC->MoveTo(sx, sy);
			pcDC->LineTo(sx, ey);
			sy = ey;

			if((sy + textsize) > m_nPrntYSize){
				sx = 0;
				pcDC->MoveTo(sx, ey);
				pcDC->LineTo(ex, ey);
				pcDC->EndPage();
				page++;
				nPageFirst = TRUE;
			}
			no++;
		}
	}
	if(nPageFirst == FALSE){
		sx = 0;
		pcDC->MoveTo(sx, ey);
		pcDC->LineTo(ex, ey);
		pcDC->EndPage();
	}
	pcDC->SelectObject(pcOldFont);
	cFont.DeleteObject();
}
void CDialogListDisp::InitPrintEnv(CDC *pcDC)
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
