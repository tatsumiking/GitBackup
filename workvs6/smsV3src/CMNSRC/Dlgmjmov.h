#if !defined(AFX_DLGMJMOV_H__4DDF98A2_3B99_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGMJMOV_H__4DDF98A2_3B99_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgMjMov.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiMove �_�C�A���O
class CTonView;

//C �����ړ��_�C�A���O�N���X
class CDialogMojiMove : public CDialog
{
// �R���X�g���N�V����
public:
	void SetItemElement();
	void GetItemElement();
	CDialogMojiMove(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	CTonView*	m_pcView;
	CBitmapButton	m_btnLeft;
	CBitmapButton	m_btnRight;
	CBitmapButton	m_btnUp;
	CBitmapButton	m_btnDown;
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogMojiMove)
	enum { IDD = IDD_MOJI_MOVE };
	double	m_edStepmm;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogMojiMove)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogMojiMove)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnMovedown();
	afx_msg void OnBtnMoveleft();
	afx_msg void OnBtnMoveright();
	afx_msg void OnBtnMoveup();
	afx_msg void OnDlgReset();
	afx_msg void OnDlgSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGMJMOV_H__4DDF98A2_3B99_11D2_A0A2_444553540000__INCLUDED_)
