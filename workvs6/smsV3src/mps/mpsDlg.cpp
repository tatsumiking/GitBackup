// mpsDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "mapi.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ

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
// CTonDlg ダイアログ
static const WCHAR BASED_CODE _szLicString[] =
    L"LEADTOOLS OCX Copyright (c) 1998 LEAD Technologies, Inc.";

CTonDlg::CTonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTonDlg)
	m_csSrcXDot = _T("0");
	m_csSrcYDot = _T("0");
	m_csDstXMM = _T("0");
	m_csDstYMM = _T("0");
	m_csFileName = _T("");
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CTonDlg::~CTonDlg()
{
	if(m_pcLead != NULL){
	    m_pcLead->DestroyWindow();
		delete m_pcLead;
	}
	if(m_pcLeadDlg != NULL){
		m_pcLeadDlg->DestroyWindow();
		delete m_pcLeadDlg;
	}
}

void CTonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTonDlg)
	DDX_Control(pDX, IDC_STIMG, m_stImg);
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Text(pDX, IDC_EDFILENAME, m_csFileName);
	DDX_Text(pDX, IDC_EDSRCXDOT, m_csSrcXDot);
	DDX_Text(pDX, IDC_EDSRCYDOT, m_csSrcYDot);
	DDX_Text(pDX, IDC_EDDSTXMM, m_csDstXMM);
	DDX_Text(pDX, IDC_EDDSTYMM, m_csDstYMM);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTonDlg, CDialog)
	//{{AFX_MSG_MAP(CTonDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNSEND, OnBtnsend)
	ON_BN_CLICKED(IDC_BTNREF, OnBtnref)
	ON_BN_CLICKED(IDC_BTNEND, OnBtnend)
	ON_BN_CLICKED(IDC_BTNRELOAD, OnBtnreload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTonDlg メッセージ ハンドラ

BOOL CTonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定

	InitExePath();
	InitEnvPath();
	m_pcLead = new CLead;
    BSTR lpLic = SysAllocString(_szLicString);
    m_pcLead->Create("", 0,CRect(0,0,50,50),this,0,NULL,FALSE,lpLic);
    m_pcLead->ShowWindow(SW_HIDE);
    SysFreeString(lpLic);
    UNLOCKSUPPORT(*m_pcLead); // ライセンスキーの解除
    m_pcLeadDlg = new CLTDlgOCXCtl;
    m_pcLeadDlg->Create("", 0, CRect(0,0,50,50),this,0,NULL,FALSE);
    m_pcLeadDlg->ShowWindow(SW_HIDE);

	return TRUE;
}

void CTonDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTonDlg::OnPaint() 
{
	CRect	rect;
	float	swd, shi;
	float	dsx, dsy, dwd, dhi;
	DBL		time;

	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	if (IsIconic())
	{
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
	}else{
		CDialog::OnPaint();
		GetLegalRect(&rect);
		if(m_csFileName.IsEmpty()){
			return;
		}
		swd = m_pcLead->GetBitmapWidth();
		shi = m_pcLead->GetBitmapHeight();
		dsx = (float)rect.left;
		dsy = (float)rect.top;
		if(swd < shi){
			dhi = (float)rect.Height();
			time = dhi / shi;
			dwd = (float)(swd * time);
		}else{
			dwd = (float)rect.Width();
			time = dwd / swd;
			dhi = (float)(shi * time);
		}
		m_pcLead->Render((long)(dc.m_hDC), dsx, dsy, dwd, dhi); 
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CTonDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTonDlg::OnBtnsend() 
{
	char	szSendEnvFile[FNAMEMAX];
	FILE	*fp;
	char	szBuf[RECMAX];
	char	szMailAdrs[RECMAX];
	float	dstwd, dsthi;
	char	szBmpFName[FNAMEMAX];
	char	szCmpFName[FNAMEMAX];
	char	szCmpName[QRECMAX];
	char	szMsg[QRECMAX];
	int		ret;

	sprintf(szSendEnvFile, "%s\\SendMail.env", m_szExePath);
	fp = fopen(szSendEnvFile, "r");
	if(fp == NULL){
		strcpy(szMailAdrs, "mobile@c-alfo.com");
	}else{
		fgets(szBuf, RECMAX, fp);
		szBuf[strlen(szBuf)-1] = '\0';
		strcpy(szMailAdrs, szBuf);
		fclose(fp);
	}
	ULONG (PASCAL *lpfnSendMail)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG);
	HMODULE hInstMail;
	MapiMessage message;
	MapiFileDesc fileDesc;
	strcpy(szCmpName, "test.cmp");
	sprintf(szBmpFName, "%s\\test.bmp", m_szTmpPath);
	sprintf(szCmpFName, "%s\\%s", m_szTmpPath, szCmpName);
	dstwd = (float)(atof(m_csDstXMM) / 25.4 * 150);	
	dsthi = (float)(atof(m_csDstYMM) / 25.4 * 150);
	dstwd = (((int)dstwd))/32*32;
	dsthi = (((int)dsthi))/32*32;
	m_pcLead->Size(dstwd, dsthi, RESIZE_NORMAL);
	//m_pcLead->Size(dstwd, dsthi, RESIZE_RESAMPLE);
	m_pcLead->Flip();
	m_pcLead->Save(szBmpFName, FILE_BMP, 1, 1, 0);
	m_pcLead->Flip();
	BmpCmpConvBytelen(szBmpFName, szCmpFName);
	sprintf(szBmpFName, "%s\\test2.bmp", m_szTmpPath);
	CmpBmpConvBytelen(szCmpFName, szBmpFName);
	memset(&fileDesc, 0, sizeof(fileDesc));

	fileDesc.nPosition = (ULONG)-1;
	fileDesc.lpszPathName = szCmpFName;
	fileDesc.lpszFileName = szCmpName;

	memset(&message, 0, sizeof(message));
	message.nFileCount = 1;
	message.lpFiles = &fileDesc;

	MapiRecipDesc aRecips;
	message.nRecipCount = 1;
	memset((void*)&aRecips, 0, sizeof(MapiRecipDesc));
	aRecips.ulRecipClass = 1;
	aRecips.lpszName = szMailAdrs;
	aRecips.lpszAddress = szMailAdrs;
	message.lpRecips = &aRecips;

	message.lpszSubject = m_szTitle;
	strcpy(szMsg, "お疲れ様です\n" );
	message.lpszNoteText = szMsg;

	hInstMail = LoadLibrary("MAPI32.DLL");
	(FARPROC&)lpfnSendMail = GetProcAddress(hInstMail, "MAPISendMail");
	ret = lpfnSendMail(0L, (ULONG)(m_hWnd),
		&message, MAPI_LOGON_UI|MAPI_DIALOG, 0L);
	FreeLibrary(hInstMail);
}

void CTonDlg::OnBtnref() 
{
	int		ret, len;
	CString	csXSize, csYSize;
	int		nXSize, nYSize;
	char	szPath[FNAMEMAX];

	m_csFileName = _T("");
	m_pcLeadDlg->SetEnableMethodErrors(FALSE);
	m_pcLeadDlg->SetDialogTitle("画像読込");
	m_pcLeadDlg->SetFilter("画像ﾌｧｲﾙ|*.jpg;*.bmp");
	m_pcLeadDlg->SetUIFlags( DLG_FO_FILEINFO |
									DLG_FO_MULTIPAGE |
									DLG_FO_SHOWPREVIEW |
									DLG_FO_SHOWSTAMP |
									DLG_FO_95STYLE );
	ret = m_pcLeadDlg->FileOpen((long)this->m_hWnd);
	if(ret == 0){
		m_csFileName = m_pcLeadDlg->GetFileName();
		len = m_csFileName.GetLength();
		csXSize = m_csFileName.Mid(len-6, 2);
		csYSize = m_csFileName.Mid(len-8, 2);
		nXSize = atoi(csXSize) * 10;
		nYSize = atoi(csYSize) * 10;
		m_csSrcXDot.Format("%d", nXSize);
		m_csSrcYDot.Format("%d", nYSize);
		m_csDstXMM.Format("%d", nXSize);
		m_csDstYMM.Format("%d", nYSize);
		strcpy(szPath, m_csFileName.GetBuffer(FNAMEMAX));
		len = GetParentPath(szPath);
		strcpy(szPath, m_csFileName.GetBuffer(FNAMEMAX));
		strcpy(m_szTitle, &(szPath[len+1]));
	}
	UpdateData(FALSE);
	SelecttionBmp();
}

void CTonDlg::OnBtnreload() 
{
	if(m_csFileName.IsEmpty()){
		return;
	}
	UpdateData(TRUE);
	SelecttionBmp();
}

void CTonDlg::OnBtnend() 
{
	CDialog::OnOK();
}

void CTonDlg::SelecttionBmp()
{
	int		nSX, nSY;
	int		nXSize, nYSize;
	int		srcysize;

	nXSize = atoi(m_csSrcXDot);
	nYSize = atoi(m_csSrcYDot);
	m_pcLead->SetBitmap(0);
	m_pcLead->Load(m_csFileName, 0, 0, -1);
	srcysize = (int)(m_pcLead->GetBitmapHeight());
	nSX = 0;
	nSY = srcysize-nYSize;
	m_pcLead->Trim((float)nSX, (float)nSY, (float)nXSize, (float)nYSize);
	this->Invalidate(TRUE);
}

void CTonDlg::GetLegalRect(CRect *pcRect)
{
	CRect	toprect;

	m_stImg.GetWindowRect(pcRect);
	m_stTop.GetWindowRect(&toprect);
	pcRect->left = pcRect->left - toprect.left;
	pcRect->right = pcRect->right - toprect.left;
	pcRect->top = pcRect->top - toprect.top;
	pcRect->bottom = pcRect->bottom - toprect.top;

}

void CTonDlg::InitExePath()
{
	int		len;

	len = ::GetModuleFileName(AfxGetInstanceHandle( ), m_szExePath, FNAMEMAX);
	while(1){
		if(len <= 0){
			m_szExePath[0] = '\0';
			break;
		}
		if(m_szExePath[len] == ':' || m_szExePath[len] == '\\'){
			m_szExePath[len] = '\0';
			break;
		}
		len--;
	}
}

void CTonDlg::InitEnvPath()
{
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	szBuf[RECMAX];
	int		len;

	sprintf(m_szTmpPath, "%s\\temp", m_szExePath);
	sprintf(fname, "%s\\ses_sys.env", m_szExePath);
	if((fp = fopen(fname, "r")) != NULL){
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		fgets(szBuf, RECMAX, fp);
		strcpy(m_szTmpPath, szBuf);
		m_szTmpPath[strlen(m_szTmpPath)-1] = '\0';
		fclose(fp);
	}
	mkdir(m_szTmpPath);
}

int CTonDlg::GetParentPath(LPSTR lpPath)
{
	LPSTR	sp;
	LPSTR	lpLast;
	int		idx, lastidx;
	int		len;
	UINT	hc;

	lastidx = 0;
	sp = lpPath;
	len = strlen(lpPath);
	if(len <= 2 && lpPath[1] == ':'){
		return(2);
	}
	lpLast = 0;
	if(lpPath[1] == ':'){
		lpLast = &(lpPath[2]);
		lastidx = 2;
	}
	idx = 0;
	while(1){
		if((*sp) == '\0'){
			break;
		}
		hc = (*sp) & 0x00ff;
		if(IsKanji(hc) == ON){
			sp += 2;
			idx += 2;
		}
		else{
			if(*sp == '\\'){
				lpLast = sp;
				lastidx = idx;
			}
			sp++;
			idx++;
		}
	}
	if(lpLast == NULL){
		*lpPath = '\0';
		return(0);
	}
	*lpLast = '\0';
	return(lastidx);
}

void CTonDlg::BmpCmpConvBytelen(LPSTR lpBmpFName, LPSTR lpCmpFName)
{
	FILE	*ifp;
	FILE	*ofp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	RGBQUAD	clr[2];
	int		x, y;
	int		xdot, ydot, xbyte;
	BYTE	imgSrc[RECMAX2];
	BYTE	imgDst[RECMAX2];
	int		btDst, cnt, flag;
	int		btSrc;

	ifp = fopen(lpBmpFName, "rb");
	if(ifp == NULL){
		return;
	}
	ofp = fopen(lpCmpFName, "wb");
	if(ofp == NULL){
		fclose(ifp);
		return;
	}

	fread(&bmpfh, sizeof(BITMAPFILEHEADER), 1, ifp);
	fread(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, ifp);
	fread(clr, sizeof(RGBQUAD), 2, ifp);

	xdot = bmpinfo.bmiHeader.biWidth;
	ydot = bmpinfo.bmiHeader.biHeight;
	xbyte = xdot / 8;

	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, ofp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, ofp);
	fwrite(clr, sizeof(RGBQUAD), 2, ofp);
	for(y = 0; y < ydot; y++){
		fread(imgSrc, 1, xbyte, ifp);
		flag = 1;
		cnt = 0;
		btSrc = 0;
		btDst = 1;
		for(btSrc = 0; btSrc < xbyte; btSrc++){
			if(imgSrc[btSrc] == 0){
				if(flag == -1){
					cnt++;
				}else if(flag == 1){
					imgDst[btDst++] = 0xff;
					imgDst[btDst++] = cnt;
					cnt = 1;
				}else{
					cnt = 1;
				}
				flag = -1;
			}else if(imgSrc[btSrc] == 0xff){
				if(flag == 1){
					cnt++;
				}else if(flag == -1){
					imgDst[btDst++] = 0x00;
					imgDst[btDst++] = cnt;
					cnt = 1;
				}else{
					cnt = 1;
				}
				flag = 1;
			}else{
				if(flag == -1){
					imgDst[btDst++] = 0x00;
					imgDst[btDst++] = cnt;
					cnt= 0;
				}else if(flag == 1){
					imgDst[btDst++] = 0xff;
					imgDst[btDst++] = cnt;
					cnt = 0;
				}
				imgDst[btDst++] = imgSrc[btSrc];
				flag = 0;
			}
		}
		if(flag == -1){
			imgDst[btDst++] = 0x00;
			imgDst[btDst++] = cnt;
		}else if(flag == 1){
			imgDst[btDst++] = 0xff;
			imgDst[btDst++] = cnt;
		}
		imgDst[0] = btDst-1;
		fwrite(imgDst, 1, btDst, ofp);
	}
	fclose(ifp);
	fclose(ofp);
}

