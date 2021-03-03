// IpFrame.cpp : CInPlaceFrame �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"

#include "IpFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame

IMPLEMENT_DYNCREATE(CInPlaceFrame, COleIPFrameWnd)

BEGIN_MESSAGE_MAP(CInPlaceFrame, COleIPFrameWnd)
	//{{AFX_MSG_MAP(CInPlaceFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame �N���X�̍\�z/����

CInPlaceFrame::CInPlaceFrame()
{
}

CInPlaceFrame::~CInPlaceFrame()
{
}

int CInPlaceFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleIPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// CResizeBar �͖��ߍ��ݐ�ł̃T�C�Y�̕ύX���������܂��B
	if (!m_wndResizeBar.Create(this))
	{
		TRACE0("Failed to create resize bar\n");
		return -1;      // �쐬�Ɏ��s
	}

	// �f�t�H���g�ŁA�h���b�v �^�[�Q�b�g���t���[���E�B���h�E�Ƌ��ɓo�^��
	// �Ȃ��̂́A�ǂ��l���ł��B����� "falling through" �h���b�v����R
	// ���e�i�̃h���b�O �A���h �h���b�v �T�|�[�g��ی삵�܂��B
	m_dropTarget.Register(this);

	return 0;
}

// OnCreateControlBars �̓R���e�i �A�v���P�[�V�����̃E�B���h�E��Ƀc�[���o�[���쐬���邽�߂ɁA
// �t���[�����[�N����Ă΂�܂��BpWndFrame �̓R���e�i�̃g�b�v ���x���̃t���[�� �E�B���h�E�ŁA
// ��� NULL �ł͂���܂���BpWndDoc �̓h�L�������g ���x���̃t���[�� �E�B���h�E�ŃR���e�i�� SDI
// �A�v���P�[�V�����̂Ƃ��� NULL �ɂȂ�܂��B�T�[�o�[ �A�v���P�[�V�����͑��̃E�B���h�E�� MFC ��
// �R���g���[�� �o�[��z�u�ł��܂��B
BOOL CInPlaceFrame::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc)
{
	// pWndDoc ���g�p������͂�����폜���܂��B
	UNREFERENCED_PARAMETER(pWndDoc);

	// ���b�Z�[�W���������A�v���P�[�V�����ɓn�����悤�ɁA���̃E�B���h�E�ɃI�[�i�[��ݒ肵�܂��B
	m_wndToolBar.SetOwner(this);

	// �N���C�A���g�̃t���[�� �E�B���h�E��Ƀc�[���o�[���쐬�B
	if (!m_wndToolBar.Create(pWndFrame) ||
		!m_wndToolBar.LoadToolBar(IDR_SGSTYPE_SRVR_IP))
	{
		TRACE0("Failed to create toolbar\n");
		return FALSE;
	}

	// TODO: �c�[�� �`�b�v�X���K�v�Ȃ��ꍇ�A�������폜���Ă��������B
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: �c�[�� �o�[���h�b�L���O�\�ɂ��Ȃ��ꍇ�͈ȉ��̂R�s��
	//       �폜���Ă��������B
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	pWndFrame->EnableDocking(CBRS_ALIGN_ANY);
	pWndFrame->DockControlBar(&m_wndToolBar);

	return TRUE;
}

BOOL CInPlaceFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C��
	//       ���C�����Ă��������B

	return COleIPFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame �N���X�̐f�f

#ifdef _DEBUG
void CInPlaceFrame::AssertValid() const
{
	COleIPFrameWnd::AssertValid();
}

void CInPlaceFrame::Dump(CDumpContext& dc) const
{
	COleIPFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame �R�}���h
