
#pragma once

// DlInPair.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogInputNumPair �_�C�A���O
class CTonView;

//C �y�A�i�w�x�Ȃǁj���l���̓_�C�A���O�N���X
class CDialogInputNumPair : public CDialog
{
// �R���X�g���N�V����
public:
	void SetItemElement();
	void GetItemElement();
	void SetModal();
	void SetModaless();

	void SetTitleText(LPTSTR lpTitleTextX, LPTSTR lpTitleTextY);
	void SetUnitText(LPTSTR lpUnitTextX, LPTSTR lpUnitTextY);
	void SetDialogTitle(LPTSTR lpTitle);
	CDialogInputNumPair(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	LPTSTR m_lpTitle;
	LPTSTR m_lpTitleTextX;
	LPTSTR m_lpTitleTextY;
	LPTSTR m_lpUnitTextX;
	LPTSTR m_lpUnitTextY;
	CTonView*	m_pcView;
	int m_nMode;
	int	m_nFirst;
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogInputNumPair)
	enum { IDD = IDD_INNUMPAIR };
	CEdit	m_edInputNumX;
	CEdit	m_edInputNumY;
	CButton	m_cbtnOK;
	CButton	m_cbtnCancel;
	CStatic	m_stUnitX;
	CStatic	m_stUnitY;
	CStatic	m_stTitleX;
	CStatic	m_stTitleY;
	double	m_dInputNumX;
	double	m_dInputNumY;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogInputNumPair)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogInputNumPair)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B


