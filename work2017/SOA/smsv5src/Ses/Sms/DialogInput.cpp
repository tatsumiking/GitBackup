// DlgInput.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "io.h"
#include "DialogInput.h"
#include "DialogWakuSlct.h"
#include "DialogListDisp.h"
#include "DialogImageDisp.h"
#include "DialogGaijiSlct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogInput ダイアログ

//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Senvlib.h"
	#include "../../ObjLib/Wakuenv.h"
//}

#define	TIMERINTERVAL	100
static TCHAR BASED_CODE szFilter[] = _T("貯込みファイル (*.rec)|*.rec||");

CDialogInput::CDialogInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogInput::IDD, pParent)
{
	int		i;

	//{{AFX_DATA_INIT(CDialogInput)
	m_csPaperName = _T("");
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;
	m_pcDoc = m_pcView->GetDocument();
	m_pcCmdBase = new CCmdBase(m_pcView);
	m_nCrtTop = 1;
	for(i = 0; i < INWAKUMAX; i++){
		m_csWkno[i] = _T("");
		m_csWakuStr[i] = _T("");
	}
	for(i = 0; i < WAKUMAX; i++){
		m_szWakuText[i][0] = '\0';
	}
	m_szCrtWakuName[0] = '\0';
	m_nFirst = TRUE;
	m_nNewCrct = TRUE;
}

void CDialogInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogInput)
	DDX_Control(pDX, IDC_STIMAGE, m_stImage);
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_CMBPAPER, m_cmbPaper);
	DDX_Control(pDX, IDC_LSTWKSLCT, m_lstWkSlct);
	DDX_CBString(pDX, IDC_CMBPAPER, m_csPaperName);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STWKNO1, m_csWkno[0]);
	DDX_Text(pDX, IDC_STWKNO2, m_csWkno[1]);
	DDX_Text(pDX, IDC_STWKNO3, m_csWkno[2]);
	DDX_Text(pDX, IDC_STWKNO4, m_csWkno[3]);
	DDX_Text(pDX, IDC_STWKNO5, m_csWkno[4]);
	DDX_Text(pDX, IDC_STWKNO6, m_csWkno[5]);
	DDX_Text(pDX, IDC_STWKNO7, m_csWkno[6]);
	DDX_Text(pDX, IDC_STWKNO8, m_csWkno[7]);
	DDX_Text(pDX, IDC_STWKNO9, m_csWkno[8]);
	DDX_Text(pDX, IDC_STWKNO10, m_csWkno[9]);
	DDX_Text(pDX, IDC_EDWAKUSTR1, m_csWakuStr[0]);
	DDX_Text(pDX, IDC_EDWAKUSTR2, m_csWakuStr[1]);
	DDX_Text(pDX, IDC_EDWAKUSTR3, m_csWakuStr[2]);
	DDX_Text(pDX, IDC_EDWAKUSTR4, m_csWakuStr[3]);
	DDX_Text(pDX, IDC_EDWAKUSTR5, m_csWakuStr[4]);
	DDX_Text(pDX, IDC_EDWAKUSTR6, m_csWakuStr[5]);
	DDX_Text(pDX, IDC_EDWAKUSTR7, m_csWakuStr[6]);
	DDX_Text(pDX, IDC_EDWAKUSTR8, m_csWakuStr[7]);
	DDX_Text(pDX, IDC_EDWAKUSTR9, m_csWakuStr[8]);
	DDX_Text(pDX, IDC_EDWAKUSTR10, m_csWakuStr[9]);
	DDX_Control(pDX, IDC_EDWAKUSTR1, m_edWakuStr[0]);
	DDX_Control(pDX, IDC_EDWAKUSTR2, m_edWakuStr[1]);
	DDX_Control(pDX, IDC_EDWAKUSTR3, m_edWakuStr[2]);
	DDX_Control(pDX, IDC_EDWAKUSTR4, m_edWakuStr[3]);
	DDX_Control(pDX, IDC_EDWAKUSTR5, m_edWakuStr[4]);
	DDX_Control(pDX, IDC_EDWAKUSTR6, m_edWakuStr[5]);
	DDX_Control(pDX, IDC_EDWAKUSTR7, m_edWakuStr[6]);
	DDX_Control(pDX, IDC_EDWAKUSTR8, m_edWakuStr[7]);
	DDX_Control(pDX, IDC_EDWAKUSTR9, m_edWakuStr[8]);
	DDX_Control(pDX, IDC_EDWAKUSTR10, m_edWakuStr[9]);
}


