#ifndef __CMD_CLR_H
#define __CMD_CLR_H

//G　概要
//　オブジェクトに対する特殊な色属性を設定する
//	特色設定、カット設定、グラデーション設定
//

#include "..\object\datafdib.h"
#include "DlgGrdtn.h"

//C 色付け関係コマンド処理クラス
class CCmdClr : public CCmdBase
{
public:
	CCmdClr(CScrollView* pcview);
	UINT ExecClrSet(UINT event, DBL dx, DBL dy);
	UINT ExecClrBackSet(UINT event, DBL dx, DBL dy);
	UINT ExecClrBackReset(UINT event, DBL dx, DBL dy);
	UINT ExecClrCutOnOff(UINT event, DBL dx, DBL dy);
	UINT ExecClrSlctCutOn(UINT event, DBL dx, DBL dy);
	UINT ExecClrSlctCutOff(UINT event, DBL dx, DBL dy);
	UINT ExecGradation(UINT event, DBL dx, DBL dy);
public:
	UINT ExecClrPicup(UINT event, DBL dx, DBL dy);
	void DataFDibCreate();
	void CreateGradationDib();
	void LocateGradationDlg();

private:
	CDialogGradation*	m_pcDialogGradation;
	char	m_szBmpFname[FNAMEMAX];
	CDataFDib*	m_pcDataFDib;

private:
};

#endif
