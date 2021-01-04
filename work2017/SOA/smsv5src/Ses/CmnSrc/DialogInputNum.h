
#pragma once

// DlgInNum.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogInputNum �_�C�A���O
class CTonView;

//C ���l���̓_�C�A���O�N���X
class CDialogInputNum : public CDialog
{
// �R���X�g���N�V����
public:
	void SetItemElement();
	void GetItemElement();
	void SetModal();
	void SetModaless();

	void SetTitleText(LPTSTR lpTitleText);
	void SetUnitText(LPTSTR lpUnitText);
	void SetDialogTitle(LPTSTR lpTitle);
	CDialogInputNum(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	LPTSTR m_lpTitle;
	LPTSTR m_lpTitleText;
	LPTSTR m_lpUnitText;
	CTonView*	m_pcView;
	int m_nMode;
	int	m_nFirst;
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogInputNum)
	enum { IDD = IDD_INNUM };
	CEdit	m_edInputNum;
	CButton	m_cbtnOK;
	CButton	m_cbtnCancel;
	CStatic	m_stUnit;
	CStatic	m_stTitle;
	double	m_dInputNum;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogInputNum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogInputNum)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

