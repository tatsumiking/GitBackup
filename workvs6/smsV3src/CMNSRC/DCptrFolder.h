#if !defined(DCPTRFOLDER_H)
#define DCPTRFOLDER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCptrFolder.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogCptrFolder �_�C�A���O

class CDialogCptrFolder : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogCptrFolder(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	DBL		m_dXTime, m_dYTime;
	//{{AFX_DATA(CDialogCptrFolder)
	enum { IDD = IDD_CPTR_FOLDER };
	CString	m_csSrcFolder;
	CString	m_csDstFolder;
	CString	m_csSrcXSize;
	CString	m_csSrcYSize;
	CString	m_csDstXSize;
	CString	m_csDstYSize;
	//}}AFX_DATA


// �I�[�o�[���C�h
public:
	static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogCptrFolder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void InitElement();
	void GetSelectDir(LPSTR lpTitle, LPSTR lpPath);

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogCptrFolder)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpnsrcxsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpnsrcysize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpndstxsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpndstysize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtndstfolder();
	afx_msg void OnBtnsrcfolder();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DCPTRFOLDER_H)
