// CntrItem.cpp : CEs98CntrItem �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"

#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTonCntrItem �C���v�������e�[�V����

IMPLEMENT_SERIAL(CTonCntrItem, COleClientItem, 0)

CTonCntrItem::CTonCntrItem(CTonDoc* pContainer)
	: COleClientItem(pContainer)
{
	// TODO: ���̏ꏊ�ɍ\�z�����p�̃R�[�h��ǉ����Ă��������B

	m_rectPosition.SetRect(10, 10, 200, 200);
}

CTonCntrItem::~CTonCntrItem()
{
	// TODO: ���̏ꏊ�Ɍ㏈���p�R�[�h��ǉ����Ă��������B
	
}

void CTonCntrItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam)
{
	ASSERT_VALID(this);

	COleClientItem::OnChange(nCode, dwParam);

	// �A�C�e�����ҏW����Ă��鎞�ɂ� (�g�ݍ��ݐ�ł� �J���Ă��Ă�)
	// �A�C�e���̏�Ԃ܂��͂��̓��e�̉�ʕ\���̕ύX�ɑ΂��AOnChange
	// �ʒm�𑗂�܂��B

	// TODO: UpdateAllViews ���Ăяo���ăA�C�e���𖳌��ɂ��Ă��������B
	//  (�A�v���P�[�V�������L�̃q���g��Y����)

	GetDocument()->UpdateAllViews(NULL);
		// ���ׂẴr���[���q���g���Ȃ��ŃA�b�v�f�[�g���܂����B
}

BOOL CTonCntrItem::OnChangeItemPosition(const CRect& rectPos)
{
	ASSERT_VALID(this);

	// �g�ݍ��ݐ悪�L���ȊԂ� CTonCntrItem::OnChangeItemPosition �͑g�ݍ��ݐ�
	// �E�B���h�E�̈ʒu��ύX���邽�߂ɃT�[�o�[�ɂ���ČĂяo����܂��B�ʏ�A�����
	// �T�[�o�[ �h�L�������g�̃f�[�^��ύX�������Ƃɂ���Ĕ͈͂��ς������A�g�ݍ���
	// ��̃T�C�Y��ύX�������Ƃ̌��ʂł��B
	//
	//�A�C�e����V�����ʒu�Ɉړ����邽�߂Ɋ�{
	// �N���X�� COleClientItem::SetItemRects ���Ăяo���܂��B
	//
	//

	if (!COleClientItem::OnChangeItemPosition(rectPos))
		return FALSE;

	// TODO: �A�C�e���̋�`/�͈͂̃L���b�V�����X�V���Ă��������B
	m_rectPosition = rectPos;
	return TRUE;
}

void CTonCntrItem::OnGetItemPosition(CRect& rPosition)
{
	ASSERT_VALID(this);
	rPosition = m_rectPosition;
	// �g�ݍ��ݐ悪�N�����Ă���ԁACTonCntrItem::OnGetItemPosition �̓A�C�e����
	// �ʒu�����肷�邽�߂ɌĂ΂�܂��BAppWizard �ō쐬���ꂽ�f�t�H���g�̃R�[�h�͒P��
	// �Ƀn�[�h�R�[�f�B���O���ꂽ��`��Ԃ��܂��B
	// �ʏ�A���̋�`�͎g���Ă���r���[�Ɋ֘A����A�C�e���̌��݂̈ʒu�𔽉f���܂��B
	// CTonCntrItem::GetActiveView ���Ăяo�����Ƃɂ���ăr���[�𓾂邱�Ƃ��ł�
	// �܂��B

	// TODO: rPosition �ɂ̓s�N�Z���P�ʂŐ�������`��Ԃ��Ă��������B

	rPosition.SetRect(10, 10, 210, 210);
}

void CTonCntrItem::OnActivate()
{
    // �t���[�����ƂɈ�����A�C�e���̃r�W���A���ҏW���\�ɂ��܂��B
    CTonView* pView = GetActiveView();
    ASSERT_VALID(pView);
    COleClientItem* pItem = GetDocument()->GetInPlaceActiveItem(pView);
    if (pItem != NULL && pItem != this)
        pItem->Close();
    
    COleClientItem::OnActivate();
}

void CTonCntrItem::OnDeactivateUI(BOOL bUndoable)
{
	COleClientItem::OnDeactivateUI(bUndoable);

    // �I�u�W�F�N�g���A�E�g�T�C�h �C�� �I�u�W�F�N�g�ł͂Ȃ��ꍇ�͔�\���ɂ��܂��B
    DWORD dwMisc = 0;
    m_lpObject->GetMiscStatus(GetDrawAspect(), &dwMisc);
    if (dwMisc & OLEMISC_INSIDEOUT)
        DoVerb(OLEIVERB_HIDE, NULL);
}

void CTonCntrItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// �ŏ��̊�{�N���X�̌Ăяo���� COleClientItem
	// �f�[�^�֓ǂݍ��܂�܂��BCTonCntrItem::GetDocument
	// ����Ԃ��ꂽ m_pDocument �|�C���^��ݒ肵�Ă����{�N��
	// �X���Ăяo���čŏ��̃V���A���C�Y���s���Ă��������B
	COleClientItem::Serialize(ar);

	// CTonCntrItem �ɑ΂��ăf�[�^�̕ۑ�/�ǂݍ��݂��s���܂��B
	if (ar.IsStoring())
	{
		// TODO: ���̏ꏊ�ɕۑ��p�̃R�[�h��ǉ����܂��B
		ar << m_rectPosition;
	}
	else
	{
		// TODO: ���̏ꏊ�ɓǂݍ��ݗp�̃R�[�h��ǉ����܂��B
		ar >> m_rectPosition;
	}
}

BOOL CTonCntrItem::CanActivate()
{
	// �T�[�o�[�ŕҏW����Ă���Ƃ��ɂ͑g�ݍ��ݐ�ł̕ҏW��
	// �T�|�[�g����Ă��܂���B ���̏ꍇ�͑g�ݍ��ݐ�̋N��  
	// �𖳌��ɂ��Ă��������B
	CTonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	ASSERT(pDoc->IsKindOf(RUNTIME_CLASS(COleServerDoc)));
	if (pDoc->IsInPlaceActive())
		return FALSE;

	RefreshSize();
	// ���̑��̓f�t�H���g�̏����ɔC���܂��B
	return COleClientItem::CanActivate();
}

void CTonCntrItem::RefreshSize()
{
	CSize size;
	if (GetCachedExtent(&size))
	{
		CClientDC dc(NULL);
		dc.HIMETRICtoDP(&size);

		m_rectPosition.bottom = m_rectPosition.top + size.cy;
		m_rectPosition.right = m_rectPosition.left + size.cx;

		CDocument* pDoc = GetDocument();
		ASSERT(pDoc != NULL);
		pDoc->SetModifiedFlag();
		pDoc->UpdateAllViews(NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTonCntrItem �N���X�̐f�f

#ifdef _DEBUG
void CTonCntrItem::AssertValid() const
{
	COleClientItem::AssertValid();
}

void CTonCntrItem::Dump(CDumpContext& dc) const
{
	COleClientItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
