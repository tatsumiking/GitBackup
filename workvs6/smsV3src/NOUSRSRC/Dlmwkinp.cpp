// DlmWkInp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "io.h"

#include "DlmWkInp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	INPUTEDITMAX	10

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuInput ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	void poutploternameget(LPSTR plotername);
	void wakuenvgetgrupname(LPSTR grupname);
	void wakuenvgetpapername(LPSTR papername);
	void wakuenvgetfullpaperpath(LPSTR paperpath);
	void wakuenvgetselectwakuidx(int *idx);
	void wakuenvloadoutputenv(LPSTR wakuloadfname);
	void wakuenvlistgetwakuname(int slctidx, LPSTR wakufname);
	int DispatchWindows(HWND hWin);
}

CDialogWakuInput::CDialogWakuInput(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogWakuInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogWakuInput)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	m_pcView = (CTonView*)pParent;
}

void CDialogWakuInput::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuInput)
	DDX_Control(pDX, IDC_DISP, m_btnDisp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWakuInput, CMyDialog)
	//{{AFX_MSG_MAP(CDialogWakuInput)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DISP, OnDisp)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SETBTN, OnSetbtn)
	ON_BN_CLICKED(IDC_NEXTBTN, OnNextpage)
	ON_BN_CLICKED(IDC_PREBTN, OnBeforpage)
	ON_BN_CLICKED(IDC_CRCTOUT, OnCrctout)
	ON_BN_CLICKED(IDC_CRCTSAVE, OnCrctsave)
	ON_BN_CLICKED(IDC_CRCTDEL, OnCrctdel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuInput メッセージ ハンドラ

int CDialogWakuInput::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if (CMyDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください
	return 0;
}

BOOL CDialogWakuInput::OnInitDialog() 
{
	CRect	rect;
	int		idx;

	CMyDialog::OnInitDialog();

	rect.left = 32;
	rect.right = 320;
	rect.top = 2;
	rect.bottom = rect.top + 24;
	for(idx = 0; idx < 10; idx++){
		m_pcEdit[idx] = new CMyEdit();
		m_pcEdit[idx]->Create(EDITSTYLE, rect, this, IDC_EDIT1+idx);
		m_pcEdit[idx]->SetIMEMode(ON);
		rect.top = rect.bottom + 2;
		rect.bottom = rect.top + 24;
	}
	
	m_nCrtIdxTop = 0;
	m_nNextFocus = -1;
	// TODO: この位置に初期化の補足処理を追加してください
	GetWakuMax();
	LoadCrtWakuText();
	m_fFirstFlag = TRUE;
	//WakuSetFocus(0);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogWakuInput::OnDestroy() 
{
	int		idx;

	for(idx = 0; idx < 10; idx++){
		delete(m_pcEdit[idx]);
	}
	CDialog::OnDestroy();
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
}

void CDialogWakuInput::OnDisp() 
{
	// TODO: この位置に初期化の補足処理を追加してください
	SaveCrtWakuText();
	m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);
	KeyIn(VK_DOWN, IDC_EDIT1+m_nNextFocus);
	//KeyIn(VK_RETURN, IDC_EDIT1+m_nNextFocus);
	//WakuSetFocus();
}

void CDialogWakuInput::LoadCrtWakuText()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	long	seekp;
	char	buf[RECMAX+16];
	char	strg[RECMAX+16];
	int		idx;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\string.lst", exepath);
	if((fp = fopen(fname, "rb")) == NULL){
		return;
	}
	seekp = m_nCrtIdxTop * RECMAX;
	fseek(fp, seekp, 0);
	for(idx = 0; idx < 10; idx++){
		if(fread(buf, RECMAX, 1, fp) != 1){
			break;
		}
		strcpy(strg, buf);
		SetDlgItemText(IDC_EDIT1+idx, strg);
	}
	strg[0] = '\0';
	for(;idx < 10; idx++){
		SetDlgItemText(IDC_EDIT1+idx, strg);
	}
	fclose(fp);
}

