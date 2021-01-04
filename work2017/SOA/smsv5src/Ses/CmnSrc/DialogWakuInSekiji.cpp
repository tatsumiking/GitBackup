// dwkinskj.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "io.h"

#include "DialogWakuInSekiji.h"

#include "DialogGaijiSlct.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Pout.h"
#include "../../ObjLib/Mojienv.h"
#include "../../ObjLib/Wakuenv.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuInSekiji ダイアログ

CDialogWakuInSekiji::CDialogWakuInSekiji(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogWakuInSekiji::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogWakuInSekiji)
	//}}AFX_DATA_INIT
	
	m_pcView = (CTonView*)pParent;
	m_pcDoc = m_pcView->GetDocument();
	m_pcCmdBase = NULL;
}
void CDialogWakuInSekiji::SetCmdBase(CCmdBase *pcCmdBae)
{
	m_pcCmdBase = pcCmdBae;
}
void CDialogWakuInSekiji::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuInSekiji)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC1, m_stWkno[0]);
	DDX_Control(pDX, IDC_STATIC2, m_stWkno[1]);
	DDX_Control(pDX, IDC_STATIC3, m_stWkno[2]);
	DDX_Control(pDX, IDC_STATIC4, m_stWkno[3]);
	DDX_Control(pDX, IDC_STATIC5, m_stWkno[4]);
	DDX_Control(pDX, IDC_STATIC6, m_stWkno[5]);
	DDX_Control(pDX, IDC_STATIC7, m_stWkno[6]);
	DDX_Control(pDX, IDC_STATIC8, m_stWkno[7]);
	DDX_Control(pDX, IDC_STATIC9, m_stWkno[8]);
	DDX_Control(pDX, IDC_STATIC10, m_stWkno[9]);
	DDX_Control(pDX, IDC_CHKKTGK1, m_chkKtgk[0]);
	DDX_Control(pDX, IDC_CHKKTGK2, m_chkKtgk[1]);
	DDX_Control(pDX, IDC_CHKKTGK3, m_chkKtgk[2]);
	DDX_Control(pDX, IDC_CHKKTGK4, m_chkKtgk[3]);
	DDX_Control(pDX, IDC_CHKKTGK5, m_chkKtgk[4]);
	DDX_Control(pDX, IDC_CHKKTGK6, m_chkKtgk[5]);
	DDX_Control(pDX, IDC_CHKKTGK7, m_chkKtgk[6]);
	DDX_Control(pDX, IDC_CHKKTGK8, m_chkKtgk[7]);
	DDX_Control(pDX, IDC_CHKKTGK9, m_chkKtgk[8]);
	DDX_Control(pDX, IDC_CHKKTGK10, m_chkKtgk[9]);
	DDX_Control(pDX, IDC_EDIT1, m_edText[0]);
	DDX_Control(pDX, IDC_EDIT2, m_edText[1]);
	DDX_Control(pDX, IDC_EDIT3, m_edText[2]);
	DDX_Control(pDX, IDC_EDIT4, m_edText[3]);
	DDX_Control(pDX, IDC_EDIT5, m_edText[4]);
	DDX_Control(pDX, IDC_EDIT6, m_edText[5]);
	DDX_Control(pDX, IDC_EDIT7, m_edText[6]);
	DDX_Control(pDX, IDC_EDIT8, m_edText[7]);
	DDX_Control(pDX, IDC_EDIT9, m_edText[8]);
	DDX_Control(pDX, IDC_EDIT10, m_edText[9]);
	DDX_Control(pDX, IDC_EDIT11, m_edText[10]);
	DDX_Control(pDX, IDC_EDIT12, m_edText[11]);
	DDX_Control(pDX, IDC_EDIT13, m_edText[12]);
	DDX_Control(pDX, IDC_EDIT14, m_edText[13]);
	DDX_Control(pDX, IDC_EDIT15, m_edText[14]);
	DDX_Control(pDX, IDC_EDIT16, m_edText[15]);
	DDX_Control(pDX, IDC_EDIT17, m_edText[16]);
	DDX_Control(pDX, IDC_EDIT18, m_edText[17]);
	DDX_Control(pDX, IDC_EDIT19, m_edText[18]);
	DDX_Control(pDX, IDC_EDIT20, m_edText[19]);
	DDX_Control(pDX, IDC_EDIT21, m_edText[20]);
	DDX_Control(pDX, IDC_EDIT22, m_edText[21]);
	DDX_Control(pDX, IDC_EDIT23, m_edText[22]);
	DDX_Control(pDX, IDC_EDIT24, m_edText[23]);
	DDX_Control(pDX, IDC_EDIT25, m_edText[24]);
	DDX_Control(pDX, IDC_EDIT26, m_edText[25]);
	DDX_Control(pDX, IDC_EDIT27, m_edText[26]);
	DDX_Control(pDX, IDC_EDIT28, m_edText[27]);
	DDX_Control(pDX, IDC_EDIT29, m_edText[28]);
	DDX_Control(pDX, IDC_EDIT30, m_edText[29]);
}

