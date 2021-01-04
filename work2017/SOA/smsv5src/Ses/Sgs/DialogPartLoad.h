#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DPrtLoad.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPartLoad �_�C�A���O

class CDialogPartLoad : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogPartLoad(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CMainFrame*	m_pcWnd;
	CTonView*	m_pcView;
	int	m_nFirstFlag;	
	int m_nCrtTop;
	int	m_nLstMax;
	int	m_nPageMax;
	HANDLE		m_hDib;   // �摜�\�p�o�b�t�@�n���h��
	ULONG		m_nDibSize;   // �摜�\�p�o�b�t�@�T�C�Y
	TCHAR	m_szName[RECMAX];
	//{{AFX_DATA(CDialogPartLoad)
	enum { IDD = IDD_PART_LOAD };
	CListBox	m_lstImg;
	CStatic	m_stTop;
	CStatic	m_stImg;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogPartLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void BoxsDraw(CDC *pcDC, int sx, int sy, int ex, int ey);
	void CreatePartList();
	void InitListImage();
	void DrawPartImg(CDC *pcDC, CRect rect);
	void ImageDraw(CDC *pcDC, int sx, int sy, LPTSTR filename, ULONG nSize);
	void LegalRect(CRect *rect);

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogPartLoad)
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