BEGIN_MESSAGE_MAP(CDialogInput, CDialog)
	//{{AFX_MSG_MAP(CDialogInput)
	ON_BN_CLICKED(IDC_BTNCMD1, OnBtncmd1)
	ON_BN_CLICKED(IDC_BTNCMD2, OnBtncmd2)
	ON_BN_CLICKED(IDC_BTNCMD3, OnBtncmd3)
	ON_BN_CLICKED(IDC_BTNCMD4, OnBtncmd4)
	ON_BN_CLICKED(IDC_BTNCMD5, OnBtncmd5)
	ON_BN_CLICKED(IDC_BTNCMD6, OnBtncmd6)
	ON_BN_CLICKED(IDC_BTNCMD7, OnBtncmd7)
	ON_BN_CLICKED(IDC_BTNCMD8, OnBtncmd8)
	ON_BN_CLICKED(IDC_BTNCMD9, OnBtncmd9)
	ON_CBN_SELCHANGE(IDC_CMBPAPER, OnSelchangeCmbpaper)
	ON_LBN_SELCHANGE(IDC_LSTWKSLCT, OnSelchangeLstwkslct)
	ON_EN_SETFOCUS(IDC_EDWAKUSTR1, OnSetfocusEdwakustr1)
	ON_EN_SETFOCUS(IDC_EDWAKUSTR2, OnSetfocusEdwakustr2)
	ON_EN_SETFOCUS(IDC_EDWAKUSTR3, OnSetfocusEdwakustr3)
	ON_EN_SETFOCUS(IDC_EDWAKUSTR4, OnSetfocusEdwakustr4)
	ON_EN_SETFOCUS(IDC_EDWAKUSTR5, OnSetfocusEdwakustr5)
	ON_EN_SETFOCUS(IDC_EDWAKUSTR6, OnSetfocusEdwakustr6)
	ON_EN_SETFOCUS(IDC_EDWAKUSTR7, OnSetfocusEdwakustr7)
	ON_EN_SETFOCUS(IDC_EDWAKUSTR8, OnSetfocusEdwakustr8)
	ON_EN_SETFOCUS(IDC_EDWAKUSTR9, OnSetfocusEdwakustr9)
	ON_EN_SETFOCUS(IDC_EDWAKUSTR10, OnSetfocusEdwakustr10)
	ON_EN_KILLFOCUS(IDC_EDWAKUSTR1, OnKillfocusEdwakustr1)
	ON_EN_KILLFOCUS(IDC_EDWAKUSTR2, OnKillfocusEdwakustr2)
	ON_EN_KILLFOCUS(IDC_EDWAKUSTR3, OnKillfocusEdwakustr3)
	ON_EN_KILLFOCUS(IDC_EDWAKUSTR4, OnKillfocusEdwakustr4)
	ON_EN_KILLFOCUS(IDC_EDWAKUSTR5, OnKillfocusEdwakustr5)
	ON_EN_KILLFOCUS(IDC_EDWAKUSTR6, OnKillfocusEdwakustr6)
	ON_EN_KILLFOCUS(IDC_EDWAKUSTR7, OnKillfocusEdwakustr7)
	ON_EN_KILLFOCUS(IDC_EDWAKUSTR8, OnKillfocusEdwakustr8)
	ON_EN_KILLFOCUS(IDC_EDWAKUSTR9, OnKillfocusEdwakustr9)
	ON_EN_KILLFOCUS(IDC_EDWAKUSTR10, OnKillfocusEdwakustr10)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogInput メッセージ ハンドラ

