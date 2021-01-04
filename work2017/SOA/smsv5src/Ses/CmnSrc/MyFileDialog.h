#pragma once

// MyFileDl.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CMyFileDialog �_�C�A���O

class CMyFileDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CMyFileDialog)

public:
	virtual void OnFileNameChange();
	void SetBtnText(LPTSTR msg);
	void SetTitle(LPTSTR LPTSTR);
	void SetInitPath(LPTSTR LPTSTR);
	CMyFileDialog(BOOL bOpenFileDialog, // TRUE �Ȃ�� FileOpen�A FALSE �Ȃ�� FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);
	
	LPTSTR m_lpBtnMsg;
protected:
	//{{AFX_MSG(CMyFileDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

