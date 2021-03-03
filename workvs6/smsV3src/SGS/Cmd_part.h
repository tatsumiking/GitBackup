#ifndef __CMD_PART_H
#define __CMD_PART_H

//C 部品関係コマンド処理クラス
class CCmdPart : public CCmdBaseBmp
{
public:
	CCmdPart(CScrollView* pcview);
	UINT ExecPartLoad(UINT event, DBL dx, DBL dy);
	UINT ExecPartSave(UINT event, DBL dx, DBL dy);
public:
	void LoadPart(LPSTR lpName);
	void AddPartList(LPSTR lpName);
	void SavePart(LPSTR lpName);

private:


};

#endif
