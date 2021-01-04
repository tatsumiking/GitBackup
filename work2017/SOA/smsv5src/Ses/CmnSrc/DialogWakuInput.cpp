// DlgWkInp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "io.h"

#include "DialogWakuInput.h"
#include "DialogWakuLoad.h"
#include "DialogGaijiSlct.h"
#include "DialogCrctLoad.h"
#include "DialogCrctDisp.h"

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
// CDialogWakuInput ダイアログ

CDialogWakuInput::CDialogWakuInput(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogWakuInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogWakuInput)
	m_csCrctCnt = _T("");
	//}}AFX_DATA_INIT

	m_pcView = (CTonView*)pParent;
	m_pcDoc = m_pcView->GetDocument();
	m_pcCmdBase = NULL;
	m_nCrctDisp = SW_HIDE;
}

void CDialogWakuInput::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuInput)
	DDX_Text(pDX, IDC_STCRCTCNT, m_csCrctCnt);
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
}


BEGIN_MESSAGE_MAP(CDialogWakuInput, CMyDialog)
	//{{AFX_MSG_MAP(CDialogWakuInput)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DISP, OnDisp)
	ON_BN_CLICKED(IDC_CRCT, OnCrct)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_OUT, OnOut)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PLOTOUT, OnPlotout)
	ON_BN_CLICKED(IDC_GAIJI, OnGaiji)
	ON_BN_CLICKED(IDC_CRCTLOAD, OnCrctload)
	ON_BN_CLICKED(IDC_CRCTSAVE, OnCrctsave)
	ON_BN_CLICKED(IDC_CRCTDISP, OnCrctdisp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuInput メッセージ ハンドラ

int CDialogWakuInput::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMyDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CDialogWakuInput::OnInitDialog() 
{
	CRect	rect;
	int		idx;

	CMyDialog::OnInitDialog();
	
	m_nCrtIdxTop = 0;
	m_nNextFocus = -1;
	for(idx = 0; idx < INPUTEDITMAX; idx++){
		m_edText[idx].m_nID = IDC_EDIT1+idx;
		m_edText[idx].m_pcMyDialog = this;
		m_edText[idx].SetIMEMode(ON);
	}
	GetWakuMax();
	LoadCrtWakuText();
	m_fFirstFlag = TRUE;
	//WakuSetFocus(0);
	return TRUE;
}
void CDialogWakuInput::SetSubDialog(CDialog *pcDialog)
{
	m_pcSubDialog = pcDialog;
}
void CDialogWakuInput::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CDialogWakuInput::OnDisp() 
{
	// TODO: この位置に初期化の補足処理を追加してください
	SaveCrtWakuText();
	m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);
	KeyIn(VK_DOWN, IDC_EDIT1+m_nNextFocus);
	//WakuSetFocus();
}

void CDialogWakuInput::LoadCrtWakuText()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX+16];
	TCHAR	strg[RECMAX+16];
	int		idx;
	long	seekp;

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\string.lst"), exepath);
	_tfopen_s(&fp, fname, _T("rb"));
	if(fp == NULL){
		return;
	}
	seekp = m_nCrtIdxTop * RECMAX;
	fseek(fp, seekp, 0);
	for(idx = 0; idx < INPUTEDITMAX; idx++){
		if(fread(buf, RECMAX, 1, fp) != 1){
			break;
		}
		_tcscpy_s(strg, buf);
		SetDlgItemText(IDC_EDIT1+idx, strg);
	}
	strg[0] = _T('\0');
	for(;idx < INPUTEDITMAX; idx++){
		SetDlgItemText(IDC_EDIT1+idx, strg);
	}
	fclose(fp);
}
void CDialogWakuInput::SaveCrtWakuText()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	strg[RECMAX+16];
	int		idx;
	long	seekp;

	idx = 0;
	UpdateData(TRUE);
	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\string.lst"), exepath);
	_tfopen_s(&fp, fname, _T("r+b"));
	if(fp != NULL){
		seekp = m_nCrtIdxTop * RECMAX;
		fseek(fp, seekp, 0);
		idx = 0;
		for(idx = 0; idx < INPUTEDITMAX; idx++){
			memset(strg, _T(' '), RECMAX);
			strg[RECMAX-1] = 0x0a;
			strg[RECMAX-2] = 0x0d;
			GetDlgItemText(IDC_EDIT1+idx, strg, RECMAX);
			fwrite(strg, RECMAX, 1, fp);
		}
		fclose(fp);
	}
}

