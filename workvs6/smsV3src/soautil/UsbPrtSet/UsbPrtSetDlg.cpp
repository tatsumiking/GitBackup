// UsbPrtSetDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <afxcoll.h>
#include <direct.h>
#include "UsbPrtSet.h"
#include "UsbPrtSetDlg.h"
#include "setupapi.h"
#include "Shlwapi.h"
#include "UsbPrt.h"

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
// CUsbPrtSetDlg ダイアログ

CUsbPrtSetDlg::CUsbPrtSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsbPrtSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUsbPrtSetDlg)
	m_csID = _T("0");
	m_csPath = _T("A:\\");
	m_csSrchKey = _T("");
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUsbPrtSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUsbPrtSetDlg)
	DDX_Control(pDX, IDC_CMBSRCHKEY, m_cmbSrchKey);
	DDX_Text(pDX, IDC_EDID, m_csID);
	DDX_Text(pDX, IDC_EDPATH, m_csPath);
	DDX_CBString(pDX, IDC_CMBSRCHKEY, m_csSrchKey);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUsbPrtSetDlg, CDialog)
	//{{AFX_MSG_MAP(CUsbPrtSetDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNSLCT, OnBtnslct)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsbPrtSetDlg メッセージ ハンドラ

BOOL CUsbPrtSetDlg::OnInitDialog()
{
	char	szDrv[QRECMAX];
	int		idx;
	int		ret;

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
	
	LoadDefKeyNo();

	InitSrchKey();

	strcpy(szDrv,"A:\\");
	for(idx = 3; idx < 26; idx++){
		szDrv[0] = 'A' + idx;
		ret = GetDriveType(szDrv);
		if(ret == DRIVE_REMOVABLE){
			break;
		}
	}
	m_csPath = _T(szDrv);
	UpdateData(FALSE);
	return TRUE;
}

void CUsbPrtSetDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CUsbPrtSetDlg::OnPaint() 
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
HCURSOR CUsbPrtSetDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUsbPrtSetDlg::OnOK() 
{
	long	nID;
	char	szVol[QRECMAX];
	char	szPath[RECMAX];
	char	szMsg[RECMAX];
	char	szKeyFileName[FNAMEMAX];
	FILE	*fp;
	char	szWriteBuf[TEMPBUFMAX];
	char	szKey[RECMAX4];
	CStringArray	cStrArry;
	CString	csStr;
	int		idx, setidx;
	int		add, sumcrt;

	UpdateData(TRUE);
	strcpy(szKey, m_csSrchKey.GetBuffer(RECMAX4));
	strcpy(szPath, m_csPath.GetBuffer(QRECMAX));
	nID = atoi(m_csID);
	strcpy(szVol, "USBPROTECT");
	SetVol(szPath, szVol);
	sprintf(szKeyFileName, "%ssoabackupwaku", szPath);
	mkdir(szKeyFileName);
	sprintf(szKeyFileName, "%s.system", szPath);
	mkdir(szKeyFileName);
	sprintf(szKeyFileName, "%s.system\\.system%08ld", szPath, nID);
	fp = fopen(szKeyFileName, "wb");
	if(fp == NULL){
		sprintf(szMsg, "「.system%08ld」にプロテクトを設定できません", nID);
		::AfxMessageBox(szMsg, MB_OK);
		return;
	}
	srand((unsigned)time( NULL ));
	for(idx = 0; idx < TEMPBUFMAX; idx++){
		szWriteBuf[idx] = (rand() % 96) + ' ';
	}
	//strcpy(szKey, "usbstor#disk&ven_&prod_usb_flash_memory&rev_pmap#");
	//strcpy(szKey, "usbstor#disk&ven_pqi&prod_intelligentstick");
	GetInterFaceName(szKey, &cStrArry);
	idx = cStrArry.GetSize();
	if(idx != 1){
		::AfxMessageBox("USBメモリーが正しく接続されていません", MB_OK);
	}
	csStr = cStrArry.GetAt(0);
	strcpy(szKey, csStr.GetBuffer(2048));
	idx = nID % 256;
	setidx = 0;	
	sumcrt = 0;
	while(1){
		if(strncmp(&(szKey[idx]), "53f56307-b6bf-11d0-94f2-00a0c91efb8b", 26) == 0){
			szWriteBuf[idx] = 'T';
			break;
		}
		if(szWriteBuf[idx] == 'T'){
			szWriteBuf[idx] = 'S';
		}
		add = (szWriteBuf[idx]) % 10 + 4;
		sumcrt += add;
		szWriteBuf[8050-sumcrt] = szKey[idx];
		TRACE("sumcrt %d\n", sumcrt);
		idx++;
	}
	fwrite(szWriteBuf, 1, TEMPBUFMAX, fp);
	fclose(fp);
	if(CheckUsbProtect(nID) == -1){
		sprintf(szMsg, "プロテクトの設定に失敗しました\n一度「.system%08ld」ファイルを削除して再実行してください", nID);
		::AfxMessageBox(szMsg, MB_OK);
	}else{
		::AfxMessageBox("プロテクトの確認が出来ました\n", MB_OK);
	}

	SaveDefKeyNo(nID);

	CDialog::OnOK();
}

