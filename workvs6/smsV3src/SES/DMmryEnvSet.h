#if !defined(DMMRYENVSET_H_)
#define DMMRYENVSET_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMmryEnvSet.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMemoryEnvSet �_�C�A���O

class CDialogMemoryEnvSet : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogMemoryEnvSet(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CTonApp*	m_pcApp;
	CMainFrame*	m_pcWnd;
	//{{AFX_DATA(CDialogMemoryEnvSet)
	enum { IDD = IDD_MEMOENVSET };
	CString	m_csMemoTempPath;
	CString	m_csMemoBackPath;
	CString	m_csMemoPhotoPath;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogMemoryEnvSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	void GetSelectDir(LPSTR lpTitle, LPSTR lpPath);

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogMemoryEnvSet)
	afx_msg void OnBtnmemobackpath();
	afx_msg void OnBtnmemophotopath();
	afx_msg void OnBtnmemotemppath();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DMMRYENVSET_H_)
