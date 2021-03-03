#if !defined(AFX_DLGKMNLD_H__21A36C42_36E3_11D2_A0A2_444553540000__INCLUDED_)
#define AFX_DLGKMNLD_H__21A36C42_36E3_11D2_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgKmnLd.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogKamonLoad �_�C�A���O

#define	LISTMAX	10

//C �Ɩ�W�ďo�_�C�A���O�N���X
class CDialogKamonLoad : public CDialog
{
// �R���X�g���N�V����
public:
	void KamnSelectDisp(int idx);
	void NameListInit();
	CFont* m_pcfontView;
	CFont* m_pcoldFont;
	CDialogKamonLoad(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

public:
	char m_szKamonChar[16];
	char m_szKamonFont[RECMAX];

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogKamonLoad)
	enum { IDD = IDD_KAMON_LOAD };
	double	m_dKmnSize;
	CString	m_csSrchChar;
	CString	m_csSrchStrg;
	CString	m_csKmnView;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogKamonLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogKamonLoad)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeSrchchar();
	afx_msg void OnChangeSrchstrg();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeNamelist();
	afx_msg void OnDeltaposSpkmnsize(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGKMNLD_H__21A36C42_36E3_11D2_A0A2_444553540000__INCLUDED_)
