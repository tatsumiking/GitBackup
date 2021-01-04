#pragma once

//C 部品関係コマンド処理クラス
class CCmdPart : public CCmdBaseBmp
{
public:
	CCmdPart(CScrollView* pcview);
	UINT ExecPartLoad(UINT event, DBL dx, DBL dy);
	UINT ExecPartSave(UINT event, DBL dx, DBL dy);
public:
	void LoadPart(LPTSTR lpName);
	void AddPartList(LPTSTR lpName);
	void SavePart(LPTSTR lpName);

private:


};