void CDialogWakuInput::KeyIn(UINT nTCHAR, UINT nID)
{
	int		next;

	switch(nTCHAR){
	//case VK_RETURN:
	case VK_DOWN:
		next = m_nNextFocus + 1;
		if(INPUTEDITMAX <= next){
			m_nCrtIdxTop = m_nCrtIdxTop + 5;
			if(m_nCrtIdxTop + 5 > m_nWakuMax){
				m_nCrtIdxTop = m_nWakuMax - 5;
				next = 0;
			}
			else{
				next = 5;
			}
			LoadCrtWakuText();
		}
		WakuSetFocus(next);
		Invalidate(TRUE);
		break;
 	case VK_UP:
		next = m_nNextFocus - 1;
		if(next < 0){
			m_nCrtIdxTop = m_nCrtIdxTop - 5;
			if(m_nCrtIdxTop < 0){
				m_nCrtIdxTop = 0;
			}
			next = 0;
			LoadCrtWakuText();
		}
		WakuSetFocus(next);
		Invalidate(TRUE);
		break;
	}
}

void CDialogWakuInput::SelectEdit(UINT nID)
{
	int		nNo;
	CDC		*pcDC;

	nNo = nID - IDC_EDIT1;
	if(m_nNextFocus == nNo){
		return;
	}
	m_nNextFocus = nNo;
	m_pcView->m_nCrtWaku = m_nNextFocus+m_nCrtIdxTop;
	pcDC = m_pcView->GetDC();
	m_pcView->DrawSelectWaku(pcDC);
	m_pcView->ReleaseDC(pcDC);
}

void CDialogWakuInput::OnCrct() 
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	crctfname[FNAMEMAX];
	FILE	*crctfp;
	int		crctcnt;
	TCHAR	buf[RECMAX8+16];

	SaveCrtWakuText();
	m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);

	senvgetexepath(exepath);
	_stprintf_s(crctfname, _T("%s\\outcrect.dat"), exepath);
	_tfopen_s(&crctfp, crctfname, _T("r+,ccs=UNICODE"));
	crctcnt = 0;
	
	if(crctfp == NULL){
		_tfopen_s(&crctfp, crctfname, _T("w+,ccs=UNICODE"));
		if(crctfp == NULL){
			return;
		}
	}
	while(1){
		if(_fgetts(buf, RECMAX8/sizeof(TCHAR), crctfp) == NULL){
			break;
		}
		crctcnt++;
	}
	
	SaveRecString(crctcnt, crctfp);

	fclose(crctfp);

	crctcnt++;
	_stprintf_s(buf, _T("貯込%d枚"), crctcnt);
	m_csCrctCnt = buf;
	UpdateData(FALSE);
}

