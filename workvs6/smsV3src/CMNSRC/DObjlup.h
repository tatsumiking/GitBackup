#if !defined(AFX_DOBJLUP_H)
#define AFX_DOBJLUP_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DObjLup.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogObjLineup �_�C�A���O

class CDialogObjLineup : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogObjLineup(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

public:
	int m_nMode;
	int	m_nSlctMode;
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogObjLineup)
	enum { IDD = IDD_OBJ_LINEUP };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogObjLineup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void EndProcess();
	void SetModal();
	void SetModaless();
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogObjLineup)
	afx_msg void OnMode1();
	afx_msg void OnMode2();
	afx_msg void OnMode3();
	afx_msg void OnMode4();
	afx_msg void OnMode5();
	afx_msg void OnMode6();
	afx_msg void OnMode7();
	afx_msg void OnMode8();
	afx_msg void OnMode9();
	afx_msg void OnMode10();
	afx_msg void OnMode11();
	afx_msg void OnMode12();
	afx_msg void OnMode13();
	afx_msg void OnMode14();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DOBJLUP_H)