#define HD_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) static const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 }}

HD_DEFINE_GUID(GUID_DEVINTERFACE_USBDISK, 0x53f56307, 0xb6bf, 0x11d0
			   , 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);

// インターフェース名をN番目の取得する
long CUsbPrtSetDlg::GetInterFaceName(LPSTR lpSrchKety, CStringArray *lpStrArry)
{
	PSP_DEVICE_INTERFACE_DETAIL_DATA	interface_detail;
	SP_DEVICE_INTERFACE_DATA	devinterface;
	SP_DEVINFO_DATA	devinfo;
	GUID		intfce;
	HDEVINFO	devs;
	DWORD		devcount;
	DWORD		size;
	int			ret;
	DWORD		dataType;
	CString		*lpcsStr;
    char	szDriverkey[2048];
    char	szInterfacename[2048];
	
	intfce = GUID_DEVINTERFACE_USBDISK;
	devs = SetupDiGetClassDevs(&intfce, 0, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if (devs == INVALID_HANDLE_VALUE) {
		return(FALSE);
	}
	devcount = 0;
	devinterface.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	while(1){
		ret = SetupDiEnumDeviceInterfaces(devs, 0, &intfce, devcount, &devinterface);
		if(ret == FALSE){
			break;
		}
		devcount++;
		size = 0;
		SetupDiGetDeviceInterfaceDetail(devs, &devinterface, 0, 0, &size, 0);
		devinfo.cbSize = sizeof(SP_DEVINFO_DATA);
		interface_detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)calloc(1, size);
		if(interface_detail){
			interface_detail->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
			devinfo.cbSize = sizeof(SP_DEVINFO_DATA);
			ret = SetupDiGetDeviceInterfaceDetail(devs, &devinterface, interface_detail, size, 0, &devinfo);
			if(ret == 0){
				free(interface_detail);
				break;
			}
			strcpy(szInterfacename, interface_detail->DevicePath);
			free(interface_detail);
			size = sizeof(szDriverkey);
			szDriverkey[0] = 0;
			ret = SetupDiGetDeviceRegistryProperty(devs, &devinfo, SPDRP_DRIVER, &dataType, (LPBYTE)szDriverkey, size, 0);
			if(ret == 0){
				break;
			}
			TRACE(szInterfacename);
			TRACE("\n");
			if(StrStr(szInterfacename, lpSrchKety)){
				lpStrArry->Add(szInterfacename);
			}
		}
	}
	SetupDiDestroyDeviceInfoList(devs);
	return(TRUE);
}

// ドライブボリューム名を取得する
BOOL CUsbPrtSetDlg::GetVolFromDrv()
{
	char	szDrv[QRECMAX];
	char	szVol[FNAMEMAX];
	char	szMsg[RECMAX];
	BOOL bRet;
	CDskInfo	*pcDskInfo;
	DWORD	nVolNo;
	DWORD	nMaxLen;
	DWORD	nSysFlag;
	char	szSysName[FNAMEMAX];

	strcpy(szDrv, "A:\\");
	for ( int i = 0; i < 25; i++ )
	{
		szDrv[0] = 'A' + i;
		bRet = m_lpGetVolumeNameForVolumeMountPoint(szDrv, szVol, FNAMEMAX);
		if( bRet ){
			pcDskInfo = new CDskInfo();
			pcDskInfo->m_csDrv = _T(szDrv);
			pcDskInfo->m_csVol = _T(szVol);
			m_cDskInfoList.AddTail(pcDskInfo);
			sprintf(szMsg, "<%s><%s>", szDrv, szVol);
			//::AfxMessageBox(szMsg, MB_OK);
		}
		bRet = m_lpGetVolumeInformation(szDrv, szVol, FNAMEMAX, &nVolNo, &nMaxLen, &nSysFlag, szSysName, FNAMEMAX);
		if( bRet ){
			sprintf(szMsg, "<%s><%s>", szDrv, szSysName);
			//::AfxMessageBox(szMsg, MB_OK);
		}
	}
	return TRUE;
}