void CDialogWakuInput::OnLoad() 
{
	TCHAR	paperpath[FNAMEMAX];
	TCHAR	wakuname[FNAMEMAX];
	TCHAR	wakufilename[FNAMEMAX];
	int		slctidx;
	int		ret;
	CDialogWakuLoad*	pDialogWakuLoad;

	CTonDoc* pcDoc = (CTonDoc*)m_pcView->GetDocument(); 
	//pcDoc->WakuLoad(wakupath);
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	ShowWindow(SW_HIDE);
	pDialogWakuLoad = new CDialogWakuLoad(m_pcView);
	ret = pDialogWakuLoad->DoModal();
	if(ret == IDOK){
		wakuenvgetfullpaperpath(paperpath);
		wakuenvgetselectwakuidx(&slctidx);
		wakuenvlistgetwakuname(slctidx, wakuname);
		pcDoc->SetTitle(wakuname);
		_stprintf_s(wakufilename, _T("%s\\%s"), paperpath, wakuname);

		WakuLoad(wakufilename);
		pcDoc->OnFlag(ACTIVEFLAG);
		pcDoc->SaveStringStoreFile(SWAKVECTID);
		GetWakuMax();
		LoadCrtWakuText();
		wakuenvsetselectwakuidx(-1);
	}
	delete(pDialogWakuLoad);
	//m_pcView->CommandExec(DLGSETEVENT, 0, 0);
	m_pcView->Invalidate();
	baseDispatch();	
	WakuSetFocus(0);
	ShowWindow(SW_SHOW);
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

void CDialogWakuInput::OnOut() 
{
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	m_pcView->CommandExec(DLGSETEVENT, 0L, 0L);
	pcWnd->ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
	m_pcSubDialog->ShowWindow(SW_HIDE);
	m_pcDialogCrctDisp->ShowWindow(SW_HIDE);
	pcWnd->EnableWindow(FALSE);
	EnableWindow(FALSE);
	m_pcSubDialog->EnableWindow(FALSE);
	m_pcDialogCrctDisp->EnableWindow(FALSE);
	m_pcDoc->SaveUndo();
	m_pcView->ExecSmsout();
	m_pcDoc->LoadUndo();
	pcWnd->EnableWindow(TRUE);
	EnableWindow(TRUE);
	m_pcSubDialog->EnableWindow(TRUE);
	m_pcSubDialog->ShowWindow(SW_SHOW);
	m_pcDialogCrctDisp->EnableWindow(TRUE);
	m_pcDialogCrctDisp->ShowWindow(m_nCrctDisp);
	ShowWindow(SW_SHOW);
	pcWnd->ShowWindow(SW_SHOW);
}

void CDialogWakuInput::OnExit() 
{
	m_pcView->m_nCrtWaku = -1;
	m_pcView->CommandExec(ENDEVENT, 0L, 0L);
}

void CDialogWakuInput::GetWakuMax()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	int		idx;
	TCHAR	buf[RECMAX+16];

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\string.lst"), exepath);
	_tfopen_s(&fp, fname, _T("rb"));
	if(fp == NULL){
		return;
	}
	idx = 0;
	while(1){
		if(fread(buf, RECMAX, 1, fp) != 1){
			break;
		}
		idx++;
	}
	m_nWakuMax = idx;
	fclose(fp);
}

void CDialogWakuInput::OnPaint() 
{
	int	idx;
	CRect	rect;
	TCHAR	numstr[QRECMAX];

	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	if(m_fFirstFlag == TRUE){
		m_fFirstFlag = FALSE;
		WakuSetFocus(0);
	}
	for(idx = 0; idx < INPUTEDITMAX; idx++){
		if(m_nWakuMax <= m_nCrtIdxTop + idx){
			break;
		}
		_stprintf_s(numstr, _T("%03d"), m_nCrtIdxTop + idx + 1);
		m_stWkno[idx].SetWindowText(numstr);
	}
	_tcscpy_s(numstr, _T("   "));
	for(; idx < INPUTEDITMAX; idx++){
		m_stWkno[idx].SetWindowText(numstr);
	}
}

void CDialogWakuInput::WakuSetFocus(int next)
{
	CDC	*pcDC;

	m_edText[next].SetFocus();
	m_pcView->m_nCrtWaku = m_nNextFocus+m_nCrtIdxTop;
	pcDC = m_pcView->GetDC();
	m_pcView->DrawSelectWaku(pcDC);
	m_pcView->ReleaseDC(pcDC);
}

void CDialogWakuInput::OnPlotout() 
{
	TCHAR	plotername[QRECMAX];

	poutploternameget(plotername);
	if(_tcscmp(plotername, _T("プリンタ")) == 0){
		m_pcView->PrinterOutput();
	}
	else{
		m_pcView->PloterOutput();
	}
}

void CDialogWakuInput::OnGaiji() 
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

