#ifndef __CMD_BSHI_H
#define __CMD_BSHI_H

//G　概要
//　墓誌用の配置枠作成機能を提供する
//	タイプ１　１行型の作成
//	タイプ２　最初は１行、途中から２行になる作成
//

class CCmdBoshi : public CCmdBase
{
public:
	CCmdBoshi(CScrollView* pcview);
	~CCmdBoshi();
	UINT ExecBoshiType1(UINT event, DBL dx, DBL dy);
	UINT ExecBoshiType2(UINT event, DBL dx, DBL dy);
public:
	void ActiveDataRemoveAt();
	void CreateWakuSort();
	void CreateWaku();
	
private:
	int	m_nLineCount;
	DBL	m_dLineSpace;
	DBL	m_dSpace;
	DBL	m_dWidth;
	DBL	m_dHight;
	int	m_nSide;
};

#endif
