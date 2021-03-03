#if !defined(DLGTXTIN_H)
#define DLGTXTIN_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgTxtIn.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogTextInput �_�C�A���O

class CDialogTextInput : public CMyDialog
{
// �R���X�g���N�V����
public:
	CDialogTextInput(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

public:
	void SetTitleText(LPSTR lpTitleText);
	void SetDialogTitle(LPSTR lpTitle);
// �_�C�A���O �f�[�^
public:

	LPSTR	m_lpTitle;
	LPSTR	m_lpTitleText;
	int		m_nIMEMode;	
	//{{AFX_DATA(CDialogTextInput)
	enum { IDD = IDD_TEXTINPUT };
	CStatic	m_stTitle;
	CMyEdit	m_edText;
	CString	m_csText;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogTextInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogTextInput)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DLGTXTIN_H)