void CDialogInput::OnBtncmd1() 
{
	int		samecnt;
	int		i;

	UpdateData(TRUE);
	ResetWakuString();
	GetLastWakuNo();
	samecnt = CheckSameWaku();
	if(samecnt == 0){
		m_szCrtWakuName[0] = _T('\0');
		::AfxMessageBox(_T("該当する枠が見つかりません"), MB_OK);
		return;
	}
	if(samecnt != 1){
		SlctCrtWaku();
	}
	if(m_szCrtWakuName[0] == _T('\0')){
		return;
	}
	m_pcDoc->DataAllCls();
	m_pcDoc->WakuLoad(m_szCrtWakuName);
	m_pcDoc->OnFlag(ACTIVEFLAG);
	AllWakuSetString();
	SaveCrctData();
	m_nNewCrct = TRUE;
	for(i = 0; i < WAKUMAX; i++){
		m_szWakuText[i][0] = _T('\0');
	}
	m_nCrtTop = 1;
	SetWakuString(m_nCrtTop);
	UpdateData(FALSE);
	Invalidate(TRUE);
	m_nSlctIdx = 0;
	m_edWakuStr[m_nSlctIdx].SetFocus();
}

void CDialogInput::OnBtncmd2() 
{
	TCHAR	szExePath[FNAMEMAX];	
	TCHAR	szCrctFName[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX8];
	TCHAR	lastbuf[RECMAX8];
	LPTSTR	sp;
	int		idx;

	lastbuf[0] = _T('\0');
	senvgetexepath(szExePath);
	_stprintf_s(szCrctFName, _T("%s\\outcrect.dat"), szExePath);
	_tfopen_s(&fp, szCrctFName, _T("r,ccs=UNICODE"));
	if (fp == NULL) {
		return;
	}
	while(1){
		if(_fgetts(buf, RECMAX8, fp) == NULL){
			break;
		}
		_tcscpy_s(lastbuf, buf);
	}
	fclose(fp);
	if(lastbuf[0] == _T('\0')){
		return;
	}
	sp = lastbuf;
	sp = basecsvstrdivcpy(sp, buf);	// "未"or"済"
	sp = basecsvstrdivcpy(sp, buf);	// 枠グループ名
	sp = basecsvstrdivcpy(sp, buf); // 用紙名
	sp = basecsvstrdivcpy(sp, buf); // 枠フルパス名
	sp = basecsvstrdivcpy(sp, buf); // 枚数
	sp = basecsvstrdivcpy(sp, buf); // 枠文字列
	_tcscpy_s(lastbuf, &(buf[1]));
	sp = lastbuf;
	idx = 0;
	while(1){
		if(*sp == _T('\0')){
			break;
		}
		sp = basedivstrg(sp, buf, _T('/'));
		if(idx == m_nCrtTop + m_nSlctIdx - 1){
			_tcscpy_s(m_szWakuText[idx], buf);
		}
		idx++;
	}
	SetWakuString(m_nCrtTop);
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogInput::OnBtncmd3() 
{
	TCHAR	szTitle[QRECMAX];
	TCHAR	szExePath[FNAMEMAX];	
	TCHAR	szFileName[FNAMEMAX];
	TCHAR	szCrctFName[FNAMEMAX];
	int		ret;
	FILE	*ofp;
	FILE	*ifp;
	TCHAR	buf[RECMAX8];

	_tcscpy_s(szTitle, _T("貯込み保存"));
	CFileDialog	*pcFileDialog = new CFileDialog(FALSE, NULL, _T(""), NULL, szFilter, m_pcView);
	(pcFileDialog->m_ofn).lpstrFileTitle = (LPTSTR)szTitle;
	if(pcFileDialog->DoModal( ) == IDOK){
		CString cpStr = pcFileDialog->GetPathName();
		_tcscpy_s(szFileName, cpStr.GetBuffer(FNAMEMAX));
		baseaddsafix(szFileName, _T("rec"));
		if(_taccess_s(szFileName, 0) == 0){
			ret = ::AfxMessageBox(_T("上書きしてよろしいですか"), MB_OKCANCEL);
			if(ret == IDCANCEL){
				return;
			}
		}

		senvgetexepath(szExePath);
		_stprintf_s(szCrctFName, _T("%s\\outcrect.dat"), szExePath);
		_tfopen_s(&ifp, szCrctFName, _T("r,ccs=UNICODE"));
		if (ifp == NULL) {
			::AfxMessageBox(_T("貯込んであるデータがありません"), MB_OK);
			return;
		}
		_tfopen_s(&ofp, szFileName, _T("w,ccs=UNICODE"));
		if (ofp == NULL) {
			fclose(ifp);
			::AfxMessageBox(_T("ファイルが作成できませんでした"), MB_OK);
			return;
		}
		while(1){
			if(_fgetts(buf, RECMAX8, ifp) == NULL){
				break;
			}
			_fputts(buf, ofp);
		}
		fclose(ofp);
		fclose(ifp);
	}
}

void CDialogInput::OnBtncmd4() 
{
	TCHAR	szTitle[QRECMAX];
	TCHAR	szExePath[FNAMEMAX];	
	TCHAR	szFileName[FNAMEMAX];
	TCHAR	szCrctFName[FNAMEMAX];
	FILE	*ofp;
	FILE	*ifp;
	TCHAR	buf[RECMAX8];

	_tcscpy_s(szTitle, _T("貯込み呼出"));
	CFileDialog	*pcFileDialog = new CFileDialog(TRUE, NULL, _T("*.rec"), NULL, szFilter, m_pcView);
	(pcFileDialog->m_ofn).lpstrFileTitle = (LPTSTR)szTitle;
	if(pcFileDialog->DoModal( ) == IDOK){
		CString cpStr = pcFileDialog->GetPathName();
		_tcscpy_s(szFileName, cpStr.GetBuffer(FNAMEMAX));
		baseaddsafix(szFileName, _T("rec"));
		senvgetexepath(szExePath);
		_stprintf_s(szCrctFName, _T("%s\\outcrect.dat"), szExePath);
		_tfopen_s(&ofp, szCrctFName, _T("w,ccs=UNICODE"));
		if (ofp == NULL) {
			::AfxMessageBox(_T("データが読込めませんでした"), MB_OK);
			return;
		}
		_tfopen_s(&ifp, szFileName, _T("r,ccs=UNICODE"));
		if (ifp == NULL) {
			fclose(ofp);
			::AfxMessageBox(_T("ファイルが開けません"), MB_OK);
			return;
		}
		while(1){
			if(_fgetts(buf, RECMAX8, ifp) == NULL){
				break;
			}
			_fputts(buf, ofp);
		}
		fclose(ofp);
		fclose(ifp);
	}
}

void CDialogInput::OnBtncmd5() 
{
	TCHAR	szExePath[FNAMEMAX];
	TCHAR	szFileName[FNAMEMAX];

	senvgetexepath(szExePath);
	_stprintf_s(szFileName, _T("%s\\kden.exe"), szExePath);
	ProgExec(szFileName);
	SetFocus();
}

// 一覧
void CDialogInput::OnBtncmd6() 
{
	int		nExitCall;
	CDialogListDisp	*pcDialogListDisp;

	nExitCall = 0;
	pcDialogListDisp = new CDialogListDisp(m_pcView);
	pcDialogListDisp->m_pcDoc = m_pcDoc;
	if(pcDialogListDisp->DoModal() == IDOK){
		nExitCall = pcDialogListDisp->m_nExitCall;
	}
	delete(pcDialogListDisp);
	if(nExitCall == 1){
		CallImageDisp();
	}
}

void CDialogInput::CallImageDisp()
{
	int		nExitCall;
	CDialogImageDisp	*pcDialogImageDisp;

	nExitCall = 0;
	pcDialogImageDisp = new CDialogImageDisp(m_pcView);
	pcDialogImageDisp->m_pcDoc = m_pcDoc;
	if(pcDialogImageDisp->DoModal() == IDOK){
		nExitCall = pcDialogImageDisp->m_nExitCall;
	}
	delete(pcDialogImageDisp);
	if(nExitCall == 1){
		OnBtncmd6();
	}
}

// 挿入
void CDialogInput::OnBtncmd7() 
{
	int		idx;

	UpdateData(TRUE);
	ResetWakuString();
	for(idx = WAKUMAX-1; idx >= 0; idx--){
		if(idx == m_nCrtTop + m_nSlctIdx - 1){
			_tcscpy_s(m_szWakuText[idx+1], m_szWakuText[idx]);
			m_szWakuText[idx][0] = '\0';
			break;
		}
		_tcscpy_s(m_szWakuText[idx+1], m_szWakuText[idx]);
	}
	SetWakuString(m_nCrtTop);
	m_edWakuStr[m_nSlctIdx].SetFocus();
	UpdateData(FALSE);
	Invalidate(TRUE);
}

// 外字
void CDialogInput::OnBtncmd8() 
{
	CDialogGaijiSlct *pcDialogGaijiSlct;
	TCHAR	szStr[RECMAX];

	UpdateData(TRUE);
	pcDialogGaijiSlct = new CDialogGaijiSlct(m_pcView);
	if(pcDialogGaijiSlct->DoModal() == IDOK){
		_tcscpy_s(szStr, m_csWakuStr[m_nSlctIdx].GetBuffer(RECMAX));
		m_csWakuStr[m_nSlctIdx].Format(_T("%s%s"), szStr, pcDialogGaijiSlct->m_szStr);
	}
	delete(pcDialogGaijiSlct);
	UpdateData(FALSE);
}

// 中断
void CDialogInput::OnBtncmd9() 
{
	m_pcDoc->DataAllCls();
	CDialog::OnOK();
}

BOOL CDialogInput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitLstWkSlct();
	InitCmbPaper();
	UpdateInWakuString(1);
	m_nSlctIdx = 0;
	UpdateData(FALSE);
	return TRUE;
}
void CDialogInput::ProgExec(LPTSTR lpExexProg)
{
	static STARTUPINFO siStartInfo;
	static PROCESS_INFORMATION piProcInfo;

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	pcWnd->ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
	pcWnd->EnableWindow(FALSE);
	EnableWindow(FALSE);
	if (_taccess_s(lpExexProg, 0) == 0) {
		GetStartupInfo(&siStartInfo);
		if (CreateProcess(NULL, lpExexProg, NULL, NULL,
			FALSE, 0,
			NULL, NULL, &siStartInfo,
			&piProcInfo) == 0) {
			return;
		}
		m_hProcess = piProcInfo.hProcess;
		ProgExitLoop();
		CloseHandle(piProcInfo.hThread);
		CloseHandle(piProcInfo.hProcess);
		m_hProcess = 0;
	}
	pcWnd->EnableWindow(TRUE);
	EnableWindow(TRUE);
	ShowWindow(SW_SHOW);
	pcWnd->ShowWindow(SW_SHOW);
}

