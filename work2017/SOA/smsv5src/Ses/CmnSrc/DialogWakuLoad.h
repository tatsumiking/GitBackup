
#pragma once

// DlgWkLd.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuLoad �_�C�A���O
class CTonView;

class CDialogWakuLoad : public CDialog
{
// �R���X�g���N�V����
public:
	CTonView*	m_pcView;
	int	m_nFirstFlag;	
	int	m_nDlgWidth;
	int	m_nDlgHight;
	int	m_nImgTopX;
	int	m_nImgTopY;
	int m_nWakuMax;
	int m_nCrtTop;
	int m_nSlctWaku;
	int	m_nOneLineCount;
	DBL m_dTrnsTime;
	HANDLE		m_hDib;   // �摜�\�p�o�b�t�@�n���h��
	ULONG		m_nDibSize;   // �摜�\�p�o�b�t�@�T�C�Y

	CDialogWakuLoad(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	void WakuAllDraw(CDC *pcDC);
	void WakuDraw(CDC *pcDC, int sx, int sy, LPTSTR wakufname);
	void WakuImageDraw(CDC *pcDC, int sx, int sy, LPTSTR wakufname, ULONG size);
	void WakuBoxsDraw(CDC *pcDC, DBL sx, DBL sy, LPTSTR wakufname);
	void TrnsTimeSet(DBL paperwidth, DBL paperhight);
	void BoxsDraw(CDC *pcDC, long sx, long sy, long ex, long ey);
	void GetWakuCountMax();
	void GetWakuSize(TCHAR *wenvfile, DBL *wakuxsize, DBL *wakuysize);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogWakuLoad)
	enum { IDD = IDD_WAKU_LOAD };
	CButton	m_btnNextPage;
	CListBox	m_lstWakuName;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogWakuLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWakuLoad)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBeforpage();
	afx_msg void OnNextpage();
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeLstwakuname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B



