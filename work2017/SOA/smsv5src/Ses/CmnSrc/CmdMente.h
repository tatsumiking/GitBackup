
#pragma once


//C �g�����e�i���X�R�}���h�����N���X
class CCmdMente : public CCmdBase
{
public:
	CCmdMente(CScrollView* pcview);
	UINT ExecCopyTrnsFolder(UINT event, DBL dx, DBL dy);
	UINT ExecCopyTrnsFile(UINT event, DBL dx, DBL dy);
private:
	TCHAR	m_szSrcPath[FNAMEMAX];
	TCHAR	m_szDstPath[FNAMEMAX];
	DBL		m_dXTime, m_dYTime;
	DBL		m_dDstXSize, m_dDstYSize;	
public:

public:
	void WakuSave(LPTSTR lpWakuFileName);
	void WakuLoad(LPTSTR lpWakuName);
	void WakuCopyTrnsLoop(LPTSTR lpWakuSrcList);
	void WakuCopyTrns(LPTSTR lpWakuName);
	void SaveWakuElement();

private:


};

