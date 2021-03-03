#if !defined(AFX_DIALOGMOJIFITING_H__12EB70E1_19AE_11D3_A712_00A0B0086478__INCLUDED_)
#define AFX_DIALOGMOJIFITING_H__12EB70E1_19AE_11D3_A712_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMjFit.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiFiting �_�C�A���O

class CDialogMojiFiting : public CDialog
{
// �R���X�g���N�V����
public:
	void SetItemElement();
	void GetItemElement();
	CDialogMojiFiting(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	CTonView*	m_pcView;
	CBitmapButton	m_btnBig;
	CBitmapButton	m_btnSmall;
	CBitmapButton	m_btnRLBig;
	CBitmapButton	m_btnRLSmall;
	CBitmapButton	m_btnUDSmall;
	CBitmapButton	m_btnUDBig;
	CBitmapButton	m_btnUpMove;
	CBitmapButton	m_btnDownMove;
	CBitmapButton	m_btnLeftMove;
	CBitmapButton	m_btnRightMove;
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogMojiFiting)
	enum { IDD = IDD_MOJI_FITING };
	double	m_dStep;
	double	m_dXSize;
	double	m_dYSize;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogMojiFiting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogMojiFiting)
	afx_msg void OnBtnBig();
	afx_msg void OnBtnSmall();
	afx_msg void OnBtnRLBig();
	afx_msg void OnBtnRLSmall();
	afx_msg void OnBtnUDBig();
	afx_msg void OnBtnUDSmall();
	afx_msg void OnBtnUpMove();
	afx_msg void OnBtnDownMove();
	afx_msg void OnBtnLeftMove();
	afx_msg void OnBtnRightMove();
	afx_msg void OnReset();
	afx_msg void OnSet();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpstep(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DIALOGMOJIFITING_H__12EB70E1_19AE_11D3_A712_00A0B0086478__INCLUDED_)
