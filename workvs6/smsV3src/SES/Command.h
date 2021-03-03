#ifndef __COMMAND_H
#define __COMMAND_H

#include	"..\cmnsrc\cmdsystm.h"
#include	"..\cmnsrc\cmd_base.h"
#include	"..\cmnsrc\cmdbsbmp.h"
#include	"..\cmnsrc\cmd_slct.h"
#include	"cmddbedt.h"
#include	"cmd_edit.h"
#include	"..\cmnsrc\cmd_strg.h"
#include	"..\cmnsrc\cmd_moji.h"
#include	"..\cmnsrc\cmd_waku.h"
#include	"..\cmnsrc\cmd_swak.h"
#include	"..\cmnsrc\cmdsprop.h"
#include	"cmd_figu.h"
#include	"..\cmnsrc\cmd_rule.h"
#include	"cmd_zoom.h"
#include	"cmd_trns.h"
#include	"..\cmnsrc\cmd_line.h"
#include	"..\cmnsrc\cmd_pint.h"
#include	"..\cmnsrc\cmd_spcl.h"
#include	"..\cmnsrc\cmd_dibc.h"
#include	"..\cmnsrc\cmdcrdnt.h"
#include	"..\cmnsrc\cmd_atr.h"
#include	"..\cmnsrc\cmd_sub.h"
#include	"..\cmnsrc\cmd_clr.h"
#include	"cmd_file.h"
//#include	"..\cmnsrc\cmdsline.h"
#include	"..\cmnsrc\cmdsstrg.h"
//#include	"..\cmnsrc\cmdastrg.h"
#include	"..\cmnsrc\cmd_msur.h"
#include	"..\cmnsrc\cmdplout.h"
//#include	"..\cmnsrc\cmdcross.h"
#include	"cmdkamon.h"
#include	"..\cmnsrc\cmdkcrct.h"
#include	"..\cmnsrc\cmdmente.h"
#include	"..\cmnsrc\cmd_send.h"
#include	"..\cmnsrc\cmd_ftp.h"
#include	"..\cmnsrc\cmdeftp.h"

//#include	"..\cmnsrc\cmdprout.h"

/////////////////////////////////////////////////////////////////////////////
// class CCommand

class CCommand
{
public:
	CCommand(CScrollView* pcview);
	virtual ~CCommand();
	void SetDocument(CDocument* pDoc);
	UINT Exec(UINT event, DBL x, DBL y);

protected:

// �A�g���r���[�g
protected:
	int		m_nBackMode;
	int		m_nBackFillClr;
	UINT	m_nCrtCommand;
	UINT	m_nSubCommand;
public:

// �I�y���[�V����
public:

public:
	UINT ExecKamon(UINT event, DBL dx, DBL dy);
	UINT ExecFile(UINT event, DBL dx, DBL dy);
	UINT ExecDBEdit(UINT event, DBL dx, DBL dy);
	UINT ExecSub(UINT event, DBL dx, DBL dy);
	UINT ExecMsur(UINT event, DBL x, DBL y);
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
	UINT ExecRule(UINT event, DBL x, DBL y);
	UINT ExecZoom(UINT event, DBL x, DBL y);
	UINT ExecMente(UINT event, DBL x, DBL y);
	void SetSubCommand(int nSubCommand);
	UINT GetSubCommand();
	void SetCrtCommand(UINT command);
	UINT GetCrtCommand();
	int m_nFirstFlag;
private:
	CDocument*	m_pcDoc;
	CScrollView*	m_pcView;
	// �R�}���h���s�N���X
	CCmdFile*	m_pcCmdFile;
	CCmdSlct*	m_pcCmdSlct;
	CCmdDBEdit*	m_pcCmdDBEdit;
	CCmdEdit*	m_pcCmdEdit;
	CCmdStrg*	m_pcCmdStrg;
	CCmdMoji*	m_pcCmdMoji;
	CCmdStrgProp*	m_pcCmdStrgProp;
	//CCmdStrgArc*	m_pcCmdStrgArc;
	CCmdSpclStrg*	m_pcCmdSpclStrg;
	CCmdKamonCrct*	m_pcCmdKamonCrct;
	CCmdWaku*	m_pcCmdWaku;
	CCmdSWak*	m_pcCmdSWak;
	CCmdFigu*	m_pcCmdFigu;
	CCmdRule*	m_pcCmdRule;
	CCmdZoom*	m_pcCmdZoom;
	CCmdTrns*	m_pcCmdTrns;
	CCmdLine*	m_pcCmdLine;
	//CCmdSpclLine*	m_pcCmdSpclLine;
	CCmdPoint*	m_pcCmdPoint;
	CCmdSpcl*	m_pcCmdSpcl;
	CCmdDibC*	m_pcCmdDibC;
	CCmdClr*	m_pcCmdClr;
	CCmdAtr*	m_pcCmdAtr;
	CCmdSub*	m_pcCmdSub;
	CCmdCrdnt*	m_pcCmdCrdnt;
	CCmdSystem*	m_pcCmdSystem;
	CCmdMsur*	m_pcCmdMsur;
	CCmdPloter*	m_pcCmdPloter;
	//CCmdPrint*	m_pcCmdPrint;
	//CCmdCross*	m_pcCmdCross;
	CCmdKamon*	m_pcCmdKamon;
	CCmdMente*	m_pcCmdMente;
	CCmdSend*	m_pcCmdSend;
	CCmdFtp*	m_pcCmdFtp;
	CCmdEFtp*	m_pcCmdEFtp;
};

#endif