void CDialogInput::ProgExitLoop()
{
	DWORD	dwExitCode;
	int		ret;

	while (1) {
		ret = GetExitCodeProcess(m_hProcess, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE) {
			break;
		}
		Sleep(TIMERINTERVAL);
		//Sleep(1000);
	}
}
void CDialogInput::InitLstWkSlct()
{
	int		i;
	TCHAR	numstr[QRECMAX];

	for(i = 1; i < WAKUMAX; i += 5){
		_stprintf_s(numstr, _T("%03d"), i);
		m_lstWkSlct.AddString(numstr);
	}
}

void CDialogInput::InitCmbPaper()
{
	int		idx;
	TCHAR	papername[QRECMAX];
	int		ret;

	m_cmbPaper.ResetContent();
	idx = 0;
	while(1){
		wakuenvlistgetpapername(idx, papername);
		if(papername[0] == _T('\0')){
			break;
		}
		m_cmbPaper.AddString(papername);
		idx++;
	}
	wakuenvgetpapername(papername);
	m_csPaperName = papername;
	ret = m_cmbPaper.FindStringExact(0, m_csPaperName);
	if(ret == CB_ERR){
		wakuenvlistgetpapername(0, papername);
		m_csPaperName = papername;
	}
	wakuenvsetpapername(papername);
	wakuenvgetpaperelement();
}

