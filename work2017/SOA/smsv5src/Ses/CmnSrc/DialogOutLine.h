
#pragma once

// DlgOutLn.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogOutLine �_�C�A���O

class CDialogOutLine : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogOutLine(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	void SetTitle(LPTSTR lpTitle);

	LPTSTR	m_lpTitle;
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogOutLine)
	enum { IDD = IDD_OUTLINE };
	double	m_dOutLineMM;
	int		m_nOutLineKind;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogOutLine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogOutLine)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

