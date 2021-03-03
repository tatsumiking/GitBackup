#ifndef __CMD_RULE_H
#define __CMD_RULE_H

#include "..\object\datalist.h"
#include "..\object\rulevect.h"
#include "DlRlStyl.h"

//C �r������֌W�R�}���h�����N���X
class CCmdRule : public CCmdBase
{
public:
	CDialogRuleStyle *m_pcDialogRuleStyle;
public:
	CCmdRule(CScrollView* pcview);
	UINT ExecPoint(UINT event, DBL dx, DBL dy);
	UINT ExecLine(UINT event, DBL dx, DBL dy);
	UINT ExecCrcl(UINT event, DBL dx, DBL dy);
	UINT ExecBoxs(UINT event, DBL dx, DBL dy);
	UINT ExecPlgn(UINT event, DBL dx, DBL dy);
	UINT ExecFree(UINT event, DBL dx, DBL dy);
// �����g�p�ϐ�
private:
	CDataList*	m_pcDataList;
// �����g�p�֐�
private:
	void CreateLine(DBL dx, DBL dy);
	void SetLineEndPoint(DBL dx, DBL dy);
	void CreateBoxs(DBL dx, DBL dy);
	void SetBoxsEndPoint(DBL dx, DBL dy);
	void SetBoxsPoint(CRuleVect *pcRuleVect);
	void CreateCrcl(DBL dx, DBL dy);
	void SetCrclEndPoint(DBL dx, DBL dy);
	void SetCrclPoint(CRuleVect *pcRuleVect);
	void CreatePlgn(DBL dx, DBL dy);
	void SetPlgnEndPoint(DBL dx, DBL dy);
	void SetPlgnPoint(CRuleVect *pcRuleVect);
	void CreateFree(DBL dx, DBL dy);
	void SetFreePoint(int pp, DBL dx, DBL dy);
	void SetFreeEndPoint(int pp);
	void SetStyle(int nStyle);
};

#endif

