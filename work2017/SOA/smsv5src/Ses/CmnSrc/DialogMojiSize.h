
#pragma once

// DlgMjSiz.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiSize �_�C�A���O
class CTonView;

//C �����T�C�Y�ύX�_�C�A���O�N���X
class CDialogMojiSize : public CDialog
{
// �R���X�g���N�V����
public:
	void SetItemElement();
	void GetItemElement();
	CDialogMojiSize(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	CTonView*	m_pcView;
	CBitmapButton	m_btnSmall;
	CBitmapButton	m_btnBig;
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogMojiSize)
	enum { IDD = IDD_MOJI_SIZE };
	double	m_edStepmm;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogMojiSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogMojiSize)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgReset();
	afx_msg void OnDlgSet();
	afx_msg void OnBtnSizebig();
	afx_msg void OnBtnSizesmall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