BEGIN_MESSAGE_MAP(CDialogWakuInSekiji, CMyDialog)
	//{{AFX_MSG_MAP(CDialogWakuInSekiji)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OUT, OnOut)
	ON_BN_CLICKED(IDC_DISP, OnDisp)
	ON_BN_CLICKED(IDC_GAIJI, OnGaiji)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuInSekiji メッセージ ハンドラ
int CDialogWakuInSekiji::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMyDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CDialogWakuInSekiji::OnInitDialog() 
{
	CRect	rect;
	int		idx;

	CMyDialog::OnInitDialog();

	m_nCrtIdxTop = 0;
	m_nNextFocus = -1;
	for(idx = 0; idx < INSEKIJIMAX*3; idx++){
		m_edText[idx].m_nID = IDC_EDIT1+idx;
		m_edText[idx].m_pcMyDialog = this;
		m_edText[idx].SetIMEMode(ON);
	}
	GetGrupMax();
	LoadCrtGrupText();
	m_fFirstFlag = TRUE;
	return TRUE;
}

void CDialogWakuInSekiji::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CDialogWakuInSekiji::OnDisp() 
{
	// TODO: この位置に初期化の補足処理を追加してください
	SaveCrtGrupText();
	m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);
	KeyIn(VK_DOWN, IDC_EDIT1+m_nNextFocus);
}

void CDialogWakuInSekiji::LoadCrtGrupText()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAXAD];
	LPTSTR	lpret;
	LPTSTR	csvsp[RECMAX];
	int		cnt;
	int		flag;
	TCHAR	strg[RECMAXAD];
	int		idx;
	CButton	*pcBtn;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\sekiji.lst"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	baselibCrDelFgets(buf, RECMAX, fp);
	for(idx = 0; idx < m_nCrtIdxTop; idx++){
		lpret = baselibCrDelFgets(buf, RECMAX, fp);
		if(lpret == NULL){
			break;
		}
	}
	if(lpret == NULL){
		fclose(fp);
		return;
	}
	for(idx = 0; idx < INSEKIJIMAX; idx++){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(buf[0] == _T('\0')){
			break;
		}
		cnt = basedivcsvdata(csvsp, buf);

		flag = _ttoi(csvsp[0]);
		pcBtn = (CButton*)GetDlgItem(IDC_CHKKTGK1+idx);
		pcBtn->SetCheck(flag);

		SetDlgItemText(IDC_EDIT1+idx*3, csvsp[1]);
		SetDlgItemText(IDC_EDIT1+idx*3+1, csvsp[2]);
		SetDlgItemText(IDC_EDIT1+idx*3+2, csvsp[3]);
	}
	strg[0] = _T('\0');
	for(;idx < INSEKIJIMAX; idx++){
		pcBtn = (CButton*)GetDlgItem(IDC_CHKKTGK1+idx);
		pcBtn->SetCheck(OFF);
		SetDlgItemText(IDC_EDIT1+idx*3, strg);
		SetDlgItemText(IDC_EDIT1+idx*3+1, strg);
		SetDlgItemText(IDC_EDIT1+idx*3+2, strg);
	}
	fclose(fp);
}

