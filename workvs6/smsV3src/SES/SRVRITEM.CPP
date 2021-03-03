// SrvrItem.cpp : CTonSrvrItem �A�v���P�[�V�����p�N���X�̋@�\��`���s���܂��B
//

#include "stdafx.h"

#include "SrvrItem.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTonSrvrItem �C���v�������e�[�V����

IMPLEMENT_DYNAMIC(CTonSrvrItem, COleServerItem)

CTonSrvrItem::CTonSrvrItem(CTonDoc* pContainerDoc)
	: COleServerItem(pContainerDoc, TRUE)
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
	//  (�Ⴆ�΁A�A�C�e�� �f�[�^ �\�[�X�p�̓��ʂȃN���b�v�{�[�h�`���̒ǉ�)
}

CTonSrvrItem::~CTonSrvrItem()
{
	// TODO: ���̏ꏊ�Ɍ㏈����ǉ����Ă��������B
}

void CTonSrvrItem::Serialize(CArchive& ar)
{
	// CTonSrvrItem::Serialize �̓A�C�e�����N���b�v�{�[�h�ɃR�s�[��
	// ��Ă���Ƃ��ɂ́A�t���[�����[�N�ɂ���ČĂ΂�܂��B����� OLE �R�[��
	// �o�b�N OnGetClipboardData ��ʂ�z���Ď����I�ɍs�킹�邱�Ƃ��ł��܂��B
	// ���ߍ��݃A�C�e���̃f�t�H���g�͊ȒP�ȃh�L�������g�̃V���A���C�Y�֐��̑�
	// �������܂��B
	// �����N���T�|�[�g���Ă���Ƃ��́A�h�L�������g�̈ꕔ���V���A���C�Y���܂��B

	if (!IsLinkedItem())
	{
		CTonDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->Serialize(ar);
	}
}

BOOL CTonSrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
	// �����̃A�v���P�[�V�����ł́A�A�C�e���̃R���e���c �A�X�y�N�g�̕`�悾�����n���h�����܂��B
	//  OnDrawEx ���I�[�o�[���C�h���Ă��� DVASPECT_THUMBNAIL �ő��̃A�X�y�N�g���T�|�[�g����
	// ���ɂ́AOnGetExtent ��ǉ�����A�X�y�N�g���n���h������悤�ɕύX���Ă��������B
	//
	//

	if (dwDrawAspect != DVASPECT_CONTENT)
		return COleServerItem::OnGetExtent(dwDrawAspect, rSize);

	// CTonSrvrItem::OnGetExtent �̓A�C�e���S�̂� HIMETRIC �P�ʂ͈̔͂𓾂�
	// ���߂� �Ă΂�܂��B�f�t�H���g�́A�P���Ƀn�[�h�R�[�f�B���O���ꂽ�l��Ԃ��܂��B
	//

	CTonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: �K���ȃT�C�Y�ɒu�������Ă��������B

	rSize = CSize(3000, 3000);   // 3000 x 3000 HIMETRIC �P��

	return TRUE;
}

BOOL CTonSrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
	// rSize ���g�p����ꍇ�͂������폜���Ă�������
	UNREFERENCED_PARAMETER(rSize);

	CTonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: �}�b�s���O ���[�h�Ɣ͈͂̐ݒ�B
	//  (�͈͂͒ʏ� OnGetExtent ����Ԃ��ꂽ�T�C�Y�Ɠ����ł�)
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowOrg(0,0);
	pDC->SetWindowExt(3000, 3000);

	// TODO: ���̏ꏊ�ɕ`��p�̃R�[�h��ǉ����܂��B�C�ӂ� HIMETRIC �͈̔͂�h��Ԃ��Ă��������B
	//       ���^�t�@�C�� �f�o�C�X �R���e�L�X�g (pDC) �̂��ׂĂ�`�悵�܂��B

	// TODO: CTonCntrItem �̖��ߍ��݃I�u�W�F�N�g���`�悵�Ă��������B

	// �ȉ��Ɏ����R�[�h�͍ŏ��̃A�C�e����K���Ȉʒu�֕`�悵�܂��B

	// TODO: ���̃R�[�h�͎��ۂ̕`��p�̃R�[�h���m�肵����폜���Ă��������B

	POSITION pos = pDoc->GetStartPosition();
	CTonCntrItem* pItem = (CTonCntrItem*)pDoc->GetNextClientItem(pos);
	if (pItem != NULL)
		pItem->Draw(pDC, CRect(10, 10, 1010, 1010));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTonSrvrItem �N���X�̐f�f

#ifdef _DEBUG
void CTonSrvrItem::AssertValid() const
{
	COleServerItem::AssertValid();
}

void CTonSrvrItem::Dump(CDumpContext& dc) const
{
	COleServerItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
