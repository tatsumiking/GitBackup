#pragma once

#include	"../CmnSrc/CmdSystem.h"
#include	"../CmnSrc/CmdBase.h"
#include	"../CmnSrc/CmdBaseBmp.h"
#include	"../CmnSrc/CmdSlct.h"
#include	"../CmnSrc/CmdEdit.h"
#include	"../CmnSrc/CmdStrg.h"
#include	"../sgs/CmdMoji.h"
#include	"../sgs/CmdPart.h"
#include	"../CmnSrc/CmdWaku.h"
#include	"../Sgs/CmdSWak.h"
#include	"../CmnSrc/CmdStrgProp.h"
#include	"../CmnSrc/CmdFigu.h"
#include	"../CmnSrc/CmdZoom.h"
#include	"../CmnSrc/CmdTrns.h"
#include	"../Sgs/CmdLine.h"
#include	"../Sgs/CmdSLine.h"
#include	"../Sgs/CmdPoint.h"
#include	"../CmnSrc/CmdSpcl.h"
#include	"../CmnSrc/CmdDibC.h"
#include	"../CmnSrc/CmdCrdnt.h"
#include	"../CmnSrc/CmdAtr.h"
#include	"../CmnSrc/CmdSub.h"
#include	"../CmnSrc/CmdClr.h"
#include	"../CmnSrc/CmdFile.h"
#include	"../CmnSrc/CmdSpclStrg.h"
#include	"../CmnSrc/CmdPloter.h"
#include	"../CmnSrc/CmdKamon.h"
#include	"../CmnSrc/CmdKamonCrct.h"

/////////////////////////////////////////////////////////////////////////////
// class CCommand

class CCommand
{
public:
	CCommand(CScrollView* pcview);
	virtual ~CCommand();
	void SetDocument(CDocument* pDoc);
	UINT Exec(UINT event, DBL dx, DBL dy);

protected:

// アトリビュート
protected:
	int		m_nBackMode;
	int		m_nBackFillClr;
	UINT	m_nCrtCommand;
	UINT	m_nSubCommand;
public:

// オペレーション
public:

public:
	UINT ExecKamon(UINT event, DBL dx, DBL dy);
	UINT ExecFile(UINT event, DBL dx, DBL dy);
	UINT ExecSub(UINT event, DBL dx, DBL dy);
	UINT ExecWaku(UINT event, DBL dx, DBL dy);
	UINT ExecTool(UINT event, DBL dx, DBL dy);
	UINT ExecAtr(UINT event, DBL dx, DBL dy);
	UINT ExecCrdnt(UINT event, DBL dx, DBL dy);
	UINT ExecStrg(UINT event, DBL dx, DBL dy);
	UINT ExecAtrClr(UINT event, DBL x, DBL y);
	UINT ExecTrace(UINT event, DBL dx, DBL dy);
	UINT ExecDibC(UINT event, DBL x, DBL y);
	UINT ExecSlct(UINT event, DBL x, DBL y);
	UINT ExecEdit(UINT event, DBL x, DBL y);
	UINT ExecSpcl(UINT event, DBL x, DBL y);
	UINT ExecTrns(UINT event, DBL x, DBL y);
	UINT ExecLine(UINT event, DBL x, DBL y);
	UINT ExecPoint(UINT event, DBL x, DBL y);
	UINT ExecFigu(UINT event, DBL x, DBL y);
	UINT ExecZoom(UINT event, DBL x, DBL y);
	void SetSubCommand(int nSubCommand);
	UINT GetSubCommand();
	void SetCrtCommand(UINT command);
	UINT GetCrtCommand();
	int m_nFirstFlag;
private:
	CDocument*	m_pcDoc;
	CScrollView*	m_pcView;
	// コマンド実行クラス
	CCmdFile*	m_pcCmdFile;
	CCmdSlct*	m_pcCmdSlct;
	CCmdEdit*	m_pcCmdEdit;
	CCmdStrg*	m_pcCmdStrg;
	CCmdMoji*	m_pcCmdMoji;
	CCmdPart*	m_pcCmdPart;
	CCmdStrgProp*	m_pcCmdStrgProp;
	CCmdSpclStrg*	m_pcCmdSpclStrg;
	CCmdKamonCrct*	m_pcCmdKamonCrct;
	CCmdWaku*	m_pcCmdWaku;
	CCmdSWak*	m_pcCmdSWak;
	CCmdFigu*	m_pcCmdFigu;
	CCmdZoom*	m_pcCmdZoom;
	CCmdTrns*	m_pcCmdTrns;
	CCmdLine*	m_pcCmdLine;
	CCmdSLine*	m_pcCmdSLine;
	CCmdPoint*	m_pcCmdPoint;
	CCmdSpcl*	m_pcCmdSpcl;
	CCmdDibC*	m_pcCmdDibC;
	CCmdClr*	m_pcCmdClr;
	CCmdAtr*	m_pcCmdAtr;
	CCmdSub*	m_pcCmdSub;
	CCmdCrdnt*	m_pcCmdCrdnt;
	CCmdSystem*	m_pcCmdSystem;
	CCmdPloter*	m_pcCmdPloter;
	CCmdKamon*	m_pcCmdKamon;
};

