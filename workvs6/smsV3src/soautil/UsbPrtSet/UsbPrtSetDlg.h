// UsbPrtSetDlg.h : ヘッダー ファイル
//

#if !defined(USBPRTSETDLG_H)
#define USBPRTSETDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"DskInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CUsbPrtSetDlg ダイアログ

extern "C" {
	typedef BOOL (__stdcall *LPGVNFVMP)(LPCSTR, LPSTR, DWORD);
	typedef BOOL (__stdcall *LPGVINF)(LPCSTR, LPSTR, DWORD, LPDWORD, LPDWORD, LPDWORD, LPSTR, DWORD);
};

class CUsbPrtSetDlg : public CDialog
{
// 構築
public:
	CUsbPrtSetDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
public:
	CTypedPtrList <CObList,CDskInfo*>	m_cDskInfoList;
protected:
	char	m_szExePath[FNAMEMAX];
	HICON m_hIcon;
	HMODULE	m_hModule;
	LPGVNFVMP	m_lpGetVolumeNameForVolumeMountPoint;
	LPGVINF		m_lpGetVolumeInformation;

	//{{AFX_DATA(CUsbPrtSetDlg)
	enum { IDD = IDD_USBPRTSET_DIALOG };
	CComboBox	m_cmbSrchKey;
	CString	m_csID;
	CString	m_csPath;
	CString	m_csSrchKey;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CUsbPrtSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void SaveDefKeyNo(long nID);
	void LoadDefKeyNo();
	int CheckUsbProtect(int nID);
	static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
protected:
	void InitSrchKey();
	void SetVol(LPSTR lpPath, LPSTR lpVol);
	void EndLibrary();
	void InitLibrary();
	long GetInterFaceName(LPSTR lpSrchKety, CStringArray *lpStrArry);
	BOOL GetVolFromDrv();
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CUsbPrtSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnBtnslct();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(USBPRTSETDLG_H)