void CDialogWakuInput::OnCrctload() 
{
	CDialogCrctLoad*		pcDialogCrctLoad;
	TCHAR	exepath[FNAMEMAX];
	TCHAR	crctfname[FNAMEMAX];
	FILE	*crctfp;
	TCHAR	csvbuf[RECMAX8+16];
	LPTSTR	csvsp[10];
	LPTSTR	sp;
	int		idx, len;
	TCHAR	wakufilename[FNAMEMAX];
	int		slctidx;
	TCHAR	fname[FNAMEMAX];
	TCHAR	szStr[RECMAX];
	FILE	*fp;

	slctidx = -1;
	pcDialogCrctLoad = new CDialogCrctLoad(this);
	if(pcDialogCrctLoad->DoModal() == IDOK){
		slctidx = pcDialogCrctLoad->m_nSlctIdx;
	}
	delete(pcDialogCrctLoad);
	if(slctidx == -1){
		return;
	}
	senvgetexepath(exepath);
	_stprintf_s(crctfname, _T("%s\\outcrect.dat"), exepath);
	_tfopen_s(&crctfp, crctfname, _T("r+,ccs=UNICODE"));
	if(crctfp == NULL){
		return;
	}
	csvbuf[0] = _T('\0');
	idx = 0;
	while(1){
		if(_fgetts(csvbuf, RECMAX8, crctfp) == NULL){
			break;
		}
		len = _tcslen(csvbuf);
		csvbuf[len-1] = _T('\0');
		if(idx == slctidx){
			break;
		}
		idx++;
	}
	fclose(crctfp);
	if(csvbuf[0] == _T('\0')){
		return;
	}

	m_pcView->m_nCrctIdx = slctidx;

	basedivcsvdata(csvsp, csvbuf);
	_tcscpy_s(wakufilename, csvsp[3]);

	WakuLoad(wakufilename);

	senvgetexepath(exepath);
	_stprintf_s(fname, _T("%s\\string.lst"), exepath);
	_tfopen_s(&fp, fname, _T("wb"));
	if(fp == NULL){
		return;
	}
	sp = csvsp[5];
	sp++;
	while(1){
		// 2001.10.18 立身 途中に文字が入っていない枠があると以降処理されない
		if(*sp == _T('\0')){
			break;
		}
		memset(szStr, _T(' '), RECMAX);
		szStr[RECMAX-1] = 0x0a;
		szStr[RECMAX-2] = 0x0d;
		sp = basedivstrg(sp, szStr, '/');
		fwrite(szStr, RECMAX, 1, fp);
	}
	fclose(fp);

	CTonDoc* pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->LoadStringStoreFile(SWAKVECTID);

	m_pcView->m_nCrtWaku = -1;
	pcDoc->SaveStringStoreFile(SWAKVECTID);
	GetWakuMax();
	LoadCrtWakuText();
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

void CDialogWakuInput::OnCrctsave() 
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	crctfname[FNAMEMAX];
	TCHAR	backfname[FNAMEMAX];
	FILE	*crctfp;
	FILE	*backfp;
	TCHAR	csvbuf[RECMAX8+16];
	int		idx, len;

	senvgetexepath(exepath);
	_stprintf_s(crctfname, _T("%s\\outcrect.dat"), exepath);
	_stprintf_s(backfname, _T("%s\\outcrect.bak"), exepath);

	_tfopen_s(&backfp, backfname, _T("w,ccs=UNICODE"));
	if(backfp == NULL){
		return;
	}
	_tfopen_s(&crctfp, crctfname, _T("r,ccs=UNICODE"));
	if(crctfp == NULL){
		fclose(backfp);
		return;
	}
	idx = 0;
	while(1){
		if(_fgetts(csvbuf, RECMAX8, crctfp) == NULL){
			break;
		}
		len = _tcslen(csvbuf);
		csvbuf[len-1] = _T('\0');
		_fputts(csvbuf, backfp);
	}
	fclose(backfp);
	fclose(crctfp);

	_tfopen_s(&crctfp, crctfname, _T("w,ccs=UNICODE"));
	if(crctfp == NULL){
		return;
	}
	_tfopen_s(&backfp, backfname, _T("r,ccs=UNICODE"));
	if(backfp == NULL){
		fclose(crctfp);
		return;
	}
	idx = 0;
	while(1){
		if(_fgetts(csvbuf, RECMAX8, backfp) == NULL){
			break;
		}
		len = _tcslen(csvbuf);
		csvbuf[len-1] = _T('\0');
		if(m_pcView->m_nCrctIdx == idx){
			SaveRecString(idx, crctfp);
		}
		else{
			_fputts(csvbuf, crctfp);
		}
		idx++;
	}
	fclose(backfp);
	fclose(crctfp);

}
void CDialogWakuInput::OnCrctdisp()
{
	::SetWindowPos(m_pcDialogCrctDisp->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	m_pcDialogCrctDisp->ShowWindow(SW_SHOW);
	m_pcDialogCrctDisp->InitLstCrct();
	m_nCrctDisp = SW_SHOW;
}
void CDialogWakuInput::SaveCrtWaku(int crctno, LPTSTR paperpath, LPTSTR wakuname)
{
	TCHAR	filename[FNAMEMAX];

	CTonDoc* pcDoc = (CTonDoc*)m_pcView->GetDocument();
	_stprintf_s(wakuname, FNAMEMAX, _T("crct$%03d.wak"), crctno);
	_stprintf_s(filename, _T("%s\\%s"), paperpath, wakuname);
	pcDoc->WakuSave(filename);
	basechangesafix(filename, _T("env"));
	wakuenvsaveoutputenv(filename);
	basechangesafix(filename, _T("bin"));
	((CTonView*)m_pcView)->PrintEnvSave(filename);
}

void CDialogWakuInput::WakuLoad(LPTSTR wakufilename)
{
	int		len;
	TCHAR	basefname[FNAMEMAX];
	TCHAR	printenvfname[FNAMEMAX];
	TCHAR	outputenvfname[FNAMEMAX];

	CTonDoc* pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->DataAllCls();

	_tcscpy_s(basefname, wakufilename);
	len = _tcslen(basefname);
	basefname[len-4] = _T('\0');

	_stprintf_s(outputenvfname, _T("%s.env"), basefname);
    if((_taccess_s(outputenvfname, 0 )) == 0){
		wakuenvloadoutputenv(outputenvfname);
	}
	m_pcView->WorkSizeReset();
	m_pcView->WorkAreaReset();

	pcDoc->WakuLoad(wakufilename);
	pcDoc->OnFlag(ACTIVEFLAG);

	_stprintf_s(printenvfname, _T("%s.bin"), basefname);
    if((_taccess_s(printenvfname, 0 )) == 0){
		m_pcView->PrintEnvLoad(printenvfname);
	}
	else{
		basegetparentpath(printenvfname);
		_tcscat_s(printenvfname, FNAMEMAX, _T("\\defprnt.bin"));
		m_pcView->PrintEnvLoad(printenvfname);
	}
}

void CDialogWakuInput::SaveRecString(int crctcnt, FILE *crctfp)
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	textfname[FNAMEMAX];
	FILE	*textfp;
	TCHAR	grupname[RECMAX];
	TCHAR	papername[RECMAX];
	TCHAR	paperpath[FNAMEMAX];
	TCHAR	wakufname[FNAMEMAX];
	TCHAR	wakupath[FNAMEMAX];
	TCHAR	recbuf[RECMAX8];
	TCHAR	buf[RECMAX];
	TCHAR	strg[RECMAX];
	int		idx;

	senvgetexepath(exepath);
	wakuenvgetgrupname(grupname);
	wakuenvgetpapername(papername);
	wakuenvgetfullpaperpath(paperpath);

	SaveCrtWaku(crctcnt, paperpath, wakufname);	
	_stprintf_s(wakupath, _T("%s\\%s"), paperpath, wakufname);

	_stprintf_s(buf, _T("\"未\",\"%s\",\"%s\",\"%s\",\"1\",\""),
		grupname, papername, wakupath);
	_fputts(buf, crctfp);

	_stprintf_s(textfname, _T("%s\\string.lst"), exepath);
	_tfopen_s(&textfp, textfname, _T("r+b"));
	if(textfp == NULL){
		return;
	}
	idx = 0;
	while(1){
		if(m_nWakuMax <= idx){
			break;
		}
		if(fread(buf, RECMAX, 1, textfp) != 1){
			break;
		}
		_tcscpy_s(strg, buf);
		_stprintf_s(buf, _T("/%s"), strg);
		_fputts(buf, crctfp);
		idx++;
	}
	_stprintf_s(buf, _T("\"\n"));
	_fputts(buf, crctfp);

	fclose(textfp);
}

void CDialogWakuInput::SetString(LPTSTR LPTSTR)
{
	CEdit	*pcEdit;

	pcEdit = (CEdit*)GetDlgItem(IDC_EDIT1+m_nNextFocus);
	pcEdit->ReplaceSel(LPTSTR, FALSE);
}
