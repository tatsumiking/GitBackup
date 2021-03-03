#if !defined(DLGMAIN_H)
#define DLGMAIN_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMain.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMain �_�C�A���O

class CDialogMain : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogMain(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogMain)
	enum { IDD = IDD_MAIN };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^ �����o��ǉ����܂��B
	//}}AFX_DATA
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �f�[�^�[
public:
	CTonView	*m_pcView;	
	HANDLE	m_hDib;
	int		m_nWidth;
	int		m_nHight;

	int		m_nTimer;
	UINT	m_nTimerID;
	HANDLE	m_hProcess;
	int		m_nExitProcess;

// �C���v�������e�[�V����
public:
	void InitShowHideBtn();
	void ProgExec(LPSTR lpExexProg);
	void DibLoad();
	void ProgExitLoop();

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogMain)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnInput();
	afx_msg void OnBtnWakuedit();
	afx_msg void OnBtnGaijiedit();
	afx_msg void OnBtnOption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DLGMAIN_H)
