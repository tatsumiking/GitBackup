
#pragma once

// DialogEditBar.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogEditBar �_�C�A���O

//C ������ύX�o�[�N���X
class CDialogEditBar : public CDialogBar
{
// �R���X�g���N�V����
public:
	CDialogEditBar(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogEditBar)
	enum { IDD = IDD_EDITBAR };
	CString	m_csEditStrg;
	//}}AFX_DATA


// �I�[�o�[���C�h
	HIMC m_hImc;
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogEditBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogEditBar)
	afx_msg void OnSetfocusEditstrg();
	afx_msg void OnChangeEditstrg();
	afx_msg void OnKillfocusEditstrg();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