void CTonDlg::CmpBmpConvBytelen(LPSTR lpCmpFName, LPSTR lpBmpFName)
{
	FILE	*ifp;
	FILE	*ofp;
	BITMAPINFO	bmpinfo;
	BITMAPFILEHEADER	bmpfh;
	RGBQUAD	clr[2];
	int		xdot, ydot, xbyte;
	int		x, y;
	BYTE	imgSrc[RECMAX2];
	BYTE	imgDst[RECMAX2];
	int		nXCnt, nCnt;
	int		btSrc, btDst;
	int		i, j;

	ifp = fopen(lpCmpFName, "rb");
	if(ifp == NULL){
		return;
	}
	ofp = fopen(lpBmpFName, "wb");
	if(ofp == NULL){
		fclose(ifp);
		return;
	}

	fread(&bmpfh, sizeof(BITMAPFILEHEADER), 1, ifp);
	fread(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, ifp);
	fread(clr, sizeof(RGBQUAD), 2, ifp);

	xdot = bmpinfo.bmiHeader.biWidth;
	ydot = bmpinfo.bmiHeader.biHeight;
	xbyte = xdot / 8;

	fwrite(&bmpfh, sizeof(BITMAPFILEHEADER), 1, ofp);
	fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, ofp);
	fwrite(clr, sizeof(RGBQUAD), 2, ofp);
	for(y = 0; y < ydot; y++){
		fread(&nXCnt, 1, 1, ifp);
		nXCnt &= 0x00ff;
		fread(imgSrc, 1, nXCnt, ifp);
		memset(imgDst, 0xff, RECMAX2);
		btSrc = 0;
		btDst = 0;
		for(i = 0; i < nXCnt; i++){
			if(imgSrc[i] == 0xff){
				i++;
				nCnt = (imgSrc[i] & 0x00ff);
				for(j = 0; j < nCnt; j++){
					imgDst[btDst++] = 0xff;
				}
			}else if(imgSrc[i] == 0x00){
				i++;
				nCnt = (imgSrc[i] & 0x00ff);
				for(j = 0; j < nCnt; j++){
					imgDst[btDst++] = 0x00;
				}
			}else{
				imgDst[btDst++] = imgSrc[i];
			}
		}
		fwrite(imgDst, 1, xbyte, ofp);
	}
	fclose(ifp);
	fclose(ofp);
}

BOOL CTonDlg::IsKanji(UINT hc)
{
	if((0x0080 <= hc && hc <= 0x009f)
	|| (0x00e0 <= hc && hc <= 0x00ff)){
		return(TRUE);
	}
	else{
		return(FALSE);
	}
}