void CDialogWakuInput::SaveCrtWakuText()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	strg[RECMAX+16];
	long	seekp;
	int		idx;

	idx = 0;
	UpdateData(TRUE);
	senvgetexepath(exepath);
	sprintf(fname, "%s\\string.lst", exepath);
	if((fp = fopen(fname, "r+b")) != NULL){
		seekp = m_nCrtIdxTop * RECMAX;
		fseek(fp, seekp, 0);
		idx = 0;
		for(idx = 0; idx < 10; idx++){
			memset(strg, ' ', RECMAX);
			strg[RECMAX-1] = 0x0a;
			strg[RECMAX-2] = 0x0d;
			GetDlgItemText(IDC_EDIT1+idx, strg, RECMAX);
			fwrite(strg, RECMAX, 1, fp);
		}
		fclose(fp);
	}
}

void CDialogWakuInput::KeyIn(UINT nChar, UINT nID)
{
	int		next;

	switch(nChar){
	case VK_RETURN:
		break;
	case VK_DOWN:
		SaveCrtWakuText();
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
		Invalidate();
		//m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);
		break;
 	case VK_UP:
		SaveCrtWakuText();
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
		Invalidate();
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

void CDialogWakuInput::OnSetbtn() 
{
	SaveCrtWakuText();
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

void CDialogWakuInput::OnExit() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_pcView->m_nCrtWaku = -1;
	m_pcView->CommandExec(ENDEVENT, 0, 0);
}

void CDialogWakuInput::GetWakuMax()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		idx;
	char	buf[RECMAX+16];

	senvgetexepath(exepath);
	sprintf(fname, "%s\\string.lst", exepath);
	if((fp = fopen(fname, "rb")) == NULL){
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
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	char	numstr[QRECMAX];

	if(m_fFirstFlag == TRUE){
		m_fFirstFlag = FALSE;
		m_pcView->m_nCrtWaku = 0;
		WakuSetFocus(0);
	}
	rect.left = 2;
	rect.right = 30;
	rect.top = 2;
	rect.bottom = rect.top + 24;
	CBrush *pcBrush = new CBrush(RGB(128, 128, 128));
	for(idx = 0; idx < 10; idx++){
		dc.FillRect(&rect,pcBrush);
		sprintf(numstr, "%03d", m_nCrtIdxTop + idx + 1);
		dc.TextOut(rect.left, rect.top, numstr);
		rect.top = rect.bottom + 2;
		rect.bottom = rect.top + 24;
	}
	delete pcBrush;
}

void CDialogWakuInput::WakuSetFocus(int next)
{
	CDC	*pcDC;

	m_pcEdit[next]->SetFocus();
	m_pcView->m_nCrtWaku = m_nNextFocus+m_nCrtIdxTop;
	pcDC = m_pcView->GetDC();
	m_pcView->DrawSelectWaku(pcDC);
	m_pcView->ReleaseDC(pcDC);
}

void CDialogWakuInput::OnNextpage() 
{
	SaveCrtWakuText();
	m_nCrtIdxTop = m_nCrtIdxTop + 10;
	if(m_nCrtIdxTop + 10 > m_nWakuMax){
		m_nCrtIdxTop = m_nWakuMax - 10;
	}
	LoadCrtWakuText();
	WakuSetFocus(0);
	Invalidate();
}

void CDialogWakuInput::OnBeforpage() 
{
	SaveCrtWakuText();
	m_nCrtIdxTop = m_nCrtIdxTop - 10;
	if(m_nCrtIdxTop < 0){
		m_nCrtIdxTop = 0;
	}
	LoadCrtWakuText();
	WakuSetFocus(0);
	Invalidate();
}

void CDialogWakuInput::OnCrctout() 
{
	SaveCrtWakuText();
	m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);
	ShowWindow(SW_HIDE);
	m_pcView->CallCrctOut();
	ShowWindow(SW_SHOW);
}

void CDialogWakuInput::OnCrctsave() 
{
	SaveCrtWakuText();
	m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);
	m_pcView->CallCrctSave();
}

void CDialogWakuInput::OnCrctdel() 
{
	m_pcView->CallCrctDel();
}