void CDialogInput::OnSelchangeCmbpaper() 
{
	int		idx;
	TCHAR	papername[QRECMAX];

	UpdateData(TRUE);
	idx = m_cmbPaper.GetCurSel();	
	m_cmbPaper.GetLBText(idx, papername);
	wakuenvsetpapername(papername);
	m_csPaperName = papername;
	UpdateData(FALSE);
}

void CDialogInput::OnSelchangeLstwkslct() 
{
	int		idx;
	TCHAR	numstr[QRECMAX];
	int		num;

	UpdateData(TRUE);
	idx = m_lstWkSlct.GetCurSel();
	m_lstWkSlct.GetText(idx, numstr);
	num = _ttoi(numstr);
	UpdateInWakuString(num);
	UpdateData(FALSE);
	Invalidate(TRUE);
	m_nSlctIdx = 0;
	m_edWakuStr[m_nSlctIdx].SetFocus();
}

void CDialogInput::UpdateInWakuString(int num)
{
	ResetWakuString();
	m_nCrtTop = num;
	SetWakuString(m_nCrtTop);
}

void CDialogInput::OnSetfocusEdwakustr1() 
{
	m_nSlctIdx = 0;
	SetFocusEdit();
}

void CDialogInput::OnSetfocusEdwakustr2() 
{
	m_nSlctIdx = 1;
	SetFocusEdit();
}