// ライブラリー関数ポインターの初期化
void CUsbPrtSetDlg::InitLibrary()
{
	m_hModule = LoadLibrary("kernel32.dll");
	m_lpGetVolumeNameForVolumeMountPoint = (LPGVNFVMP)GetProcAddress(m_hModule, "GetVolumeNameForVolumeMountPointA");
	m_lpGetVolumeInformation = (LPGVINF)GetProcAddress(m_hModule, "GetVolumeInformationA");
}

// ライブラリーの開放
void CUsbPrtSetDlg::EndLibrary()
{
	FreeLibrary(m_hModule);
}

void CUsbPrtSetDlg::SetVol(LPSTR lpPath, LPSTR lpVol)
{
	SetVolumeLabel(lpPath, lpVol);
}

void CUsbPrtSetDlg::OnBtnslct() 
{
	char	szPath[QRECMAX];
	BROWSEINFO	bInfo;
	LPITEMIDLIST	lpFoldPointer;

	strcpy(szPath, m_csPath.GetBuffer(QRECMAX));
	szPath[strlen(szPath)-1] = '\0';
	memset(&bInfo, 0, sizeof(BROWSEINFO));
	bInfo.hwndOwner = this->m_hWnd;
	bInfo.pidlRoot = 0;
	bInfo.lpszTitle = "USBドライブを指定してください";
	bInfo.ulFlags = 1;
	bInfo.lpfn   = BrowseCallbackProc; // コールバーク関数
	bInfo.lParam = (LPARAM)(szPath);       // 初期フォルダ
	lpFoldPointer = SHBrowseForFolder(&bInfo);
	SHGetPathFromIDList(lpFoldPointer, szPath);
	//strcat(szPath, "\\");
	m_csPath = _T(szPath);
	UpdateData(FALSE);
}

void CUsbPrtSetDlg::InitSrchKey()
{
	char	szKey[RECMAX4];
	CStringArray	cStrArry;
	int		max, idx;
	CString	csStr;
	int		spos, epos, len;
	CString	csKeyStr;

	strcpy(szKey, "usbstor");
	GetInterFaceName(szKey, &cStrArry);
	max = cStrArry.GetSize();
	for(idx = 0; idx < max; idx++){
		csStr = cStrArry.GetAt(idx);
		len = csStr.GetLength();
		spos = csStr.Find(_T("#"), 0);
#ifdef _DEBUG
		epos = csStr.Find(_T("{"), spos+1);
#else
		epos = csStr.Find(_T("#"), spos+1);
#endif
		csKeyStr = csStr.Mid(spos+1, epos-spos-1);
		m_cmbSrchKey.AddString(csKeyStr);
	}
	m_cmbSrchKey.GetLBText(0, m_csSrchKey);
}

int CALLBACK CUsbPrtSetDlg::BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if((BFFM_INITIALIZED == uMsg) && lpData){
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
	}
	return 0;
}

int CUsbPrtSetDlg::CheckUsbProtect(int nID)
{
	long	nRet;
	HMODULE		hModule;
	LPUSBPRTCHK	fncUsbPrtCheck;

	hModule = LoadLibrary("UsbPrt.dll");
	fncUsbPrtCheck = (LPUSBPRTCHK)GetProcAddress(hModule, "Check");
	if(fncUsbPrtCheck == NULL){
		::AfxMessageBox("「UsbPrt.dll」が確認できません", MB_OK);
		return(-1);
	}
	nRet = fncUsbPrtCheck(nID);
	if(nRet == nID){
		return(TRUE);
	}else{
		return(-1);
	}
}

void CUsbPrtSetDlg::LoadDefKeyNo()
{
	int		len;
	char	szFileName[FNAMEMAX];
	char	szBuf[RECMAX];
	long	nID;
	FILE	*fp;

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
	nID = 0;
	sprintf(szFileName, "%s\\defkey.txt", m_szExePath);
	fp = fopen(szFileName, "r");
	if(fp != NULL){
		fgets(szBuf, RECMAX, fp);
		fclose(fp);
		nID = atoi(szBuf);
	}
	m_csID.Format("%ld", nID);
}

void CUsbPrtSetDlg::SaveDefKeyNo(long nID)
{
	char	szFileName[FNAMEMAX];
	FILE	*fp;

	sprintf(szFileName, "%s\\defkey.txt", m_szExePath);
	fp = fopen(szFileName, "w");
	if(fp != NULL){
		fprintf(fp, "%ld\n", nID);
		fclose(fp);
	}
}