void CDialogWakuInSekiji::SaveCrtGrupText()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	srcfname[FNAMEMAX];
	TCHAR	bakfname[FNAMEMAX];
	FILE	*infp, *outfp;
	TCHAR	buf[RECMAXAD];
	LPTSTR	lpret;
	int		idx;
	CButton	*pcBtn;
	int		flag;
	TCHAR	szStr1[RECMAXAD];
	TCHAR	szStr2[RECMAXAD];
	TCHAR	szStr3[RECMAXAD];

	UpdateData(TRUE);
	senvgetexepath(exepath);
	_stprintf_s(srcfname, _T("%s\\sekiji.lst"), exepath);
	_stprintf_s(bakfname, _T("%s\\sekiji.bak"), exepath);
	CopyFile(srcfname, bakfname, FALSE);
	_tfopen_s(&infp, bakfname, _T("r"));
	if(infp == NULL){
		return;
	}
	_tfopen_s(&outfp, srcfname, _T("w"));
	lpret = baselibCrDelFgets(buf, RECMAX, infp);
	baselibFputs(buf, outfp);
	for(idx = 0; idx < m_nCrtIdxTop; idx++){
		lpret = baselibCrDelFgets(buf, RECMAX, infp);
		if(lpret == NULL){
			break;
		}
		baselibFputs(buf, outfp);
	}
	if(lpret == NULL){
		fclose(infp);
		fclose(outfp);
		return;
	}
	for(idx = 0; idx < INSEKIJIMAX; idx++){
		lpret = baselibCrDelFgets(buf, RECMAX, infp);
		if(lpret == NULL){
			fclose(infp);
			fclose(outfp);
			return;
		}
		if(buf[0] == _T('\0')){
			fclose(infp);
			fclose(outfp);
			return;
		}

		pcBtn = (CButton*)GetDlgItem(IDC_CHKKTGK1+idx);
		flag = pcBtn->GetCheck();

		GetDlgItemText(IDC_EDIT1+idx*3, szStr1, RECMAX);
		GetDlgItemText(IDC_EDIT1+idx*3+1, szStr2, RECMAX);
		GetDlgItemText(IDC_EDIT1+idx*3+2, szStr3, RECMAX);
		_stprintf_s(buf, _T("%d,%s,%s,%s,\n"), flag, szStr1, szStr2, szStr3);
		baselibFputs(buf, outfp);
	}
	
	while(1){
		lpret = baselibCrDelFgets(buf, RECMAX, infp);
		if(lpret == NULL){
			break;
		}
		baselibFputs(buf, outfp);
	}
	fclose(infp);
	fclose(outfp);
}

void CDialogWakuInSekiji::KeyIn(UINT nTCHAR, UINT nID)
{
	int		next;
	int		crttop;

	switch(nTCHAR){
	//case VK_RETURN:
	case VK_DOWN:
		next = m_nNextFocus + 1;
		if(INSEKIJIMAX*3 <= next){
			crttop = m_nCrtIdxTop + INSEKIJIMAX;
			if(m_nGrupMax <= crttop){
				crttop = m_nCrtIdxTop;
			}
			next = 0;
			m_nCrtIdxTop = crttop;
			LoadCrtGrupText();
		}
		WakuSetFocus(next);
		Invalidate(TRUE);
		break;
 	case VK_UP:
		next = m_nNextFocus - 1;
		if(next < 0){
			crttop = m_nCrtIdxTop - INSEKIJIMAX;
			if(crttop < 0){
				crttop = 0;
			}
			next = 0;
			m_nCrtIdxTop = crttop;
			LoadCrtGrupText();
		}
		WakuSetFocus(next);
		Invalidate(TRUE);
		break;
	}
}
void CDialogWakuInSekiji::OnPrev() 
{
	int		next;
	int		crttop;

	crttop = m_nCrtIdxTop - INSEKIJIMAX;
	if(crttop < 0){
		crttop = 0;
	}
	next = 0;
	m_nCrtIdxTop = crttop;
	LoadCrtGrupText();
	WakuSetFocus(next);
	Invalidate(TRUE);
}

