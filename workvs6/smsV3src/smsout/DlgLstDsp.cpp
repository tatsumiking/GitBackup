// DlgLstDsp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgLstDsp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogListDisp ダイアログ
extern "C" {
	LPSTR basedivstrg(LPSTR sp, LPSTR retsp, char far sparator);
	LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR retstr);
	void senvgetexepath(LPSTR path);
	int	senvsetdrawmode(int mode);
}


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
	int		i;
	char	szText[RECMAX8];
	char	szStr[RECMAX];
	LPSTR	sp;
	int		len;

	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	m_pcRdsOutCrct->MoveFirst();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(m_pcRdsOutCrct->m_column8 == "1"){
			strcpy(szText, m_pcRdsOutCrct->m_column5.GetBuffer(RECMAX8));
			sp = &(szText[1]);
			for(i = 0; i < CNTTBLMAX; i++){
				if(*sp == '\0'){
					break;
				}
				sp = basedivstrg(sp, szStr, '/');
				len = strlen(szStr);
				if(CNTMINI > len){
					len = CNTMINI;
				}
				if(m_nCntTbl[i] < len){
					m_nCntTbl[i] = len;
				}
			}
			m_lstRecText.AddString(szText);
		}
		m_pcRdsOutCrct->MoveNext();
	}
DBLOADEND:
	EndCrctDB();
}

void CDialogListDisp::InitCrctDB()
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

void CDialogListDisp::EndCrctDB()
{
	m_pcRdsOutCrct->Close();
	delete(m_pcRdsOutCrct);
	m_dbOutcrct.Close();
}

void CDialogListDisp::PrintOutMain(CDC *pcDC)
{
	int		textsize;
	CFont	cFont;
	CFont	*pcOldFont;
	int		sx, sy, ex, ey;
	int		i;
	char	szText[RECMAX8];
	char	szStr[RECMAX];
	LPSTR	sp;
	int		no;
	CString	csStr;
	int		page;
	int		nPageFirst;

	InitPrintEnv(pcDC);
	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	textsize = (int)(5.0 * m_dPrntTime);
	cFont.CreateFont(textsize, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, "ＭＳ 明朝");
	pcOldFont = pcDC->SelectObject(&cFont);
	m_pcRdsOutCrct->MoveFirst();
	page = 1;
	nPageFirst = TRUE;
	no = 1;
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(nPageFirst == TRUE){
			nPageFirst = FALSE;
			pcDC->StartPage();
			sx = 0;
			sy = 0;
			if(m_szTitle[0] != '\0'){
				csStr.Format("%s (Page%d)", m_szTitle, page);
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
				csStr.Format(" 項目%d", i+1);
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
		if(m_pcRdsOutCrct->m_column8 == "1"){
			sx = 0;
			ey = sy + textsize;
			pcDC->MoveTo(sx, sy);
			pcDC->LineTo(ex, sy);
			pcDC->MoveTo(sx, sy);
			pcDC->LineTo(sx, ey);
			csStr.Format("%03d", no);
			pcDC->TextOut(sx+2, sy+2, csStr);
			sx += (NOCLMSIZE * textsize);
			strcpy(szText, m_pcRdsOutCrct->m_column5.GetBuffer(RECMAX8));
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
				sp = basedivstrg(sp, szStr, '/');
				csStr = _T(szStr);
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
		m_pcRdsOutCrct->MoveNext();
	}
	if(nPageFirst == FALSE){
		sx = 0;
		pcDC->MoveTo(sx, ey);
		pcDC->LineTo(ex, ey);
		pcDC->EndPage();
	}
	pcDC->SelectObject(pcOldFont);
	cFont.DeleteObject();
DBLOADEND:
	EndCrctDB();
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

void CDialogListDisp::InitSetDB()
{
	char	szExePath[FNAMEMAX];
	char	srcfile[FNAMEMAX];
	char	dstfile[FNAMEMAX];
	char	szCrctFName[FNAMEMAX];
	FILE	*fp;
	char	rbuf[RECMAX8];
	char	buf[RECMAX8];
	LPSTR	sp;
	int		idx;

	senvgetexepath(szExePath);

	sprintf(srcfile, "%s\\orgcrect.mdb", szExePath);
	sprintf(dstfile, "%s\\outcrect.mdb", szExePath);
	CopyFile(srcfile, dstfile, FALSE);

	sprintf(szCrctFName, "%s\\outcrect.dat", szExePath);
	fp = fopen(szCrctFName, "r");
	if(fp == NULL){
		return;
	}

	m_nInSort = 1;
	InitCrctDB();
	m_nInSort = 0;
	idx = 0;
	while(1){
		if(fgets(rbuf, RECMAX8, fp) == NULL){
			break;
		}
		sp = rbuf;
		m_pcRdsOutCrct->AddNew();
		sp = basecsvstrdivcpy(sp, buf);	// "未"or"済"
		m_pcRdsOutCrct->m_column6 = _T(buf);
		if(m_pcRdsOutCrct->m_column6 == "未"){
			m_pcRdsOutCrct->m_column8 = _T("1");
		}
		else{	// 済
			m_pcRdsOutCrct->m_column8 = _T("0");
		}
		sp = basecsvstrdivcpy(sp, buf);	// 枠グループ名
		m_pcRdsOutCrct->m_column2 = _T(buf);
		sp = basecsvstrdivcpy(sp, buf); // 用紙名
		m_pcRdsOutCrct->m_column3 = _T(buf);
		sp = basecsvstrdivcpy(sp, buf); // 枠フルパス名
		m_pcRdsOutCrct->m_column4 = _T(buf);
		sp = basecsvstrdivcpy(sp, buf); // 枚数
		m_pcRdsOutCrct->m_column7 = _T(buf);
		sp = basecsvstrdivcpy(sp, buf); // 枠文字列
		m_pcRdsOutCrct->m_column5 = _T(buf);
		m_pcRdsOutCrct->Update();
		idx++;
	}
	EndCrctDB();
	fclose(fp);
}
