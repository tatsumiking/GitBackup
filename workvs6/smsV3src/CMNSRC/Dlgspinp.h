#if !defined(AFX_DLGSPINP_H__5D2C5F25_36E7_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGSPINP_H__5D2C5F25_36E7_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgSpInp.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSPtnInput �_�C�A���O

class CTonView;

class CDialogSPtnInput : public CMyDialog
{
// �R���X�g���N�V����
public:
	CDialogSPtnInput(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

public:
	CTonView*	m_pcView;
	CMyEdit*	m_pcEdit[10];
	BOOL m_fFirstFlag;
	int m_nWakuMax;
	int m_nNextFocus;
	int	m_nCrtIdxTop;

public:
	virtual void KeyIn(UINT nChar, UINT nID);
	virtual void SelectEdit(UINT nID);
	void WakuSetFocus(int no);
	void GetWakuMax();
	void SaveCrtWakuText();
	void LoadCrtWakuText();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogSPtnInput)
	enum { IDD = IDD_PTN_INPUT };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogSPtnInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogSPtnInput)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDisp();
	afx_msg void OnExit();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGWKINP_H__5D2C5F25_36E7_11D2_A0A2_444553540000__INCLUDED_)