
#pragma once

// DlgTwPnt.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialog2PointSize �_�C�A���O

//C �Q�_�T�C�Y�_�C�A���O�N���X
class CDialog2PointSize : public CDialog
{
// �R���X�g���N�V����
public:
	CDialog2PointSize(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialog2PointSize)
	enum { IDD = IDD_2POINTSIZETRNS };
	double	m_dXSize;
	double	m_dYSize;
	double	m_dLength;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialog2PointSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialog2PointSize)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B


