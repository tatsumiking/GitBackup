#pragma once

// DlgStrVS.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgVStruct �_�C�A���O

class CTonView;

class CDialogStrgVStruct : public CDialog
{
// �R���X�g���N�V����
public:
	void SetItemElement();
	void GetItemElement();
	int m_nSetMode;
	CTonView	*m_pcView;

public:
	CDialogStrgVStruct(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogStrgVStruct)
	enum { IDD = IDD_STRG_VSTRUCT };
	CStatic	m_stTop;
	BOOL	m_chkStrgEqual;
	CStatic	m_stSetMode1;
	CStatic	m_stSetMode2;
	CStatic	m_stSetMode3;
	BOOL	m_chkNoround;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogStrgVStruct)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogStrgVStruct)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

