// UsbPrtSetDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(USBPRTSETDLG_H)
#define USBPRTSETDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"DskInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CUsbPrtSetDlg �_�C�A���O

extern "C" {
	typedef BOOL (__stdcall *LPGVNFVMP)(LPCSTR, LPSTR, DWORD);
	typedef BOOL (__stdcall *LPGVINF)(LPCSTR, LPSTR, DWORD, LPDWORD, LPDWORD, LPDWORD, LPSTR, DWORD);
};

class CUsbPrtSetDlg : public CDialog
{
// �\�z
public:
	CUsbPrtSetDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
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

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CUsbPrtSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
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
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(USBPRTSETDLG_H)