void CDialogInput::OnSetfocusEdwakustr3() 
{
	m_nSlctIdx = 2;
	SetFocusEdit();
}

void CDialogInput::OnSetfocusEdwakustr4() 
{
	m_nSlctIdx = 3;
	SetFocusEdit();
}

void CDialogInput::OnSetfocusEdwakustr5() 
{
	m_nSlctIdx = 4;
	SetFocusEdit();
}

void CDialogInput::OnSetfocusEdwakustr6() 
{
	m_nSlctIdx = 5;
	SetFocusEdit();
}

void CDialogInput::OnSetfocusEdwakustr7() 
{
	m_nSlctIdx = 6;
	SetFocusEdit();
}

void CDialogInput::OnSetfocusEdwakustr8() 
{
	m_nSlctIdx = 7;
	SetFocusEdit();
}

void CDialogInput::OnSetfocusEdwakustr9() 
{
	m_nSlctIdx = 8;
	SetFocusEdit();
}

void CDialogInput::OnSetfocusEdwakustr10() 
{
	m_nSlctIdx = 9;
	SetFocusEdit();
}

void CDialogInput::OnKillfocusEdwakustr1() 
{
	KillFocusEdit();
	//m_nSlctIdx = -1;
}

void CDialogInput::OnKillfocusEdwakustr2() 
{
	KillFocusEdit();
	//m_nSlctIdx = -1;
}

void CDialogInput::OnKillfocusEdwakustr3() 
{
	KillFocusEdit();
	//m_nSlctIdx = -1;
}

void CDialogInput::OnKillfocusEdwakustr4() 
{
	KillFocusEdit();
	//m_nSlctIdx = -1;
}

void CDialogInput::OnKillfocusEdwakustr5() 
{
	KillFocusEdit();
	//m_nSlctIdx = -1;
}

void CDialogInput::OnKillfocusEdwakustr6() 
{
	KillFocusEdit();
	//m_nSlctIdx = -1;
}

void CDialogInput::OnKillfocusEdwakustr7() 
{
	KillFocusEdit();
	//m_nSlctIdx = -1;
}

void CDialogInput::OnKillfocusEdwakustr8() 
{
	KillFocusEdit();
	//m_nSlctIdx = -1;
}

void CDialogInput::OnKillfocusEdwakustr9() 
{
	KillFocusEdit();
	//m_nSlctIdx = -1;
}

void CDialogInput::OnKillfocusEdwakustr10() 
{
	KillFocusEdit();
	//m_nSlctIdx = -1;
}

