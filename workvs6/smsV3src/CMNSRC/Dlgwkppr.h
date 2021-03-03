#if !defined(AFX_DLGWKPPR_H__5D2C5F26_36E7_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGWKPPR_H__5D2C5F26_36E7_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgWkPpr.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuPaper �_�C�A���O
class CTonView;

class CDialogWakuPaper : public CDialog
{
// �R���X�g���N�V����
public:
	CTonView	*m_pcView;

	CDialogWakuPaper(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	void CBInitPaperName();
	void GetPaperElement(LPSTR srchname);
	void SavePaperElement(LPSTR srchname);
	void DelPaperName(LPSTR srchname);
	void BackupPaperName();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogWakuPaper)
	enum { IDD = IDD_WAKU_PAPER };
	double	m_dPaperHight;
	double	m_dPaperSizeX;
	double	m_dPaperSizeY;
	double	m_dPaperSX;
	double	m_dPaperSY;
	double	m_dPaperWidth;
	CString	m_csPaperName;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogWakuPaper)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWakuPaper)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangePapername();
	afx_msg void OnSave();
	afx_msg void OnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGWKPPR_H__5D2C5F26_36E7_11D2_A0A2_444553540000__INCLUDED_)