void CDialogWakuInSekiji::OnNext() 
{
	int		next;
	int		crttop;

	crttop = m_nCrtIdxTop + INSEKIJIMAX;
	if(m_nGrupMax <= crttop){
		crttop = m_nCrtIdxTop;
	}
	next = 0;
	m_nCrtIdxTop = crttop;
	LoadCrtGrupText();
	WakuSetFocus(next);
	Invalidate(TRUE);
}
void CDialogWakuInSekiji::SelectEdit(UINT nID)
{
	int		nNo;

	nNo = nID - IDC_EDIT1;
	if(m_nNextFocus == nNo){
		return;
	}
	m_nNextFocus = nNo;
	// 処理をグループに変更する必要あり
	m_pcView->m_nCrtGroup = (m_nNextFocus) / 3 + m_nCrtIdxTop;
	m_pcView->m_nCrtWaku = (m_nNextFocus) % 3;
	m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);
}
void CDialogWakuInSekiji::OnOut() 
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	wakpath[FNAMEMAX];
	TCHAR	grupname[RECMAX];
	TCHAR	papername[RECMAX];
	TCHAR	wakuname[FNAMEMAX];
	TCHAR	wakupath[FNAMEMAX];
	TCHAR	sekijifname[FNAMEMAX];
	TCHAR	crctfname[FNAMEMAX];
	TCHAR	backfname[FNAMEMAX];
	FILE	*infp;
	FILE	*crctfp;
	TCHAR	csvbuf[RECMAXAD];
	LPTSTR	csvsp[RECMAX];
	int		cnt, flag;
	TCHAR	outbuf[RECMAX];

	SaveCrtGrupText();
	senvgetexepath(exepath);
	senvgetwakpath(wakpath);
	wakuenvgetgrupname(grupname);
	_stprintf_s(sekijifname, _T("%s\\sekiji.env"), exepath);
	_tfopen_s(&infp, sekijifname, _T("r"));
	baselibCrDelFgets(csvbuf, RECMAX, infp);
	baselibCrDelFgets(csvbuf, RECMAX, infp);
	cnt = basedivcsvdata(csvsp, csvbuf);
	_tcscpy_s(papername, csvsp[0]);
	_tcscpy_s(wakuname, csvsp[1]);
	fclose(infp);
	_stprintf_s(wakupath, _T("%s\\%s\\%s"), wakpath, papername, wakuname);

	_stprintf_s(crctfname, _T("%s\\outcrect.dat"), exepath);
	_stprintf_s(backfname, _T("%s\\outcrect.bak"), exepath);
	CopyFile(crctfname, backfname, FALSE);

	_tfopen_s(&crctfp, crctfname, _T("w,ccs=UNICODE"));

	_stprintf_s(sekijifname, _T("%s\\sekiji.lst"), exepath);

	_tfopen_s(&infp, sekijifname, _T("r"));
	if(infp == NULL){
		return;
	}
	baselibCrDelFgets(csvbuf, RECMAX, infp);
	while(1){
		if(baselibCrDelFgets(csvbuf, RECMAX, infp) == NULL){
			break;
		}
		cnt = basedivcsvdata(csvsp, csvbuf);
		flag = _ttoi(csvsp[0]);
		if(flag == 1){
			_stprintf_s(outbuf, _T("\"未\",\"%s\",\"%s\",\"%s\",\"1\",\"/%s/%s/%s\",\n"),
				grupname, papername, wakupath,csvsp[1],csvsp[2],csvsp[3]);
		}else{
			_stprintf_s(outbuf, _T("\"未\",\"%s\",\"%s\",\"%s\",\"1\",\"//%s/%s\",\n"),
				grupname, papername, wakupath,csvsp[2],csvsp[3]);
		}
		baselibFputs(outbuf, crctfp);
	}
	fclose(infp);
	fclose(crctfp);

	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();

	pcWnd->ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
	pcWnd->EnableWindow(FALSE);
	EnableWindow(FALSE);
	m_pcView->ExecSmsout();
	pcWnd->EnableWindow(TRUE);
	EnableWindow(TRUE);
	ShowWindow(SW_SHOW);
	pcWnd->ShowWindow(SW_SHOW);

	CopyFile(backfname, crctfname, FALSE);

}
void CDialogWakuInSekiji::OnExit() 
{
	m_pcView->m_nCrtGroup = -1;
	m_pcView->m_nCrtWaku = -1;
	m_pcView->CommandExec(ENDEVENT, 0L, 0L);
}
void CDialogWakuInSekiji::GetGrupMax()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	int		idx;
	TCHAR	buf[RECMAX+16];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\sekiji.lst"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if(fp == NULL){
		return;
	}
	baselibCrDelFgets(buf, RECMAX, fp);
	idx = 0;
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(buf[0] == _T('\0')){
			break;
		}
		idx++;
	}
	m_nGrupMax = idx;
	fclose(fp);
}
void CDialogWakuInSekiji::OnPaint() 
{
	int	no;
	CRect	rect;
	TCHAR	numstr[QRECMAX];

	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	if(m_fFirstFlag == TRUE){
		m_fFirstFlag = FALSE;
		WakuSetFocus(0);
	}
	for(no = 0; no < INSEKIJIMAX; no++){
		if(m_nGrupMax <= (m_nCrtIdxTop + no)){
			break;
		}
		_stprintf_s(numstr, _T("%03d"), m_nCrtIdxTop+no+1);
		m_stWkno[no].SetWindowText(numstr);
	}
	_tcscpy_s(numstr, _T("   "));
	for(; no < INSEKIJIMAX; no++){
		m_stWkno[no].SetWindowText(numstr);
	}
}
void CDialogWakuInSekiji::WakuSetFocus(int next)
{
	m_edText[next].SetFocus();
	// 処理をグループに変更する必要あり
	m_pcView->m_nCrtGroup = (m_nNextFocus) / 3 +m_nCrtIdxTop;
	m_pcView->m_nCrtWaku = (m_nNextFocus) % 3;
	m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);
}
void CDialogWakuInSekiji::OnGaiji() 
{
	CDialogGaijiSlct *pcDialogGaijiSlct;
	TCHAR	szFontName[RECMAX];
	TCHAR	szFolderName[RECMAX];
	int		idx, retftype;
	TCHAR	strg[RECMAX];
	TCHAR	setstrg[RECMAX];
	int		slen, elen;
	
	idx = m_nNextFocus;
	mojienvgetfontname(szFontName);
	mojienvflistopen(_T("rb"));
	mojienvgetfonttypename(szFontName, &retftype, szFolderName);
	mojienvflistclose();
	pcDialogGaijiSlct = new CDialogGaijiSlct(this);
	_tcscpy_s(pcDialogGaijiSlct->m_szFolderName, szFolderName);
	if(pcDialogGaijiSlct->DoModal() == IDOK){
		GetDlgItemText(IDC_EDIT1+idx, strg, RECMAX);
		m_edText[idx].GetSel(slen, elen);
		setstrg[0] = _T('\0');
		if(slen != 0){
			_tcscpy_s(setstrg, RECMAX, strg);
			setstrg[slen] = _T('\0');
		}
		_tcscat_s(setstrg, RECMAX, pcDialogGaijiSlct->m_szGaijiText);
		_tcscat_s(setstrg, RECMAX, &(strg[elen]));
		SetDlgItemText(IDC_EDIT1+idx, setstrg);
	}
	delete(pcDialogGaijiSlct);
	UpdateData(FALSE);
}
void CDialogWakuInSekiji::SetString(LPTSTR LPTSTR)
{
	CEdit	*pcEdit;

	pcEdit = (CEdit*)GetDlgItem(IDC_EDIT1+m_nNextFocus);
	pcEdit->ReplaceSel(LPTSTR, FALSE);
	pcEdit->SetFocus();
}
