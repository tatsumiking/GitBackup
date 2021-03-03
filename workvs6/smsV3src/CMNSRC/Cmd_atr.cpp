
/////////////////////////////////////////////////////////////////////////////
// CCmdAtr

#include "stdafx.h"
#include "math.h"

#include "dlgcnrkd.h"
#include "cmd_base.h"
#include "cmd_atr.h"

extern "C" {
	void senvseteditconerkind(int nkind);
	void senvgeteditconerkind(LPINT nkind);
	void senvsetwakuconerkind(int nkind);
	void senvgetwakuconerkind(LPINT nkind);
	void senvsetmojiconerkind(int nkind);
	void senvgetmojiconerkind(LPINT nkind);
}

//P �x�_�ݒ�R�}���h�����N���X�R���X�g���N�^
CCmdAtr::CCmdAtr(CScrollView* pcview)
:CCmdBase(pcview)
{
}

//P �i�g�A�ҏW�A�����j�n�_�ݒ�R�}���h�����֐�
UINT CCmdAtr::ExecAtrConer(UINT event, DBL dx, DBL dy)
{
	CDialogConerKind *pcDialogConerKind;

	if(event != INITEVENT){
		return(0);
	}
	pcDialogConerKind = new CDialogConerKind(m_pcView);
	pcDialogConerKind->SetDialogTitle("�x�_�ݒ�");
	senvgeteditconerkind(&(pcDialogConerKind->m_nConerKind));
	if(pcDialogConerKind->DoModal() == IDOK){
		senvsetwakuconerkind(pcDialogConerKind->m_nConerKind);
		senvseteditconerkind(pcDialogConerKind->m_nConerKind);
		senvsetmojiconerkind(pcDialogConerKind->m_nConerKind);
	}
	delete(pcDialogConerKind);
	return(ENDEVENT);	
}

//P �g�n�_�ݒ�R�}���h�����֐�
UINT CCmdAtr::ExecAtrWakuConer(UINT event, DBL dx, DBL dy)
{
	CDialogConerKind *pcDialogConerKind;

	if(event != INITEVENT){
		return(0);
	}
	pcDialogConerKind = new CDialogConerKind(m_pcView);
	pcDialogConerKind->SetDialogTitle("�g�x�_�ݒ�");
	senvgetwakuconerkind(&(pcDialogConerKind->m_nConerKind));
	if(pcDialogConerKind->DoModal() == IDOK){
		senvsetwakuconerkind(pcDialogConerKind->m_nConerKind);
	}
	delete(pcDialogConerKind);
	return(ENDEVENT);	
}

//P �ҏW�n�_�ݒ�R�}���h�����֐�
UINT CCmdAtr::ExecAtrEditConer(UINT event, DBL dx, DBL dy)
{
	CDialogConerKind *pcDialogConerKind;

	if(event != INITEVENT){
		return(0);
	}
	pcDialogConerKind = new CDialogConerKind(m_pcView);
	pcDialogConerKind->SetDialogTitle("�ҏW�x�_�ݒ�");
	senvgeteditconerkind(&(pcDialogConerKind->m_nConerKind));
	if(pcDialogConerKind->DoModal() == IDOK){
		senvseteditconerkind(pcDialogConerKind->m_nConerKind);
	}
	delete(pcDialogConerKind);
	return(ENDEVENT);	
}

//P �����n�_�ݒ�R�}���h�����֐�
UINT CCmdAtr::ExecAtrMojiConer(UINT event, DBL dx, DBL dy)
{
	CDialogConerKind *pcDialogConerKind;

	if(event != INITEVENT){
		return(0);
	}
	pcDialogConerKind = new CDialogConerKind(m_pcView);
	pcDialogConerKind->SetDialogTitle("������x�_�ݒ�");
	senvgetmojiconerkind(&(pcDialogConerKind->m_nConerKind));
	if(pcDialogConerKind->DoModal() == IDOK){
		senvsetmojiconerkind(pcDialogConerKind->m_nConerKind);
	}
	delete(pcDialogConerKind);
	return(ENDEVENT);	
}
