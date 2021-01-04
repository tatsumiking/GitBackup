
#pragma once

// DWkBar1.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBar1 �_�C�A���O
class CTonView;

class CDialogWakuBar1 : public CDialogBar
{
// �R���X�g���N�V����
public:
	CDialogWakuBar1(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CTonApp*	m_pcApp;
	CTonView*	m_pcView;
	int		m_nFirstFlag;
	//{{AFX_DATA(CDialogWakuBar1)
	enum { IDD = IDD_WAKUBAR1 };
	CComboBox	m_cmbPaperName;
	CString		m_csPaperName;
	//}}AFX_DATA


// �I�[�o�[���C�h
public:
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogWakuBar1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void InitItemElement();
	void SetItemElement();
	void GetItemElement();
	void InitWakuPaperCombo();
	void SetCrtView(CTonView * pcView);
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWakuBar1)
	afx_msg void OnSelchangeCmbpaper();
	afx_msg void OnPaint();
	afx_msg void OnWakuPaperAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

