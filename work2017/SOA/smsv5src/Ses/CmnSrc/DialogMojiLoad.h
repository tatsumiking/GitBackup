
#pragma once

// DlgMjLd.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiLoad �_�C�A���O

//C �����ďo�_�C�A���O�N���X
class CDialogMojiLoad : public CDialog
{
// �R���X�g���N�V����
public:
	void SetDialogTitle(LPTSTR lpTitle);
	void FontCBInit();
	CDialogMojiLoad(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	LPTSTR m_lpTitle;

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogMojiLoad)
	enum { IDD = IDD_MOJILOAD };
	CString	m_csFontName;
	CString	m_csMojiCode;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogMojiLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogMojiLoad)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