BOOL CDialogInput::PreTranslateMessage(MSG* pMsg) 
{
	int		crtidx;

	if(m_nNewCrct == TRUE){
		if(pMsg->message == WM_KEYDOWN){
			m_pcDoc->DataAllCls();
			m_nNewCrct = FALSE;
			Invalidate(TRUE);
		}
	}
	if(pMsg->message == WM_KEYDOWN
	|| pMsg->message == WM_SYSKEYDOWN){
		switch(pMsg->wParam){
		case VK_PRIOR:
			crtidx = LegalCrtTop(m_nCrtTop - INWAKUMAX / 2);
			UpdateInWakuString(crtidx);
			UpdateData(FALSE);
			Invalidate(TRUE);
			break;
		case VK_NEXT:
			crtidx = LegalCrtTop(m_nCrtTop + INWAKUMAX / 2);
			UpdateInWakuString(crtidx);
			UpdateData(FALSE);
			Invalidate(TRUE);
			break;
		case VK_UP:
			m_nSlctIdx--;
			if(m_nSlctIdx < 0){
				m_nSlctIdx = 0;
				crtidx = LegalCrtTop(m_nCrtTop - 1);
				UpdateInWakuString(crtidx);
				UpdateData(FALSE);
				Invalidate(TRUE);
			}
			m_edWakuStr[m_nSlctIdx].SetFocus();
			break;
		case VK_DOWN:
			m_nSlctIdx++;
			if(m_nSlctIdx >= INWAKUMAX){
				m_nSlctIdx = INWAKUMAX-1;
				crtidx = LegalCrtTop(m_nCrtTop + 1);
				UpdateInWakuString(crtidx);
				UpdateData(FALSE);
				Invalidate(TRUE);
			}
			m_edWakuStr[m_nSlctIdx].SetFocus();
			break;
		case VK_F1:
			OnBtncmd1();
			break;
		case VK_F2:
			OnBtncmd2();
			break;
		case VK_F4:
			OnBtncmd4();
			break;
		case VK_F5:
			OnBtncmd5();
			break;
		case VK_F6:
			OnBtncmd6();
			break;
		case VK_F7:
			OnBtncmd7();
			break;
		case VK_F9:
			OnBtncmd8();
			break;
		case VK_F10:
			OnBtncmd9();
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CDialogInput::SetFocusEdit()
{
	cImeHndl.ImeHndlOn(this->m_hWnd);
}

void CDialogInput::KillFocusEdit()
{
	cImeHndl.ImeHndlOff(this->m_hWnd);
}

void CDialogInput::MoveNextEdit()
{
	int		crtidx;

	m_nSlctIdx++;
	if(m_nSlctIdx >= INWAKUMAX){
		crtidx = m_nCrtTop + 5;
		if(crtidx + INWAKUMAX >= WAKUMAX){
			crtidx = WAKUMAX - INWAKUMAX;
			m_nSlctIdx = INWAKUMAX-1;
		}
		else{
			m_nSlctIdx = INWAKUMAX-5;
		}
		UpdateInWakuString(crtidx);
		UpdateData(FALSE);
		Invalidate(TRUE);
	}
	m_edWakuStr[m_nSlctIdx].SetFocus();
}

void CDialogInput::OnOK() 
{
	UpdateData(TRUE);
	MoveNextEdit();
}

void CDialogInput::OnPaint() 
{
	CRect	imgrect;
	CRect	toprect;

	CPaintDC dc(this);
	
	if(m_nFirst == TRUE){
		m_nFirst = FALSE;
		m_edWakuStr[m_nSlctIdx].SetFocus();
	}
	m_stTop.GetWindowRect(&toprect);
	m_stImage.GetWindowRect(&imgrect);
	imgrect.left -= toprect.left;
	imgrect.right -= toprect.left;
	imgrect.top -= toprect.top;
	imgrect.bottom -= toprect.top;
	senvsetdrawmode(PREVIEW);
	m_pcView->DrawWakuInRect(&dc, imgrect);
}

int CDialogInput::CheckSameWaku()
{
	TCHAR	paperpath[FNAMEMAX];
	TCHAR	wakulistfname[FNAMEMAX];
	FILE	*fp;
	int		samecnt;
	TCHAR	buf[RECMAX8];
	LPTSTR	sp;
	TCHAR	wakuname[FNAMEMAX];
	TCHAR	wakufile[FNAMEMAX];
	FILE	*wkfp;
	int		cnt;

	wakuenvgetfullpaperpath(paperpath);
	_stprintf_s(wakulistfname, _T("%s\\wakuname.lst"), paperpath);
	_tfopen_s(&fp, wakulistfname, _T("r"));
	if (fp == NULL) {
		return(0);
	}
	samecnt = 0;
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		sp = buf;
		sp = basecsvstrdivcpy(sp, wakuname);
		_stprintf_s(wakufile, _T("%s\\%s"), paperpath, wakuname);
		_tfopen_s(&wkfp, wakufile, _T("r"));
		if (wkfp == NULL) {
			return(0);
		}
		cnt = 0;
		while(1){
			if(baselibCrDelFgets(buf, RECMAX8, wkfp) == NULL){
				break;
			}
			cnt++;
		}
		fclose(wkfp);
		if(cnt == m_nCrtWakuMax){
			_tcscpy_s(m_szCrtWakuName, wakufile);
			samecnt++;
		}
	}
	fclose(fp);
	return(samecnt);
}

void CDialogInput::ResetWakuString()
{
	int		i, idx;

	for(i = 0, idx = m_nCrtTop; i < INWAKUMAX; i++, idx++){
		_tcscpy_s(m_szWakuText[idx-1], m_csWakuStr[i].GetBuffer(RECMAX));
	}
}

void CDialogInput::SetWakuString(int top)
{
	int		i, idx;

	for(i = 0, idx = top; i < INWAKUMAX; i++, idx++){
		m_csWkno[i].Format(_T("%03d"), idx);
		m_csWakuStr[i] = m_szWakuText[idx-1];
	}
}

void CDialogInput::AllWakuSetString()
{
	CSWakVect	*pcSWakVect;
	int		idx;

	m_pcCmdBase->SetDataListSelectID(SWAKVECTID, 0);
	POSITION pos = m_pcCmdBase->m_pcPtrList->GetHeadPosition();
	idx = 0;
	while (pos != NULL)
	{
		pcSWakVect = (CSWakVect*)(m_pcCmdBase->m_pcPtrList->GetNext(pos));
		if(pcSWakVect->m_nFixFlag != ON){
			pcSWakVect->SetString(m_szWakuText[idx]);
			if(m_szWakuText[idx][0] != '\0'){
				pcSWakVect->StrgLineStruct();
				pcSWakVect->RenewMiniMax();
			}
		}
		idx++;
	}
}

void CDialogInput::GetLastWakuNo()
{
	int		i;

	m_nCrtWakuMax = 0;
	for(i = 0; i < WAKUMAX; i++){
		if(m_szWakuText[i][0] != '\0'){
			m_nCrtWakuMax = i+1;
		}
	}
}

void CDialogInput::SlctCrtWaku()
{
	CDialogWakuSlct	*pcDialogWakuSlct;
	int		i;

	pcDialogWakuSlct = new CDialogWakuSlct(m_pcView);
	pcDialogWakuSlct->m_szSetStr[0] = _T('\0');
	for(i = 0; i < 	m_nCrtWakuMax; i++){
		_tcscat_s(pcDialogWakuSlct->m_szSetStr, _T("/"));
		_tcscat_s(pcDialogWakuSlct->m_szSetStr, m_szWakuText[i]);
	}
	_tcscat_s(pcDialogWakuSlct->m_szSetStr, _T("/"));
	pcDialogWakuSlct->m_nWakuCnt = m_nCrtWakuMax;
	if(pcDialogWakuSlct->DoModal() == IDOK){
		_tcscpy_s(m_szCrtWakuName, pcDialogWakuSlct->m_csWakuName.GetBuffer(FNAMEMAX));
	}
	delete(pcDialogWakuSlct);
}

void CDialogInput::SaveCrctData()
{
	TCHAR	szExePath[FNAMEMAX];	
	TCHAR	szCrctFName[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX8];
	TCHAR	grupname[RECMAX];
	TCHAR	papername[RECMAX];
	int		idx;

	senvgetexepath(szExePath);
	_stprintf_s(szCrctFName, _T("%s\\outcrect.dat"), szExePath);
	_tfopen_s(&fp, szCrctFName, _T("r+,ccs=UNICODE"));
	if (fp == NULL) {
		_tfopen_s(&fp, szCrctFName, _T("w+,ccs=UNICODE"));
		if (fp == NULL) {
			return;
		}
	}
	while(1){
		if(_fgetts(buf, RECMAX8, fp) == NULL){
			break;
		}
	}
	wakuenvgetgrupname(grupname);
	wakuenvgetpapername(papername);
	_ftprintf_s(fp, _T("\"未\",\"%s\",\"%s\",\"%s\",\"1\",\""),
		grupname, papername, m_szCrtWakuName);
	for(idx = 0; idx < m_nCrtWakuMax; idx++){
		_ftprintf_s(fp, _T("/%s"), m_szWakuText[idx]);
	}
	_ftprintf_s(fp, _T("\"\n"));
	fclose(fp);
}

int CDialogInput::LegalCrtTop(int crttop)
{
	if(crttop + INWAKUMAX >= WAKUMAX){
		crttop = WAKUMAX - INWAKUMAX;
	}
	if(crttop < 1){
		crttop = 1;
	}
	return(crttop);
}

