#if !defined(AFX_DLGDIRSL_H__83801882_1AA2_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGDIRSL_H__83801882_1AA2_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgDirSl.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogDirSlct �_�C�A���O

//C �t�H���_�[�I���_�C�A���O�N���X
class CDialogDirSlct : public CDialog
{
// �R���X�g���N�V����
public:
	void SetInitPath(LPSTR path);
	void DirlistUpdate();
	char m_szPathName[FNAMEMAX];
	CDialogDirSlct(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogDirSlct)
	enum { IDD = IDD_DIRSLCT };
	CString	m_csSlctDir;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogDirSlct)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogDirSlct)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDirlist();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGDIRSL_H__83801882_1AA2_11D2_A0A2_444553540000__INCLUDED_)
