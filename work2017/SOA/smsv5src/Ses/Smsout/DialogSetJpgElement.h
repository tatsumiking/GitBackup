
#pragma once

#include "CmdBase.h"
#include "BmpFile.h"

// DSetJpgEle.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSetJpgElement �_�C�A���O

class CDialogSetJpgElement : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogSetJpgElement(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CDialogSetJpgElement();
// �_�C�A���O �f�[�^
public:
	CTonView	*m_pcView;
	CTonDoc		*m_pcDoc;
	CBmpFile	*m_pcBmpFile;
	int			m_nInSort;
	int			m_nImageFitSizeX;
	int			m_nImageFitSizeY;
	int			m_nDotSizeX;
	int			m_nDotSizeY;
	//{{AFX_DATA(CDialogSetJpgElement)
	enum { IDD = IDD_SETJPGELEMENT };
	CString	m_csBaseName;
	int		m_nJpgSizeX;
	int		m_nJpgSizeY;
	int		m_nJpgSX;
	int		m_nJpgSY;
	int		m_nImageSizeX;
	int		m_nImageSizeY;
	int		m_nImageSPX;
	int		m_nImageSPY;
	BOOL	m_nChkNoWakuOut;
	int		m_nJpgQuality;
	int		m_nRoundRag;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogSetJpgElement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void SaveJpgoutElement();
	void CreateJpgFile(int nPageNo);
	void SetFitingElement();
	void JpgOutMain(CDC *pcDC);

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogSetJpgElement)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

