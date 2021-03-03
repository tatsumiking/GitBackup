// prtctDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <io.h>
#include "prtct.h"
#include "prtctDlg.h"
#include "WINIOCTL.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	DEFSTR	"KEYSTRINGSOA"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラがありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrtctDlg ダイアログ

CPrtctDlg::CPrtctDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrtctDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrtctDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPrtctDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrtctDlg)
		// メモ: この場所には ClassWizard によって DDX と DDV の呼び出しが追加されます。
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPrtctDlg, CDialog)
	//{{AFX_MSG_MAP(CPrtctDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNPRTCTSET, OnBtnprtctset)
	ON_BN_CLICKED(IDC_BTNPRTCTRESET, OnBtnprtctreset)
	ON_BN_CLICKED(IDC_BTNSET, OnBtnset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrtctDlg メッセージ ハンドラ

BOOL CPrtctDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	strcpy(m_szSetLabel, "");
	strcpy(m_szDelLabel, "");

	InitExePath(m_szExePath);
	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CPrtctDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CPrtctDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CPrtctDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPrtctDlg::OnBtnprtctset() 
{
	char	fname[FNAMEMAX];
	char	szWinDir[FNAMEMAX];
	char	buf[RECMAX+16];
	FILE	*fp;

	SetDriveNo();
	ReadLabel(buf);
	if(strcmp(DEFSTR, buf) == 0){
		//ObjectCopy();
		memset(buf, '\0', RECMAX);
		WriteLabel(m_szDelLabel);
		::GetSystemDirectory(szWinDir, FNAMEMAX);
		sprintf(fname, "%s\\winsleep.sys", szWinDir);
		fp = fopen(fname, "wb");
		memset(buf, '\0', 32);
		fwrite(buf, 1, 158, fp);
		fclose(fp);
	}
	else{
		MessageBox("このフロッピーはご使用になれません", "確認", MB_OK);
	}
	CDialog::OnOK();
}

void CPrtctDlg::OnBtnprtctreset() 
{
	char	fname[FNAMEMAX];
	char	szWinDir[FNAMEMAX];
	char	buf[RECMAX+16];
	FILE	*fp;

	SetDriveNo();
	::GetSystemDirectory(szWinDir, FNAMEMAX);
	sprintf(fname, "%s\\winsleep.sys", szWinDir);
	fp = fopen(fname, "r");
	if(fp != NULL){
		fclose(fp);
		unlink(fname);
		memset(buf, '\0', RECMAX);
		strcpy(buf, DEFSTR); 
		WriteLabel(m_szSetLabel);
	}
	else{
		MessageBox("キーを戻すことが出来ません", "確認", MB_OK);
	}
	CDialog::OnOK();
}

void CPrtctDlg::OnBtnset() 
{
	char	buf[RECMAX+16];

	SetDriveNo();
	memset(buf, '\0', RECMAX);
	strcpy(buf, DEFSTR); 
	WriteLabel(m_szSetLabel);
}

void CPrtctDlg::InitExePath(LPSTR lpExePath)
{
	int		len;

	len = ::GetModuleFileName(AfxGetInstanceHandle( ), lpExePath, FNAMEMAX);
	while(1){
		if(len <= 0){
			lpExePath[0] = '\0';
			break;
		}
		if(lpExePath[len] == ':' || lpExePath[len] == '\\'){
			lpExePath[len] = '\0';
			break;
		}
		len--;
	}
}

void CPrtctDlg::ReadLabel(LPSTR buf)
{
	char	szlabel[129];
	char	szsysname[129];
	DWORD	nmaxlen;
	DWORD	nvolno;
	DWORD	flag;

	memset(buf, '\0', RECMAX);
	GetVolumeInformation(m_szDriveName, szlabel, sizeof(szlabel)-1, &nvolno, &nmaxlen, &flag, szsysname, sizeof(szsysname)-1);
	if(strcmp(szlabel, m_szSetLabel) == 0){
		strcpy(buf, DEFSTR);
	}
}

void CPrtctDlg::WriteLabel(LPSTR buf)
{
	int		ret;

	ret = SetVolumeLabel(m_szDriveName, buf);
}

void CPrtctDlg::SetDriveNo()
{
	int		ret;
	int		no;
	char	fname[FNAMEMAX];

	m_nDriveNo = 0;
	strcpy(m_szDriveName,"A:\\");
	for(no = 1; no < 26; no++){
		/*
		ret = GetDriveType(m_szDriveName);
		if(ret == DRIVE_REMOVABLE){
			m_nDriveNo = no;
			break;
		}
		*/
		sprintf(fname, "%sprtct.exe", m_szDriveName);
		if(_access(fname, 0) == 0){
			break;
		}
		m_szDriveName[0]++;
	}
}

void CPrtctDlg::ObjectCopy()
{
}
