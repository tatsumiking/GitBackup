#if !defined(_DPRTSAVE_H_)
#define _DPRTSAVE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DPrtSave.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPartSave �_�C�A���O

class CDialogPartSave : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogPartSave(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CMainFrame*	m_pcWnd;
	CTonView*	m_pcView;
	int	m_nFirstFlag;	
	int m_nCrtTop;
	int	m_nLstMax;
	int	m_nPageMax;
	char	m_szName[RECMAX];
	//{{AFX_DATA(CDialogPartSave)
	enum { IDD = IDD_PART_SAVE };
	CListBox	m_lstImg;
	CStatic	m_stTop;
	CStatic	m_stImg;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogPartSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void BoxsDraw(CDC *pcDC, int sx, int sy, int ex, int ey);
	void CreatePartList();
	void InitListImage();
	void DrawPartImg(CDC *pcDC, CRect rect);
	void LegalRect(CRect *rect);

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogPartSave)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBeforpage();
	afx_msg void OnNextpage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeLstimg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(_DPRTSAVE_H_)