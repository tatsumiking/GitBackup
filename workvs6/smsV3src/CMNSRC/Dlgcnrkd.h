#if !defined(AFX_DLGCNRKD_H__F8B40F02_322E_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGCNRKD_H__F8B40F02_322E_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgCnrKd.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogConerKind �_�C�A���O

//C �x�_�ݒ�_�C�A���O�N���X
class CDialogConerKind : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogConerKind(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	void SetDialogTitle(LPSTR lpTitle);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogConerKind)
	enum { IDD = IDD_CONER_KIND };
	CComboBox	m_cbConerKind;
	CString	m_csConerKind;
	//}}AFX_DATA
	int		m_nConerKind;
	LPSTR	m_lpTitle;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogConerKind)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogConerKind)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGCNRKD_H__F8B40F02_322E_11D2_A0A2_444553540000__INCLUDED_)

