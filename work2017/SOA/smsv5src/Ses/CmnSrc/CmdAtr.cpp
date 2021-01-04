
/////////////////////////////////////////////////////////////////////////////
// CCmdAtr

#include "stdafx.h"

#include "CmdBase.h"
#include "CmdAtr.h"

#include "DialogConerKind.h"

//extern "C" {
	#include "../../ObjLib/Senvlib.h"
//}

//P 支点設定コマンド処理クラスコンストラクタ
CCmdAtr::CCmdAtr(CScrollView* pcview)
:CCmdBase(pcview)
{
}

//P （枠、編集、文字）始点設定コマンド処理関数
UINT CCmdAtr::ExecAtrConer(UINT event, DBL dx, DBL dy)
{
	CDialogConerKind *pcDialogConerKind;

	if(event != INITEVENT){
		return(0);
	}
	pcDialogConerKind = new CDialogConerKind(m_pcView);
	pcDialogConerKind->SetDialogTitle(_T("支点設定"));
	senvgeteditconerkind(&(pcDialogConerKind->m_nConerKind));
	if(pcDialogConerKind->DoModal() == IDOK){
		senvsetwakuconerkind(pcDialogConerKind->m_nConerKind);
		senvseteditconerkind(pcDialogConerKind->m_nConerKind);
		senvsetmojiconerkind(pcDialogConerKind->m_nConerKind);
	}
	delete(pcDialogConerKind);
	return(ENDEVENT);	
}

//P 枠始点設定コマンド処理関数
UINT CCmdAtr::ExecAtrWakuConer(UINT event, DBL dx, DBL dy)
{
	CDialogConerKind *pcDialogConerKind;

	if(event != INITEVENT){
		return(0);
	}
	pcDialogConerKind = new CDialogConerKind(m_pcView);
	pcDialogConerKind->SetDialogTitle(_T("枠支点設定"));
	senvgetwakuconerkind(&(pcDialogConerKind->m_nConerKind));
	if(pcDialogConerKind->DoModal() == IDOK){
		senvsetwakuconerkind(pcDialogConerKind->m_nConerKind);
	}
	delete(pcDialogConerKind);
	return(ENDEVENT);	
}

//P 編集始点設定コマンド処理関数
UINT CCmdAtr::ExecAtrEditConer(UINT event, DBL dx, DBL dy)
{
	CDialogConerKind *pcDialogConerKind;

	if(event != INITEVENT){
		return(0);
	}
	pcDialogConerKind = new CDialogConerKind(m_pcView);
	pcDialogConerKind->SetDialogTitle(_T("編集支点設定"));
	senvgeteditconerkind(&(pcDialogConerKind->m_nConerKind));
	if(pcDialogConerKind->DoModal() == IDOK){
		senvseteditconerkind(pcDialogConerKind->m_nConerKind);
	}
	delete(pcDialogConerKind);
	return(ENDEVENT);	
}

//P 文字始点設定コマンド処理関数
UINT CCmdAtr::ExecAtrMojiConer(UINT event, DBL dx, DBL dy)
{
	CDialogConerKind *pcDialogConerKind;

	if(event != INITEVENT){
		return(0);
	}
	pcDialogConerKind = new CDialogConerKind(m_pcView);
	pcDialogConerKind->SetDialogTitle(_T("文字列支点設定"));
	senvgetmojiconerkind(&(pcDialogConerKind->m_nConerKind));
	if(pcDialogConerKind->DoModal() == IDOK){
		senvsetmojiconerkind(pcDialogConerKind->m_nConerKind);
	}
	delete(pcDialogConerKind);
	return(ENDEVENT);	
}
