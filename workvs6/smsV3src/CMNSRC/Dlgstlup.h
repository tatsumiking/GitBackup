#if !defined(AFX_DLGSTLUP_H__5D2C5F22_36E7_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGSTLUP_H__5D2C5F22_36E7_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgStLup.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgLineup �_�C�A���O

class CDialogStrgLineup : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogStrgLineup(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	int	m_nSlctMode;
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogStrgLineup)
	enum { IDD = IDD_STRG_LINEUP };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogStrgLineup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogStrgLineup)
	afx_msg void OnMode1();
	afx_msg void OnMode2();
	afx_msg void OnMode3();
	afx_msg void OnMode4();
	afx_msg void OnMode5();
	afx_msg void OnMode6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGSTLUP_H__5D2C5F22_36E7_11D2_A0A2_444553540000__INCLUDED_)
