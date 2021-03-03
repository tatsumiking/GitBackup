#ifndef __CMDMENTE_H
#define __CMDMENTE_H

//C 枠メンテナンスコマンド処理クラス
class CCmdMente : public CCmdBase
{
public:
	CCmdMente(CScrollView* pcview);
	UINT ExecCopyTrnsFolder(UINT event, DBL dx, DBL dy);
	UINT ExecCopyTrnsFile(UINT event, DBL dx, DBL dy);
private:
	char	m_szSrcPath[FNAMEMAX];
	char	m_szDstPath[FNAMEMAX];
	DBL		m_dXTime, m_dYTime;
	DBL		m_dDstXSize, m_dDstYSize;	
public:

public:
	void WakuSave(LPSTR lpWakuFileName);
	void WakuLoad(LPSTR lpWakuName);
	void WakuCopyTrnsLoop(LPSTR lpWakuSrcList);
	void WakuCopyTrns(LPSTR lpWakuName);
	void SaveWakuElement();

private:


};

#endif
