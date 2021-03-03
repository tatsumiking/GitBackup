#if !defined(AFX_DDBEDIT_H__FB14F3C7_0FE3_45E4_95A9_D769DA5D9E10__INCLUDED_)
#define AFX_DDBEDIT_H__FB14F3C7_0FE3_45E4_95A9_D769DA5D9E10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DDBEdit.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogDBEdit �_�C�A���O
class	CDaoDBLib;

class CDialogDBEdit : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogDBEdit(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CTonApp		*m_pcApp;
	CTonDoc		*m_pcDoc;	// �h�L�������g�N���X
	CTonView	*m_pcView;	// �r���[�N���X
	CDaoDBLib	*m_pcDaoDBLib;	// XLS�ACSV�AMDB�����N�����N���X
	int		m_nFirst;
	int		m_nMode;
	int		m_nFildMax;
	int		m_nLineMax;
	int		m_nCrtCol;	// �I���J�����ԍ���ێ�
	UINT	m_nCrtLine;	// �I�����R�[�h�ԍ���ێ�
	//{{AFX_DATA(CDialogDBEdit)
	enum { IDD = IDD_DBEDIT };
	CStatic	m_stTop;
	CListCtrl	m_lstFileList;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogDBEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void GetFildElement(int nFildNo, LPSTR lpStr);
	int GetFildNo(LPSTR lpFildName);
	int GetFildElement(LPSTR lpFildName, LPSTR lpStr);
	void GetListCtrlSize(int *xsize, int *ysize, int *oneysize);
	int InitRecord();
	int InitFildName();
	void EndDaoDBLib();
	void InitDaoDBLib();
	void InitItemElement();
	void SetModaless();
	void GetFildName(int nFildNo, LPSTR lpFildName);
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogDBEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickLstitem(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DDBEDIT_H__FB14F3C7_0FE3_45E4_95A9_D769DA5D9E10__INCLUDED_)
