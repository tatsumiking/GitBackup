
#pragma once

// DlgWkGrp.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuGrup �_�C�A���O

class CDialogWakuGrup : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogWakuGrup(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	TCHAR	m_szGrupPath[FNAMEMAX];
	//{{AFX_DATA(CDialogWakuGrup)
	enum { IDD = IDD_WAKU_GRUP };
	CString	m_csWakuGrupPath;
	CString	m_csWakuGrupName;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogWakuGrup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void CheckAddWakuGrup(LPTSTR gruppath);
	void InitWakuGrupCombo();

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWakuGrup)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeWkgrupname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B


