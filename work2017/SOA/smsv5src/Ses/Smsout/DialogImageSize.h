
#pragma once

// DlgImgSize.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogImageSize �_�C�A���O

class CDialogImageSize : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogImageSize(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogImageSize)
	enum { IDD = IDD_IMAGESIZE };
	BOOL	m_nNoWakuOut;
	double	m_dPrntBlkXStart;
	double	m_dPrntBlkYStart;
	double	m_dPrntBlkXSize;
	double	m_dPrntBlkYSize;
	double	m_dPrntBlkXSpace;
	double	m_dPrntBlkYSpace;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogImageSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogImageSize)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

